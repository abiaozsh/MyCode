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
			//this.textBox2.DragDrop += new DragEventHandler(Form1_DragDrop);
			//this.textBox2.DragEnter += new DragEventHandler(Form1_DragEnter);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			if (port == null)
			{
				//COM4为Arduino使用的串口号，需根据实际情况调整
				port = new SerialPort(textBox1.Text, 1000000, Parity.None, 8, StopBits.One);
				port.Open();
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
		//private void button4_Click(object sender, EventArgs e)
		//{
		//	FileStream fs = new FileStream("d:\\outdata.txt", FileMode.Create, FileAccess.Write);
		//	FileStream fs1 = new FileStream("d:\\outdata.bin", FileMode.Create, FileAccess.Write);
		//	StreamWriter sw = new StreamWriter(fs);
		//
		//	portWrite("of");
		//	for (int i = 0; i < 1024*1024; i+=128)
		//	{
		//		portWrite("on");
		//		portWrite("fr" + getHex6(i) + "80");
		//		for (int j = 0; j < 128; j++)
		//		{
		//			string read = readFromPort(2);
		//			sw.Write(read);
		//			fs1.WriteByte(Convert.ToByte(read, 16));
		//		}
		//		fs.Flush();
		//		sw.WriteLine();
		//		sw.Flush();
		//		portWrite("of");//ed End
		//	}
		//
		//}
		private void button4_Click(object sender, EventArgs e)
		{
			//FileStream fs = new FileStream("d:\\outdata.txt", FileMode.Create, FileAccess.Write);
			FileStream fs1 = new FileStream("e:\\data.bin", FileMode.Create, FileAccess.Write);
			//StreamWriter sw = new StreamWriter(fs);

			portWrite("of");
			for (int i = 0; i < 4096 * int.Parse(textBox2.Text); i += 4096)
			{
				portWrite("on");
				portWrite("br" + getHex6(i) + "0C");

				byte[] buff = readFromPortBin(4096);
				fs1.Write(buff, 0, 4096);
				fs1.Flush();
				//sw.WriteLine();
				//sw.Flush();
				portWrite("of");//ed End
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

		private void button3_Click(object sender, EventArgs e)
		{
			FileStream fs = new FileStream("e:\\dataout.bin", FileMode.Open, FileAccess.Read);

			int idx = 0;
			for (int i = 0; i < 256; i++)//256*16*256 = 1M
			{
				portWrite("of");
				portWrite("on");
				portWrite("sd06");//we
				portWrite("of");

				portWrite("on");
				portWrite("sd20");//se
				portWrite("sd" + convt[((idx & 0xF00000) >> 20)] + convt[((idx & 0x0F0000) >> 16)]);//ad
				portWrite("sd" + convt[((idx & 0x00F000) >> 12)] + convt[((idx & 0x000F00) >> 8)]);//ad
				portWrite("sd" + convt[((idx & 0x0000F0) >> 4)] + convt[((idx & 0x00000F))]);//ad
				portWrite("of");
				WaitUntilReady();

				for (int j = 0; j < 64; j++)
				{
					portWrite("of");
					portWrite("on");
					portWrite("sd06");//we
					portWrite("of");

					portWrite("of");
					portWrite("on");
					portWrite("sd02");//pp
					portWrite("sd" + convt[((idx & 0xF00000) >> 20)] + convt[((idx & 0x0F0000) >> 16)]);//ad
					portWrite("sd" + convt[((idx & 0x00F000) >> 12)] + convt[((idx & 0x000F00) >> 8)]);//ad
					portWrite("sd" + convt[((idx & 0x0000F0) >> 4)] + convt[((idx & 0x00000F))]);//ad

					for (int k = 0; k < 64; k++)
					{
						int val = fs.ReadByte();
						portWrite("sd" + convt[((val & 0x0000F0) >> 4)] + convt[((val & 0x00000F))]);//ad
						idx++;
					}

					portWrite("of");
					WaitUntilReady();
				}
				this.Text = idx.ToString();
				Application.DoEvents();
			}
			fs.Close();
		}

		void WaitUntilReady()
		{
			portWrite("of");
			while (true)
			{
				portWrite("on");
				portWrite("sd05");//Read Status Register
				portWrite("gt");//ad
				string read = readFromPort(2);
				if (read == "00")
				{
					break;
				}
				//if (read == "FF")
				//{
				//	break;
				//}
				portWrite("of");
			}
			portWrite("of");
		}

		void checkOK()
		{
			//if (readFromPort(2) != "OK")
			//{
			//	textBox3.Text += "error!!\r\n";
			//}
		}

		void portWrite(string val)
		{
			if (port != null)
			{
				port.Write(val);
			}
			//textBox3.Text += val + "\r\n";
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

		void Form1_DragEnter(object sender, DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}
		void Form1_DragDrop(object sender, DragEventArgs e)
		{
			//string[] files = (string[])e.Data.GetData("FileDrop");
			//FileStream fs = new FileStream(files[0], FileMode.Open, FileAccess.Read);
			//StreamReader sr = new StreamReader(fs);
			//while (true)
			//{
			//	string line = sr.ReadLine();
			//	if (line == null) return;
			//
			//	int length = int.Parse(line.Substring(1, 2), NumberStyles.HexNumber);
			//
			//	string data = line.Substring(9, length * 2);
			//
			//	textBox2.Text += data;
			//}
		}
		private void textBox2_TextChanged(object sender, EventArgs e)
		{

		}

		private void textBox3_TextChanged(object sender, EventArgs e)
		{

		}

		private void button5_Click(object sender, EventArgs e)
		{
			//portWrite("of");
			//portWrite("on");
			//portWrite("sd06");//we
			//portWrite("of");
            //
			//portWrite("of");
			//portWrite("on");
			//portWrite("sd20");//be
			//portWrite("sd00");//ad
			//portWrite("sd00");//ad
			//portWrite("sd00");//ad
			//portWrite("of");

			portWrite("of");
			portWrite("on");
			portWrite("sd9f");//se
			portWrite("gt");//ad
			string read = readFromPort(2);
			portWrite("gt");//ad
			string read2 = readFromPort(2);
            portWrite("gt");//ad
            string read3 = readFromPort(2);
            portWrite("of");
            textBox3.Text = read + read2 + read3;
		}

		private void button6_Click(object sender, EventArgs e)
		{
			portWrite("of");
			portWrite("on");
			portWrite("sd06");//we
			portWrite("of");

			portWrite("of");
			portWrite("on");
			portWrite("sd02");//se
			portWrite("sd00");//ad
			portWrite("sd00");//ad
			portWrite("sd01");//ad
			portWrite("sd56");//ad
			portWrite("of");


		}

		private void button7_Click(object sender, EventArgs e)
		{
			portWrite("of");
			portWrite("on");
			portWrite("sd05");//se
			portWrite("gt");//ad
			string read = readFromPort(2);
			portWrite("gt");//ad
			string read2 = readFromPort(2);
			portWrite("of");
			textBox3.Text = read + read2;
		}

		private void button8_Click(object sender, EventArgs e)
		{
			portWrite("of");
			portWrite("on");
			portWrite("sd06");//we
			portWrite("of");

			portWrite("of");
			portWrite("on");
			portWrite("sd60");//be
			portWrite("of");
            WaitUntilReady();

		}
	}
}
