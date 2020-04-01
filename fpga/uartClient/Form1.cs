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
			this.textBox1.KeyDown += textBox1_KeyDown;

			this.comboBox2.SelectedIndex = 0;
		}
		private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			portName = (string)this.comboBox1.SelectedItem;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			connect(portName);
		}

		StringBuilder sb1 = new StringBuilder();
		StringBuilder sb2 = new StringBuilder();
		private void timer1_Tick(object sender, EventArgs e)
		{
			while (fifo.Count > 0)
			{
				char c = fifo.Dequeue();
				sb1.Append(c);
				sb2.Append(" " + getHex2((byte)c));
			}
			textBox4.Text = sb1.ToString().Replace('\0', ' ');
			textBox3.Text = sb2.ToString().Replace('\0', ' ');
		}

		void textBox1_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Enter)
			{
				port.Write(textBox1.Text);
				if (this.comboBox2.Text == "\\0")
				{
					port.Write(new byte[] { (byte)0 }, 0, 1);
				}
				if (this.comboBox2.Text == "\\n")
				{
					port.Write(new byte[] { (byte)'\n' }, 0, 1);
				}
				if (this.comboBox2.Text == "\\r\\n")
				{
					port.Write(new byte[] { (byte)'\r', (byte)'\n' }, 0, 2);
				}
				if (this.comboBox2.Text == "\\n\\0")
				{
					port.Write(new byte[] { (byte)'\n', (byte)0 }, 0, 2);
				}
				if (this.comboBox2.Text == "\\r\\n\\0")
				{
					port.Write(new byte[] { (byte)'\r', (byte)'\n', (byte)0 }, 0, 3);
				}
				textBox1.Text = "";
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			sb1 = new StringBuilder();
			sb2 = new StringBuilder();
		}


	}
}
