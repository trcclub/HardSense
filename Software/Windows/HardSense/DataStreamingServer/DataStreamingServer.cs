using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace HardSense.DataStreamingServer
{
    static class TransKeys
    {
        public static Dictionary<string, string> TRANSMISSION_KEYS = new Dictionary<string, string>();
        
        public static void init()
        {
            TRANSMISSION_KEYS.Add("TRANS_START", "\u0002");
            TRANSMISSION_KEYS.Add("TRANS__ETX", "\u0003");
            TRANSMISSION_KEYS.Add("TRANS__DATA_SIZE_END", "\u001F");
            TRANSMISSION_KEYS.Add("TRANS__PACKET_SIZE_END", "\u001E");
            TRANSMISSION_KEYS.Add("TRANS__PACKET_END", "\u001D");
            TRANSMISSION_KEYS.Add("TRANS__ACK", "\u0006");
            TRANSMISSION_KEYS.Add("TRANS__NACK", "\u0015");

            TRANSMISSION_KEYS.Add("TRANS__CONNECTION_REQUEST", "\u0005");
            TRANSMISSION_KEYS.Add("TRANS__CONNECTION_ACK", "\u0001");
            TRANSMISSION_KEYS.Add("TRANS__DISCONNECT", "\u0004");

            TRANSMISSION_KEYS.Add("TRANS__REQUESTING_FULL_SENSOR_LIST", "\u0021");
            TRANSMISSION_KEYS.Add("TRANS__SENDING_FULL_SENSOR_LIST", "\u0022");
            TRANSMISSION_KEYS.Add("TRANS__REQUEST_SENSORS_FOR_STREAMING", "\u0023");
            TRANSMISSION_KEYS.Add("TRANS__UPDATE_SENSOR_VALUE", "\u0024");

        }

    }

    public class StateObject
    {
        // Client  socket.  
        public Socket workSocket = null;
        // Size of receive buffer.  
        public const int BufferSize = 1024;
        // Receive buffer.  
        public byte[] buffer = new byte[BufferSize];
        // Received data string.  
        public StringBuilder sb = new StringBuilder();
    }
    public class DataStreamingServer
    {
        public static ManualResetEvent allDone = new ManualResetEvent(false);
        public static bool running = false;
        private static bool continueMonitoring = false;
        private static Thread socketServerThread;
        private static Socket listener;
        
        public static void StartServer()
        {
            TransKeys.init();  
            continueMonitoring = true;
            socketServerThread = new Thread(SocketListener);
            socketServerThread.Start();
            running = true;
        }
        public static void StopServer()
        {
            if (!running)
            {
                return;
            }

            continueMonitoring = false;
            listener.Close();
            socketServerThread.Join();
            running = false;
        }

        private static void SocketListener()
        {
            // Establish the local endpoint for the socket.  
            // The DNS name of the computer  
            // running the listener is "host.contoso.com".  
            IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = ipHostInfo.AddressList[1];
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, Properties.Settings.Default.DefaultDataStreamingServerPort);

            // Create a TCP/IP socket.  
            listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            
            // Bind the socket to the local endpoint and listen for incoming connections.  
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(100);

                while (continueMonitoring)
                {
                    // Set the event to nonsignaled state.  
                    allDone.Reset();

                    // Start an asynchronous socket to listen for connections.  
                    //Console.WriteLine("Waiting for a connection...");
                    listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);

                    // Wait until a connection is made before continuing.  
                    allDone.WaitOne();
                }
                
            }
            catch (Exception e)
            {

            }
        }

        /*
        private static void AcceptCallback(IAsyncResult ar)
        {
            // Signal the main thread to continue.  
            allDone.Set();
            try
            {
                Socket listener = (Socket)ar.AsyncState;
                Socket handler = listener.EndAccept(ar);

                while (continueMonitoring)
                {
                    StringBuilder sb = new StringBuilder();
                    byte[] inByte = new byte[1];
                    int byteRead = handler.Receive(inByte, 1, SocketFlags.None);

                    if (byteRead > 0 && inByte[0] == (byte)TRANS__KEY.TRANS__START)
                    {
                        while (handler.Receive(inByte, 1, SocketFlags.None) > 0)
                        {
                            if (inByte[0] == (byte)TRANS__KEY.TRANS__ETX)
                            {
                                break;
                            }
                            sb.Append((char)inByte[0]);
                        }
                        sb.Append(" HardSense!");
                        Send(handler, sb.ToString());
                    }
                }
            }
            catch (Exception e)
            {

            }
        }
        */


        public static void AcceptCallback(IAsyncResult ar)
        {
                // Signal the main thread to continue.  
            allDone.Set();
            try
            {
                // Get the socket that handles the client request.  
                Socket listener = (Socket)ar.AsyncState;
                Socket handler = listener.EndAccept(ar);

                // Create the state object.  
                StateObject state = new StateObject();
                state.workSocket = handler;
                handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReadCallback), state);
            }
            catch (Exception e)
            {

            }
        }

        public static void ReadCallback(IAsyncResult ar)
        {
            try
            {
                StateObject state = (StateObject)ar.AsyncState;
                Socket handler = state.workSocket;
                int bytesRead = handler.EndReceive(ar);
                if (bytesRead > 0 && continueMonitoring)
                {
                    state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                    string leftOverContent = HandleInput(state.sb.ToString(), handler);
                    state.sb.Clear();
                    state.sb.Append(leftOverContent);
                }

                if (continueMonitoring)
                {
                    handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReadCallback), state);
                }
            }
            catch (Exception e)
            {

            }

        }

        private static string HandleInput(string inputData, Socket socketHandler)
        {
            string ret = "";
            if(inputData.Length == 0)
            {
                return ret;
            }

            int firstInstanceOfStart = inputData.IndexOf(TransKeys.TRANSMISSION_KEYS["TRANS_START"]);
            if (firstInstanceOfStart > 0)
            {
                inputData = inputData.Substring(firstInstanceOfStart);
            }

            if (!inputData.StartsWith(TransKeys.TRANSMISSION_KEYS["TRANS_START"]))
            {
                if (inputData.IndexOf(TransKeys.TRANSMISSION_KEYS["TRANS_START"]) > 0)
                {
                    inputData = inputData.Substring(inputData.IndexOf(TransKeys.TRANSMISSION_KEYS["TRANS_START"]));
                }
                else
                {
                    return ret;
                }
            }


            int endMarkerLocation = inputData.IndexOf(TransKeys.TRANSMISSION_KEYS["TRANS__ETX"]);
            if (endMarkerLocation != inputData.Length)
            {
                if (endMarkerLocation > 0)
                {
                    string tmpContent = inputData.Substring(0, inputData.IndexOf(TransKeys.TRANSMISSION_KEYS["TRANS__ETX"]) + 1);
                    ret = inputData.Substring(inputData.IndexOf(TransKeys.TRANSMISSION_KEYS["TRANS__ETX"]) + 1);
                    parseInput(tmpContent, socketHandler);
                    return HandleInput(ret,socketHandler);
                }
                else
                {
                    return inputData;
                }
            }


            parseInput(inputData, socketHandler);
            return ret;
        }

        private static void parseInput(string inputData, Socket socketHandler)
        {
            string strippedData = inputData.Substring(1, inputData.Length - 2);
            strippedData += TransKeys.TRANSMISSION_KEYS["TRANS__ETX"];
            Send(socketHandler, strippedData);
        }

        private static void Send(Socket handler, String data)
        {
            // Convert the string data to byte data using ASCII encoding.  
            byte[] byteData = Encoding.ASCII.GetBytes(data);

            // Begin sending the data to the remote device.  
            handler.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), handler);
        }

        private static void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                Socket handler = (Socket)ar.AsyncState;

                // Complete sending the data to the remote device.  
                int bytesSent = handler.EndSend(ar);


                //handler.Shutdown(SocketShutdown.Both);
                //handler.Close();

            }
            catch (Exception e)
            {

            }
        }
    }
}

