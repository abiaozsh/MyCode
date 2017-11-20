using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;

namespace GUI
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}
		MNIST mnist = new MNIST();
		MNIST.MainNet mainet;
		MNIST.Lv1TrainNet train1net;
		MNIST.Lv2TrainNet train2net;
		MNIST.Lv3TrainNet train3net;

		Bitmap baseimg;

		byte[] bigImg;
		int bigImgW;

		private void Form1_Load(object sender, EventArgs e)
		{
			MNISTData.proc();

			Bitmap bmp = (Bitmap)Bitmap.FromFile(@"E:\MNIST\无标题.png");

			pictureBox2.Image = bmp;
			pictureBox2.Width = bmp.Width;
			pictureBox2.Height = bmp.Height;

			bigImg = new byte[bmp.Width * bmp.Height];
			bigImgW = bmp.Width;

			pictureBox2.MouseMove += pictureBox2_MouseMove;
			try
			{
				for (int j = 0; j < bmp.Height; j++)
				{
					for (int i = 0; i < bmp.Width; i++)
					{
						bigImg[j * bigImgW + i] = bmp.GetPixel(i, j).R;
					}
				}
			}
			catch (Exception ex)
			{
				ex.ToString();
			}

			mainet = new MNIST.MainNet();
			mainet.init();


			train1net = new MNIST.Lv1TrainNet();
			train1net.init();

			train2net = new MNIST.Lv2TrainNet();
			train2net.init();

			train3net = new MNIST.Lv3TrainNet();
			train3net.init();


		}

		void pictureBox2_MouseMove(object sender, MouseEventArgs e)
		{
			var ins = mainet.getInstance();

			pictureBox1.Image = _getBmp(e.X, e.Y);

			var img = _getImg(e.X, e.Y);

			var v10 = mainet.forward(ins, img);

			String s = MNISTData.report(v10, -1, -1);
			textBox2.Text = (s);


		}

		Vol v = new Vol(28, 28, 1, 0.0f);
		public Vol _getImg(int x, int y)
		{
			for (var i = 0; i < 28; i++)
			{
				for (var j = 0; j < 28; j++)
				{
					int idx = (i + x) + (j + y) * bigImgW;
					int val = 0;
					if (idx < bigImg.Length)
					{
						val = bigImg[idx];
					}
					v.w[i + j * 28] = (float)val / 255.0f;
				}
			}
			return v;
		}
		public Bitmap _getBmp(int x, int y)
		{
			Bitmap b = new Bitmap(28, 28);

			for (int i = 0; i < 28; i++)
			{
				for (int j = 0; j < 28; j++)
				{
					int idx = (i + x) + (j + y) * bigImgW;
					int val = 0;
					if (idx < bigImg.Length)
					{
						val = bigImg[idx];
					}
					b.SetPixel(i, j, Color.FromArgb(val, val, val));
				}
			}
			return b;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			Util.load(@"..\cv3_cv1.txt", (s) =>
			{
				mainet.cv1.load(s); mainet.cv1.noUpdate = true;
			});
			Util.load(@"..\cv3_cv2.txt", (s) =>
			{
				mainet.cv2.load(s); mainet.cv2.noUpdate = true;
			});
			Util.load(@"..\cv3.txt", (s) =>
			{
				mainet.cv3.load(s); mainet.cv3.noUpdate = true;
			});

			Net.Instance[] insList = new Net.Instance[10];
			for (int i = 0; i < 10; i++)
			{
				insList[i] = mainet.getInstance();
				insList[i].inact = new Vol(28, 28, 1, 0.0f);
			}

			float accu = 0;
			for (int i = 0; i < 60; i++)
			{
				Random r = new Random();

				for (int j = 0; j < 10; j++)
				{
					Parallel.For(0, 10, (k) =>
					{
						mainet.train(insList[k], j * 10 + k);
					}
					);
					mainet.endofBatch(insList, 10);
					this.Text = i + "," + j;
					Application.DoEvents();
				}
				accu = mainet.test(insList[0], 1000, 2000);
				this.textBox2.Text = "," + accu;
			}
			return;
			mainet.cv1.noUpdate = false;
			mainet.cv2.noUpdate = false;
			mainet.cv3.noUpdate = false;

			for (int i = 0; i < 30; i++)
			{
				Random r = new Random();

				for (int j = 0; j < 10; j++)
				{
					Parallel.For(0, 10, (k) =>
					{
						mainet.train(insList[k], j * 10 + k);
					}
					);
					mainet.endofBatch(insList, 10);
					this.Text = i + "," + j;
					Application.DoEvents();
				}
				accu = mainet.test(insList[0], 1000, 2000);
				this.textBox2.Text = "," + accu;
			}

			//float accu = mainet.test();
			//this.textBox2.Text =  "," + accu;
		}

		private void button2_Click(object sender, EventArgs e)
		{
			MNIST.TestNet net = new MNIST.TestNet();
			net.init();
			var ins = net.getInstance();

			Util.load("conv.txt", (s) =>
			{
				net.cv1.load(s);
				net.cv2.load(s);
				net.fc1.load(s);
				net.fc2.load(s);
			});

			int cnt = 0;
			for (int i = 64; i < 128; i++)
			{
				Vol v = new Vol(28, 28, 1, 0);
				MNISTData.getImg(v, i);
				int l = MNISTData.getLbl(i);

				this.pictureBox1.Image = v.vis(0, 255);
				
				Application.DoEvents();

				Vol outv = net.forward(ins, v);
				int pred = MNISTData.GetPredicted(outv);
				if (pred == l)
				{
					cnt++;
				}
			}
			textBox1.Text = "" + cnt;
		}

		public void vis4(ConvLayer cv1)
		{
			//float scale;
			//float.TryParse(textBox1.Text, out scale);
			string dir = @"vis\1_";
			float scale;
			float.TryParse(textBox1.Text, out scale);
			for (int i = 0; i < MNIST.Lv1TrainNet.Lv1filters; i++)
			{
				cv1.vis(i, scale).Save(dir + i + ".bmp");
			}
		}

		bool stop;

		private void button3_Click(object sender, EventArgs e)
		{
			stop = true;
		}


		private void button5_Click(object sender, EventArgs e)
		{

			Util.load(@"..\cv1.txt", (s) =>
			{
				train1net.cv1.load(s);
			});

			Util.load(@"..\cv1_ufc.txt", (s) =>
			{
				train1net.ufc.load(s);
			});

			Net.Instance[] insList = new Net.Instance[10];
			for (int i = 0; i < 10; i++)
			{
				insList[i] = train1net.getInstance();
				insList[i].inact = new Vol(4, 4, 1, 0.0f);
			}

			float loss = 0;
			Stopwatch sw = new Stopwatch();
			sw.Start();
			int n = 0;
			while (true)
			{
				loss = 0;
				int samples = 0;
				Parallel.For(0, 10, (i) =>
				//for (int i = 0; i < 10; i++)
				{
					Random r = new Random();
					int sample = 0;
					loss += train1net.train(insList[i], (int)(r.NextDouble() * MNISTData.Count), out sample);
					samples += sample;
				}
				);
				n++;
				if (n >= MNISTData.Count * 2 / 10)
				{
					break;
				}

				train1net.endofBatch(insList, samples);

				Text = n + "," + loss;
				Application.DoEvents();
				if (n % 100 == 0)
				{
					test4();
					vis4(train1net.cv1);
					Util.save("cv1.txt", (s) =>
					{
						train1net.cv1.save(s);
					});
					Util.save("cv1_ufc.txt", (s) =>
					{
						train1net.ufc.save(s);
					});
					sw.Stop();
					this.textBox2.Text = sw.Elapsed.ToString();

					sw.Restart();
				}
				if (stop) { stop = false; break; }
			}


		}

		public void test4()
		{
			Vol v = new Vol(28, 28, 1, 0.0f);
			Vol v4 = new Vol(4, 4, 1, 0.0f);
			Net.Instance ins = train1net.getInstance();
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				MNISTData.getImg(v, n);

				MNIST.Lv1TrainNet.get4x4(v, v4, x, y);
				g.DrawImage(v4.vis(0, 255), 40 + i * 10, 30);

				Vol ret = train1net.forward(ins, v4);
				g.DrawImage(ret.vis(0, 255), 40 + i * 10, 40);
			}
			g.Flush();
			g.Dispose();

			pictureBox2.Image = b;
		}

		private void button6_Click(object sender, EventArgs e)
		{
			Util.load(@"..\cv1.txt", (s) =>
			{
				train2net.cv1.load(s); train2net.cv1.noUpdate = true;
			});

			Util.load(@"..\cv2_cv1.txt", (s) =>
			{
				train2net.cv1.load(s); train2net.cv1.noUpdate = false;
			});

			Util.load(@"..\cv2.txt", (s) =>
			{
				train2net.cv2.load(s);
			});
			Util.load(@"..\cv2_ufc.txt", (s) =>
			{
				train2net.ufc.load(s);
			});

			Net.Instance[] insList = new Net.Instance[10];
			for (int i = 0; i < 10; i++)
			{
				insList[i] = train2net.getInstance();
				insList[i].inact = new Vol(8, 8, 1, 0.0f);
			}

			float loss = 0;
			Stopwatch sw = new Stopwatch();
			sw.Start();
			int n = 0;
			while (true)
			{
				loss = 0;
				int samples = 0;
				Parallel.For(0, 10, (i) =>
				//for (int i = 0; i < 10; i++)
				{
					Random r = new Random();
					int sample = 0;
					loss += train2net.train(insList[i], (int)(r.NextDouble() * MNISTData.Count), out sample);
					samples += sample;
				}
				);
				n++;
				if (n >= MNISTData.Count * 2)
				{
					break;
				}

				train2net.endofBatch(insList, samples);

				Text = n + "," + loss;
				Application.DoEvents();
				if (n % 10 == 0)
				{
					test8();
					vis4(train2net.cv1);
					vis8();
					sw.Stop();
					this.textBox2.Text = sw.Elapsed.ToString();

					sw.Restart();

					Util.save("cv2_cv1.txt", (s) =>
					{
						train2net.cv1.save(s);
					});
					Util.save("cv2.txt", (s) =>
					{
						train2net.cv2.save(s);
					});
					Util.save("cv2_ufc.txt", (s) =>
					{
						train2net.ufc.save(s);
					});

				}
				if (stop) { stop = false; break; }
			}

		}
		public void vis8()
		{
			//float scale;
			//float.TryParse(textBox1.Text, out scale);
			string dir = @"vis\2_";
			float scale;
			float.TryParse(textBox1.Text, out scale);
			for (int i = 0; i < MNIST.Lv2TrainNet.Lv2filters; i++)
			{
				var v = train2net.vis(i);
				v.vis(0, scale).Save(dir + i + ".bmp");
			}
		}


		public void test8()
		{
			Net.Instance ins = train2net.getInstance();
			Vol v8 = new Vol(8, 8, 1, 0);
			Vol v = new Vol(28, 28, 1, 0);
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				MNISTData.getImg(v, n);

				MNIST.Lv2TrainNet.get8x8(v, v8, x, y);
				g.DrawImage(v8.vis(0, 255), 40 + i * 20, 30);

				Vol ret = train2net.forward(ins, v8);
				g.DrawImage(ret.vis(0, 255), 40 + i * 20, 50);
			}
			g.Flush();
			g.Dispose();

			pictureBox2.Image = b;
		}
		private void button4_Click(object sender, EventArgs e)
		{
			Util.load(@"..\cv2_cv1.txt", (s) =>
			{
				train3net.cv1.load(s); train3net.cv1.noUpdate = true;
			});

			Util.load(@"..\cv3_cv1.txt", (s) =>
			{
				train3net.cv1.load(s); train3net.cv1.noUpdate = false;
			});


			Util.load(@"..\cv2.txt", (s) =>
			{
				train3net.cv2.load(s); train3net.cv2.noUpdate = true;
			});
			Util.load(@"..\cv3_cv2.txt", (s) =>
			{
				train3net.cv2.load(s); train3net.cv2.noUpdate = false;
			});


			Util.load(@"..\cv3.txt", (s) =>
			{
				train3net.cv3.load(s);
			});
			Util.load(@"..\cv3_ufc.txt", (s) =>
			{
				train3net.ufc.load(s);
			});
			//
			//vis16();
			//test16();
			//
			//return;

			int threads = 8;

			Net.Instance[] insList = new Net.Instance[threads];
			for (int i = 0; i < threads; i++)
			{
				insList[i] = train3net.getInstance();
				insList[i].inact = new Vol(16, 16, 1, 0.0f);
			}

			float loss = 0;
			Stopwatch sw = new Stopwatch();
			sw.Start();
			int n = 0;
			while (true)
			{
				loss = 0;
				int samples = 0;
				Parallel.For(0, threads, (i) =>
				//for (int i = 0; i < 10; i++)
				{
					Random r = new Random();
					int sample = 0;
					loss += train3net.train(insList[i], (int)(r.NextDouble() * MNISTData.Count), out sample);
					samples += sample;
				}
				);
				n++;
				if (n >= MNISTData.Count * 4)
				{
					break;
				}

				train3net.endofBatch(insList, samples);

				Text = n + "," + loss;
				Application.DoEvents();
				if (n % 1 == 0)
				{
					sw.Stop();
					this.textBox2.Text = sw.Elapsed.ToString();
					sw.Restart();
				}

				if (n % 10 == 0)
				{
					vis16();
					test16();
					Util.save("cv3_cv1.txt", (s) =>
					{
						train3net.cv1.save(s);
					});
					Util.save("cv3_cv2.txt", (s) =>
					{
						train3net.cv2.save(s);
					});
					Util.save("cv3.txt", (s) =>
					{
						train3net.cv3.save(s);
					});
					Util.save("cv3_ufc.txt", (s) =>
					{
						train3net.ufc.save(s);
					});
				}
				if (stop) { stop = false; break; }
			}


		}
		public void vis16()
		{
			string dir = @"vis\";
			float scale;
			float.TryParse(textBox1.Text, out scale);
			for (int i = 0; i < MNIST.Lv3TrainNet.Lv3filters; i++)
			{
				var v = train3net.vis(i);
				Bitmap b = v.vis(0, scale);
				b.Save(dir + i + ".bmp");
			}
		}

		public void test16()
		{
			Net.Instance ins = train3net.getInstance();
			Vol v16 = new Vol(16, 16, 1, 0);
			Vol v = new Vol(28, 28, 1, 0);
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				MNISTData.getImg(v, n);

				MNIST.Lv3TrainNet.get16x16(v, v16, x, y);
				g.DrawImage(v16.vis(0, 255), 40 + i * 20, 30);

				Vol ret = train3net.forward(ins, v16);
				g.DrawImage(ret.vis(0, 255), 40 + i * 20, 50);
			}
			g.Flush();
			g.Dispose();

			pictureBox2.Image = b;
		}

	}
}
