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
                port = new SerialPort(textBox1.Text, 9600, Parity.None, 8, StopBits.One);
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


        string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
        private string getHex4(int val)//"ll"+"hh"
        {
            return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))] + convt[((val & 0xF000) >> 12)] + convt[((val & 0x0F00) >> 8)];
        }
        private string getHex2(int val)//"ll"+"hh"
        {
            return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
        }



        private void button3_Click(object sender, EventArgs e)
        {
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        void portWrite(string val)
        {
            if (port != null)
            {
                port.Write(val);
            }
        }

        string readFromPort(int count)
        {
            if (count <= 0) return "";
            StringBuilder _sb = new StringBuilder();
            while (true)
            {
                if (port.BytesToRead > 0)
                {
                    _sb.Append((char)port.ReadChar());
                    count--;
                    if (count <= 0) return _sb.ToString();
                }
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            portWrite("st");//st Start
            checkOK();
            portWrite("si");//st Start
            string sret = readFromPort(6);
            textBox3.Text = sret;
            portWrite("ed");//st Start
            checkOK();
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            portWrite("st");//st Start
            checkOK();
            portWrite("ru");//st Start
            string sret = readFromPort(6);
            textBox3.Text = sret;
            portWrite("ed");//st Start
            checkOK();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            portWrite("st");//st Start
            checkOK();
            portWrite("wu");//st Start
            checkOK();
            portWrite("ed");//st Start
            checkOK();
        }

        void checkOK()
        {
            if (readFromPort(2) != "OK")
            {
                Console.WriteLine("error!!");
            }
        }

    }
}
