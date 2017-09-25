using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace ConvNet
{
	class Img_reg
	{
		public static void Proc()
		{

			List<Def> layer_defs = new List<Def>();
			layer_defs.Add(new Def { type = "input", out_sx = 1, out_sy = 1, out_depth = 2 }); // 2 inputs= x, y \n\
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });// relus like a bit of positive bias to get gradients early
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });// otherwise it's technically possible that a relu unit will never turn on (by chance)
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });// and will never get any gradient and never contribute any computation. Dead relu.
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "fc", num_neurons = 30, bias_pref = 0.1f });
			layer_defs.Add(new Def { type = "relu" });
			layer_defs.Add(new Def { type = "regression", num_neurons = 3 }); // 3 outputs= r,g,b \n\

			Net net = new Net();
			net.makeLayers(layer_defs);

			Trainer trainer = new Trainer(net, new Trainer.Option() { learning_rate = 0.01f, momentum = 0.9f, batch_size = 5, l2_decay = 0.0f });

			Bitmap b = new Bitmap("d:\\cat.jpg");

			for (int learn = 0; learn < 20; learn++)
			{
				for (int i = 0; i < 100; i++)
				{
					update(b, trainer);
					counter++;
				}
				Bitmap bout;
				draw(net, b, out bout);
				bout.Save("d:\\out.jpg");
			}

		}


		static int counter;
		static float smooth_loss = -1;

		public static void update(Bitmap b, Trainer trainer)
		{
			// forward prop the data
			int W = b.Width;
			int H = b.Height;

			Vol v = new Vol(1, 1, 2, null);
			float loss = 0;
			int lossi = 0;

			DataSet r = new DataSet();
			r.data = new float[3];
			Random rnd = new Random();
			int aax = (int)(rnd.NextDouble() * 10);
			int aay = (int)(rnd.NextDouble() * 10);

			for (int x = aax; x < W; x += 10)
			{
				for (int y = aay; y < H; y += 10)
				{
					//var r = [p[ix]/255.0, p[ix+1]/255.0, p[ix+2]/255.0]; // r g b
					Color c = b.GetPixel(x, y);
					r.data[0] = c.R / 255.0f;
					r.data[1] = c.G / 255.0f;
					r.data[2] = c.B / 255.0f;
					v.w[0] = (x - W / 2.0f) / W;
					v.w[1] = (y - H / 2.0f) / H;
					Trainer.Report stats = trainer.train(v, r);
					loss += stats.loss;
					lossi += 1;
				}
			}
			loss /= lossi;

			if (counter == 0) smooth_loss = loss;
			else smooth_loss = 0.99f * smooth_loss + 0.01f * loss;

			string t = "";
			t += "loss: " + smooth_loss;
			t += "iteration: " + counter;
			Console.WriteLine(t);
		}

		public static void draw(Net net, Bitmap b, out Bitmap bout)
		{
			bout = new Bitmap(b.Width, b.Height);
			// iterate over all pixels in the target array, evaluate them
			// and draw
			int W = b.Width;
			int H = b.Height;

			Vol v = new Vol(1, 1, 2, null);
			for (int x = 0; x < W; x++)
			{
				v.w[0] = (x - W / 2.0f) / W;
				for (int y = 0; y < H; y++)
				{
					v.w[1] = (y - H / 2.0f) / H;

					Vol r = net.forward(v, false);
					int R = (int)(255 * r.w[0]);
					int G = (int)(255 * r.w[1]);
					int B = (int)(255 * r.w[2]);
					if (R < 0)
					{
						//Console.Write("R:" + R + ";");
						R = 0;
					}
					if (G < 0)
					{
						//Console.Write("G:" + G + ";");
						G = 0;
					}
					if (B < 0)
					{
						//Console.Write("B:" + B + ";");
						B = 0;
					}

					if (R > 255)
					{
						//Console.Write("R:" + R + ";");
						R = 255;
					}
					if (G > 255)
					{
						//Console.Write("G:" + G + ";");
						G = 255;
					}
					if (B > 255)
					{
						//Console.Write("B:" + B + ";");
						B = 255;
					}
					Color c = Color.FromArgb(R, G, B);
					bout.SetPixel(x, y, c);
				}
			}

		}

	}
}

