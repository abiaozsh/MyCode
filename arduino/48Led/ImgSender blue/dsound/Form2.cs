using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO.Ports;
using System.Threading;

namespace dsound
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {

            try
            {
                this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

                if (port == null)
                {
                    //COM4为Arduino使用的串口号，需根据实际情况调整
                    port = new SerialPort("COM19", 250000, Parity.None, 8, StopBits.One);
                    port.Open();
                }
            }
            catch
            {
                MessageBox.Show("error");
            }



        }
        void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }

        SerialPort port;
        int swf = 2;
        private void Send(byte[] data, int idx)
        {
            byte[] light = new byte[1];

            if (port != null && port.IsOpen)
            {
                port.Write(data, idx, 48);
                light[0] = 140;
                port.Write(light, 0, 1);
                if (swf == 1)
                {
                    swf = 2;
                }
                else
                {
                    swf = 1;
                }
                light[0] = (byte)swf;
                port.Write(light, 0, 1);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Bitmap p1 = (Bitmap)Bitmap.FromFile(@"..\..\..\..\bitmap.png");

            byte[] data = new byte[1000000];

            for (int i = 0; i < p1.Width; i++)
            {
                for (int j = 0; j < p1.Height; j++)
                {
                    data[i * 48 + j] = p1.GetPixel(i, j).R;
                }
            }

            for (int i = 0; i < p1.Width; i++)
            {
                Send(data, i * 48);
                Thread.Sleep(10);
            }


        }



    }
}
