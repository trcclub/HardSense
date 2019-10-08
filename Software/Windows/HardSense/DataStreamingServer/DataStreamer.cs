using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
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

        public DataStreamer(Socket socket)
        {
            socketToStreamTo = socket;
            dataToSend = new Sender(socket);
            sdStreamer = new SensorDataStreamer(dataToSend);

            readThread = new Thread(readThreadProc);
            writeThread = new Thread(writeThreadProc);
        }

        
        public void ThreadProc()
        {
            readThread.Start();
            writeThread.Start();
            //sdStreamer.StartStreaming();
            while(DataStreamingServer.continueRunning)
            {
                Thread.Sleep(50);
            }

            Stop();
        }


        private void Stop()
        {
            if(!running)
            {
                return;
            }
            readThread.Join();
            writeThread.Join();
            running = false;
        }
        private void readThreadProc()
        {
            while(DataStreamingServer.continueRunning)
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
                    sdStreamer.StartStreaming();
                    dataToSend.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["TRANS__CONNECTION_ACK"], "Started data streaming");
                    break;
                case (char)TRANS__KEY.TRANS__STOP_SENSOR_DATA_STREAM:
                    sdStreamer.StopStreaming();
                    dataToSend.AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["TRANS__CONNECTION_ACK"], "Stoped data streaming");
                    break;
                default:
                    break;
            }
        }

        private void writeThreadProc()
        {
            while (DataStreamingServer.continueRunning)
            {
                dataToSend.SendData();
                Thread.Sleep(250);
            }

        }
        
    }
}
