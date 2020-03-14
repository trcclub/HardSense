using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using HardSense.HardwareMonitor;
using HardSense.MemFile;

namespace HardSense.GUI.Bluetooth
{
    public partial class LayoutEditor_HomeScreen : Form
    {

        public LayoutEditor_HomeScreen()
        {
            InitializeComponent();

            Build_CPU_Section();
            Build_GPU_Section();
            Build_Network_Section();
        }

        private void Build_CPU_Section()
        {
            if (LocalHardwareMonitor.motherBoardInfo.Count == 0)
            {
                label_CPU_Motherboard_Name.Text = "No motherboards found.";
                return;
            }

            LocalHardwareItem motherBoard = LocalHardwareMonitor.motherBoardInfo[0];
            label_CPU_Motherboard_Name.Text = motherBoard.Name;
            label_CPU_Motherboard_SensorCount.Text = motherBoard.NumberOfSensors.ToString();


            if (LocalHardwareMonitor.cpuInfo.Count == 0)
            {
                label_CPU_CPU_Name.Text = "No CPUs found.";
                return;
            }

            LocalHardwareItem cpu = LocalHardwareMonitor.cpuInfo[0];
            label_CPU_CPU_Name.Text = cpu.Name;
            label_CPU_CPU_SensorCount.Text = cpu.NumberOfSensors.ToString();

            foreach (LocalSensor currSensor in cpu.SensorList)
            {
                comboBox_CPU_Load.Items.Add(currSensor.Id);
                if (currSensor.Id.Contains("load/0"))
                {
                    comboBox_CPU_Load.SelectedItem = currSensor.Id;
                }

                comboBox_CPU_Temp.Items.Add(currSensor.Id);
                if (currSensor.Id.Contains("temperature"))
                {
                    comboBox_CPU_Temp.SelectedItem = currSensor.Id;
                }

                comboBox_CPU_Power.Items.Add(currSensor.Id);
                if (currSensor.Id.Contains("power/0"))
                {
                    comboBox_CPU_Power.SelectedItem = currSensor.Id;
                }

                comboBox_CPU_Clock.Items.Add(currSensor.Id);
                if (currSensor.Id.Contains("clock/1"))
                {
                    comboBox_CPU_Clock.SelectedItem = currSensor.Id;
                }
            }
        }

        private void button_Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void comboBox_CPU_Load_SelectedValueChanged(object sender, EventArgs e)
        {
            label_CPU_Current_Value_Load.Text = HardSenseMemFile.GetValueByKey(comboBox_CPU_Load.Text).ToString();
        }

        private void comboBox_CPU_Temp_SelectedValueChanged(object sender, EventArgs e)
        {
            label_CPU_Current_Value_Temp.Text = HardSenseMemFile.GetValueByKey(comboBox_CPU_Temp.Text).ToString();
        }

        private void comboBox_CPU_Power_SelectedValueChanged(object sender, EventArgs e)
        {
            label_CPU_Current_Value_Power.Text = HardSenseMemFile.GetValueByKey(comboBox_CPU_Power.Text).ToString();
        }

        private void comboBox_CPU_Clock_SelectedValueChanged(object sender, EventArgs e)
        {
            label_CPU_Current_Value_Clock.Text = HardSenseMemFile.GetValueByKey(comboBox_CPU_Clock.Text).ToString();
        }

        private void Build_GPU_Section()
        {
            if (LocalHardwareMonitor.gpuInfo.Count == 0)
            {
                return;
            }

            label_GPU_GPU_Count.Text = LocalHardwareMonitor.gpuInfo.Count.ToString();
            foreach (LocalHardwareItem currGPU in LocalHardwareMonitor.gpuInfo)
            {
                comboBox_GPU_List.Items.Add(currGPU.Id);
                if (currGPU.Id.Contains("/0"))
                {
                    comboBox_GPU_List.SelectedItem = currGPU.Id;
                }

            }
        }

        private void comboBox_GPU_List_SelectedValueChanged(object sender, EventArgs e)
        {
            comboBox_GPU_Load.Items.Clear();
            comboBox_GPU_Load.ResetText();

            comboBox_GPU_Temp.Items.Clear();
            comboBox_GPU_Temp.ResetText();

            comboBox_GPU_Fan.Items.Clear();
            comboBox_GPU_Fan.ResetText();

            comboBox_GPU_Clock.Items.Clear();
            comboBox_GPU_Clock.ResetText();

            foreach (LocalHardwareItem currGPU in LocalHardwareMonitor.gpuInfo)
            {
                if (currGPU.Id.Equals(comboBox_GPU_List.Text))
                {
                    label_GPU_GPU_Sensor_Count.Text = currGPU.NumberOfSensors.ToString();
                    label_GPU_GPU_Name.Text = currGPU.Name;

                    foreach (LocalSensor currSensor in currGPU.SensorList)
                    {
                        comboBox_GPU_Load.Items.Add(currSensor.Id);
                        if (currSensor.Id.Contains("load/0"))
                        {
                            comboBox_GPU_Load.SelectedItem = currSensor.Id;
                        }

                        comboBox_GPU_Temp.Items.Add(currSensor.Id);
                        if (currSensor.Id.Contains("temperature/0"))
                        {
                            comboBox_GPU_Temp.SelectedItem = currSensor.Id;
                        }

                        comboBox_GPU_Fan.Items.Add(currSensor.Id);
                        if (currSensor.Id.Contains("control/0"))
                        {
                            comboBox_GPU_Fan.SelectedItem = currSensor.Id;
                        }

                        comboBox_GPU_Clock.Items.Add(currSensor.Id);
                        if (currSensor.Id.Contains("clock/0"))
                        {
                            comboBox_GPU_Clock.SelectedItem = currSensor.Id;
                        }
                    }

                }


            }

        }

        private void comboBox_GPU_Load_SelectedIndexChanged(object sender, EventArgs e)
        {
            label_GPU_Current_Value_Load.Text = HardSenseMemFile.GetValueByKey(comboBox_GPU_Load.Text).ToString();
        }

        private void comboBox_GPU_Temp_SelectedValueChanged(object sender, EventArgs e)
        {
            label_GPU_Current_Value_Temp.Text = HardSenseMemFile.GetValueByKey(comboBox_GPU_Temp.Text).ToString();
        }

        private void comboBox_GPU_Fan_SelectedValueChanged(object sender, EventArgs e)
        {
            label_GPU_Current_Value_Fan.Text = HardSenseMemFile.GetValueByKey(comboBox_GPU_Fan.Text).ToString();
        }

        private void comboBox_GPU_Clock_SelectedIndexChanged(object sender, EventArgs e)
        {
            label_GPU_Current_Value_Clock.Text = HardSenseMemFile.GetValueByKey(comboBox_GPU_Clock.Text).ToString();
        }

        private void LayoutEditor_HomeScreen_FormClosed(object sender, FormClosedEventArgs e)
        {
            Properties.Settings.Default.Save();
        }

        private void Build_Network_Section()
        {
            if (LocalHardwareMonitor.nicInfo.Count == 0)
            {
                return;
            }

            label_NET_Number_Interfaces.Text = LocalHardwareMonitor.nicInfo.Count.ToString();
            foreach (LocalHardwareItem currNIC in LocalHardwareMonitor.nicInfo)
            {
                comboBox_NET_NET_ID.Items.Add(currNIC.Id);
                if (currNIC.Id.Contains("Ethernet"))
                {
                    comboBox_NET_NET_ID.SelectedItem = currNIC.Id;
                }
            }
        }

        private void comboBox_NET_NET_ID_SelectedValueChanged(object sender, EventArgs e)
        {
            comboBox_NET_Up.Items.Clear();
            comboBox_NET_Up.ResetText();

            comboBox_NET_Down.Items.Clear();
            comboBox_NET_Down.ResetText();

            label_NET_Name.Text = "";
            foreach (LocalHardwareItem currNic in LocalHardwareMonitor.nicInfo)
            {
                if (currNic.Id.Equals(comboBox_NET_NET_ID.Text))
                {
                    label_NET_Number_Interfaces.Text = currNic.NumberOfSensors.ToString();
                    label_NET_Name.Text = currNic.Name;

                    foreach (LocalSensor currSensor in currNic.SensorList)
                    {
                        comboBox_NET_Up.Items.Add(currSensor.Id);
                        if (currSensor.Id.Contains("send"))
                        {
                            comboBox_NET_Up.SelectedItem = currSensor.Id;
                        }

                        comboBox_NET_Down.Items.Add(currSensor.Id);
                        if (currSensor.Id.Contains("recv"))
                        {
                            comboBox_NET_Down.SelectedItem = currSensor.Id;
                        }
                    }
                }
            }


        }
    }
}
