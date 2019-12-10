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
        public string com = "COM4";
        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = com;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (port == null)
            {
                //COM4为Arduino使用的串口号，需根据实际情况调整  115200    9600
                port = new SerialPort(textBox1.Text, 115200, Parity.None, 8, StopBits.One);
                port.Open();
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
                port = null;
            }
        }
        void portWrite(byte val1)
        {
            byte[] buff = new byte[1];
            buff[0] = val1;
            if (port != null)
            {
                port.Write(buff, 0, 1);
            }
        }



        private void button3_Click(object sender, EventArgs e)
        {
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        byte[] readFromPort(int count)
        {
            var _sb = new byte[count];
            int idx = 0;
            while (true)
            {
                if (port.BytesToRead > 0)
                {
                    _sb[idx++] = (byte)port.ReadByte();
                    count--;
                    if (count <= 0) return _sb;
                }
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            portWrite(0);

            for (int n = 50; n <= 80; n++)
            {
                byte[] buff = readFromPort(256);
                bool correct = true;
                for (var i = 0; i < 256; i++)
                {
                    if (buff[i] != i)
                    {
                        correct = false;
                    }
                }

                textBox2.Text += "[" + n + (correct ? "T" : "F") + "]";
                this.Text = n.ToString();
                Application.DoEvents();
            }


        }

        private void button2_Click_1(object sender, EventArgs e)
        {
              byte[] buff = new byte[256];

            for (var i = 0; i < 256; i++)
            {
                buff[i] = (byte)i;
            }
            port.Write(buff,0,256);

            byte[] result = readFromPort(1);

            textBox2.Text += result[0];

            byte[] buff2 = readFromPort(256);
            bool correct = true;
            for (var i = 0; i < 256; i++)
            {
                if (buff2[i] != i)
                {
                    correct = false;
                }
            }
            textBox2.Text += (correct ? "T" : "F");

        }


    }
}
