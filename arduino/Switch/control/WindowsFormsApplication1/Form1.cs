using System;
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

namespace WindowsFormsApplication1
{

	public partial class Form1 : Form
	{
		SerialPort port;
		FileStream fs;
		StreamWriter sw;
		private delegate void FlushClient(string s);
		public Form1()
		{


            InitializeComponent();

            this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);


			FileStream fs = new FileStream("d:\\comconf.txt", FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);

			string com = sr.ReadLine();
			this.Text = com;

            string remark = sr.ReadLine();
            textBox1.Text += remark;

			sr.Close();
			fs.Close();


			//fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
			//sw = new StreamWriter(fs);
			if (port == null)
			{
				try
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort(com, 9600, Parity.None, 8, StopBits.One);
					port.Open();
				}
				catch
				{ }
			}
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
		}

		byte CMD_ON1 = 1;
		byte CMD_ON2 = 2;
		byte CMD_ON3 = 3;
		byte CMD_ON4 = 4;
		byte CMD_OFF1 = 5;
		byte CMD_OFF2 = 6;
		byte CMD_OFF3 = 7;
		byte CMD_OFF4 = 8;

		byte CMD_TIM1_1S = 9;
		byte CMD_TIM1_10S = 10;
		byte CMD_TIM1_1M = 11;
		byte CMD_TIM1_1H = 12;
		byte CMD_TIM1_2H = 13;
		byte CMD_TIM1_3H = 14;
		byte CMD_TIM1_4H = 15;
		byte CMD_TIM1_5H = 16;


		public void send(byte cmd)
		{
			byte[] data = new byte[1];
			data[0] = cmd;
			try
			{
				port.Write(data, 0, 1);
			}
			catch
			{
				this.Text = "error";
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			send(CMD_ON1);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			send(CMD_ON2);
		}

		private void button7_Click(object sender, EventArgs e)
		{
			send(CMD_ON3);
		}

		private void button6_Click(object sender, EventArgs e)
		{
			send(CMD_ON4);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			send(CMD_OFF1);
		}

		private void button4_Click(object sender, EventArgs e)
		{
			send(CMD_OFF2);
		}

		private void button8_Click(object sender, EventArgs e)
		{
			send(CMD_OFF3);
		}

		private void button5_Click(object sender, EventArgs e)
		{
			send(CMD_OFF4);
		}

		private void button9_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_1S);

		}

		private void button10_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_10S);

		}

		private void button11_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_1M);

		}

		private void button12_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_1H);

		}

		private void button13_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_2H);

		}

		private void button15_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_3H);

		}

		private void button14_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_4H);

		}

		private void button16_Click(object sender, EventArgs e)
		{
			send(CMD_TIM1_5H);

		}


	}
}


