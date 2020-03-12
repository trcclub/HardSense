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
        public double lastValue;
        
        public SensorItem(string id, char newKey)
        {
            sensorId = id;
            key = newKey;
            lastValue = -1;
        }
    }
    public class SensorDataStreamer
    {
        private Sender sender;
        private Thread monitorThread;
        private bool continueRunning = false;
        private List<SensorItem> listOfSensorsToRetrieve = new List<SensorItem>();

        public SensorDataStreamer(Sender newSender)
        {
            sender = newSender;
            monitorThread = new Thread(ThreadProc);

        }

        public bool StartStreaming()
        {
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
                foreach (SensorItem currSensorItem in listOfSensorsToRetrieve)
                {
                    try
                    {
                        double currValue = HardSenseMemFile.GetValueByKey(currSensorItem.sensorId);
                        if (currValue != currSensorItem.lastValue)
                        {
                            currSensorItem.lastValue = currValue;
                            //Sensor update format:
                            //<key>,<value>
                            string tmp = currSensorItem.key + "," + currValue.ToString();
                            sender.AddStringToMessage((char)TRANS__KEY.UPDATE_SENSOR_VALUE, tmp);
                        }
                    }
                    catch (Exception e)
                    {
                    }
                }
                Thread.Sleep(Properties.Settings.Default.DefaultSensorDataStreamer_UpdateTime);
            }
        }

        

    }
}
