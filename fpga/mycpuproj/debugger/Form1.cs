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



        private void button4_Click(object sender, EventArgs e)
        {
            getstatus();
        }
        public void getstatus()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("eax:");
            portWrite((byte)(0xA3), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA2), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA1), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA0), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("ebx:");
            portWrite((byte)(0xA7), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA6), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA5), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA4), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("ecx:");
            portWrite((byte)(0xAB), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xAA), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA9), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xA8), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("edx:");
            portWrite((byte)(0xAF), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xAE), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xAD), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xAC), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("ebp:");
            portWrite((byte)(0xB3), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xB2), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xB1), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xB0), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("esp:");
            byte[] temp;
            int esp = 0;
            portWrite((byte)(0xB7), (byte)0x00); temp = readFromPort(1); esp |= temp[0] << 24; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xB6), (byte)0x00); temp = readFromPort(1); esp |= temp[0] << 16; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xB5), (byte)0x00); temp = readFromPort(1); esp |= temp[0] << 8; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xB4), (byte)0x00); temp = readFromPort(1); esp |= temp[0] << 0; sb.Append(getHex2(temp[0]));
            sb.AppendLine();

            sb.Append("esi:");
            portWrite((byte)(0xBB), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xBA), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xB9), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xB8), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("edi:");
            portWrite((byte)(0xBF), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xBE), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xBD), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xBC), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("ra:");
            portWrite((byte)(0xC3), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xC2), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xC1), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xC0), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();

            sb.Append("rb:");
            portWrite((byte)(0xC7), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xC6), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xC5), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            portWrite((byte)(0xC4), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
            sb.AppendLine();


            sb.Append("cs:");
            int cs = 0;
            portWrite((byte)(0xDB), (byte)0x00); temp = readFromPort(1); cs |= temp[0] << 24; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xDA), (byte)0x00); temp = readFromPort(1); cs |= temp[0] << 16; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xD9), (byte)0x00); temp = readFromPort(1); cs |= temp[0] << 8; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xD8), (byte)0x00); temp = readFromPort(1); cs |= temp[0] << 0; sb.Append(getHex2(temp[0]));
            sb.AppendLine();

            sb.Append("ds:");
            int ds = 0;
            portWrite((byte)(0xDF), (byte)0x00); temp = readFromPort(1); ds |= temp[0] << 24; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xDE), (byte)0x00); temp = readFromPort(1); ds |= temp[0] << 16; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xDD), (byte)0x00); temp = readFromPort(1); ds |= temp[0] << 8; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xDC), (byte)0x00); temp = readFromPort(1); ds |= temp[0] << 0; sb.Append(getHex2(temp[0]));
            sb.AppendLine();


            sb.Append("status: ");
            //...
            byte statusbyte0;
            portWrite((byte)(0xE0), (byte)0x00); statusbyte0 = (readFromPort(1)[0]);

            sb.Append(((statusbyte0 & 0x08) != 0) ? "overflow," : "no overflow,");
            sb.Append(((statusbyte0 & 0x04) != 0) ? "sign neg," : "sign pos,");
            sb.Append(((statusbyte0 & 0x02) != 0) ? "carry," : "no carry,");
            sb.Append(((statusbyte0 & 0x01) != 0) ? "zero," : "no zero,");
            sb.AppendLine();

            sb.Append("pc:");
            int pc = 0;
            portWrite((byte)(0xE7), (byte)0x00); temp = readFromPort(1); pc |= temp[0] << 24; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xE6), (byte)0x00); temp = readFromPort(1); pc |= temp[0] << 16; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xE5), (byte)0x00); temp = readFromPort(1); pc |= temp[0] << 8; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xE4), (byte)0x00); temp = readFromPort(1); pc |= temp[0] << 0; sb.Append(getHex2(temp[0]));
            sb.AppendLine();

            sb.AppendLine("stack: ");
            string s;
            int idx = 0;
            getmem(esp + ds + idx, out s); sb.AppendLine(" " + idx + ":" + s); idx += 4;
            getmem(esp + ds + idx, out s); sb.AppendLine(" " + idx + ":" + s); idx += 4;
            getmem(esp + ds + idx, out s); sb.AppendLine(" " + idx + ":" + s); idx += 4;
            getmem(esp + ds + idx, out s); sb.AppendLine("" + idx + ":" + s); idx += 4;
            getmem(esp + ds + idx, out s); sb.AppendLine("" + idx + ":" + s); idx += 4;
            getmem(esp + ds + idx, out s); sb.AppendLine("" + idx + ":" + s); idx += 4;
            getmem(esp + ds + idx, out s); sb.AppendLine("" + idx + ":" + s); idx += 4;
            getmem(esp + ds + idx, out s); sb.AppendLine("" + idx + ":" + s); idx += 4;

            sb.AppendLine("code: ");
            idx = 0;
            for (int i = 0; i < 16; i++)
            {
                getmem(pc + cs + idx, out s); sb.AppendLine(" " + idx + ":" + s); idx += 4;
            }
            sb.Append("cmd reg reg:");
            portWrite((byte)(0xF0), (byte)0x00); temp = readFromPort(1); pc |= temp[0] << 24; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xF1), (byte)0x00); temp = readFromPort(1); pc |= temp[0] << 16; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0xF2), (byte)0x00); temp = readFromPort(1); pc |= temp[0] << 8; sb.Append(getHex2(temp[0]));
            sb.AppendLine();

            sb.Append("exec_address:");
            int exec_address = 0;
            portWrite((byte)(0x53), (byte)0x00); temp = readFromPort(1); exec_address |= temp[0] << 24; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0x52), (byte)0x00); temp = readFromPort(1); exec_address |= temp[0] << 16; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0x51), (byte)0x00); temp = readFromPort(1); exec_address |= temp[0] << 8; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0x50), (byte)0x00); temp = readFromPort(1); exec_address |= temp[0] << 0; sb.Append(getHex2(temp[0]));
            sb.AppendLine();

            sb.Append("exec_writedata:");
            int exec_writedata = 0;
            portWrite((byte)(0x57), (byte)0x00); temp = readFromPort(1); exec_writedata |= temp[0] << 24; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0x56), (byte)0x00); temp = readFromPort(1); exec_writedata |= temp[0] << 16; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0x55), (byte)0x00); temp = readFromPort(1); exec_writedata |= temp[0] << 8; sb.Append(getHex2(temp[0]));
            portWrite((byte)(0x54), (byte)0x00); temp = readFromPort(1); exec_writedata |= temp[0] << 0; sb.Append(getHex2(temp[0]));
            sb.AppendLine();

            this.textBox4.Text = sb.ToString();

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        public int getmem(int addr, out string s)
        {
            portWrite((byte)(0x20), (byte)((addr >> 0) & 0xFF));
            portWrite((byte)(0x21), (byte)((addr >> 8) & 0xFF));
            portWrite((byte)(0x22), (byte)((addr >> 16) & 0xFF));
            portWrite((byte)(0x23), (byte)((addr >> 24) & 0xFF));

            portWrite((byte)(0x30), 0);
            byte[] temp;
            s = "";
            int val = 0;
            portWrite((byte)(0x13), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 24; s += (getHex2(temp[0]));
            portWrite((byte)(0x12), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 16; s += (getHex2(temp[0]));
            portWrite((byte)(0x11), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 8; s += (getHex2(temp[0]));
            portWrite((byte)(0x10), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 0; s += (getHex2(temp[0]));
            return val;
        }
        public void setmem(int addr, int data)
        {
            portWrite((byte)(0x20), (byte)((addr >> 0) & 0xFF));
            portWrite((byte)(0x21), (byte)((addr >> 8) & 0xFF));
            portWrite((byte)(0x22), (byte)((addr >> 16) & 0xFF));
            portWrite((byte)(0x23), (byte)((addr >> 24) & 0xFF));

            portWrite((byte)(0x24), (byte)((data >> 0) & 0xFF));
            portWrite((byte)(0x25), (byte)((data >> 8) & 0xFF));
            portWrite((byte)(0x26), (byte)((data >> 16) & 0xFF));
            portWrite((byte)(0x27), (byte)((data >> 24) & 0xFF));

            portWrite((byte)(0x31), 0);
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            int addr = Convert.ToInt32(textBox1.Text, 16);
            string s;
            getmem(addr, out s);
            textBox2.Text = s;

        }
        private void button6_Click(object sender, EventArgs e)
        {
            int addr = Convert.ToInt32(textBox1.Text, 16);
            setmem(addr, Convert.ToInt32(textBox2.Text, 16));
        }


        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                portWrite((byte)(0x01), 0);
            }
            else
            {
                portWrite((byte)(0x02), 0);
            }

        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            portWrite((byte)(0x03), 0);
            getstatus();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            readFromPort(10);
        }


    }
}
