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
    class Sender
    {
        private StringBuilder message = new StringBuilder();
        private Socket socketToStreamTo;
        private Mutex dataLock = new Mutex();

        public Sender(Socket socket)
        {
            socketToStreamTo = socket;
        }

        public void AddKeyToMessage(string key)
        {
            AddStringToMessage(key, "");
        }

        public void AddIntToMessage(string key, int value)
        {
            AddStringToMessage(key, value.ToString());
        }

        public void AddDoubleToMessage(string key, double value)
        {
            AddStringToMessage(key, value.ToString());
        }

        public void AddStringToMessage(string key, string value)
        {
            if (message.Length + key.Length > Properties.Settings.Default.DefaultMaxLengthDataToSend)
            {
                SendData();
            }

            int dataPacketLength = 1 + value.Length;
            int dataPacketTotalLength = dataPacketLength + 5;

            if (dataPacketTotalLength + message.Length > Properties.Settings.Default.DefaultMaxLengthDataToSend)
            {
                SendData();
            }

            StringBuilder tmpSB = new StringBuilder();
            tmpSB.Append(dataPacketTotalLength.ToString());
            tmpSB.Append(ProtocolKeys.TRANSMISSION_KEYS["TRANS__PACKET_SIZE_END"]);
            tmpSB.Append(key);
            tmpSB.Append(value);
            tmpSB.Append(ProtocolKeys.TRANSMISSION_KEYS["TRANS__PACKET_END"]);

            dataLock.WaitOne();
            message.Append(tmpSB.ToString());
            dataLock.ReleaseMutex();
        }

        public void SendData()
        {
            dataLock.WaitOne();
            StringBuilder tmpSB = new StringBuilder(message.ToString());
            message.Clear();
            dataLock.ReleaseMutex();

            tmpSB.Insert(0, ProtocolKeys.TRANSMISSION_KEYS["TRANS_START"]);
            tmpSB.Append(ProtocolKeys.TRANSMISSION_KEYS["TRANS__ETX"]);

            byte[] byteData = Encoding.ASCII.GetBytes(tmpSB.ToString());
            

            socketToStreamTo.Send(byteData, 0, byteData.Length, 0);
        }
    }
}
