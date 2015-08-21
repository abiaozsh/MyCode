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

		int[] Rarray = {
//last
//ByteB
1000,//102 0 
47000,//473 1 
10000,//103 2 
10000,//103 3 
100000,//104 4 
100000,//104 5 
220000,//224 6 
4700,//472 7 
//
//ByteA
4700,//472 0 
220000,//224 1 
6800,//682 2 
470000,//474 3 
2200,//222 4 
75000,//753 5 
22000,//223 6 
47000,//473 7 
		};

		class value
		{
			public double voltage;
			public int conf;
			public int iv;
			public double cur;
			public double error;
		}
		value[] values = new value[257];

		/*

		*/
		private void Form1_Load(object sender, EventArgs e)
		{
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < 1024; i++)
			{
				FileStream fs = new FileStream(@"D:\MyCode\arduino\DAC\新建文本文档.txt", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				while (true)
				{
					string line = sr.ReadLine();
					if (line == null) break;
					string[] s = line.Split('\t');

					if (i == int.Parse(s[5]) && int.Parse(s[5]) == int.Parse(s[6]))
					{
						sb.AppendLine(line);
						break;
					}

				}
				sr.Close();
				fs.Close();
			}
			textBox1.Text = sb.ToString();
			//aa();
		}


		public void aa()
		{
			try
			{
				StringBuilder sb = new StringBuilder();
				for (int i = 0; i < 65535; i++)
				{
					double Rtop = 0;
					double Rbottom = 0;
					for (int j = 0; j < 16; j++)
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

					double currency = 1024 / (Rtop + Rbottom); // i = v/r
					double voltage = currency * Rbottom;//v=i*r

					value v = new value();
					v.voltage = voltage;
					v.conf = i;
					v.cur = currency;
					v.error = Math.Abs(Math.Floor(v.voltage) - v.voltage);
					v.iv = (int)Math.Floor(v.voltage);

					//if (values[v.iv] == null || v.error * v.cur < values[v.iv].error * v.cur)
					//{
					//	values[v.iv] = v;
					//}

					sb.Append("byteA:" + (i >> 8) + ",byteB:" + (i & 0xFF) + ",value:" + voltage + "\r\n");

				}

				for (int i = 0; i < 256; i++)
				{
					//sb.Append(values[i].iv + "\t" + values[i].error + "\t" + values[i].cur + "\t" + values[i].voltage + "\t" + values[i].conf + "\r\n");
				}

				textBox1.Text = sb.ToString();
			}
			catch (Exception ex)
			{
			}


		}
	}
}


