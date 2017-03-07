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


            FileStream fs = new FileStream("d:\\comconf.txt", FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            string com = sr.ReadLine();
            this.Text = com;

            string remark = sr.ReadLine();
            textBox1.Text += remark;

            sr.Close();
            fs.Close();


            //fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
            //sw = new StreamWriter(fs);
            if (port == null)
            {
                try
                {
                    //COM4为Arduino使用的串口号，需根据实际情况调整
                    port = new SerialPort(com, 9600, Parity.None, 8, StopBits.One);
                    port.Open();
                }
                catch
                { }
            }
        }

        void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        byte[] data = new byte[1];

        public void send()
        {
            data[0] &= 0x0F;
            data[0] |= 0xA0;
            if ((data[0] & 0x0F) != 0)
            {
                data[0] |= 16;
            }
            else
            {
                data[0] &= 0xFF ^ 16;
            }
            try
            {
                port.Write(data, 0, 1);
            }
            catch
            {
                this.Text = "error";
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            data[0] |= 1;
            send();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            data[0] |= 2;
            send();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            data[0] |= 4;
            send();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            data[0] |= 8;
            send();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            data[0] &= 0xFF ^ 1;
            send();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            data[0] &= 0xFF ^ 2;
            send();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            data[0] &= 0xFF ^ 4;
            send();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            data[0] &= 0xFF ^ 8;
            send();
        }


    }
}


