using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ConsoleApplication1;

namespace puzzle
{
	public partial class Form1 : Form
	{

		List<ulong[]> Data;

		string Index;

		Color[] Colors;


		Bitmap bmp = new Bitmap(200, 200);
		Graphics g;
		ulong data = 0;

		Bitmap bmp1 = new Bitmap(100, 400);
		Graphics g1;

		Bitmap bmp2 = new Bitmap(680, 100);
		Graphics g2;

		Bitmap bmp3 = new Bitmap(680, 100);
		Graphics g3;

		int[] startXa = { 0, 0, 0, 0, 0, 0 };
		int[] startYa = { 0, 0, 40, 100, 180, 280 };

		int[] startXb = { 0, 0, 40, 100, 180, 280, 400, 500,580, 640, 1000, 1000 };
		int[] startYb = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		int[] startXc = { 0, 0, 40, 100, 180, 280, 400, 500, 580, 640, 1000, 1000 };
		int[] startYc = { 0, 0, 0, 0, 0, 0 };

//0,0
//0,2
//1,2
//0,3
//1,3
//0,5
//1,5
//2,5
//0,6
//1,6
//2,6
//0,7
//1,7
//2,7
//0,9
//1,9
//2,9
//3,9
//0,10
//1,10
//2,10
//3,10
//0,11
//1,11
//2,11
//3,11
//0,12
//1,12
//2,12
//3,12
//0,14
//1,14
//2,14
//3,14
//4,14
//0,15
//1,15
//2,15
//3,15
//4,15
//0,16
//1,16
//2,16
//3,16
//4,16
//0,17
//1,17
//2,17
//3,17
//4,17
//0,18
//1,18
//2,18
//3,18
//4,18


		public Form1()
		{
			InitializeComponent();
			pictureBox1.Image = bmp;
			g = Graphics.FromImage(bmp);
			g.Clear(Color.Gray);
			Pen p = new Pen(Color.Black);
			for (int i = 1; i < 10; i++)
			{
				g.DrawLine(p, i * 20, 0, i * 20, 220 - i * 20);
			}
			for (int i = 1; i < 10; i++)
			{
				g.DrawLine(p, 0, i * 20, 220 - i * 20, i * 20);
			}
			g.Flush();
			pictureBox1.Refresh();

			pictureBoxa.Image = bmp1;
			g1 = Graphics.FromImage(bmp1);
			g1.Clear(Color.Gray);
			pictureBoxb.Image = bmp2;
			g2 = Graphics.FromImage(bmp2);
			g2.Clear(Color.Gray);
			pictureBoxc.Image = bmp3;
			g3 = Graphics.FromImage(bmp3);
			g3.Clear(Color.Gray);
			for (int j = 1; j <= 5; j++)
			{
				for (int i = 0; i < j + 1; i++)
				{
					g1.DrawLine(p,
						0 + startXa[j],
						i * 20 + startYa[j],
						20 * j + startXa[j],
						i * 20 + startYa[j]);
					g1.DrawLine(p,
						i * 20 + startXa[j],
						0 + startYa[j],
						i * 20 + startXa[j],
						20 * j + startYa[j]);
				}

				for (int i = 0; i < j + 1; i++)
				{
					g2.DrawLine(p,
						0 + startXb[j],
						i * 20 + startYb[j],
						20 * j + startXb[j],
						i * 20 + startYb[j]);
					g2.DrawLine(p,
						i * 20 + startXb[j],
						0 + startYb[j],
						i * 20 + startXb[j],
						20 * j + startYb[j]);

					g2.DrawLine(p,
						0 + startXb[10-j],
						i * 20 + startYb[10 - j],
						20 * j + startXb[10 - j],
						i * 20 + startYb[10 - j]);
					g2.DrawLine(p,
						i * 20 + startXb[10 - j],
						0 + startYb[10 - j],
						i * 20 + startXb[10 - j],
						20 * j + startYb[10 - j]);

					g3.DrawLine(p,
						0 + startXb[j],
						i * 20 + startYb[j],
						20 * j + startXb[j],
						i * 20 + startYb[j]);
					g3.DrawLine(p,
						i * 20 + startXb[j],
						0 + startYb[j],
						i * 20 + startXb[j],
						20 * j + startYb[j]);

					g3.DrawLine(p,
						0 + startXb[10 - j],
						i * 20 + startYb[10 - j],
						20 * j + startXb[10 - j],
						i * 20 + startYb[10 - j]);
					g3.DrawLine(p,
						i * 20 + startXb[10 - j],
						0 + startYb[10 - j],
						i * 20 + startXb[10 - j],
						20 * j + startYb[10 - j]);
				}

			}
		}

		void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			int x = e.X / 20;
			int y = e.Y / 20;

			if (CData.conv[x + y * 10] != -1)
			{
				data = data ^ ((ulong)1 << CData.conv[x + y * 10]);
			}
			refresh(data);

		}

		void refresh(ulong _data)
		{
			Pen p1 = new Pen(Color.Red);
			Pen p2 = new Pen(Color.Gray);
			Brush b1 = new SolidBrush(Color.Red);
			Brush b2 = new SolidBrush(Color.Gray);

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (CData.conv[i + j * 10] != -1)
					{
						if ((_data & ((ulong)1 << CData.conv[i + j * 10])) != 0)
						{
							g.FillEllipse(b1, i * 20, j * 20, 19, 19);
						}
						else
						{
							g.FillEllipse(b2, i * 20, j * 20, 19, 19);
						}
					}
				}
			}
			g.Flush();
			pictureBox1.Refresh();
			textBox1.Text = getHex4(_data);
		}

		static string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
		static string getHex4(ulong val)//"ll"+"hh"
		{
			return
				convt[((val & 0xF000000000000000) >> 60)] +
				convt[((val & 0x0F00000000000000) >> 56)] +
				convt[((val & 0x00F0000000000000) >> 52)] +
				convt[((val & 0x000F000000000000) >> 48)] +
				convt[((val & 0x0000F00000000000) >> 44)] +
				convt[((val & 0x00000F0000000000) >> 40)] +
				convt[((val & 0x000000F000000000) >> 36)] +
				convt[((val & 0x0000000F00000000) >> 32)] +
				convt[((val & 0x00000000F0000000) >> 28)] +
				convt[((val & 0x000000000F000000) >> 24)] +
				convt[((val & 0x0000000000F00000) >> 20)] +
				convt[((val & 0x00000000000F0000) >> 16)] +
				convt[((val & 0x000000000000F000) >> 12)] +
				convt[((val & 0x0000000000000F00) >> 8)] +
				convt[((val & 0x00000000000000F0) >> 4)] +
				convt[((val & 0x000000000000000F) >> 0)]
				;
		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{
			try
			{
				data = Convert.ToUInt64("0x" + textBox1.Text, 16);
			}
			catch
			{
			}
			refresh(data);
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			Data = new List<ulong[]>();

			Data.Add(proc(CData.d0));
			Data.Add(proc(CData.d1));
			Data.Add(proc(CData.d2));
			Data.Add(proc(CData.d3));
			Data.Add(proc(CData.d4));
			Data.Add(proc(CData.d5));
			Data.Add(proc(CData.d6));
			Data.Add(proc(CData.d7));
			Data.Add(proc(CData.d8));
			Data.Add(proc(CData.d9));
			Data.Add(proc(CData.d10));
			Data.Add(proc(CData.d11));


			Colors = new Color[12];
			Colors[0] = Color.FromArgb(252, 142, 203);
			Colors[1] = Color.FromArgb(250, 250, 250);
			Colors[2] = Color.FromArgb(6, 198, 79);
			Colors[3] = Color.FromArgb(244, 220, 70);
			Colors[4] = Color.FromArgb(1, 87, 196);
			Colors[5] = Color.FromArgb(121, 65, 174);
			Colors[6] = Color.FromArgb(249, 105, 35);
			Colors[7] = Color.FromArgb(229, 30, 61);
			Colors[8] = Color.FromArgb(30, 30, 30);
			Colors[9] = Color.FromArgb(41, 205, 232);
			Colors[10] = Color.FromArgb(1, 151, 122);
			Colors[11] = Color.FromArgb(200, 200, 200);
		}

		ulong[] proc(string s)
		{
			int cnt = 0;
			foreach (string s1 in s.Split('\n'))
			{
				string[] s2 = s1.Split(',');
				//if (s2[4] != "x")
				{
					cnt++;
				}
			}
			ulong[] dat = new ulong[cnt];
			cnt = 0;
			foreach (string s1 in s.Split('\n'))
			{
				string[] s2 = s1.Split(',');
				//if (s2[4] != "x")
				{
					dat[cnt++] = Convert.ToUInt64("0x" + (s2[0].Substring(1)), 16);
				}
			}

			return dat;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			int[] vals = {
1,1,0,0,1,1,0,0,0,1,0,0,1,0,0,0, 1,1,0,0,1,1,0,0,0,1,0,0,1,0,0,0,//0
1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0, 1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
1,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0, 1,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,//1
1,0,0,0,1,0,1,0,0,1,0,0,1,1,1,0, 1,0,0,0,1,0,1,0,0,1,0,0,1,1,1,0,
1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0, 1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
1,1,1,1,0,1,0,0,0,1,0,0,1,0,0,0, 0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,0,//2
0,0,1,0,0,1,0,0,1,1,1,1,1,1,0,0, 1,1,1,1,1,0,0,0,0,1,0,0,1,1,0,0,
0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,

1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0, 1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,//3
1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0, 1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
1,1,1,0,1,1,1,0,0,0,1,0,1,0,0,0, 1,1,1,0,1,1,1,0,0,0,1,0,1,0,0,0,//4
1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0, 1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,
1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,0, 1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0, 1,1,1,0,0,1,0,0,1,1,0,0,0,1,0,0,//5
1,1,1,0,1,0,0,0,1,1,0,0,1,1,0,0, 0,0,1,1,0,1,0,0,0,1,1,1,1,1,0,0,
0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0, 0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
                                 
0,1,1,0,1,1,0,0,0,0,1,0,1,0,0,0, 0,1,1,0,1,1,0,0,0,0,1,0,1,0,0,0,//6
1,1,0,0,0,1,1,0,0,1,1,0,1,1,0,0, 1,1,0,0,0,1,1,0,0,1,1,0,1,1,0,0,
1,0,0,0,0,0,1,0,1,1,0,0,0,1,1,0, 1,0,0,0,0,0,1,0,1,1,0,0,0,1,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0, 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,//7
1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0, 1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,
0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0, 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
1,1,0,0,1,1,1,0,0,1,0,0,1,0,0,0, 1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,0,//8
1,0,0,0,0,0,1,0,0,1,0,0,1,1,1,0, 0,1,0,0,1,1,1,0,1,0,0,0,1,0,0,0,
1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
1,1,1,0,0,1,0,0,1,1,0,0,1,1,0,0, 0,1,1,0,1,0,0,0,1,1,1,0,1,1,0,0,//9
0,1,1,0,1,1,0,0,1,1,1,0,1,1,0,0, 1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,
0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 
1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0, 1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,//10
0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
                                 
1,1,1,1,0,1,0,0,1,0,0,0,1,1,0,0, 0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,//11
0,0,0,1,0,1,0,0,1,1,1,1,1,0,0,0, 1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,
0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0, 0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,
                         };

			{
				int i = 9;
				int j = 2;
				ulong d = 0;
				for (int x = 0; x < 4; x++)
				{
					for (int y = 0; y < 4; y++)
					{
						int v = vals[i * 128 + j * 4 + x + y * 32];
						if (v == 1)
						{
							d |= ((ulong)1 << CData.conv[x + y * 10]);
						}
					}
				}
				//textBox1.Text = getHex4(d);
			}
			Dictionary<ulong, ulong> exist = new Dictionary<ulong, ulong>();
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < 12; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					for (int ax = 0; ax < 10; ax++)
					{
						for (int ay = 0; ay < 10; ay++)
						{
							int error = 0;
							ulong d = 0;
							if (CData.conv[ax + ay * 10] != -1)
							{
								for (int x = ax; x < ax + 4; x++)
								{
									for (int y = ay; y < ay + 4; y++)
									{
										int v = vals[i * 128 + j * 4 + (x - ax) + (y - ay) * 32];
										if (v == 1)
										{
											if (x >= 10)
											{
												error = 1;
												break;
											}
											if (y >= 10)
											{
												error = 1;
												break;
											}
											if (CData.conv[x + y * 10] == -1)
											{
												error = 1;
												break;
											}
											else
											{
												d |= ((ulong)1 << CData.conv[x + y * 10]);
											}
										}
									}
								}
								if (error == 0)
								{
									if (!exist.ContainsKey(d))
									{
										exist.Add(d, d);
									}
									else
									{
										error = 1;
									}
								}
							}
							else
							{
								error = 1;
							}

							if (error == 1)
							{
								//sb.AppendLine(i + "\t" + j + "\t" + ax + "\t" + ay + "\t" + "error");
							}
							else
							{
								sb.AppendLine(i + "\t" + j + "\t" + ax + "\t" + ay + "\t" + getHex4(d));
							}
						}
					}
				}
			}
			textBox2.Text = sb.ToString();
		}

		private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			textBox1.Text = listBox1.Text.Substring(1, 16);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			int[] Idx = new int[12];
			for (int i = 0; i < 12; i++)
			{
				Idx[i] = 0;
			}

			string s = "";
			for (int i = 0; i < 12; i++)
			{
				s += Idx[i] + ",";
			}
			textBox3.Text = s;
		}

		void clear()
		{
			Brush b2 = new SolidBrush(Color.Gray);

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (CData.conv[i + j * 10] != -1)
					{
						g.FillEllipse(b2, i * 20, j * 20, 19, 19);
					}
				}
			}
			g.Flush();
			pictureBox1.Refresh();
		}
		void draw(ulong mask, Color color, int size, int idx)
		{
			Brush b1 = new SolidBrush(color);
			Brush black = new SolidBrush(Color.Black);
			Font drawFont = new Font("Arial", 7);
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (CData.conv[i + j * 10] != -1)
					{
						if ((mask & ((ulong)1 << CData.conv[i + j * 10])) != 0)
						{
							if (size == 0)
							{
								g.FillEllipse(b1, i * 20 + 3, j * 20 + 3, 19 - 6, 19 - 6);
							}
							else
							{
								g.FillEllipse(b1, i * 20, j * 20, 19, 19);
							}
							g.DrawString(idx.ToString(), drawFont, black, i * 20 + 3, j * 20 + 3);
						}
					}
				}
			}
		}

		private void textBox3_TextChanged(object sender, EventArgs e)
		{
			try
			{
				string s = textBox3.Text;
				int[] Idx = new int[12];
				for (int i = 0; i < 12; i++)
				{
					Idx[i] = int.Parse(s.Split(',')[i]);
				}

				ulong board = 0;
				int last = 11;
				for (int i = 0; i < 12; i++)
				{
					ulong mask = Data[i][Idx[i]];
					if ((board & mask) == 0)
					{
						board |= mask;
					}
					else
					{
						last = i;
						break;
					}
				}
				clear();
				for (int i = 0; i <= last; i++)
				{
					ulong mask = Data[i][Idx[i]];
					draw(mask, Colors[i], i == last ? 0 : 1, i);
				}
				g.Flush();
				pictureBox1.Refresh();
			}
			catch
			{
			}
		}

		private void button3_Click(object sender, EventArgs e)
		{
			foreach (string s in textBox2.Text.Split('\n'))
			{
				int[] Idx = new int[12];
				for (int i = 0; i < 12; i++)
				{
					Idx[i] = int.Parse(s.Split(',')[i]);
				}

				clear();
				for (int i = 0; i < 12; i++)
				{
					ulong mask = Data[i][Idx[i]];
					draw(mask, Colors[i], 1, i);
				}
				g.Flush();
				bmp.Save(@"d:\img\" + s.Trim() + ".jpg");
			}

		}

	}
}
