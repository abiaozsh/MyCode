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
			cfgs = Config.loadConfig(@"assembler\config.txt");


		}
		private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			portName = (string)this.comboBox1.SelectedItem;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			connect(portName);
		}

		List<CodeSym> syms;
		List<Config> cfgs;

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
			getreg(0x47, 0x00, sb);

			sb.Append("  ebx:");
			getreg(0x47, 0x01, sb);

			sb.Append("  ecx:");
			getreg(0x47, 0x02, sb);

			sb.Append("  edx:");
			getreg(0x47, 0x03, sb);

			sb.Append("  ebp:");
			getreg(0x47, 0x04, sb);

			sb.Append("  esp:");
			byte[] temp;
			int esp = getreg(0x47, 0x05, sb);

			sb.Append("  esi:");
			getreg(0x47, 0x06, sb);

			sb.Append("  edi:");
			getreg(0x47, 0x07, sb);
			sb.AppendLine();

			sb.Append(" ra:");
			getreg(0x47, 0x08, sb);

			sb.Append("   rb:");
			getreg(0x47, 0x09, sb);

			sb.Append("   cs:");
			int cs = getreg(0x47, 0x0E, sb);

			sb.Append("   ds:");
			int ds = getreg(0x47, 0x0F, sb);

			sb.AppendLine();


			sb.Append("status: ");
			//...
			int statusbyte0 = getreg(0x42, 0x00, sb);
			sb.AppendLine();
			sb.Append(((statusbyte0 & 0x08) != 0) ? "overflow," : "no overflow,");
			sb.Append(((statusbyte0 & 0x04) != 0) ? "sign neg," : "sign pos,");
			sb.Append(((statusbyte0 & 0x02) != 0) ? "carry," : "no carry,");
			sb.Append(((statusbyte0 & 0x01) != 0) ? "zero," : "no zero,");
			int of = (statusbyte0 & 0x08);
			int sf = (statusbyte0 & 0x04);
			int cf = (statusbyte0 & 0x02);
			int zf = (statusbyte0 & 0x01);
			sb.Append("       cf:" + cf + "  zf:" + zf + "  sf:" + sf + "  of:" + of + ",");
			sb.AppendLine();

			sb.Append("pc:");
			int pc = getreg(0x43, 0x00, sb);
			sb.AppendLine();

			sb.AppendLine("stack: ");
			int idx = 0;
			sb.Append(" " + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();
			sb.Append(" " + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();
			sb.Append(" " + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();
			sb.Append("" + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();
			sb.Append("" + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();
			sb.Append("" + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();
			sb.Append("" + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();
			sb.Append("" + idx + ":"); getmem(esp + ds + idx, sb); idx += 4; sb.AppendLine();

			sb.AppendLine("code: ");
			bool extend = false;
			int dummy = 0;
			for (int i = -32; i < 32; i += 4)
			{
				StringBuilder sb2 = new StringBuilder();
				int code = getmem(pc + cs + i, sb2);
				int target = pc + i;
				string foundsym = Config.getSym(target, syms);
				string scode = Config.dasm(cfgs, syms, code, pc + i, ref  extend);
				sb.AppendLine((i == 0 ? "*" : " ") + getHex8(pc + cs + i, ref dummy) + ":(" + sb2.ToString() + "," + foundsym + "):" + scode);

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
			portWrite((byte)(0x13), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 24; sb.Append(getHex2(temp[0]));
			portWrite((byte)(0x12), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 16; sb.Append(getHex2(temp[0]));
			portWrite((byte)(0x11), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 8; sb.Append(getHex2(temp[0]));
			portWrite((byte)(0x10), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 0; sb.Append(getHex2(temp[0]));
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
			portWrite((byte)(0x13), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 24; sb.Append(getHex2(temp[0]));
			portWrite((byte)(0x12), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 16; sb.Append(getHex2(temp[0]));
			portWrite((byte)(0x11), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 8; sb.Append(getHex2(temp[0]));
			portWrite((byte)(0x10), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 0; sb.Append(getHex2(temp[0]));
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
