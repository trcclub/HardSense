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
            monitorThread = new Thread(ThreadProc);
            //AddSensorToStream("/Ethernet/0/recv", 'a');
            //AddSensorToStream("/intelcpu/0/load/0", 'b');

        }

        public bool StartStreaming()
        {
            if(listOfSensorsToRetrieve.Count == 0)
            {
                return false;
            }

            continueRunning = true;
            monitorThread.Start();
            return true;
        }

        public void StopStreaming()
        {
            continueRunning = false;
            if (monitorThread.IsAlive)
            {
                monitorThread.Join();
            }
            
        }

        public void AddSensorToStream(string[] dataItem)
        {
            if (HardSenseMemFile.DoesIdExistInList(dataItem[0]))
            {
                listOfSensorsToRetrieve.Add(new SensorItem(dataItem[0], dataItem[1].ToCharArray()[0]));
            }
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
                Thread.Sleep(1000);
            }
        }

        

    }
}
