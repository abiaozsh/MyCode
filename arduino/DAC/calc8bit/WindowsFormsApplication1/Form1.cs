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
		public Form1()
		{
			InitializeComponent();

		}
		int[] dataList = {
1000,//0
2200,//1
4700,//2
6800,//3
10000,//4
22000,//5
47000,//6
75000,//7
100000,//8
220000,//9
470000//10
		};

		//int[] Rarray = new int[8];
		int[] Rarray = {22000,2200,10000,47000,22000,4700,220000,75000};
		class value
		{
			public double voltage;
			public int conf;
			public int iv;
			public double cur;
			public double error;
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			double totalerror;
			double maxerror;
			textBox1.Text = aa(Rarray, out  totalerror, out  maxerror);

			return;

			Random r = new Random();
			StringBuilder sb = new StringBuilder();
			for (int count = 0; count < 100000; count++)
			{
				for (int i = 0; i < 8; i++)
				{
					Rarray[i] = dataList[(int)(r.NextDouble() * 10)];
				}
				aa(Rarray, out  totalerror, out  maxerror);

				sb.AppendLine(totalerror + "\t" + maxerror + "\t" + Rarray[0] + "," + Rarray[1] + "," + Rarray[2] + "," + Rarray[3] + "," + Rarray[4] + "," + Rarray[5] + "," + Rarray[6] + "," + Rarray[7] + ",");
			}
			textBox1.Text = sb.ToString();
		}

		public static string aa(int[] Rarray, out double totalerror, out double maxerror)
		{
			List<value> values = new List<value>();
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < 256; i++)
			{
				double Rtop = 0;
				double Rbottom = 0;
				for (int j = 0; j < 8; j++)
				{
					if ((i & (1 << j)) != 0)
					{
						Rtop += (1f / Rarray[j]);
					}
					else
					{
						Rbottom += (1f / Rarray[j]);
					}
				}

				Rtop = 1f / Rtop;
				Rbottom = 1f / Rbottom;

				double currency = 256 / (Rtop + Rbottom); // i = v/r
				double voltage = currency * Rbottom;//v=i*r

				value v = new value();
				v.voltage = double.IsNaN(voltage) ? 255 : voltage;
				//v.conf = i;
				//v.cur = currency;
				//v.error = Math.Abs(Math.Floor(v.voltage) - v.voltage);
				//v.iv = (int)Math.Floor(v.voltage);
				values.Add(v);

				sb.AppendLine(i + "\t" + voltage);
			}
			totalerror = 0;
			maxerror = 0;

			return sb.ToString();
			values.Sort(delegate(value a, value b) { return a.voltage.CompareTo(b.voltage); });

			for (int i = 0; i < 256; i++)
			{
				double abserror = Math.Abs(values[i].voltage - i);
				totalerror += abserror;
				if (abserror > maxerror)
				{
					maxerror = abserror;
				}
			}
		}
	}
}


