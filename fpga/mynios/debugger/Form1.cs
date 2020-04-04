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
using debugger;

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
            this.comboBox2.SelectedIndex = 0;

            foreach (var item in this.Controls)
            {
                (item as Control).Enabled = false;
            }
            this.comboBox1.Enabled = true;
            this.button1.Enabled = true;

            loadSym();
        }
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            portName = (string)this.comboBox1.SelectedItem;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            connect(portName);
            foreach (var item in this.Controls)
            {
                (item as Control).Enabled = true;
            }
        }

        List<CodeSym> syms;

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
            int baseaddr = 0;

            StringBuilder sb = new StringBuilder();
            sb.Append("pc:");
            int pc = getreg(0x43, 0x00, sb);
            byte[] temp;
            sb.Append("  last cmd:");
            portWrite((byte)(0x18), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
            sb.Append("  Rtype:");
            portWrite((byte)(0x19), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));

            int halt_cpu;
            sb.Append("  halt_cpu:");
            portWrite((byte)(0x14), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0])); halt_cpu = temp[0];
            int halt_uart;
            sb.Append("  halt_uart:");
            portWrite((byte)(0x15), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0])); halt_uart = temp[0];
            sb.AppendLine();

            if (halt_cpu != 0 || halt_uart != 0)
            {

                sb.Append("r0:");
                getreg(0x47, 0x00, sb);
                sb.Append(" r1:");
                getreg(0x47, 0x01, sb);

                sb.Append("   r2:");
                getreg(0x47, 0x02, sb);
                sb.Append(" r3:");
                getreg(0x47, 0x03, sb);
                sb.AppendLine();

                for (int i = 4; i < 8; i++)
                {
                    sb.Append(" r" + i + ":");
                    getreg(0x47, (byte)i, sb);
                }
                sb.AppendLine();

                for (int i = 8; i < 16; i++)
                {
                    sb.Append(" r" + i + ":");
                    getreg(0x47, (byte)i, sb);
                }
                sb.AppendLine();

                for (int i = 16; i < 24; i++)
                {
                    sb.Append(" r" + i + ":");
                    getreg(0x47, (byte)i, sb);
                }
                sb.AppendLine();

                sb.Append("sp:");
                int sp = getreg(0x47, 27, sb);

                sb.Append("    fp:");
                getreg(0x47, 28, sb);
                sb.AppendLine();

                sb.Append("cs:");
                int cs = getreg(0x44, 0, sb);
                sb.Append("   ds:");
                int ds = getreg(0x45, 0, sb);
                sb.AppendLine();

                sb.Append(" ra:");
                getreg(0x47, 31, sb);
                sb.AppendLine();


                sb.AppendLine("stack: ");
                baseaddr = sp + ds;
                for (int i = -Convert.ToInt32(this.comboBox2.Text) * 4; i < Convert.ToInt32(this.comboBox2.Text) * 4; i += 4)
                {
                    sb.Append((i == 0 ? "*" : " ") + Util.getHex8((uint)(i + baseaddr)) + ":"); getmem(i + baseaddr, sb); sb.AppendLine();
                }

                baseaddr = 0;
                sb.AppendLine("code: ");
                for (int i = -32; i < 32; i += 4)
                {
                    StringBuilder sb2 = new StringBuilder();
                    int code = getmem(pc + cs + i, sb2);
                    int target = pc + i;
                    string foundsym = Config.getSym(target, syms);
                    string scode = Config.dasm(syms, (uint)code, pc + i, baseaddr);
                    if (i == 0)
                    {
                        sb.AppendLine("----------------------------------------------------------------------------------");
                    }
                    sb.AppendLine((i == 0 ? "*" : " ") + Util.getHex8((uint)(pc + cs + i)) + ":(" + sb2.ToString() + "," + foundsym + "):" + scode);
                    if (i == 0)
                    {
                        sb.AppendLine("----------------------------------------------------------------------------------");
                    }
                }

                sb.AppendLine("mem: ");
                baseaddr = Convert.ToInt32(textBox1.Text, 16);
                for (int i = 0; i < Convert.ToInt32(this.comboBox2.Text) * 4; i += 4)
                {
                    sb.Append("" + Util.getHex8((uint)(i + baseaddr)) + ":"); getmem(i + baseaddr, sb); sb.AppendLine();
                }
            }

            this.textBox4.Text = sb.ToString();

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }
        public int getreg(byte cmd, byte reg, StringBuilder sb)
        {
            portWrite(cmd, reg);
            byte[] temp;
            int val = 0;
            portWrite((byte)(0x13), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 24; sb.Append(Util.getHex2(temp[0]));
            portWrite((byte)(0x12), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 16; sb.Append(Util.getHex2(temp[0]));
            portWrite((byte)(0x11), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 8; sb.Append(Util.getHex2(temp[0]));
            portWrite((byte)(0x10), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 0; sb.Append(Util.getHex2(temp[0]));
            return val;
        }

        public int getmem(int addr, StringBuilder sb)
        {
            portWrite((byte)(0x20), (byte)((addr >> 0) & 0xFF));
            portWrite((byte)(0x21), (byte)((addr >> 8) & 0xFF));
            portWrite((byte)(0x22), (byte)((addr >> 16) & 0xFF));
            portWrite((byte)(0x23), (byte)((addr >> 24) & 0xFF));

            portWrite((byte)(0x30), 0);
            byte[] temp;
            int val = 0;
            portWrite((byte)(0x13), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 24; sb.Append(Util.getHex2(temp[0]));
            portWrite((byte)(0x12), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 16; sb.Append(Util.getHex2(temp[0]));
            portWrite((byte)(0x11), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 8; sb.Append(Util.getHex2(temp[0]));
            portWrite((byte)(0x10), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 0; sb.Append(Util.getHex2(temp[0]));
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
            StringBuilder sb = new StringBuilder();
            getmem(addr, sb);
            textBox2.Text = sb.ToString();

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
                portWrite((byte)(0x01), 1);
            }
            else
            {
                portWrite((byte)(0x01), 0);
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

        private void button7_Click(object sender, EventArgs e)
        {
            portWrite((byte)(0x16), (byte)0x00); var temp = readFromPort(1);
            textBox2.Text += temp[0];
            portWrite((byte)(0x17), (byte)0x00); temp = readFromPort(1);
            textBox2.Text += temp[0];
        }

        private void loadSym()
        {
            syms = new List<CodeSym>();
            FileStream fs = new FileStream("temp.sym", FileMode.Open, FileAccess.Read);
            StreamReader sw = new StreamReader(fs);
            string s = sw.ReadToEnd();
            sw.Close();
            fs.Close();
            foreach (var line in s.Split('\n'))
            {
                if (line != "")
                {
                    CodeSym sym = new CodeSym();
                    sym.pos = int.Parse(line.Split(',')[0]);
                    sym.name = line.Split(',')[1];
                    syms.Add(sym);
                }
            }
        }


        private void button8_Click(object sender, EventArgs e)
        {


            loadSym();

            int baseAddr = 0x02000000;

            portWrite((byte)(0x01), 1);//halt_uart

            {
                FileStream fs = new FileStream("out.hex", FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                String s = sr.ReadToEnd();
                sr.Close();
                fs.Close();

                int index = 0;
                foreach (var item in s.Split('\n'))
                {
                    if (item.Length == ":040016001005003a97".Length + 1)
                    {
                        //:04001600 1005003a 97
                        String data = item.Substring(9, 8);
                        setmem(baseAddr + index, Convert.ToInt32(data, 16));
                        index += 4;
                        this.Text = index.ToString();
                        Application.DoEvents();
                    }
                }
            }


            //      end else if (command == 8'h02) begin debug_reset_n<=data[0]; command_done<=1;

            portWrite((byte)(0x02), 0);
            portWrite((byte)(0x02), 1);
            portWrite((byte)(0x01), 0);//halt_uart
        }


    }
}
