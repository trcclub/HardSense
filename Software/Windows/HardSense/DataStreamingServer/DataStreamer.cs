using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Timers;
using System.Net.Sockets;
using System.Threading;
using HardSense.MemFile;
using System.Runtime.InteropServices;

namespace HardSense.DataStreamingServer
{

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

    public class DataStreamer
    {
        private Thread readThread;
        private Thread writeThread;
        private bool running = false;

        private Socket clientSocket;
        private StateObject state = new StateObject();
        private Sender sender;
        private SensorDataStreamer sdStreamer;

        int heartbeatsMissed = 0;
        System.Timers.Timer heartbeatTimer = new System.Timers.Timer();
        
        [DllImport("User32.dll")]
        static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, int dwExtraInfo);

        public DataStreamer(Socket socket)
        {
            clientSocket = socket;
            sender = new Sender(socket);
            sdStreamer = new SensorDataStreamer(sender);

            readThread = new Thread(readThreadProc);
            writeThread = new Thread(writeThreadProc);

            heartbeatTimer.Interval = Properties.Settings.Default.DefaultHeartbeatTime;
            heartbeatTimer.Elapsed += HeatbeatFunc;
            heartbeatTimer.AutoReset = true;
        }

        private void HeatbeatFunc(Object source, System.Timers.ElapsedEventArgs e)
        {
            if(heartbeatsMissed >= Properties.Settings.Default.DefaultMissedHeartbeatsToDisconnect)
            {
                Stop();
                return;
            }
            sender.AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["HEARTBEAT"]);
            heartbeatsMissed++;
        }
        
        public void ThreadProc()
        {
            running = true;
            readThread.Start();
            writeThread.Start();

            //
            // ** RE-ENABLE THE HEARTBEAT
            //
            heartbeatTimer.Start();
            //
            // ** RE-ENABLE THE HEARTBEAT
            //

            writeThread.Join();
            readThread.Join();

            Stop();
        }


        public void Stop()
        {
            running = false;
            heartbeatTimer.Stop();
            sdStreamer.StopStreaming();
            Thread.Sleep(100);
            if (readThread.IsAlive)
            {
                readThread.Join();
            }
            if (writeThread.IsAlive)
            {
                writeThread.Join();
            }

            if (clientSocket.Connected)
            {
                clientSocket.Shutdown(SocketShutdown.Both);
            }
            
            clientSocket.Close();
        }
        private void readThreadProc()
        {
            while (running && DataStreamingServer.continueRunning)
            {
                try
                {
                    int bytesRead = clientSocket.Receive(state.buffer, 0, StateObject.BufferSize, 0);
                    if (bytesRead > 0)
                    {
                        state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                        string leftOverContent = HandleInput(state.sb.ToString());
                        state.sb.Clear();
                        state.sb.Append(leftOverContent);
                    }
                    Thread.Sleep(20);
                }
                catch (Exception e)
                {
                }
            }

        }
        private string HandleInput(string inputData)
        {
            string ret = "";
            if (inputData.Length == 0)
            {
                return ret;
            }

            int firstInstanceOfStart = inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["STX"]);
            if (firstInstanceOfStart > 0)
            {
                inputData = inputData.Substring(firstInstanceOfStart);
            }

            if (!inputData.StartsWith(ProtocolKeys.TRANSMISSION_KEYS["STX"].ToString()))
            {
                if (inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["STX"]) > 0)
                {
                    inputData = inputData.Substring(inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["STX"]));
                }
                else
                {
                    return ret;
                }
            }


            int endMarkerLocation = inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["ETX"]);
            if (endMarkerLocation != inputData.Length)
            {
                if (endMarkerLocation > 0)
                {
                    string tmpContent = inputData.Substring(0, inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["ETX"]) + 1);
                    ret = inputData.Substring(inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["ETX"]) + 1);
                    parseInput(tmpContent);
                    return HandleInput(ret);
                }
                else
                {
                    return inputData;
                }
            }

            parseInput(inputData);
            return ret;
        }

        private void parseInput(string inputData)
        {
            string strippedData = inputData.Substring(1, inputData.Length - 3);
            string[] tokens = strippedData.Split(ProtocolKeys.TRANSMISSION_KEYS["PACKET_END"]);
            {
                foreach(string currItem in tokens)
                {
                    char key = currItem.ToCharArray()[0];
                    string value = currItem.Substring(1);
                    DispatchRequest(key, value);
                }
            }
        }

        private void DispatchRequest(char key, string value)
        {
            switch (key)
            {
                case (char)TRANS__KEY.REQUEST_NEW_CONNECTION:
                    sender.AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["NEW_CONNECTION_APPROVED"]);
                    break;
                case (char)TRANS__KEY.START_SENSOR_DATA_STREAM:
                    if (sdStreamer.StartStreaming())
                    {
                        sender.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["ACK"], "Started data streaming");
                    } else
                    {
                        sender.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["ACK"], "Failed to start data streaming");
                    }

                    break;
                case (char)TRANS__KEY.STOP_SENSOR_DATA_STREAM:
                    sdStreamer.StopStreaming();
                    sender.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["ACK"], "Stoped data streaming");
                    break;
                case (char)TRANS__KEY.ADD_SENSORS_TO_SENSOR_LIST:
                    AddSensorListToSDStreamer(value);
                    sender.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["ACK"], "Sensors aded to list");
                    break;
                case (char)TRANS__KEY.CLEAR_SENSOR_LIST:
                    sdStreamer.ClearSensorList();
                    break;
                case (char)TRANS__KEY.HEARTBEAT_ACK:
                    heartbeatsMissed = 0;
                    break;
                case (char)TRANS__KEY.HEARTBEAT:
                    sender.AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["HEARTBEAT_ACK"]);
                    break;
                case (char)TRANS__KEY.INCREASE_VOLUME:
                    keybd_event(0xAF, 0, 0, 0);
                    break;
                case (char)TRANS__KEY.DECREASE_VOLUME:
                    keybd_event(0xAE, 0, 0, 0);
                    break;
                default:
                    break;
            }
        }

        private void AddSensorListToSDStreamer(string rawSensorList)
        {
            // rawSensorList format:
            // <string>sensorID,<char>key|<string>sensorID,<char>key|...
            //

            string[] tokens = rawSensorList.Split('|');
            foreach(string currItem in tokens)
            {
                string[] currDataItem = currItem.Split(',');
                if(currDataItem.Length == 2)
                {
                    sdStreamer.AddSensorToStream(currDataItem);
                }
                
            }

        }

        private void writeThreadProc()
        {
            try
            {
                while (running && DataStreamingServer.continueRunning)
                {
                    sender.SendData();
                    Thread.Sleep(50);
                }
            }
            catch (Exception e)
            {
            }
        }
    }
}
