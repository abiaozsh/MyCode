using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;
using System.Globalization;

namespace WindowsFormsApplication1
{

    public partial class Form1 : Form
    {
        SerialPort port;
        public Form1()
        {
            InitializeComponent();

            this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);
        }

        void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }

        byte[] buff = new byte[32 * 1024 * 1024];
        private void Form1_Load(object sender, EventArgs e)
        {
            FileStream fs = new FileStream("c:\\temp\\a.bin", FileMode.Open, FileAccess.Read);
            fs.Read(buff, 0, 32 * 1024 * 1024);
            fs.Close();

            this.MouseWheel += Form1_MouseWheel;
            pictureBox1.MouseMove += pictureBox1_MouseMove;
            pictureBox1.MouseUp += pictureBox1_MouseUp;
            pictureBox1.MouseDown += pictureBox1_MouseDown;
        }

        int mousex = 0;
        int mousedown = 0;
        void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            mousex = e.X;
            mousedown = 1;
        }

        void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            int diff = end - start;

            int dist = mousex - e.X;

            start += (int)((long)dist * diff / 1024);
            end += (int)((long)dist * diff / 1024);

            draw();
            mousedown = 0;
        }

        void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            //throw new NotImplementedException();
        }

        void Form1_MouseWheel(object sender, MouseEventArgs e)
        {
            int diff = end - start;
            if (e.Delta > 0)
            {
                start = (int)(start + diff * 0.1f);
                end = (int)(end - diff * 0.1f);
            }
            else
            {
                start = (int)(start - diff * 0.1f);
                end = (int)(end + diff * 0.1f);
            }
            draw();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (port == null)
            {
                //COM4为Arduino使用的串口号，需根据实际情况调整  115200
                port = new SerialPort(textBox1.Text, 2000000, Parity.None, 8, StopBits.One);
                port.Open();
                port.DataReceived += port_DataReceived;
            }

        }
        //StringBuilder buffer = new StringBuilder();
        void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            while (true)
            {
                if (port.BytesToRead > 0)
                {
                    //buffer.Append(getHex2(port.ReadByte()));
                }
                else
                {
                    break;
                }
            }
        }

        void portWrite(byte val1, byte val2)
        {
            byte[] buff = new byte[2];
            buff[0] = val1;
            buff[1] = val2;
            if (port != null)
            {
                port.Write(buff, 0, 2);
            }
        }

        int start = 0;
        int end = 32 * 1024 * 1024;
        public void draw()
        {
            Bitmap img = new Bitmap(1024, 256);
            Graphics g = Graphics.FromImage(img);
            SolidBrush b = new SolidBrush(Color.FromArgb(0, 0, 0));
            g.FillRectangle(b, 0, 0, 1024, 256);
            Pen p = new Pen(Color.FromArgb(255, 255, 255));
            int cnt = 0;
            long accu = 0;
            int oldx = -1;
            long oldval = 0;
            for (int i = start; i < end; i += 2)//数组位置
            {
                long temp = (i - start) * 1024L;

                int x = (int)(temp / (end - start));

                if (i > 0 && i < 32 * 1024 * 1024)
                {
                    accu += buff[i & (~1)];
                }

                cnt++;
                if (x != oldx)
                {
                    int val = (int)(accu / cnt);
                    g.DrawLine(p, x, 255 - val, oldx, 255 - oldval);
                    cnt = 0;
                    accu = 0;
                    oldx = x;
                    oldval = val;
                }
            }
            g.Dispose();
            //return i;
            this.pictureBox1.Image = img;

        }
        private void button5_Click(object sender, EventArgs e)
        {
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

    }
}
