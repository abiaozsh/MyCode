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

    public sealed partial class Form1 : Base
    {
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
            connect(portName);
        }



        private void button2_Click(object sender, EventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
                port = null;
            }
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
            StringBuilder sb = new StringBuilder();
            int cnt = 256;

            int baseaddr = 0;

            byte[] data = new byte[cnt];
            Random r = new Random();
            r.NextBytes(data);
            for (int i = 0; i < cnt; i += 4)
            {
                writeDWord(baseaddr + i, data, i);
                if ((i & 15) == 0)
                {
                    this.Text = "" + i;
                    Application.DoEvents();
                }
            }

            bool err = false;
            for (int i = 0; i < cnt; i++)
            {
                uint b = readByte(baseaddr + i);
                if (b != data[i])
                {
                    err = true;
                }
                if ((i & 63) == 0)
                {
                    this.Text = "check " + i;
                    Application.DoEvents();
                }
            }


            byte[] d1 = read8Byte(baseaddr);

        }

        private void button12_Click(object sender, EventArgs e)
        {
            bool r1;
            bool r2;
            bool r3;
            bool r4;
            Random r = new Random();
            {
                byte[] data1 = new byte[1];
                byte[] data2 = new byte[1];
                byte[] data3 = new byte[1];
                byte[] data4 = new byte[1];
                r.NextBytes(data1);
                r.NextBytes(data2);
                r.NextBytes(data3);
                r.NextBytes(data4);

                writeByte(0x100, data1[0]);
                writeByte(0x101, data2[0]);
                writeByte(0x102, data3[0]);
                writeByte(0x103, data4[0]);

                byte[] d1 = read8Byte(0x100);
                byte[] d2 = read8Byte(0x100);
                r1 = compare(data1, d1);
                r2 = compare(data2, d2);
            }
            {
                byte[] data1 = new byte[8];
                byte[] data2 = new byte[8];
                r.NextBytes(data1);
                r.NextBytes(data2);

                write8Byte(0x100, data1);
                write8Byte(0x108, data2);

                byte[] d1 = read8Byte(0x100);
                byte[] d2 = read8Byte(0x108);
                r1 = compare(data1, d1);
                r2 = compare(data2, d2);
            }
            {
                byte[] data1 = new byte[16];
                byte[] data2 = new byte[16];
                r.NextBytes(data1);
                r.NextBytes(data2);

                write16Byte(0x100, data1);
                write16Byte(0x110, data2);

                byte[] d1 = read16Byte(0x100);
                byte[] d2 = read16Byte(0x110);
                r3 = compare(data1, d1);
                r4 = compare(data2, d2);
            }
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
