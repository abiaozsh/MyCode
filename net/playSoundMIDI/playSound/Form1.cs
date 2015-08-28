using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX.DirectSound;

namespace playSound
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			DxPlay dxplay = new DxPlay();
			dxplay.init(this);
			this.KeyDown += new KeyEventHandler(Form1_KeyDown);
			this.KeyUp += new KeyEventHandler(Form1_KeyUp);
		}

		Keys[] keys = new Keys[] {
			Keys.D1,
			Keys.D2,
			Keys.D3,
			Keys.D4,
			Keys.D5,
			Keys.D6,
			Keys.D7,
			Keys.D8,
			Keys.D9,
			Keys.D0
		};

		void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			for (int j = 0; j < 10; j++)
			{
				if (e.KeyCode == keys[j])
				{
					s[j] = 1;
				}
			}
		}
		void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			for (int j = 0; j < 10; j++)
			{
				if (e.KeyCode == keys[j])
				{
					s[j] = 0;
				}
			}
		}

		int[] s = new int[10];

		double[] v = new double[10] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		public void proc(byte[] data, int size)
		{
			for (int i = 0; i < size; i++)
			{
				double jv = 0.04;

				double n = 0;
				int k = 0;
				for (int j = 0; j < 10; j++)
				{
					v[j] += jv;
					jv += 0.02;

					if (s[j] == 1)
					{
						n += Math.Sin(v[j]);
					}
					k++;
				}
				if (k > 0)
				{
					n = n / k;
				}
				data[i] = (byte)(127 * n + 127);
			}


		}


	}



	public class DxPlay
	{
		Device device;
		SecondaryBuffer buffer;
		Timer timer;
		Form1 form;

		static int buffsize = 44100 * 4;
		static int notifysize = 1000;

		public void init(Form1 form)
		{
			this.form = form;
			device = new Device();
			device.SetCooperativeLevel(form, CooperativeLevel.Normal);

			WaveFormat format = new WaveFormat();
			format.SamplesPerSecond = 44100;
			//format.SamplesPerSecond = 22050;
			//format.SamplesPerSecond = 11025;
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
			buffer = new SecondaryBuffer(desc, device);

			WritePos = notifysize;

			timer = new Timer();
			timer.Tick += new EventHandler(timer_Tick);

			timer.Interval = 1;// notifysize * 10 / format.SamplesPerSecond;

			buffer.Play(0, BufferPlayFlags.Looping);
			timer.Start();
		}

		int WritePos;

		void timer_Tick(object sender, EventArgs e)
		{
			int temp;
			int PlayPos;
			buffer.GetCurrentPosition(out temp, out PlayPos);

			int diff = (WritePos - PlayPos);
			if (diff < 0)
			{
				diff = (diff + buffsize);
			}

			if (diff >= notifysize)
			{
				byte[] data = new byte[notifysize];
				form.proc(data, notifysize);
				WritePos += notifysize;
				if (WritePos >= buffsize)
				{
					WritePos = WritePos % buffsize;
				}
				buffer.Write(WritePos, data, LockFlag.None);
			}
		}
	}
}