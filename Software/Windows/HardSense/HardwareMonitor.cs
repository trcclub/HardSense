using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using OpenHardwareMonitor.Hardware;

namespace HardSense
{
    class HardwareMonitor
    {
        public String lastErrorMessage = "";
        public bool running = false;
        
        private bool continueMonitoring = false;
        private Thread monitorThread;
        
        private UpdateVisitor updateVisitor;
        private Computer computer;

        public LocalHardwareItem localMainBoard;

        public HardwareMonitor()
        {
            updateVisitor = new UpdateVisitor();
            computer = new Computer();
            localMainBoard = new LocalHardwareItem();

            computer.Open();
            //computer.Accept(updateVisitor);

            UpdateAllHardwareInfo();

            computer.CPUEnabled = true;
            computer.GPUEnabled = true;
            computer.RAMEnabled = true;
            computer.MainboardEnabled = true;
            computer.FanControllerEnabled = true;
            computer.HDDEnabled = true;

        }

        ~HardwareMonitor()
        {
            computer.Close();
        }

        private void ThreadProc()
        {
            while (continueMonitoring)
            {

                Thread.Sleep(250);
            }
        }

        public bool StartMonitor()
        {
            continueMonitoring = true;
            monitorThread = new Thread(ThreadProc);
            monitorThread.Start();
            running = true;
            return true;
        }

        public bool StopMonitor()
        {
            if (!running) return true;
            continueMonitoring = false;
            monitorThread.Join();
            running = false;
            return true;
        }

        public void UpdateAllHardwareInfo()
        {
            UpdateMainboardInfo();
        }

        public void UpdateMainboardInfo()
        {
            computer.MainboardEnabled = true;
            computer.CPUEnabled = false;
            computer.GPUEnabled = false;
            computer.RAMEnabled = false;
            computer.FanControllerEnabled = false;
            computer.HDDEnabled = false;

            computer.Traverse(updateVisitor);
            if(computer.Hardware.Length > 0)
            {
                IHardware currItem = computer.Hardware[0];
                localMainBoard.Name = currItem.Name;
            }

        }

    }



    public class UpdateVisitor : IVisitor
    {
        public void VisitComputer(IComputer computer)
        {
            computer.Traverse(this);
        }
        public void VisitHardware(IHardware hardware)
        {
            hardware.Update();
            foreach (IHardware subHardware in hardware.SubHardware) subHardware.Accept(this);
        }
        public void VisitSensor(ISensor sensor) { }
        public void VisitParameter(IParameter parameter) { }
    }
}
