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
        string portName;
        private void Form1_Load(object sender, EventArgs e)
        {
            String[] list = SerialPort.GetPortNames();
            foreach (String s in list)
            {
                this.comboBox1.Items.Add(s);
            }
        }
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            portName = (string)this.comboBox1.SelectedItem;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (port == null)
            {
                //COM4为Arduino使用的串口号，需根据实际情况调整  115200
                port = new SerialPort(portName, 2000000, Parity.None, 8, StopBits.One);
                port.Open();
            }

        }

        byte[] readFromPort(int count)
        {
            if (count <= 0) return null;
            byte[] buff = new byte[count];
            int idx = 0;
            long t = DateTime.Now.Ticks;
            while (true)
            {
                if (port.BytesToRead > 0)
                {
                    buff[idx] = (byte)port.ReadByte();
                    idx++;
                    count--;
                    if (count <= 0) return buff;
                }
                var diff = DateTime.Now.Ticks - t;
                if (diff > 250000)
                {
                    return buff;
                    //throw new Exception();
                }
            }
        }
        bool readFromPort(int count, byte[] buff, int idx)
        {
            if (count <= 0) return false;
            if (port == null) return false;
            int pos = 0;
            long t = DateTime.Now.Ticks;
            while (true)
            {
                int num = port.BytesToRead;
                if (num > 0)
                {
                    if (num > count)
                    {
                        num = count;
                        count = 0;
                    }
                    else
                    {
                        count -= num;
                    }
                    port.Read(buff, idx + pos, num);
                    pos += num;
                    if (count <= 0) return true;
                }
                var diff = DateTime.Now.Ticks - t;
                if (diff > 250000)
                {
                    throw new Exception();
                }
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
        void portWrite(byte val1)
        {
            byte[] buff = new byte[1];
            buff[0] = val1;
            if (port != null)
            {
                port.Write(buff, 0, 1);
            }
        }

        void portWrite(byte val1, byte val2, Stream s)
        {
            s.WriteByte(val1);
            s.WriteByte(val2);

        }
        void portWrite(byte val1, Stream s)
        {
            s.WriteByte(val1);
        }

        void sendall(byte[] buff)
        {
            port.Write(buff, 0, buff.Length);
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
        private string getHex(byte[] buff, int idx, int len)//"ll"+"hh"
        {
            StringBuilder sb = new StringBuilder();
            for (int i = idx; i < idx + len; i++)
            {
                byte val = buff[i];
                sb.Append(convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))]);
            }
            return sb.ToString();
        }


        private string getdata(int addr)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);

            portWrite((byte)(0x30), (byte)0x00);

            string data = "";
            portWrite((byte)(0x20), (byte)0x00);
            data += getHex2(readFromPort(1)[0]);
            portWrite((byte)(0x21), (byte)0x00);
            data += getHex2(readFromPort(1)[0]);
            return data;
        }
        private void writedata(int addr, int data)
        {
            //portWrite((byte)(0x10), (byte)(0x30/4));
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);

            int data0 = (data) & 0xFF;
            int data1 = (data >> 8) & 0xFF;
            portWrite((byte)(0x14), (byte)data0);
            portWrite((byte)(0x15), (byte)data1);

            portWrite((byte)(0x31), (byte)0x00);
        }


        private void button3_Click(object sender, EventArgs e)
        {

            writedata(22, 0x2232);
            writedata(23, 0x3333);
            writedata(24, 0x4434);
            writedata(25, 0x5535);
            writedata(26, 0x6636);

        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            string temp = "";
            temp += getdata(21) + " ";
            temp += getdata(22) + " ";
            temp += getdata(23) + " ";
            temp += getdata(24) + " ";
            temp += getdata(25) + " ";
            temp += getdata(26) + " ";
            this.textBox1.Text = temp;
        }


    }
}
