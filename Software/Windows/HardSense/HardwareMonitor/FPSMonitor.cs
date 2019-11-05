using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Threading;
using Microsoft.Diagnostics.Tracing.Session;


namespace HardSense.HardwareMonitor
{
    class FPSMonitor
    {
        //event codes (https://github.com/GameTechDev/PresentMon/blob/40ee99f437bc1061a27a2fc16a8993ee8ce4ebb5/PresentData/PresentMonTraceConsumer.cpp)
        private const int EventID_D3D9PresentStart = 1;
        private const int EventID_DxgiPresentStart = 42;

        //ETW provider codes
        private readonly Guid DXGI_provider = Guid.Parse("{CA11C036-0102-4A2D-A6AD-F03CFED5D3C9}");
        private readonly Guid D3D9_provider = Guid.Parse("{783ACA0A-790E-4D7F-8451-AA850511C6B9}");

        private TraceEventSession EtwSession;
        private static int MaxNumberSamples = 200;
        private static int MaxNumberAverages = 25;
        private static int PollTime = 20;

        private Thread EventThread;
        private Thread ProcessingThread;

        private List<long> timestamps = new List<long>(MaxNumberSamples);
        private List<int> rollingAverage = new List<int>(MaxNumberAverages);

        private Stopwatch watch = null;
        private object sync = new object();

        private bool continueMonitoring = false;

        public int averageFPS = 0;

        public FPSMonitor()
        {
            //create ETW session and register providers
            EtwSession = new TraceEventSession("mysess");
            EtwSession.StopOnDispose = true;
            EtwSession.EnableProvider("Microsoft-Windows-D3D9");
            EtwSession.EnableProvider("Microsoft-Windows-DXGI");

            watch = new Stopwatch();
            //handle event
            EtwSession.Source.AllEvents += data =>
            {
                //filter out frame presentation events
                if (((int)data.ID == EventID_D3D9PresentStart && data.ProviderGuid == D3D9_provider) || ((int)data.ID == EventID_DxgiPresentStart && data.ProviderGuid == DXGI_provider))
                {
                    long t = watch.ElapsedMilliseconds;

                    lock (sync)
                    {
                        if (timestamps.Count == MaxNumberSamples)
                        {
                            timestamps.RemoveAt(0);
                        }
                        timestamps.Add(t);
                    }
                }
            };
        }

        ~FPSMonitor()
        {
            EtwSession.Dispose();
            StopMonitor();
        }

        public void StartMonitor()
        {
            watch.Start();
            continueMonitoring = true;
            EventThread = new Thread(EventThreadFunction);
            EventThread.IsBackground = true;
            EventThread.Start();

            ProcessingThread = new Thread(FPSProcessingFunction);
            ProcessingThread.Start();
        }


        public void StopMonitor()
        {
            continueMonitoring = false;
            ProcessingThread.Join();
        }

        void EventThreadFunction()
        {
            EtwSession.Source.Process();
        }

        private void FPSProcessingFunction()
        {
            long t1, t2;
            long duration = 1000;

            while(continueMonitoring)
            {
                t2 = watch.ElapsedMilliseconds;
                t1 = t2 - duration;
                int fullRangeAverage = 0;
                lock (sync)
                {
                    foreach (var ts in timestamps)
                    {
                        if (ts >= t1 && ts <= t2)
                        {
                            fullRangeAverage++;
                        }
                    }
                }

                if (rollingAverage.Count == MaxNumberAverages)
                {
                    rollingAverage.RemoveAt(0);
                }
                rollingAverage.Add(fullRangeAverage);

                int average = 0;
                foreach (var i in rollingAverage)
                {
                    average += i;
                }
                averageFPS = average / rollingAverage.Count;

                Thread.Sleep(PollTime);
            }
        }
        private int CalculateAverageFPS(long from, long to)
        {
            int ret = 0;
            int c = 0;

            lock (sync)
            {
                foreach (var ts in timestamps)
                {
                    if (ts >= from && ts <= to) c++;
                }
            }

            if (rollingAverage.Count == MaxNumberAverages)
            {
                rollingAverage.RemoveAt(0);
            }
            rollingAverage.Add(c);
            int a = 0;
            foreach (var i in rollingAverage)
            {
                a += i;
            }
            ret = a / rollingAverage.Count;

            return ret;
        }
    }
}
