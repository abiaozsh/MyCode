﻿using System;
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

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (port == null)
            {
                //COM4为Arduino使用的串口号，需根据实际情况调整  115200
                port = new SerialPort(textBox1.Text, 2000000, Parity.None, 8, StopBits.One);
                port.Open();
                //port.DataReceived += port_DataReceived;
            }

        }
        byte[] readFromPort(int count)
        {
            if (count <= 0) return null;
            byte[] buff = new byte[count];
            int idx = 0;
            while (true)
            {
                if (port.BytesToRead > 0)
                {
                    buff[idx] = (byte)port.ReadByte();
                    idx++;
                    count--;
                    if (count <= 0) return buff;
                }
            }
        }
        void readFromPort(int count, byte[] buff, int idx)
        {
            if (count <= 0) return;
            int pos = 0;
            while (true)
            {
                if (port.BytesToRead > 0)
                {
                    buff[idx + pos] = (byte)port.ReadByte();
                    pos++;
                    count--;
                    if (count <= 0) return;
                }
            }
        }

        StringBuilder buffer = new StringBuilder();
        //void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        //{
        //	while (true)
        //	{
        //		if (port.BytesToRead > 0)
        //		{
        //			buffer.Append(getHex2(port.ReadByte()));
        //		}
        //		else
        //		{
        //			break;
        //		}
        //	}
        //}

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

        void portWrite(byte val1, byte val2, Stream s)
        {
            s.WriteByte(val1);
            s.WriteByte(val2);

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



        private void button3_Click(object sender, EventArgs e)
        {
            port.Write(textBox2.Text);
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }


        private bool test()
        {
            int size = 256 * 2;//words
            byte[] buff = new byte[size * 2];
            Random rnd = new Random();
            rnd.NextBytes(buff);

            writeall(buff);

            byte[] buff2 = readall(size);

            bool err = false;
            int idx = 0;
            for (int i = 0; i < size; i++)
            {
                if (buff[i * 2 + 0] != buff2[i * 2 + 0])
                {
                    err = true;
                    idx = i;
                    break;
                }
                if (buff[i * 2 + 1] != buff2[i * 2 + 1])
                {
                    err = true;
                    idx = i;
                    break;
                }
            }
            return err;
        }
        private byte[] readall(int size)
        {
            //int size = 65536 * 2;//words
            byte[] buff = new byte[size * 2];
            //pr200//addr
            //pr300//addr
            //pr400//addr
            //scA1//sdram read
            //gr0//data
            //gr1//data
            //gr7//timer
            //#br
            MemoryStream s = new MemoryStream();
            for (int i = 0; i < size; i++)
            {
                int addr0 = (i) & 0xFF;
                int addr1 = (i >> 8) & 0xFF;
                int addr2 = (i >> 16) & 0xFF;

                //if (item.StartsWith("pr"))//put reg
                portWrite((byte)(0x40 + 2), (byte)addr0, s);
                portWrite((byte)(0x40 + 3), (byte)addr1, s);
                portWrite((byte)(0x40 + 4), (byte)addr2, s);

                //if (item.StartsWith("sc"))//special command
                portWrite((byte)(0xA1), 0, s);


                //if (item.StartsWith("gr"))//get reg
                portWrite((byte)(0x50 + 0), 0, s);//low first
                portWrite((byte)(0x50 + 1), 0, s);

            }
            s.Flush();
            sendall(s.GetBuffer());

            readFromPort(size * 2, buff, 0);
            return buff;
        }
        private void writeall(byte[] buff)
        {
            //pr080//data
            //pr144//data
            //pr200//addr
            //pr300//addr
            //pr400//addr
            //scA0//sdram write
            //gr7//timer
            //#br
            int size = buff.Length / 2;//65536 * 2;//words
            //byte[] buff = new byte[size * 2];

            MemoryStream s = new MemoryStream();
            for (int i = 0; i < size; i++)
            {
                int addr0 = (i) & 0xFF;
                int addr1 = (i >> 8) & 0xFF;
                int addr2 = (i >> 16) & 0xFF;
                //if (item.StartsWith("pr"))//put reg
                portWrite((byte)(0x40 + 2), (byte)addr0, s);
                portWrite((byte)(0x40 + 3), (byte)addr1, s);
                portWrite((byte)(0x40 + 4), (byte)addr2, s);

                portWrite((byte)(0x40 + 0), (byte)buff[i * 2 + 0], s);
                portWrite((byte)(0x40 + 1), (byte)buff[i * 2 + 1], s);

                //if (item.StartsWith("sc"))//special command
                portWrite((byte)(0xA0), 0, s);

            }
            s.Flush();
            byte[] buff2 = s.ToArray();
            sendall(buff2);
        }

        /*


        scA2//long write



        */

        private void button4_Click(object sender, EventArgs e)
        {
            var list = textBox4.Text.Split('\n');

            foreach (var temp in list)
            {
                var item = temp.Trim();
                var idx = item.IndexOf("//");
                if (idx >= 0)
                {
                    item = item.Substring(0, idx);
                }

                if (item == "#dly")
                {
                    Thread.Sleep(10);
                }
                else if (item == "#br")
                {
                    Thread.Sleep(10); buffer.AppendLine();
                }
                else if (item == "ch") //10：out_clk on
                {
                    portWrite((byte)0x10, 0);
                }
                else if (item == "cl")//11：out_clk off
                {
                    portWrite((byte)0x11, 0);
                }
                else if (item == "rh")//12：out_rst on
                {
                    portWrite((byte)0x12, 0);
                }
                else if (item == "rl")//13：out_rst off
                {
                    portWrite((byte)0x13, 0);
                }
                else if (item.StartsWith("in"))//in
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    portWrite((byte)(0x20 + reg), 0);
                    this.textBox3.Text += this.getHex2(readFromPort(1)[0]);
                }
                else if (item.StartsWith("ou"))//out
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    int val = System.Convert.ToInt32(item.Substring(3).Trim(), 16);
                    portWrite((byte)(0x30 + reg), (byte)val);
                }
                else if (item.StartsWith("pr"))//put reg
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    int val = System.Convert.ToInt32(item.Substring(3).Trim(), 16);
                    portWrite((byte)(0x40 + reg), (byte)val);
                }
                else if (item.StartsWith("gr"))//get reg
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    portWrite((byte)(0x50 + reg), 0);
                    this.textBox3.Text += this.getHex2(readFromPort(1)[0]);
                }
                else if (item.StartsWith("sc"))//special command
                {
                    var val = System.Convert.ToInt32(item.Substring(2, 2).Trim(), 16);
                    portWrite((byte)(val), 0);
                }


            }
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            buffer.Clear();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            test();
        }

    }
}
