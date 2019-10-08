using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Threading;
using HardSense.HardwareMonitor;
using HardSense.MemFile;

namespace HardSense.DataStreamingServer
{
    class SensorItem
    {
        public string sensorId;
        public char key;
        
        public SensorItem(string id, char newKey)
        {
            sensorId = id;
            key = newKey;
        }
    }
    public class SensorDataStreamer
    {
        private Sender dataToSend;
        private Thread monitorThread;
        private bool continueRunning = false;
        private List<SensorItem> listOfSensorsToRetrieve = new List<SensorItem>();

        public SensorDataStreamer(Sender newSender)
        {
            dataToSend = newSender;
            AddSensorToStream("/Ethernet/0/recv", 'a');
            AddSensorToStream("/intelcpu/0/load/0", 'b');

        }

        public void StartStreaming()
        {
            if(listOfSensorsToRetrieve.Count == 0)
            {
                return;
            }

            continueRunning = true;
            monitorThread = new Thread(ThreadProc);
            monitorThread.Start();
        }

        public void StopStreaming()
        {
            continueRunning = false;
            monitorThread.Join();
        }

        public void AddSensorToStream(string sensorId, char key)
        {
            listOfSensorsToRetrieve.Add(new SensorItem(sensorId, key));
        }

        public void ClearSensorList()
        {
            listOfSensorsToRetrieve.Clear();
        }

        private void ThreadProc()
        {
            while (continueRunning && DataStreamingServer.continueRunning)
            {
                foreach(SensorItem currSensorItem in listOfSensorsToRetrieve)
                {
                    double value = HardSenseMemFile.GetValueByKey(currSensorItem.sensorId);
                    dataToSend.AddDoubleToMessage(currSensorItem.key, value);
                }
                Thread.Sleep(500);
            }
        }

        

    }
}
