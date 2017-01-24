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
		public Form1()
		{
			InitializeComponent();

			this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);
		}



		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		int[] targets = {
300
,9600
,115200
,921600
,2000000
,600
,900
,1200
,1800
,2400
,3600
,4800
,14400
,19200
,28800
,33600
,38400
,56000
,57600
,76800
,128000
,153600
,230400
,460800
,921600
,1500000
,2000000};

		private void button3_Click(object sender, EventArgs e)
		{
			double freq = double.Parse(textBox1.Text);

			textBox2.Text = "";
			for (int i = 0; i < targets.Length; i++)
			{
				textBox2.Text += proc8bit(freq, (double)targets[i]) + "\r\n";
			}
		}


		string proc8bit(double freq, double target)
		{
			int _prescaler = 1;
			if ((freq * 1000000  / target) > 128)
			{
				_prescaler = 2;
			}
			if ((freq * 1000000 / 8 / target) > 128)
			{
				_prescaler = 3;
			}

			string result = "PROGMEM prog_uint8_t TIMING";
			result += (Math.Round(freq * 100) / 100).ToString().PadLeft(5, '_');
			result += "_TCCR1B_" + (_prescaler).ToString();
			result += (Math.Round(target)).ToString().PadLeft(8, '_');
			result += "[] = {";

			double prescaler = 1;
			if (_prescaler == 1)
			{
				prescaler = 1;
			}
			else if (_prescaler == 2)
			{
				prescaler = 8;
			}
			else if (_prescaler == 3)
			{
				prescaler = 64;
			}

			freq *= 1000000;
			freq /= prescaler;

			for (int i = 1; i <= 10; i++)
			{
				double n = freq / target * i;
				while (n >= 256)
				{
					n -= 256;
				}

				result += Math.Round(n).ToString() + ",";
			}

			result += "};";
			return result;
		}


		string proc16bit(double freq, double target)
		{
			int _prescaler = 1;
			if ((freq * 1000000 * 10 / target) > 65536)
			{
				_prescaler = 2;
			}
			if ((freq * 1000000 * 10 / 8 / target) > 65536)
			{
				_prescaler = 3;
			}

			string result = "PROGMEM prog_uint16_t TIMING";
			result += (Math.Round(freq * 100) / 100).ToString().PadLeft(5, '_');
			result += "_TCCR1B_" + (_prescaler).ToString();
			result += (Math.Round(target)).ToString().PadLeft(8, '_');
			result += "[] = {";

			double prescaler = 1;
			if (_prescaler == 1)
			{
				prescaler = 1;
			}
			else if (_prescaler == 2)
			{
				prescaler = 8;
			}
			else if (_prescaler == 3)
			{
				prescaler = 64;
			}

			freq *= 1000000;
			freq /= prescaler;


			for (int i = 1; i <= 10; i++)
			{
				double n = freq / target * i;

				result += Math.Round(n).ToString() + ",";
			}

			result += "};";
			return result;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			double freq = double.Parse(textBox1.Text);

			textBox2.Text = "";
			for (int i = 0; i < targets.Length; i++)
			{
				textBox2.Text += proc16bit(freq, (double)targets[i]) + "\r\n";
			}

		}
	}
}
