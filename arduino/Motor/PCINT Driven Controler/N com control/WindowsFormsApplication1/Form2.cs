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
                    port = new SerialPort("COM6", 115200, Parity.None, 8, StopBits.One);
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

        private void Send1(int data)
        {
            byte[] d = new byte[1];
            d[0] = (byte)data;
            if (port != null && port.IsOpen)
            {
                port.Write(d, 0, 1);
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        int AL = 0x04;
        int AH = 0x08;
        int BL = 0x10;
        int BH = 0x20;
        int CL = 0x80;
        int CH = 0x40;
        int DL = 0x01;
        int DH = 0x02;

        int A = 0;
        int B = 0;

        void textBox1_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Q)
            {
                A = 0;
            }
            if (e.KeyCode == Keys.W)
            {
                B = 0;
            }
            if (e.KeyCode == Keys.A)
            {
                A = 0;
            }
            if (e.KeyCode == Keys.S)
            {
                B = 0;
            }

            int val = 0;
            if (A == 1)
            {
                val |= AH | BL;
            }
            if (A == -1)
            {
                val |= AL | BH;
            }
            if (B == 1)
            {
                val |= CL | DH;
            }
            if (B == -1)
            {
                val |= CH | DL;
            }
            Send1(val);
        }

        void textBox1_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Q)
            {
                A = 1;
            }
            if (e.KeyCode == Keys.W)
            {
                B = 1;
            }
            if (e.KeyCode == Keys.A)
            {
                A = -1;
            }
            if (e.KeyCode == Keys.S)
            {
                B = -1;
            }

            
            int val = 0;
            if (A == 1)
            {
                val |= AH | BL;
            }
            if (A == -1)
            {
                val |= AL | BH;
            }
            if (B == 1)
            {
                val |= CL | DH;
            }
            if (B == -1)
            {
                val |= CH | DL;
            }
            Send1(val);
        }


    }
}
