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
			//,31,0,0,0,0,15,16,109, 672,4718,33625,239093
			//,15,0,0,0,0, 0,47,313,2282,19125
			//,15,0,0,0,0,15,32,140, 953,6781


			//,0,0,0,0,15,16,94,608,4102,28923
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
		}

		private void button2_Click(object sender, EventArgs e)
		{
			////543543543*5435435342
			//BigMath t1 = new BigMath(100, "11111111111111111111111111111111111111111111111111111111111");
			//BigMath t2 = new BigMath(100, "1111111111111111111111111111111111111111");
			//t1.Div(t2);
			//textBox1.Text = t1.toString(0);
		}
	}
}
