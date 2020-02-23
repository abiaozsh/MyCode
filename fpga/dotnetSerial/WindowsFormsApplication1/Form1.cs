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



        private void button3_Click(object sender, EventArgs e)
        {
            //test uart
            Random r = new Random();
            bool err = false;
            int count = 0;
            for (int i = 0; i < 100000; i++)
            {
                byte[] buff = new byte[1];
                r.NextBytes(buff);
                port.Write(buff, 0, 1);

                byte[] buff2 = readFromPort(1);

                if (buff[0] != buff2[0])
                {
                    err = true;
                    count = i;
                    break;
                }
            }
            this.Text = "error:" + count + "," + err.ToString();
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }


        private byte[] longread(int addr)
        {
            try
            {
                int addr0 = (addr) & 0xFF;
                int addr1 = (addr >> 8) & 0xFF;
                int addr2 = (addr >> 16) & 0xFF;

                //if (item.StartsWith("pr"))//put reg
                portWrite((byte)(0x40 + 2), (byte)addr0);
                portWrite((byte)(0x40 + 3), (byte)addr1);
                portWrite((byte)(0x40 + 4), (byte)addr2);

                portWrite((byte)(0xA3));

                byte[] buff = new byte[512 * 2];
                readFromPort(1024, buff, 0);
                byte[] buff2 = readFromPort(1);
                byte check = buff2[0];
                byte b = 0;
                for (int i = 0; i < 1024; i++)
                {
                    b += buff[i];
                }
                if (b != check)
                {
                    throw new Exception();
                }
                return buff;
            }
            catch
            {
                return null;
            }
        }


        private void initA(int offset)
        {
            byte[] buff = new byte[512];
            for (int i = 0; i < 256; i++)
            {
                buff[i] = (byte)i;
            }
            for (int i = 256; i < 512; i++)
            {
                buff[i] = (byte)(511 - i);
            }
            writeFast(buff, offset);
        }
        private void initB(int offset)//256 * n
        {
            byte[] buff = new byte[512];
            for (int i = 0; i < 256; i++)
            {
                buff[i] = (byte)0xFF;
            }
            for (int i = 256; i < 512; i++)
            {
                buff[i] = (byte)0xFF;
            }
            writeFast(buff, offset);
        }

        private void writeFast(byte[] buff, int offset)
        {
            int addr = offset;
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            MemoryStream s = new MemoryStream();
            portWrite((byte)(0x40 + 2), (byte)addr0, s);
            portWrite((byte)(0x40 + 3), (byte)addr1, s);
            portWrite((byte)(0x40 + 4), (byte)addr2, s);
            portWrite((byte)(0xA2), s);
            s.Flush();
            byte[] buff2 = s.ToArray();
            sendall(buff2);

            s = new MemoryStream();
            for (int i = 0; i < 256; i++)
            {
                portWrite((byte)buff[i * 2 + 0], s);
                portWrite((byte)buff[i * 2 + 1], s);
            }
            portWrite(0, 0, s);//end
            s.Flush();
            buff2 = s.ToArray();
            sendall(buff2);

            byte[] data = readFromPort(1);
            var aa = data[0];
        }


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
                    Thread.Sleep(100);
                }
                else if (item == "#br")
                {
                    Thread.Sleep(10); //buffer.AppendLine();
                }
                else if (item == "ch") //10：out_clk on
                {
                    portWrite((byte)0x10);
                }
                else if (item == "cl")//11：out_clk off
                {
                    portWrite((byte)0x11);
                }
                else if (item == "rh")//12：out_rst on
                {
                    portWrite((byte)0x12);
                }
                else if (item == "rl")//13：out_rst off
                {
                    portWrite((byte)0x13);
                }
                else if (item.StartsWith("in"))//in
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    portWrite((byte)(0x20 + reg));
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
                    portWrite((byte)(0x50 + reg));
                    this.textBox3.Text += this.getHex2(readFromPort(1)[0]);
                }
                else if (item.StartsWith("sc"))//special command
                {
                    var val = System.Convert.ToInt32(item.Substring(2, 2).Trim(), 16);
                    portWrite((byte)(val));
                }

            }
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            //buffer.Clear();
            textBox3.Text = "";
        }

        private void button6_Click(object sender, EventArgs e)
        {
            //getdata(0x29);
            //getdata(0x30);
            //getdata(0x30/4);
            //getdata(0x30*4);
            //getdata(0x10);
            //getdata(0x20);

            getdata(9);
            getdata(10);
            getdata(11);
            getdata(12);
            getdata(13);
            getdata(14);
            this.textBox4.Text += "\r\n";
        }

        private void getdata(int addr)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)0x00);

            portWrite((byte)(0x30), (byte)0x00);

            portWrite((byte)(0x20), (byte)0x00);
            this.textBox4.Text += getHex2(readFromPort(1)[0]);
            portWrite((byte)(0x21), (byte)0x00);
            this.textBox4.Text += getHex2(readFromPort(1)[0]);
            portWrite((byte)(0x22), (byte)0x00);
            this.textBox4.Text += getHex2(readFromPort(1)[0]);
            portWrite((byte)(0x23), (byte)0x00);
            this.textBox4.Text += getHex2(readFromPort(1)[0]);
            this.textBox4.Text += " ";
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
            portWrite((byte)(0x13), (byte)0x00);

            int data0 = (data) & 0xFF;
            int data1 = (data >> 8) & 0xFF;
            int data2 = (data >> 16) & 0xFF;
            int data3 = (data >> 24) & 0xFF;
            portWrite((byte)(0x14), (byte)data0);
            portWrite((byte)(0x15), (byte)data1);
            portWrite((byte)(0x16), (byte)data2);
            portWrite((byte)(0x17), (byte)data3);

            portWrite((byte)(0x31), (byte)0x00);
        }
        private void button12_Click(object sender, EventArgs e)
        {
            writedata(10, 0x00300678);
            writedata(11, 0x11311678);
            writedata(12, 0x22322678);
            writedata(13, 0x33333678);
            writedata(14, 0x44344678);
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (port != null)
            {
                StringBuilder sb = new StringBuilder();
                while (port.BytesToRead > 0)
                {
                    sb.Append(getHex2(port.ReadByte()) + " ");
                }
                textBox3.Text += sb.ToString();
            }
        }

        private string receivePage(byte[] buff)
        {
            //byte[] buff = new byte[512 * 2];
            //readFromPort(1024, buff, 0);

            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < 32; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    sb.Append(getHex2(buff[(i * 16 + j) * 2 + 0]) + getHex2(buff[(i * 16 + j) * 2 + 1]) + " ");
                }
                sb.AppendLine();
            }
            sb.AppendLine();
            return sb.ToString();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            textBox3.Text = "";
            StringBuilder sb = new StringBuilder();
            //max 24bit
            // 0x1000000
            for (int addr = 0; addr < 0x10000; addr += 0x0200)
            {
                byte[] buff = null;
                while (buff == null)
                {
                    buff = longread(addr);
                }
                sb.Append(receivePage(buff));
                this.Text = (addr / 512).ToString();
                Application.DoEvents();
            }
            textBox3.Text += sb.ToString();

        }

        private void button9_Click(object sender, EventArgs e)
        {
            initB(0x0000);
            initB(0x0100);
            initB(0x0200);
            initB(0x0300);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            textBox4.Text = "scA4\r\n";
            button4_Click(null, null);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            initA(0x0000);
            initA(0x0100);
            initA(0x0200);
            initA(0x0300);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            FileStream fs = new FileStream("c:\\temp\\a.bin", FileMode.Create, FileAccess.Write);
            int count = 0;
            int prog = 0;
            for (int addr = 0; addr < 0x100000; addr += 0x0200)
            {
                byte[] buff = null;
                while (buff == null)
                {
                    buff = longread(addr);
                }
                fs.Write(buff, 0, 1024);
                fs.Flush();
                if (count == 16)
                {
                    count = 0;
                    this.Text = prog.ToString();
                }
                count++;
                prog++;
            }
            fs.Flush();
            fs.Close();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                portWrite((byte)(0x40), (byte)0x00);
            }
            else
            {
                portWrite((byte)(0x41), (byte)0x00);
            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked)
            {
                portWrite((byte)(0x50), (byte)0x00);
            }
            else
            {
                portWrite((byte)(0x51), (byte)0x00);
            }
        }


    }
}
