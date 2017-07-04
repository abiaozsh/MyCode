using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using BigMathLib;
using System.Runtime.InteropServices;

namespace BigMathSample
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		[DllImport("kernel32.dll")]
		public static extern uint GetTickCount();

		private void button1_Click(object sender, EventArgs e)
		{
		testPi();
		}

		public void testPi()
		{
			//,31,0,0,0,0,15,16,109, 672,4718,33625,239093
			//,15,0,0,0,0, 0,47,313,2282,19125
			//,15,0,0,0,0, 15,32,140, 953,6781
			// ,0,0,0,0,15,16,15,141,546,3760
			textBox1.Text = "";
			//,0,0,0,0, 15,16,94,608,4102,28923
			BigMath b = new BigMath(2, "3.141592653589793238462");
			for (int i = 0; i < 10; i++)
			{
				BigMath t = new BigMath(b.Precision * 2, 0);
				b.CopyTo(t);
				b = t.clone();

				uint start = GetTickCount();
				b.Sin(4);
				//b.Sin_slow();
				textBox1.Text = textBox1.Text + "," + (GetTickCount() - start);
				Application.DoEvents();
				t.Add(b);
				b = t;
				System.IO.StreamWriter sw = new System.IO.StreamWriter("d:\\" + i + ".txt");
				sw.WriteLine(b.toString(0));
				sw.Close();
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			int p = 200;
			//addA case1
			{
			//                          1.000000000000000000000000000000000000000001
				BigMath a = new BigMath(p, "1.500000000000000000000000000000000000000002");
				BigMath b = new BigMath(p, "0.499999999999999999999999999999999999999999");
				a.Add(b);
				textBox1.Text += "1 2 "+a.toString()+"\r\n";
			}
			//addA case2
			{
				BigMath a = new BigMath(p, "0.500000000000000000000000000000000000000002");
				BigMath b = new BigMath(p, "0.499999999999999999999999999999999999999999");
				a.Add(b);
				textBox1.Text += "1 1 " + a.toString() + "\r\n";
			}
			//addB case1
			{
				BigMath a = new BigMath(p, "0.500000000000000000000000000000000000000002");
				BigMath b = new BigMath(p, "1.499999999999999999999999999999999999999999");
				a.Add(b);
				textBox1.Text += "2 2 " + a.toString() + "\r\n";
			}
			//subA case1
			{
				BigMath a = new BigMath(p, " 1.500000000000000000000000000000000000000000");
				BigMath b = new BigMath(p, "-0.499999999999999999999999999999999999999999");
				a.Add(b);
				textBox1.Text += "3 1 " + a.toString() + "\r\n";
			}
			//subB case1
			{
				BigMath a = new BigMath(p, "-0.499999999999999999999999999999999999999999");
				BigMath b = new BigMath(p, " 1.500000000000000000000000000000000000000000");
				a.Add(b);
				textBox1.Text += "4 1 " + a.toString() + "\r\n";
			}
			//subC case1
			{
				BigMath a = new BigMath(p, " 2.500000000000000000000000000000000000000000");
				BigMath b = new BigMath(p, "-1.499999999999999999999999999999999999999999");
				a.Add(b);
				textBox1.Text += "5 1 " + a.toString() + "\r\n";
			}
			//subD case1
			{
				BigMath a = new BigMath(p, "-2.499999999999999999999999999999999999999999");
				BigMath b = new BigMath(p, " 3.500000000000000000000000000000000000000000");
				a.Add(b);
				textBox1.Text += "6 1 " + a.toString() + "\r\n";
			}
			//subA case1
			{
				BigMath r = new BigMath(p, " 1.000000000000000000000000000000000000000000");
				BigMath a = new BigMath(p, " 1.000000000000000000000000000000000000000000");
				BigMath b = new BigMath(p, "-0.999999999999999999999999999999999999999999");
				r.Sign = 1;
				a.Add(b);
				textBox1.Text += "3 1 " + a.toString() + "\r\n";
			}
			//subB case1
			{
				BigMath r = new BigMath(p, "-0.999999999999999999999999999999999999999999");
				BigMath a = new BigMath(p, "-0.999999999999999999999999999999999999999999");
				BigMath b = new BigMath(p, " 1.000000000000000000000000000000000000000000");
				a.Add(b);
				textBox1.Text += "4 1 " + a.toString() + "\r\n";
			}



		}

		private void button2_Click(object sender, EventArgs e)
		{
			int p = 500;

			textBox1.Text ="";

			BigMath E = BigMath.E(p);

			//textBox1.Text = E.toString();

			//BigMath v = new BigMath(p, "1.01");
			BigMath v = new BigMath(p, "0.99");

			v.Exp(E);

			textBox1.Text = v.toString(-1);

			v.Ln(E);

			textBox1.Text += "\r\n" + v.toString(-1);
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{

		}


	}
}
