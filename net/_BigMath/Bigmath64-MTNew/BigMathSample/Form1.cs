using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using BigMathLib;
using System.Runtime.InteropServices;
using System.IO;

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
			//,31,0,0,0, 0,15,16,109, 672,4718,33625,239093
			//,15,0,0,0, 0, 0,47,313,2282,19125
			//,15,0,0,0, 0,15,32,140, 953,6781
			// ,0,0,0,0,15,16,15,141, 546,3760
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
				sw.WriteLine(b.toString(-1));
				sw.Close();
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		return;
			{
				FileStream fs1 = new FileStream("d:\\1_1.txt", FileMode.Open, FileAccess.Read);
				FileStream fs2 = new FileStream("d:\\1_2.txt", FileMode.Open, FileAccess.Read);
				StreamReader sr1 = new StreamReader(fs1);
				StreamReader sr2 = new StreamReader(fs2);
				string[] s1 = sr1.ReadToEnd().Split('\n');
				string[] s2 = sr2.ReadToEnd().Split('\n');
				fs1.Close();
				fs2.Close();
				int idx = 0;
				for (double i = 0.7; i < 0.71; i += 0.001)
				{
					int err = s1[idx].Length;
					for (int j = 0; j < s1[idx].Length; j++)
					{
						if (j >= s2[idx].Length)
						{
							err = j;
							break;
						}
						if (s1[idx][j] != s2[idx][j])
						{
							err = j;
							break;
						}
					}

					textBox1.Text += i + "\t" + err + "\r\n";

					idx++;
				}
			}
			return;
			int p = 200;
			//addA case1
			{
				//                          1.000000000000000000000000000000000000000001
				BigMath a = new BigMath(p, "1.500000000000000000000000000000000000000002");
				BigMath b = new BigMath(p, "0.499999999999999999999999999999999999999999");
				a.Add(b);
				textBox1.Text += "1 2 " + a.toString() + "\r\n";
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
			//int p = 200;
			//{
			//	string a = "";
			//	BigMath E = BigMath.E(p);
			//	textBox1.Text = E.toString();
			//
			//	for (double i = 0.7; i < 0.71; i += 0.001)
			//	{
			//		string v = i.ToString();
			//		BigMath n = new BigMath(p, v);
			//		n.Exp(E);
			//		//Exp(p, n);
			//		a += n.toString() + "\r\n";
			//	}
			//
			//	FileStream fs = new FileStream("d:\\1_1.txt", FileMode.Create, FileAccess.ReadWrite);
			//	StreamWriter sw = new StreamWriter(fs);
			//	sw.Write(a);
			//	sw.Flush();
			//	fs.Close();
			//
			//	MessageBox.Show("a");
			//}




			//{
			//	BigMath a = new BigMath(p, 1);
			//	BigMath b = new BigMath(p, 3);
			//	BigMath c = new BigMath(p, 1);
			//	BigMath d = new BigMath(p, 5);
			//
			//	a.Div(b);
			//	c.Div(d);
			//
			//	a.Div(c);
			//
			//	textBox1.Text = a.toString(200);
			//	return;
			//}


			int p = 500;
			BigMath E = BigMath.E(p);
			BigMath v = new BigMath(p, "300");
			//BigMath v = new BigMath(p, "0.9");
			
			v.Exp(E);
			
			textBox1.Text = v.toString();
			
			v.Ln(E);
			
			textBox1.Text += "\r\n" + v.toString();
		}

		void Exp(int p, BigMath n)
		{

			//double num_1 = 2;
			BigMath num_1 = new BigMath(p, 1);

			//double mx = x;
			BigMath num_mx = n.clone();

			//double x = x;
			BigMath num_x = n.clone();

			////ex = (1+x)+x2/2!+x3/3!+...+xn/n!+...
			//add(Precision, Num_Array, pSign, pExponent, num_1, Snum_1, Enum_1);
			n.Add(num_1);

			//double mi = 2;
			BigMath num_i = new BigMath(p, 1);

			BigMath num_mi = new BigMath(p, 1);

			BigMath num_b = new BigMath(p, 1);

			int idx = 0;
			while (true)
			{
			idx++;
				//mx *= x;
				//mul(limit, num_mx, &Snum_mx, &Enum_mx, num_x, Snum_x, Enum_x, b);
				num_mx.Mul(num_x);


				//i += one;
				//inc(num_i, &Enum_i);
				num_i.Add(num_1);

				//mi *= i;
				//mul(limit, num_mi, &Snum_mi, &Enum_mi, num_i, Snum_i, Enum_i, b);
				num_mi.Mul(num_i);

				//b = mx;
				//for(int idx = 0; idx < Precision; idx++)
				//{
				//	num_b[idx] = num_mx[idx];
				//}
				//Snum_b = Snum_mx;
				//Enum_b = Enum_x;
				num_b = num_mx.clone();

				//b /= mi;
				//div(limit, num_b, &Snum_b, &Enum_b, num_mi, Snum_mi, Enum_mi, r_value, b);
				num_b.Div(num_mi);

				//ret += b;
				//add(Precision, Num_Array, pSign, pExponent, num_b, Snum_b, Enum_b);
				n.Add(num_b);

				if (n.Exponent - num_b.Exponent > (p - 2))
				{
					break;
				}
			}
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{

		}


	}
}
