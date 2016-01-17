using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }
        SerialPort port;

        private void Form2_Load(object sender, EventArgs e)
        {
            this.FormClosed += new FormClosedEventHandler(Form2_FormClosed);

            if (port == null)
            {
                try
                {
                    //COM4为Arduino使用的串口号，需根据实际情况调整
                    port = new SerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
                    port.Open();
                }
                catch (Exception ex)
                {
                }
            }
        }
        void Form2_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }

        private void Send1(byte data1, byte data2, byte data3, byte data4)
        {
            byte[] d = new byte[5];
            d[0] = 23;
            d[1] = data1;
            d[2] = data2;
            d[3] = data3;
            d[4] = data4;
            if (port != null && port.IsOpen)
            {
                port.Write(d, 0, 5);
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            this.Text = trackBar1.Value.ToString() + "," + trackBar2.Value.ToString();
            byte data1;
            byte data2;
            if (trackBar1.Value > 0)
            {
                data1 = 1; data2 = (byte)(256 - trackBar1.Value);
            }
            else if (trackBar1.Value < 0)
            {
                data1 = 2; data2 = (byte)(256 + trackBar1.Value);
            }
            else
            {
                data1 = 0; data2 = (byte)(128);
            }

            byte data3;
            byte data4;
            if (trackBar2.Value > 0)
            {
                data3 = 3; data4 = (byte)(256 - trackBar2.Value);
            }
            else if (trackBar2.Value < 0)
            {
                data3 = 4; data4 = (byte)(256 + trackBar2.Value);
            }
            else
            {
                data3 = 0; data4 = (byte)(128);
            }

            Send1(data1, data2, data3, data4);
        }

    }
}
