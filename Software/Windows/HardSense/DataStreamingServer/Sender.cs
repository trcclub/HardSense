using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace HardSense.DataStreamingServer
{
    public class Sender
    {
        private StringBuilder message = new StringBuilder();
        private Socket clientSocket;
        private Mutex dataLock = new Mutex();

        public Sender(Socket socket)
        {
            clientSocket = socket;
        }
        
        public void AddKeyToMessage(char key)
        {
            AddStringToMessage(key, "");
        }

        public void AddIntToMessage(char key, int value)
        {
            AddStringToMessage(key, value.ToString());
        }

        public void AddDoubleToMessage(char key, double value)
        {
            AddStringToMessage(key, value.ToString());
        }

        public void AddStringToMessage(char key, string value)
        {
            int dataPacketLength = 2 + value.Length;

            if (dataPacketLength + message.Length > Properties.Settings.Default.DefaultMaxLengthDataToSend)
            {
                SendData();
            }

            StringBuilder tmpSB = new StringBuilder();
            tmpSB.Append(key);
            tmpSB.Append(value);
            tmpSB.Append(ProtocolKeys.TRANSMISSION_KEYS["PACKET_END"]);

            dataLock.WaitOne();
            message.Append(tmpSB.ToString());
            dataLock.ReleaseMutex();
        }

        public void SendData()
        {
            if (message.Length == 0)
            {
                return;
            }
                
            dataLock.WaitOne();
            StringBuilder tmpSB = new StringBuilder(message.ToString());
            message.Clear();
            dataLock.ReleaseMutex();

            tmpSB.Insert(0, ProtocolKeys.TRANSMISSION_KEYS["STX"]);
            tmpSB.Append(ProtocolKeys.TRANSMISSION_KEYS["ETX"]);

            byte[] byteData = Encoding.ASCII.GetBytes(tmpSB.ToString());
            
            try
            {
                clientSocket.Send(byteData, 0, byteData.Length, 0);
            }
            catch (Exception e)
            {
            }
        }
    }
}
