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
			Device device;

			device = new Device();
			device.SetCooperativeLevel(this, CooperativeLevel.Normal);

			m = new Music[128];
			double f0 = 32.703;
			for (int i = 24; i < 90; i++)
			{
				m[i] = new Music();
				m[i].init(device, f0);
				f0 *= 1.05946;
				Button b = new Button();
				panel1.Controls.Add(b);
				b.Width = 10;
				b.Top = 10;
				b.Height = 30;
				b.FlatStyle = FlatStyle.Flat;

				int n = (i - 24) / 12;
				int N = 7;
				if ((i - 24) % 12 == 0)
				{
					b.Left = (n * N) * 10;
					b.BackColor = Color.White;
				}
				if ((i - 24) % 12 == 1)
				{
					b.Height = 15;
					b.Width = 7;
					b.Left = (n * N + 1) * 10 - 3;
					b.BackColor = Color.Black;
					b.BringToFront();
				}
				if ((i - 24) % 12 == 2)
				{
					b.Left = (n * N + 1) * 10;
					b.BackColor = Color.White;
				}
				if ((i - 24) % 12 == 3)
				{
					b.Height = 15;
					b.Width = 7;
					b.Left = (n * N + 2) * 10 - 3;
					b.BackColor = Color.Black;
					b.BringToFront();
				}
				if ((i - 24) % 12 == 4)
				{
					b.Left = (n * N + 2) * 10;
					b.BackColor = Color.White;
				}

				if ((i - 24) % 12 == 5)
				{
					b.Left = (n * N + 3) * 10;
					b.BackColor = Color.White;
				}
				if ((i - 24) % 12 == 6)
				{
					b.Height = 15;
					b.Width = 7;
					b.Left = (n * N + 4) * 10 - 3;
					b.BackColor = Color.Black;
					b.BringToFront();
				}
				if ((i - 24) % 12 == 7)
				{
					b.Left = (n * N + 4) * 10;
					b.BackColor = Color.White;
				}
				if ((i - 24) % 12 == 8)
				{
					b.Height = 15;
					b.Width = 7;
					b.Left = (n * N + 5) * 10 - 3;
					b.BackColor = Color.Black;
					b.BringToFront();
				}
				if ((i - 24) % 12 == 9)
				{
					b.Left = (n * N + 5) * 10;
					b.BackColor = Color.White;
				}
				if ((i - 24) % 12 == 10)
				{
					b.Height = 15;
					b.Width = 7;
					b.Left = (n * N + 6) * 10 - 3;
					b.BackColor = Color.Black;
					b.BringToFront();
				}
				if ((i - 24) % 12 == 11)
				{
					b.Left = (n * N + 6) * 10;
					b.BackColor = Color.White;
				}

				//b.BackColor = Color.Black;

				b.MouseDown += new MouseEventHandler(b_MouseDown);
				b.MouseUp += new MouseEventHandler(b_MouseUp);
				b.Tag = m[i];
				m[i].button = b;
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

		void b_MouseDown(object sender, MouseEventArgs e)
		{
			((Music)(((Button)sender).Tag)).Play();
		}

		void b_MouseUp(object sender, MouseEventArgs e)
		{
			((Music)(((Button)sender).Tag)).Stop();
		}

		void b_Click(object sender, EventArgs e)
		{
		}

		class MIDI
		{
			public class Event
			{
				public int time;
				public int dely;
				public int channel;
				public int op;
				public int key;
				public string data;

			}

			public int ff;
			public int nn;
			public int dd;

			public List<byte[]> tracks = new List<byte[]>();


			public List<Event> events = new List<Event>();

		}


		private MIDI processMIDI(string filename)
		{
			MIDI midi = new MIDI();

			System.IO.FileStream fs = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);

			byte[] buff = new byte[256 * 1024];

			fs.Read(buff, 0, (int)fs.Length);

			fs.Close();

			{
				midi.ff = (buff[8] << 8) + buff[9];
				midi.nn = (buff[10] << 8) + buff[11];
				midi.dd = (buff[12] << 8) + buff[13];
				int idx = 14;
				for (int i = 0; i < midi.nn; i++)
				{
					int size = (int)(buff[idx + 4] << 24) + (buff[idx + 5] << 16) + (buff[idx + 6] << 8) + (buff[idx + 7]);

					byte[] track = new byte[size];
					System.Array.Copy(buff, idx + 8, track, 0, size);
					midi.tracks.Add(track);
					idx += size + 8;
				}
			}

			try
			{
				int channel = 0;
				foreach (byte[] track in midi.tracks)
				{
					channel++;
					int idx = 0;

					int totaltime = 0;

					while (true)
					{
						if (idx == track.Length)
						{
							break;
						}

						int deltaTime = track[idx++];

						if (deltaTime > 127)
						{
							int tempdeltaTime = track[idx++];

							if (tempdeltaTime > 127)
							{
								int tempdeltaTime1 = track[idx++];
								if (tempdeltaTime1 > 127)
								{
									throw new Exception();
								}
								else
								{
									deltaTime = ((deltaTime & 0x7F) << 14) + ((tempdeltaTime & 0x7F) << 7) + tempdeltaTime1;
								}
							}
							else
							{
								deltaTime = ((deltaTime & 0x7F) << 7) + tempdeltaTime;
							}
						}

						totaltime += deltaTime;

						byte byteStatus = track[idx++];

						if (byteStatus == 0xFF)
						{
							#region FF
							byte byteType = track[idx++];

							if (byteType == 0x03)
							{
								byte byteLen = track[idx++];

								if (byteLen > 127)
								{
									throw new Exception();
								}
								string str = Encoding.GetEncoding("GB2312").GetString(track, idx, byteLen);
								idx += byteLen;

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + str));
							}
							else if (byteType == 0x04)
							{
								byte byteLen = track[idx++];

								if (byteLen > 127)
								{
									throw new Exception();
								}
								string str = Encoding.GetEncoding("GB2312").GetString(track, idx, byteLen);
								idx += byteLen;

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + str));
							}
							else if (byteType == 0x51)
							{
								byte byteLen = track[idx++];

								if (byteLen != 0x03)
								{
									throw new Exception();
								}

								byte tt1 = track[idx++];
								byte tt2 = track[idx++];
								byte tt3 = track[idx++];

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(tt1) + X(tt2) + X(tt3)));
								//TODO
							}
							else if (byteType == 0x58)
							{
								byte byteLen = track[idx++];

								if (byteLen != 0x04)
								{
									throw new Exception();
								}

								byte nn = track[idx++];
								byte dd = track[idx++];
								byte cc = track[idx++];
								byte bb = track[idx++];

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(nn) + X(dd) + X(cc) + X(bb)));
								//TODO
							}
							else if (byteType == 0x59)
							{
								byte byteLen = track[idx++];

								if (byteLen != 0x02)
								{
									throw new Exception();
								}

								byte sf = track[idx++];
								byte mf = track[idx++];

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(sf) + X(mf)));
								//TODO
							}
							else if (byteType == 0x2F)
							{
								byte byteLen = track[idx++];

								if (byteLen != 0x00)
								{
									throw new Exception();
								}

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType)));
								//TODO
							}
							else if (byteType == 0x21)
							{
								byte byteLen = track[idx++];
								if (byteLen != 0x01)
								{
									throw new Exception();
								}

								byte sf = track[idx++];
								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(sf)));
							}
							else
							{
								throw new Exception();
							}
							#endregion
						}
						else
						{
							if ((byteStatus & 0xF0) == 0x80)
							{
								//¥Î©`¥È¥ª¥Õ
								byte cn = track[idx++];
								byte sp = track[idx++];

								midi.events.Add(getEvent(totaltime, channel, "", -1, cn));
							}
							else if ((byteStatus & 0xF0) == 0x90)
							{
								//¥Î©`¥È¥ª¥ó
								byte cn = track[idx++];
								byte sp = track[idx++];

								midi.events.Add(getEvent(totaltime, channel, "", 1, cn));
							}
							else if ((byteStatus & 0xF0) == 0xA0)
							{
								throw new Exception();
							}
							else if ((byteStatus & 0xF0) == 0xB0)
							{
								//¥³¥ó¥È¥í©`¥ë¥Á¥§¥ó¥¸
								byte cn = track[idx++];
								byte nn = track[idx++];

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(cn) + X(nn)));
							}
							else if ((byteStatus & 0xF0) == 0xC0)
							{
								//¥×¥í¥°¥é¥à¥Á¥§¥ó¥¸
								byte cn = track[idx++];

								midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(cn)));
							}
							else if ((byteStatus & 0xF0) == 0xD0)
							{
								throw new Exception();
							}
							else if ((byteStatus & 0xF0) == 0xE0)
							{
								throw new Exception();
							}
							else
							{
								throw new Exception();
							}
						}
					}
				}
			}
			catch (Exception ex)
			{
			}


			//foreach (MIDI.Event cevent in midi.events)
			//{
			//    if (cevent.op == -1)
			//    {
			//        cevent.time += 100;
			//    }
			//}
			midi.events.Sort(sort);
			int oldtime = 0;
			foreach (MIDI.Event cevent in midi.events)
			{
				cevent.dely = cevent.time - oldtime;
				oldtime = cevent.time;
			}

			return midi;
		}

		private void play()
		{
			MIDI midi = processMIDI("d:\\hdl.mid");
			for (int i = 0; i < midi.events.Count; i++)
			{
				MIDI.Event cevent = midi.events[i];
				//textBox1.Text = cevent.time + "\t" + cevent.dely + "\t" + cevent.channel + "\t" + cevent.data + "\t" + cevent.op + "\t" + cevent.key + "\r\n" + textBox1.Text;

				if (cevent.op == 1)
				{
					m[cevent.key].Play();
				}
				if (cevent.op == -1)
				{
					m[cevent.key].Stop();
				}

				Thread.Sleep((int)(0.8 * (double)(cevent.dely)));
			}

		}

		private void button1_Click(object sender, EventArgs e)
		{

			Thread t = new Thread(play);

			t.Start();





			//            string s = @"
			//62,4;
			//67,4;
			//67,1;
			//66,1;
			//62,1;
			//64,1;
			//64,4;
			//69,4;
			//67,1;
			//66,2;
			//67,1;
			//66,2;
			//62,4;
			//67,4;
			//71,2;
			//67,1;
			//
			//
			//";
			//            string[] ss = s.Split(';');
			//            for (int i = 0; i < ss.Length - 1; i++)
			//            {
			//                string[] sss = ss[i].Split(',');

			//                int n = int.Parse(sss[0].Trim());
			//                int n2 = int.Parse(sss[1].Trim());
			//                m[n].Play();

			//                Thread.Sleep(n2 * 200);

			//                m[n].Stop();
			//            }
		}


		private string X(int n)
		{
			return "0x" + n.ToString("X") + " ";
		}
		private string X(byte n)
		{
			if (n < 0x10)
			{
				return "0x0" + n.ToString("X") + " ";
			}
			else
			{
				return "0x" + n.ToString("X") + " ";
			}
		}
		private MIDI.Event getEvent(int time, int channel, string data)
		{
			MIDI.Event cevent = new MIDI.Event();
			cevent.time = time;
			cevent.channel = channel;
			cevent.data = data;
			return cevent;
		}
		private MIDI.Event getEvent(int time, int channel, string data, int op, int key)
		{
			MIDI.Event cevent = new MIDI.Event();
			cevent.time = time;
			cevent.channel = channel;
			cevent.data = data;
			cevent.op = op;
			cevent.key = key;
			return cevent;
		}

		private int sort(MIDI.Event e1, MIDI.Event e2)
		{
			if (e1.time - e2.time == 0)
			{
				return e1.channel - e2.channel;
			}
			return e1.time - e2.time;
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{

		}

	}










	public class Music
	{
		public Button button;
		SecondaryBuffer buffer;

		static int sps = 44100;
		//format.SamplesPerSecond = 22050;
		//format.SamplesPerSecond = 11025;
		static int buffsize = sps * 10;

		public void init(Device device, double frequency)
		{
			WaveFormat format = new WaveFormat();
			format.SamplesPerSecond = sps;
			format.BitsPerSample = 8;
			//format.BitsPerSample = 16;
			format.Channels = 1;
			format.BlockAlign = (short)(format.Channels * (format.BitsPerSample / 8));
			format.AverageBytesPerSecond = format.BlockAlign * format.SamplesPerSecond;
			format.FormatTag = WaveFormatTag.Pcm;

			BufferDescription desc = new BufferDescription();
			desc.BufferBytes = buffsize;
			desc.CanGetCurrentPosition = true;
			desc.ControlPositionNotify = false;
			desc.ControlVolume = false;
			desc.Format = format;
			desc.GlobalFocus = true;
			buffer = new SecondaryBuffer(desc, device);

			initdata(frequency);
		}

		public void Play()
		{
			button.BackColor = Color.Red;
			buffer.Play(0, BufferPlayFlags.Default);
		}

		public void Stop()
		{
			button.BackColor = Color.White;
			buffer.Stop();
			buffer.SetCurrentPosition(0);
		}

		void initdata(double frequency)
		{

			byte[] data = new byte[buffsize];

			double v = frequency * Math.PI * 2 / sps;
			double n = 0;
			double fade = 1;
			for (int i = 0; i < buffsize; i++)
			{
				n += v;
				double val = Math.Sin(n) * fade;
				data[i] = (byte)(127 * val + 127);
				fade *= 0.9999;
			}

			buffer.Write(0, data, LockFlag.None);
		}

	}
}