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


		public void getStatus()
		{
			byte[] temp;
			StringBuilder sb = new StringBuilder();

			sb.Append("echo   :"); portWrite((byte)(0x01), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));

			sb.Append("\r\n c3_p0_cmd_full   :"); portWrite((byte)(0x32), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n c3_p0_wr_full    :"); portWrite((byte)(0x33), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n c3_p0_rd_full    :"); portWrite((byte)(0x34), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n c3_p0_rd_empty   :"); portWrite((byte)(0x35), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n c3_p0_rd_count   :"); portWrite((byte)(0x36), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n c3_p0_rd_overflow:"); portWrite((byte)(0x37), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n c3_p0_rd_error   :"); portWrite((byte)(0x38), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));

			sb.Append("\r\n debug31 0   :"); portWrite((byte)(0x39), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n debug31 1   :"); portWrite((byte)(0x3A), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n debug31 2   :"); portWrite((byte)(0x3B), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("\r\n debug31 3   :"); portWrite((byte)(0x3C), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));






			this.textBox4.Text = sb.ToString();
		}
		private void button4_Click(object sender, EventArgs e)
		{
			getStatus();
		}
		private uint getpixel(Color c)
		{
			uint val = 0;
			val += ((uint)(c.R) >> 3) << (5 + 6);
			val += ((uint)(c.G) >> 2) << (5);
			val += ((uint)(c.B) >> 3);
			return val;
		}

		private void button9_Click(object sender, EventArgs e)
		{
			Bitmap b = (Bitmap)Bitmap.FromFile("E:\\test.bmp");

			for (uint j = 0; j < 768; j++)
			{
				for (uint i = 0; i < 1024; i += 2)
				{
					uint c = getpixel(b.GetPixel((int)i, (int)j));
					uint c2 = getpixel(b.GetPixel((int)(i + 1), (int)j));
					setmem(j * 2048 + i *2, c + (c2 << 16));
				}
			}
		}

		private void button11_Click(object sender, EventArgs e)
		{
			portWrite((byte)(0x61), (byte)0x00);
			getStatus();
		}

		private void textBox4_TextChanged(object sender, EventArgs e)
		{

		}

		public uint getmem(uint addr, StringBuilder sb)
		{
			byte a0 = (byte)((addr >> 0) & 0xFF);
			byte a1 = (byte)((addr >> 8) & 0xFF);
			byte a2 = (byte)((addr >> 16) & 0xFF);
			byte a3 = (byte)((addr >> 24) & 0xFF);

			if (buff_a0 == null || buff_a0 != a0) { buff_a0 = a0; portWrite((byte)(0x20), buff_a0.Value); }
			if (buff_a1 == null || buff_a1 != a1) { buff_a1 = a1; portWrite((byte)(0x21), buff_a1.Value); }
			if (buff_a2 == null || buff_a2 != a2) { buff_a2 = a2; portWrite((byte)(0x22), buff_a2.Value); }
			if (buff_a3 == null || buff_a3 != a3) { buff_a3 = a3; portWrite((byte)(0x23), buff_a3.Value); }

			portWrite((byte)(0x30), 0);
			byte[] temp;
			temp = readFromPort(1);
			if (temp[0] != 123)
			{
				//throw new Exception("err");
			}
			uint val = 0;
			portWrite((byte)(0x13), (byte)0x00);
			portWrite((byte)(0x12), (byte)0x00);
			portWrite((byte)(0x11), (byte)0x00);
			portWrite((byte)(0x10), (byte)0x00);
			temp = readFromPort(4);
			val |= (uint)(temp[0]) << 24;
			val |= (uint)(temp[1]) << 16;
			val |= (uint)(temp[2]) << 8;
			val |= (uint)(temp[3]) << 0;
			if (sb != null)
			{
				sb.Append(Util.getHex2(temp[0]));
				sb.Append(Util.getHex2(temp[1]));
				sb.Append(Util.getHex2(temp[2]));
				sb.Append(Util.getHex2(temp[3]));
			}
			return val;
		}

		byte? buff_a0;
		byte? buff_a1;
		byte? buff_a2;
		byte? buff_a3;

		byte? buff_d0;
		byte? buff_d1;
		byte? buff_d2;
		byte? buff_d3;

		public void setmem(uint addr, uint data)
		{
			byte a0 = (byte)((addr >> 0) & 0xFF);
			byte a1 = (byte)((addr >> 8) & 0xFF);
			byte a2 = (byte)((addr >> 16) & 0xFF);
			byte a3 = (byte)((addr >> 24) & 0xFF);

			byte d0 = (byte)((data >> 0) & 0xFF);
			byte d1 = (byte)((data >> 8) & 0xFF);
			byte d2 = (byte)((data >> 16) & 0xFF);
			byte d3 = (byte)((data >> 24) & 0xFF);

			if (buff_a0 == null || buff_a0 != a0) { buff_a0 = a0; portWrite((byte)(0x20), buff_a0.Value); }
			if (buff_a1 == null || buff_a1 != a1) { buff_a1 = a1; portWrite((byte)(0x21), buff_a1.Value); }
			if (buff_a2 == null || buff_a2 != a2) { buff_a2 = a2; portWrite((byte)(0x22), buff_a2.Value); }
			if (buff_a3 == null || buff_a3 != a3) { buff_a3 = a3; portWrite((byte)(0x23), buff_a3.Value); }

			if (buff_d0 == null || buff_d0 != d0) { buff_d0 = d0; portWrite((byte)(0x24), buff_d0.Value); }
			if (buff_d1 == null || buff_d1 != d1) { buff_d1 = d1; portWrite((byte)(0x25), buff_d1.Value); }
			if (buff_d2 == null || buff_d2 != d2) { buff_d2 = d2; portWrite((byte)(0x26), buff_d2.Value); }
			if (buff_d3 == null || buff_d3 != d3) { buff_d3 = d3; portWrite((byte)(0x27), buff_d3.Value); }

			portWrite((byte)(0x31), 0);
			byte[] temp;
			temp = readFromPort(1);
			if (temp[0] != 123)
			{
				//throw new Exception("err");
			}
		}

		private void button2_Click_1(object sender, EventArgs e)
		{
			uint addr = Convert.ToUInt32(textBox1.Text, 16);
			StringBuilder sb = new StringBuilder();
			getmem(addr, sb);
			textBox2.Text = sb.ToString();

		}
		private void button6_Click(object sender, EventArgs e)
		{
			uint addr = Convert.ToUInt32(textBox1.Text, 16);
			setmem(addr, Convert.ToUInt32(textBox2.Text, 16));
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
			uint baseaddr = 65536 * 3;
			{
				portWrite((byte)(0x01), 0);
				var temp = readFromPort(1);
				if (temp[0] != 123)
				{
					MessageBox.Show("no echo");
					return;
				}
			}
			Random r = new Random();
			int total = 4096;
			uint addrFact = 4;
			if (sram)
			{
				addrFact = 1;
				total = 64;
			}
			uint[] arr = new uint[total];
			int idx = 0;
			for (uint i = 0; i < total * addrFact; i += addrFact)
			{
				uint val = (uint)r.Next();
				if (sram)
				{
					val = val & 0xFFFF;
				}
				arr[idx++] = val;
				setmem(baseaddr + i, val);
				this.Text = i.ToString();
				Application.DoEvents();
			}
			idx = 0;
			for (uint i = 0; i < total * addrFact; i += addrFact)
			{
				uint val = getmem(baseaddr + i, null);
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

		private void button10_Click(object sender, EventArgs e)
		{
		}


		private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
		{

		}


	}
}
