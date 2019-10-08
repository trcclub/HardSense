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
    public class DataStreamingServer
    {
        public static ManualResetEvent allDone = new ManualResetEvent(false);
        public static bool running = false;
        public static bool continueRunning = false;
        private static Thread socketServerThread;
        private static Socket listener;

        
        public static void StartServer()
        {
            ProtocolKeys.init();  
            continueRunning = true;
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

            continueRunning = false;
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

                while (continueRunning)
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
               
        public static void AcceptCallback(IAsyncResult ar)
        {
            allDone.Set(); // Signal the main thread to continue.  
            try
            {
                Socket listener = (Socket)ar.AsyncState;
                DataStreamer dataStreamer = new DataStreamer(listener.EndAccept(ar));
                Thread dataStreamerThread = new Thread(new ThreadStart(dataStreamer.ThreadProc));
                dataStreamerThread.Start();
                dataStreamerThread.Join();
            }
            catch (Exception e)
            {

            }
        }
    }
}

