using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

namespace HardSense.DataStreamingServer
{
    class Sender
    {
        private string dataToSend = "";
        private Socket socketToStreamTo;

        public Sender(Socket socket)
        {
            socketToStreamTo = socket;
        }

        public void AddKeyToOutput(string key)
        {
            if(dataToSend.Length + key.Length > Properties.Settings.Default.DefaultMaxLengthDataToSend)
            {
                SendData();
            }
            dataToSend += key;
            //dataToSend += ProtocolKeys.TRANSMISSION_KEYS[""]
        }

        private void SendData()
        {
            //byte[] byteData = Encoding.ASCII.GetBytes(data);
            //socketToStreamTo.Send(byteData, 0, byteData.Length, 0);

            //dataToSend = "";
        }
    }
}
