using System;
using System.Windows.Forms;

namespace HardSense
{
    public partial class HardSense : Form
    {
        private NotifyIcon trayIcon;
        private ContextMenu trayMenu;
        private HardwareMonitor computerMonitor = new HardwareMonitor();

        //private Thread tempThread;
        //private int counter = 0;
        public HardSense()
        {
            InitializeComponent();
            SetupTrayIcon();


            computerMonitor.StartMonitor();


            //tempThread = new Thread(ThreadProc);
            //tempThread.Start();

            LocalHardwareItem mainBoard = computerMonitor.GetMainboard();

            string s = "Mainboard: " + mainBoard.Name;
            tempDisplayBox.AppendText("Starting...");
            tempDisplayBox.AppendText(s);
        }

        /*
        private void ThreadProc()
        {
            while (true)
            {
                tempDisplayBox.Invoke((Action)delegate
                {
                    counter++;
                    String str = "Hello World! - " + counter.ToString() + "\n";
                    tempDisplayBox.AppendText(str);
                });
                Thread.Sleep(250);
            }
        }
        */

        private void SetupTrayIcon()
        {
            trayMenu = new ContextMenu();
            trayMenu.MenuItems.Add("Exit", OnExit);
            trayMenu.MenuItems.Add("Open", trayIcon_DoubleClick);

            trayIcon = new NotifyIcon();
            trayIcon.Text = "HardSense";
            trayIcon.Icon = Properties.Resources.AppIcon;
            trayIcon.DoubleClick += new System.EventHandler(this.trayIcon_DoubleClick);


            trayIcon.ContextMenu = trayMenu;
            trayIcon.Visible = true;
        }

        protected override void OnLoad(EventArgs e)
        {
            Visible = false; // Hide form window.
            //ShowInTaskbar = false; // Remove from taskbar.

            base.OnLoad(e);
        }

        private void trayIcon_DoubleClick(object Sender, EventArgs e)
        {
            // Show the form when the user double clicks on the notify icon.

            // Set the WindowState to normal if the form is minimized.
            if (this.WindowState == FormWindowState.Minimized)
                this.WindowState = FormWindowState.Normal;

            // Activate the form.
            this.Show();
        }

        private void OnExit(object sender, EventArgs e)
        {
            computerMonitor.StopMonitor();
            Application.Exit();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                this.WindowState = FormWindowState.Minimized;
            }
        }
    }
}
