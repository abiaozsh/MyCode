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
			this.textBox3.Enabled = true;
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
			byte[] temp;
			StringBuilder sb = new StringBuilder();
			portWrite((byte)(0x32), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));

			portWrite((byte)(0x33), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));

			this.textBox4.Text = sb.ToString();
		}

		private void textBox4_TextChanged(object sender, EventArgs e)
		{

		}

		public int getmem(int addr, StringBuilder sb)
		{
			portWrite((byte)(0x20), (byte)((addr >> 0) & 0xFF));
			portWrite((byte)(0x21), (byte)((addr >> 8) & 0xFF));
			portWrite((byte)(0x22), (byte)((addr >> 16) & 0xFF));
			portWrite((byte)(0x23), (byte)((addr >> 24) & 0xFF));

			if (sram)
			{
				portWrite((byte)(0x52), 0);
			}
			else
			{
				portWrite((byte)(0x50), 0);
			}
			byte[] temp;
			int val = 0;
			portWrite((byte)(0x13), (byte)0x00);
			portWrite((byte)(0x12), (byte)0x00);
			portWrite((byte)(0x11), (byte)0x00);
			portWrite((byte)(0x10), (byte)0x00);
			temp = readFromPort(4);
			val |= temp[0] << 24;
			val |= temp[1] << 16;
			val |= temp[2] << 8;
			val |= temp[3] << 0;
			if (sb != null)
			{
				sb.Append(Util.getHex2(temp[0]));
				sb.Append(Util.getHex2(temp[1]));
				sb.Append(Util.getHex2(temp[2]));
				sb.Append(Util.getHex2(temp[3]));
			}
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

			if (sram)
			{
				portWrite((byte)(0x53), 0);
			}
			else
			{
				portWrite((byte)(0x51), 0);
			}
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

		bool sram;

		private void checkBox1_CheckedChanged(object sender, EventArgs e)
		{
			if (checkBox1.Checked)
			{
				sram = true;
			}
			else
			{
				sram = false;
			}

		}

		private void button3_Click_1(object sender, EventArgs e)
		{
		}

		private void button5_Click(object sender, EventArgs e)
		{
			readFromPort(10);
		}

		private void button7_Click(object sender, EventArgs e)
		{
			Random r = new Random();
			int total = 256;
			int addrFact = 4;
			if (sram)
			{
				addrFact = 1;
				total = 64;
			}
			int[] arr = new int[total];
			int idx = 0;
			for (int i = 0; i < total * addrFact; i += addrFact)
			{
				int val = r.Next();
				if (sram)
				{
					val = val & 0xFFFF;
				}
				arr[idx++] = val;
				setmem(i, val);
				this.Text = i.ToString();
				Application.DoEvents();
			}
			idx = 0;
			for (int i = 0; i < total * addrFact; i += addrFact)
			{
				int val = getmem(i, null);
				if (val != arr[idx++])
				{
					MessageBox.Show("err");
					return;
				}
				this.Text = i.ToString();
				Application.DoEvents();
			}
			MessageBox.Show("ok");
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


			portWrite((byte)(0x30), 0);
			portWrite((byte)(0x31), (byte)int.Parse(this.textBox3.Text));
		}

		private void button9_Click(object sender, EventArgs e)
		{
			portWrite((byte)(0x02), 0);
			portWrite((byte)(0x02), 1);

		}

		private void button10_Click(object sender, EventArgs e)
		{
		}

		private void button11_Click(object sender, EventArgs e)
		{


			loadSym();

			int baseAddr = 0x02000000;

			portWrite((byte)(0x01), 1);//halt_uart

			{
				setmem(0x02000000, Convert.ToInt32("0000000A", 16));
			}

			baseAddr = 0x00000000;

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

		private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
		{

		}


	}
}
