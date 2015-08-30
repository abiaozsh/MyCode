using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;

namespace WindowsFormsApplication1
{

    public partial class Form1 : Form
    {
        SerialPort port;
        FileStream fs;
        StreamWriter sw;
        private delegate void FlushClient(string s);
        public Form1()
        {
            InitializeComponent();

            this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

            //fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
            //sw = new StreamWriter(fs);
            if (port == null)
            {
                //COM4为Arduino使用的串口号，需根据实际情况调整
                port = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);
                port.Open();
            }
        }

        void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }

        private void PortWrite(int chn, int data)
        {
            byte[] d = new byte[2];
            d[0] = (byte)(0x050 | chn);
            d[1] = (byte)(data & 0x0FF);
            if (port != null && port.IsOpen)
            {
                Thread.Sleep(10);
                port.Write(d, 1, 1);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
        }
        void trackBar1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            int v = trackBar1.Value;
            port.Write(v + "," + (checkBox1.Checked ? 1 : 0) + ",\r\n");
            this.Text = v.ToString();
            Thread.Sleep(30);
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            double v = trackBar2.Value;
            this.Text = v.ToString();
            PortWrite(2, (int)(v));

        }

        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            double v = trackBar3.Value;
            this.Text = v.ToString();
            PortWrite(3, (int)(v));

        }

        private void trackBar4_Scroll(object sender, EventArgs e)
        {
            double v = trackBar4.Value;
            this.Text = v.ToString();
            PortWrite(4, (int)(v));

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }


    }
}


