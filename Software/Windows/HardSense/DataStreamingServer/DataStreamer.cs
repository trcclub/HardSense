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

        private Socket socketToStreamTo;
        private StateObject state = new StateObject();
        private Sender dataToSend;
        private SensorDataStreamer sdStreamer;

        int heartbeatsMissed = 0;
        System.Timers.Timer heartbeatTimer = new System.Timers.Timer();


        public DataStreamer(Socket socket)
        {
            socketToStreamTo = socket;
            dataToSend = new Sender(socket);
            sdStreamer = new SensorDataStreamer(dataToSend);

            readThread = new Thread(readThreadProc);
            writeThread = new Thread(writeThreadProc);
            heartbeatTimer.Interval = 500;
            heartbeatTimer.Elapsed += HeatbeatFunc;
            heartbeatTimer.AutoReset = true;
        }

        private void HeatbeatFunc(Object source, System.Timers.ElapsedEventArgs e)
        {
            if(heartbeatsMissed >= 3)
            {
                Stop();
                return;
            }
            dataToSend.AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["TRANS__HEARTBEAT"]);
            heartbeatsMissed++;
        }
        
        public void ThreadProc()
        {
            running = true;
            readThread.Start();
            writeThread.Start();
            heartbeatTimer.Start();

            readThread.Join();
            writeThread.Join();

            Stop();
        }


        public void Stop()
        {
            if(!running)
            {
                return;
            }
            heartbeatTimer.Stop();
            running = false;
            sdStreamer.StopStreaming();
            readThread.Join();
            writeThread.Join();

            socketToStreamTo.Shutdown(SocketShutdown.Both);
            socketToStreamTo.Close();
        }
        private void readThreadProc()
        {
            try
            {
                while (running && DataStreamingServer.continueRunning)
                {
                    int bytesRead = socketToStreamTo.Receive(state.buffer, 0, StateObject.BufferSize, 0);
                    if (bytesRead > 0)
                    {
                        state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                        string leftOverContent = HandleInput(state.sb.ToString());
                        state.sb.Clear();
                        state.sb.Append(leftOverContent);
                    }
                    Thread.Sleep(20);
                }
            }
            catch (Exception e)
            {
            }

        }
        private string HandleInput(string inputData)
        {
            string ret = "";
            if (inputData.Length == 0)
            {
                return ret;
            }

            int firstInstanceOfStart = inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["TRANS_START"]);
            if (firstInstanceOfStart > 0)
            {
                inputData = inputData.Substring(firstInstanceOfStart);
            }

            if (!inputData.StartsWith(ProtocolKeys.TRANSMISSION_KEYS["TRANS_START"].ToString()))
            {
                if (inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["TRANS_START"]) > 0)
                {
                    inputData = inputData.Substring(inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["TRANS_START"]));
                }
                else
                {
                    return ret;
                }
            }


            int endMarkerLocation = inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["TRANS__ETX"]);
            if (endMarkerLocation != inputData.Length)
            {
                if (endMarkerLocation > 0)
                {
                    string tmpContent = inputData.Substring(0, inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["TRANS__ETX"]) + 1);
                    ret = inputData.Substring(inputData.IndexOf(ProtocolKeys.TRANSMISSION_KEYS["TRANS__ETX"]) + 1);
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
            string[] tokens = strippedData.Split(ProtocolKeys.TRANSMISSION_KEYS["TRANS__PACKET_END"]);
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
                case (char)TRANS__KEY.TRANS__START_SENSOR_DATA_STREAM:
                    if (sdStreamer.StartStreaming())
                    {
                        dataToSend.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["TRANS__CONNECTION_ACK"], "Started data streaming");
                    } else
                    {
                        dataToSend.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["TRANS__CONNECTION_ACK"], "Failed to start data streaming");
                    }

                    break;
                case (char)TRANS__KEY.TRANS__STOP_SENSOR_DATA_STREAM:
                    sdStreamer.StopStreaming();
                    dataToSend.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["TRANS__CONNECTION_ACK"], "Stoped data streaming");
                    break;
                case (char)TRANS__KEY.TRANS__ADD_SENSORS_SENSOR_LIST:
                    AddSensorListToSDStreamer(value);
                    dataToSend.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["TRANS__CONNECTION_ACK"], "Sensors aded to list");
                    break;
                case (char)TRANS__KEY.TRANS__HEARTBEAT_ACK:
                    heartbeatsMissed = 0;
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
                    dataToSend.SendData();
                    Thread.Sleep(250);
                }
            }
            catch (Exception e)
            {
            }
        }
        
    }
}
