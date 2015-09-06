﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
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
		FileStream fs;
		StreamWriter sw;
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

		//private void PortWrite(int chn, int data)
		//{
		//	byte[] d = new byte[2];
		//	d[0] = (byte)(0x050 | chn);
		//	d[1] = (byte)(data & 0x0FF);
		//	if (port != null && port.IsOpen)
		//	{
		//		port.Write(d, 1, 1);
		//	}
		//}

		private void Form1_Load(object sender, EventArgs e)
		{
			this.textBox2.DragDrop += new DragEventHandler(Form1_DragDrop);
			this.textBox2.DragEnter += new DragEventHandler(Form1_DragEnter);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			if (port == null)
			{
				//COM4为Arduino使用的串口号，需根据实际情况调整
				port = new SerialPort(textBox1.Text, 9600, Parity.None, 8, StopBits.One);
				port.Open();
				port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
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
		bool recev = false;
		private void button4_Click(object sender, EventArgs e)
		{
			port.Write("st");//st Start
			if (port.ReadLine() != "iStart OK.\r") return;
			port.Write("rf");//rf ReadFlash
            if (port.ReadLine() != "iRead Flash Done.\r") return;

			recev = true;
			for (int i = 0; i < 1024; i++)
			{
				port.Write("fb" + getHex(i));
			}
			recev = false;
			port.Write("ed");//ed End
            if (port.ReadLine() != "iEnd OK.\r") return;
            textBox2.Text = sb.ToString();
            sb.Clear();
		}
        StringBuilder sb = new StringBuilder();
		void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			if (recev)
			{
				while (port.BytesToRead > 0)
				{
                    sb.Append((char)port.ReadChar());
				}
			}
		}

		string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
		private string getHex(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))] + convt[((val & 0xF000) >> 12)] + convt[((val & 0x0F00) >> 8)];
		}

		void Form1_DragEnter(object sender, DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}
		void Form1_DragDrop(object sender, DragEventArgs e)
		{
			string[] files = (string[])e.Data.GetData("FileDrop");
			FileStream fs = new FileStream(files[0], FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			while (true)
			{
				string line = sr.ReadLine();
				if (line == null) return;

				int length = int.Parse(line.Substring(1, 2), NumberStyles.HexNumber);

				string data = line.Substring(9, length * 2);

				textBox2.Text += data;
			}
		}
		private void textBox2_TextChanged(object sender, EventArgs e)
		{

		}

		private void button3_Click(object sender, EventArgs e)
		{
			if(textBox2.Text.Length==0)return;

			port.Write("st");//st Start
			if (port.ReadLine() != "iStart OK.") return;
			port.Write("er");//st Start
			if (port.ReadLine() != "iStart OK.") return;
			port.Write("wf");//wf WriteFlash
			if (port.ReadLine() != "iWrite Flash Done.") return;

			recev = true;
			//16 words attiny24
			for (int i = 0; i < textBox2.Text.Length/4; i++)
			{
				port.Write("fb" + getHex(i));
			}
			recev = false;
			port.Write("ed");//ed End
			if (port.ReadLine() != "iEnd OK.") return;
		}
	}
}

//ts test
//st Start
//ed End
//er Erase
//rh ReadHighFuses
//rl ReadLowFuses
//re ReadExtendedBits
//rk ReadLockBits
//si ReadSignatureBytes
//wl WriteLowFuses
//wh WriteHighFuses
//we WriteExtendedBits
//wk WriteLockBits
//wf WriteFlash
//pb LoadFlashPageBuffer
//ha LoadFlashHighAddress
//rf ReadFlash
//fb ReadFlashLowAndHighBytes
//no NOP



//The Flash is organized in pages, see “Page Size” on page 162. When programming the Flash,
//the program data is latched into a page buffer. This allows one page of program data to be programmed
//simultaneously. The following procedure describes how to program the entire Flash
//memory:
//1. Load Command “Write Flash” (see Table 19-16 on page 171).
//2. Load Flash Page Buffer.
//3. Load Flash High Address and Program Page. Wait after Instr. 3 until SDO goes high for the “Page Programming” cycle to finish.
//4. Repeat 2 through 3 until the entire Flash is programmed or until all data has been
//programmed.
//5. End Page Programming by Loading Command “No Operation”.
//When writing or reading serial data to the ATtiny24/44/84, data is clocked on the rising edge of
//the serial clock, see Figure 20-6 on page 184, Figure 19-3 on page 167 and Table 20-13 on
//page 184 for details.
