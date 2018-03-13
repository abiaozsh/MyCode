using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX.DirectSound;
using System.Threading;

namespace playSound
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}

		Dictionary<Keys, Music> config = new Dictionary<Keys, Music>();

		void textBox1_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (config.ContainsKey(e.KeyCode))
			{
				Music m = config[e.KeyCode];
				m.Stop();
			}
		}

		void textBox1_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (config.ContainsKey(e.KeyCode))
			{
				Music m = config[e.KeyCode];
				m.Play();
			}
		}

		void Form1_KeyDown(object sender, KeyEventArgs e)
		{
		}
		void Form1_KeyUp(object sender, KeyEventArgs e)
		{
		}

		Music[] m;

		private void Form2_Load(object sender, EventArgs e)
		{
			try
			{
				Device device;

				device = new Device();
				device.SetCooperativeLevel(this, CooperativeLevel.Normal);

				m = new Music[96];

				for (int i = 0; i < 84; i++)
				{
					m[i] = new Music();
					double f = 0;
					try
					{
						f = double.Parse(Music.data[i * 2 + 1]);
					}
					catch (Exception ex)
					{
						ex.ToString();
					}
					string name = Music.data[i * 2].Trim();
					m[i].init(device, f, name);

					Button b = new Button();
					panel1.Controls.Add(b);
					b.Width = 10;
					b.Top = 10;
					b.Height = 30;
					b.FlatStyle = FlatStyle.Flat;

					int n = i / 12;
					int N = 7;
					if (i % 12 == 0)
					{
						b.Left = (n * N) * 10;
						b.BackColor = Color.White;
					}
					if (i % 12 == 1)
					{
						b.Height = 15;
						b.Width = 7;
						b.Left = (n * N + 1) * 10 - 3;
						b.BackColor = Color.Black;
						b.BringToFront();
					}
					if (i % 12 == 2)
					{
						b.Left = (n * N + 1) * 10;
						b.BackColor = Color.White;
					}
					if (i % 12 == 3)
					{
						b.Height = 15;
						b.Width = 7;
						b.Left = (n * N + 2) * 10 - 3;
						b.BackColor = Color.Black;
						b.BringToFront();
					}
					if (i % 12 == 4)
					{
						b.Left = (n * N + 2) * 10;
						b.BackColor = Color.White;
					}

					if (i % 12 == 5)
					{
						b.Left = (n * N + 3) * 10;
						b.BackColor = Color.White;
					}
					if (i % 12 == 6)
					{
						b.Height = 15;
						b.Width = 7;
						b.Left = (n * N + 4) * 10 - 3;
						b.BackColor = Color.Black;
						b.BringToFront();
					}
					if (i % 12 == 7)
					{
						b.Left = (n * N + 4) * 10;
						b.BackColor = Color.White;
					}
					if (i % 12 == 8)
					{
						b.Height = 15;
						b.Width = 7;
						b.Left = (n * N + 5) * 10 - 3;
						b.BackColor = Color.Black;
						b.BringToFront();
					}
					if (i % 12 == 9)
					{
						b.Left = (n * N + 5) * 10;
						b.BackColor = Color.White;
					}
					if (i % 12 == 10)
					{
						b.Height = 15;
						b.Width = 7;
						b.Left = (n * N + 6) * 10 - 3;
						b.BackColor = Color.Black;
						b.BringToFront();
					}
					if (i % 12 == 11)
					{
						b.Left = (n * N + 6) * 10;
						b.BackColor = Color.White;
					}

					//b.BackColor = Color.Black;

					b.MouseDown += new MouseEventHandler(b_MouseDown);
					b.MouseUp += new MouseEventHandler(b_MouseUp);
					b.Tag = m[i];
					m[i].button = b;
					Music.table.Add(name, m[i]);
				}

				{
					int i = 53;
					config.Add(Keys.Q, m[i++]);
					config.Add(Keys.D2, m[i++]);
					config.Add(Keys.W, m[i++]);
					config.Add(Keys.D3, m[i++]);
					config.Add(Keys.E, m[i++]);
					config.Add(Keys.D4, m[i++]);
					config.Add(Keys.R, m[i++]);

					i = 60;
					config.Add(Keys.T, m[i++]);
					config.Add(Keys.D6, m[i++]);
					config.Add(Keys.Y, m[i++]);
					config.Add(Keys.D7, m[i++]);
					config.Add(Keys.U, m[i++]);

					config.Add(Keys.I, m[i++]);
					config.Add(Keys.D9, m[i++]);
					config.Add(Keys.O, m[i++]);
					config.Add(Keys.D0, m[i++]);
					config.Add(Keys.P, m[i++]);
					//config.Add(Keys.D4, m[58]);
					//config.Add(Keys.R, m[59]);

				}

				this.KeyDown += new KeyEventHandler(Form1_KeyDown);
				this.KeyUp += new KeyEventHandler(Form1_KeyUp);
			}
			catch (Exception ex)
			{
				ex.ToString();
			}
		}

		void b_MouseDown(object sender, MouseEventArgs e)
		{
			((Music)(((Button)sender).Tag)).Play();
			textBox1.Text += ((Music)(((Button)sender).Tag)).name + "\r\n";
		}

		void b_MouseUp(object sender, MouseEventArgs e)
		{
			((Music)(((Button)sender).Tag)).Stop();
		}

		void b_Click(object sender, EventArgs e)
		{
		}




		private void play()
		{
			MIDI midi = MIDI.processMIDI("F:\\temp\\_music.all\\³àÉ«ÒªÈûJACKAL BY ZETA[¸ßÇå°æ].mid");
			//MIDI midi = processMIDI("F:\\temp\\_music.all\\Midi\\É³ÂÞÂüÉß\\lf-st2.mid");
			//MIDI midi = processMIDI(@"F:\temp\_music.all\Midi\midi\PAL01.MID");
			for (int i = 0; i < midi.events.Count; i++)
			{
				MIDI.Event cevent = midi.events[i];
				//textBox1.Text = cevent.time + "\t" + cevent.dely + "\t" + cevent.channel + "\t" + cevent.data + "\t" + cevent.op + "\t" + cevent.key + "\r\n" + textBox1.Text;

				if (cevent.key >= 24 && cevent.key < (96 + 24))
				{
					var mm = m[cevent.key - 24];

					if (mm != null)
					{
						if (cevent.op == 1)
						{
							mm.Play();
						}
						if (cevent.op == -1)
						{
							mm.Stop();
						}
					}
				}
				Thread.Sleep((int)(0.8 * (double)(cevent.dely)));
			}

		}

		private void button1_Click(object sender, EventArgs e)
		{

			Thread t = new Thread(play);

			t.Start();



		}



		private void textBox1_TextChanged(object sender, EventArgs e)
		{

		}

		private void panel1_Paint(object sender, PaintEventArgs e)
		{

		}

		private void textBox2_TextChanged(object sender, EventArgs e)
		{

		}

		private void button2_Click(object sender, EventArgs e)
		{
			string[] list = textBox2.Text.Split('\n');
			foreach (string item in list)
			{
				try
				{
					string note = item.Split(',')[0];
					double length = double.Parse(item.Split(',')[1]);
					//textBox1.Text = cevent.time + "\t" + cevent.dely + "\t" + cevent.channel + "\t" + cevent.data + "\t" + cevent.op + "\t" + cevent.key + "\r\n" + textBox1.Text;

					var mm = Music.table[note];

					if (mm != null)
					{
						if (length > 0)
						{
							mm.Play();

						}
						else
						{
							mm.Stop();
						}
					}
					Application.DoEvents();
					Thread.Sleep((int)(1000 * (double)(length)));
				}
				catch (Exception ex)
				{
				}
			}


		}

	}










}