using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using HardSense.HardwareMonitor;
using HardSense.MemFile;

namespace HardSense.GUI
{
    struct SensorNodeItem
    {
        public String name;
        public String id;
    }

    public partial class SensorMonitor : Form
    {
        public static bool running = false;
        private static Thread monitorThread;

        public SensorMonitor()
        {
            InitializeComponent();


            TreeNode rootNode = new TreeNode();
            rootNode.Text = Environment.MachineName;


            TreeNode mbNode = new TreeNode();
            mbNode.Text = "Motherboads";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.motherBoardInfo)
            {
                BuildSensorNodeList(currItem, mbNode);
            }
            rootNode.Nodes.Add(mbNode);


            TreeNode cpuNode = new TreeNode();
            cpuNode.Text = "CPUs";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.cpuInfo)
            {
                BuildSensorNodeList(currItem, cpuNode);
            }
            rootNode.Nodes.Add(cpuNode);


            TreeNode gpuNode = new TreeNode();
            gpuNode.Text = "GPUs";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.gpuInfo)
            {
                BuildSensorNodeList(currItem, gpuNode);
            }
            rootNode.Nodes.Add(gpuNode);


            TreeNode fanNode = new TreeNode();
            fanNode.Text = "Fans";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.fanInfo)
            {
                BuildSensorNodeList(currItem, fanNode);
            }
            rootNode.Nodes.Add(fanNode);


            TreeNode hddNode = new TreeNode();
            hddNode.Text = "Hard Drives";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.hddInfo)
            {
                BuildSensorNodeList(currItem, hddNode);
            }
            rootNode.Nodes.Add(hddNode);


            TreeNode ramNode = new TreeNode();
            ramNode.Text = "Ram";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.ramInfo)
            {
                BuildSensorNodeList(currItem, ramNode);
            }
            rootNode.Nodes.Add(ramNode);


            TreeNode netNode = new TreeNode();
            netNode.Text = "Network";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.nicInfo)
            {
                BuildSensorNodeList(currItem, netNode);
            }
            rootNode.Nodes.Add(netNode);


            TreeNode fpsNode = new TreeNode();
            fpsNode.Text = "FPS";
            foreach (LocalHardwareItem currItem in LocalHardwareMonitor.fpsInfo)
            {
                BuildSensorNodeList(currItem, fpsNode);
            }
            rootNode.Nodes.Add(fpsNode);


            rootNode.Expand();
            treeView_SensorList.Nodes.Add(rootNode);


            monitorThread = new Thread(MonitorThreadProc);
//            StartMonitor();
        }




        public void StartMonitor()
        {
            running = true;
           
            monitorThread.Start();
        }

        public void StopMonitor()
        {
            running = false;
            if(monitorThread.IsAlive)
                monitorThread.Join();

        }

        public void MonitorThreadProc()
        {
            while(running)
            {
                foreach(TreeNode currTreeNode in treeView_SensorList.Nodes)
                {
                    IterateTreeNode(currTreeNode);
                }
                Thread.Sleep(500);
            }
        }

        private void IterateTreeNode(TreeNode currNode)
        {
            if (currNode.Tag == null) 
                IterateTreeNode(currNode);


            SensorNodeItem currSensorNodeItem = (SensorNodeItem)currNode.Tag;
            currNode.Text = currSensorNodeItem.name + " : " + HardSenseMemFile.GetValueByKey(currSensorNodeItem.id);
        }



        void BuildSensorNodeList(LocalHardwareItem currHardwareItem, TreeNode parentTreeNode)
        {
            TreeNode currNode = new TreeNode();
            currNode.Text = currHardwareItem.Name + " (" + currHardwareItem.Id + ")";

            TreeNode voltageNode = new TreeNode();
            voltageNode.Text = "Voltages";

            TreeNode clockNode = new TreeNode();
            clockNode.Text = "Clock";

            TreeNode tempNode = new TreeNode();
            tempNode.Text = "Temperature";

            TreeNode loadNode = new TreeNode();
            loadNode.Text = "Load";

            TreeNode fanNode = new TreeNode();
            fanNode.Text = "Fan";

            TreeNode flowNode = new TreeNode();
            flowNode.Text = "Flow";

            TreeNode controlNode = new TreeNode();
            controlNode.Text = "Control";

            TreeNode levelNode = new TreeNode();
            levelNode.Text = "Level";

            TreeNode factorNode = new TreeNode();
            factorNode.Text = "Factor";

            TreeNode powerNode = new TreeNode();
            powerNode.Text = "Power";

            TreeNode dataNode = new TreeNode();
            dataNode.Text = "Data";

            TreeNode smallDataNode = new TreeNode();
            smallDataNode.Text = "Small Data";

            TreeNode speedNode = new TreeNode();
            speedNode.Text = "Speed (KBps)";

            TreeNode fpsNode = new TreeNode();
            fpsNode.Text = "FPS";


            foreach (LocalSensor currSensor in currHardwareItem.SensorList)
            {
                switch (currSensor.Type)
                {
                    case OpenHardwareMonitor.Hardware.SensorType.Voltage:
                        BuildSensorNode(currSensor, voltageNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Clock:
                        BuildSensorNode(currSensor, clockNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Temperature:
                        BuildSensorNode(currSensor, tempNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Load:
                        BuildSensorNode(currSensor, loadNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Fan:
                        BuildSensorNode(currSensor, fanNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Flow:
                        BuildSensorNode(currSensor, flowNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Control:
                        BuildSensorNode(currSensor, controlNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Level:
                        BuildSensorNode(currSensor, levelNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Factor:
                        BuildSensorNode(currSensor, factorNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Power:
                        BuildSensorNode(currSensor, powerNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.Data:
                        BuildSensorNode(currSensor, dataNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.SmallData:
                        BuildSensorNode(currSensor, smallDataNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.KBPS:
                        BuildSensorNode(currSensor, speedNode);
                        break;
                    case OpenHardwareMonitor.Hardware.SensorType.FPS:
                        BuildSensorNode(currSensor, fpsNode);
                        break;

                    default:
                        break;
                }
            }

            if (fpsNode.Nodes.Count > 0)
                currNode.Nodes.Add(fpsNode);

            if (speedNode.Nodes.Count > 0)
                currNode.Nodes.Add(speedNode);

            if (smallDataNode.Nodes.Count > 0)
                currNode.Nodes.Add(smallDataNode);

            if (dataNode.Nodes.Count > 0)
                currNode.Nodes.Add(dataNode);

            if (powerNode.Nodes.Count > 0)
                currNode.Nodes.Add(powerNode);

            if (factorNode.Nodes.Count > 0)
                currNode.Nodes.Add(factorNode);

            if (levelNode.Nodes.Count > 0)
                currNode.Nodes.Add(levelNode);

            if (controlNode.Nodes.Count > 0)
                currNode.Nodes.Add(controlNode);

            if (flowNode.Nodes.Count > 0)
                currNode.Nodes.Add(flowNode);

            if (fanNode.Nodes.Count > 0)
                currNode.Nodes.Add(fanNode);

            if (loadNode.Nodes.Count > 0)
                currNode.Nodes.Add(loadNode);

            if (tempNode.Nodes.Count > 0)
                currNode.Nodes.Add(tempNode);

            if (voltageNode.Nodes.Count > 0)
                currNode.Nodes.Add(voltageNode);

            if(clockNode.Nodes.Count > 0)  
                currNode.Nodes.Add(clockNode);

            if(currNode.Nodes.Count > 0)
                parentTreeNode.Nodes.Add(currNode);

        }

        private void BuildSensorNode(LocalSensor currSensor, TreeNode parentTreeNode)
        {
            TreeNode sensorNode = new TreeNode();

            SensorNodeItem currNodeItem;
            currNodeItem.name = currSensor.Name;
            currNodeItem.id = currSensor.Id;

            sensorNode.Tag = currNodeItem;

            sensorNode.Text = currSensor.Name + " : " + HardSenseMemFile.GetValueByKey(currSensor.Id);

            parentTreeNode.Nodes.Add(sensorNode);

        }


        private void SensorMonitor_FormClosed(object sender, FormClosedEventArgs e)
        {
            StopMonitor();
            Properties.Settings.Default.Save();
        }
    }
}
