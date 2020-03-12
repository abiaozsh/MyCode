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
			portWrite((byte)(0xB7), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
			portWrite((byte)(0xB6), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
			portWrite((byte)(0xB5), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
			portWrite((byte)(0xB4), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
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
			portWrite((byte)(0xE7), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
			portWrite((byte)(0xE6), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
			portWrite((byte)(0xE5), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
			portWrite((byte)(0xE4), (byte)0x00); sb.Append(getHex2(readFromPort(1)[0]));
			sb.AppendLine();

			this.textBox4.Text = sb.ToString();

		}

		private void textBox4_TextChanged(object sender, EventArgs e)
		{

		}


	}
}
