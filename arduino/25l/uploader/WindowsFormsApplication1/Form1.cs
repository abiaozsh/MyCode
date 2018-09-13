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

		private void Form1_Load(object sender, EventArgs e)
		{
			this.DragDrop += new DragEventHandler(Form1_DragDrop);
			this.DragEnter += new DragEventHandler(Form1_DragEnter);


			//COM4为Arduino使用的串口号，需根据实际情况调整
			string com = "COM22";
            //string com = "COM16";
			port = new SerialPort(com, 300, Parity.None, 8, StopBits.One);
			port.DtrEnable = true;//for 32u4
			port.DiscardNull = false;
			//port = new SerialPort("COM22");
			port.Open();
		}

		void Form1_DragEnter(object sender, DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}
		void Form1_DragDrop(object sender, DragEventArgs e)
		{
			string[] files = (string[])e.Data.GetData("FileDrop");
			textBox3.Text = files[0];
		}

		private void button2_Click(object sender, EventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
				port = null;
			}
		}

		int size = 2048;//（1024 = 4M） (1=4k block)
		private void button4_Click(object sender, EventArgs e)
		{
			//FileStream fs = new FileStream("d:\\outdata.txt", FileMode.Create, FileAccess.Write);
			FileStream fs1 = new FileStream("d:\\datar2.bin", FileMode.Create, FileAccess.Write);
			//StreamWriter sw = new StreamWriter(fs);

			byte[] addr = new byte[3];

			port.Write("of");
			for (int i = 0; i < 4096 * size; i += 4096)
			{
				port.Write("on");
				//port.Write("br" + getHex6(i) + "0C");//2^0c = 4096
				port.Write("sd0b");//br
				//port.Write("sd" + convt[((i & 0xF00000) >> 20)] + convt[((i & 0x0F0000) >> 16)]);//ad
				//port.Write("sd" + convt[((i & 0x00F000) >> 12)] + convt[((i & 0x000F00) >> 8)]);//ad
				//port.Write("sd" + convt[((i & 0x0000F0) >> 4)] + convt[((i & 0x00000F))]);//ad
				port.Write("sa");
				addr[0] = (byte)((i & 0xFF0000) >> 16);
				addr[1] = (byte)((i & 0x00FF00) >> 8);
				addr[2] = (byte)((i & 0x0000FF) >> 0);
				port.Write(addr, 0, 3);//ad

				port.Write("sd00");

				port.Write("br");
				byte[] buff = readFromPortBin(4096);
				fs1.Write(buff, 0, 4096);
				fs1.Flush();
				//sw.WriteLine();
				//sw.Flush();
				port.Write("of");//ed End
				this.Text = i.ToString();
				Application.DoEvents();
			}
			fs1.Close();
		}
		byte[] readFromPortBin(int count)
		{
			int idx = 0;
			byte[] buff = new byte[count];
			if (count <= 0) return new byte[0];
			StringBuilder _sb = new StringBuilder();
			while (true)
			{
				if (port.BytesToRead > 0)
				{
					buff[idx] = (byte)port.ReadByte();
					count--;
					idx++;
					if (count <= 0) return buff;
				}
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

		private void button3_Click(object sender, EventArgs e)
		{
			FileStream fs = new FileStream(textBox3.Text, FileMode.Open, FileAccess.Read);
			size = (int)(fs.Length / 4096);
			int idx = 0;
			byte[] addr = new byte[3];
			for (int i = 0; i < size; i++)//64*64=4k
			{
				//port.Write("of");
				//port.Write("on");
				//port.Write("sd06");//we
				//port.Write("of");
				port.Write("we");//write enable

				port.Write("on");
				port.Write("sd20");//se
				//port.Write("sd" + convt[((idx & 0xF00000) >> 20)] + convt[((idx & 0x0F0000) >> 16)]);//ad
				//port.Write("sd" + convt[((idx & 0x00F000) >> 12)] + convt[((idx & 0x000F00) >> 8)]);//ad
				//port.Write("sd" + convt[((idx & 0x0000F0) >> 4)] + convt[((idx & 0x00000F))]);//ad
				port.Write("sa");
				addr[0] = (byte)((idx & 0xFF0000) >> 16);
				addr[1] = (byte)((idx & 0x00FF00) >> 8);
				addr[2] = (byte)((idx & 0x0000FF) >> 0);
				port.Write(addr, 0, 3);//ad
				port.Write("of");
				WaitUntilReady();

				for (int j = 0; j < 64; j++)
				{
					//port.Write("of");
					//port.Write("on");
					//port.Write("sd06");//we
					//port.Write("of");
					port.Write("we");//write enable

					port.Write("of");
					port.Write("on");
					port.Write("sd02");//pp
					//port.Write("sd" + convt[((idx & 0xF00000) >> 20)] + convt[((idx & 0x0F0000) >> 16)]);//ad
					//port.Write("sd" + convt[((idx & 0x00F000) >> 12)] + convt[((idx & 0x000F00) >> 8)]);//ad
					//port.Write("sd" + convt[((idx & 0x0000F0) >> 4)] + convt[((idx & 0x00000F))]);//ad
					port.Write("sa");
					addr[0] = (byte)((idx & 0xFF0000) >> 16);
					addr[1] = (byte)((idx & 0x00FF00) >> 8);
					addr[2] = (byte)((idx & 0x0000FF) >> 0);
					port.Write(addr, 0, 3);//ad

					port.Write("bw");
					byte[] buff = new byte[64];
					fs.Read(buff, 0, 64);
					port.Write(buff, 0, 64);
					idx += 64;

					port.Write("of");
					WaitUntilReady();
				}
				this.Text = idx.ToString();
				Application.DoEvents();
			}
			fs.Close();
		}

		void WaitUntilReady()
		{
			port.Write("of");
			StringBuilder sb = new StringBuilder();
			while (true)
			{
				port.Write("on");
				port.Write("sd05");//Read Status Register
				port.Write("gt");//ad
				string read = readFromPort(2);
				sb.AppendLine(read);
				int val = Convert.ToInt32("0x0" + read, 16);
				if ((val & 1) == 0) { break; }
				//break;
				//if (read == "00")
				//{
				//	break;
				//}
				//if (read == "FF")
				//{
				//	break;
				//}
				port.Write("of");
			}
			port.Write("of");
		}



		string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
		private string getHex6(int val)//"hh"+"ll"
		{
			return convt[((val & 0xF00000) >> 20)] +
				   convt[((val & 0x0F0000) >> 16)] +
				   convt[((val & 0x00F000) >> 12)] +
				   convt[((val & 0x000F00) >> 8)] +
				   convt[((val & 0x0000F0) >> 4)] +
				   convt[((val & 0x00000F))];
		}
		private string getHex2(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
		}

		private void button5_Click(object sender, EventArgs e)
		{
			port.Write("on");
			port.Write("sd05");//Read Status Register
			port.Write("gt");//ad
			string read = readFromPort(2);
			textBox3.Text = read;
			port.Write("of");
		}

	}
}
