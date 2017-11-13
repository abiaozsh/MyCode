using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;

namespace GUI
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}
		MainNet net;
		List<Data> data = new List<Data>();

		public class MainNet : Net//net
		{
			public FullyConnLayer fc1;
			public FullyConnLayer fc2;

			public void init()
			{
				//trainer = new Trainer(this, new Trainer.Option() { learning_rate = 0.01f, momentum = 0.1f, batch_size = 10, l2_decay = 0.001f });//0.001f
				trainer = new AdaDeltaTrainer() { l2_decay = 0.0f };//0.001f method = "adadelta",

				fc1 = new FullyConnLayer(num_neurons: 6, bias_pref: 0.1f, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));

				Add(fc1);
				Add(fc2);

				Add(new SoftmaxLayer());

			}
		}
		private void Form2_Load(object sender, EventArgs e)
		{
			net = new MainNet();
			net.init();
			circle_data();
		}

		class Data
		{
			public Vol val;
			public DataSet label;
		}

		private void draw()
		{
			var ins = net.getInstance();

			Vol v = new Vol(1, 1, 2, 0.0f);
			Bitmap b = new Bitmap(100, 100);
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					v.w[0] = i / 10.0f - 5.0f;
					v.w[1] = j / 10.0f - 5.0f;
					Vol _out = net.forward(ins, v);
					Color c = _out.w[1] > 0.5f ? Color.PaleVioletRed : Color.Green;
					b.SetPixel(i, j, c);
				}

			}

			//Graphics g = Graphics.FromImage(b);
			//
			////g.
			//
			//g.Flush();
			//g.Dispose();

			pictureBox1.Image = b;
		}

		private void circle_data()
		{
			for (var i = 0; i < 50; i++)
			{
				var r = Util.randf(0.0f, 2.0f);
				var t = Util.randf(0.0f, 2 * (float)Math.PI);
				Data d = new Data();
				d.val = new Vol(1, 1, 2, 0);
				d.val.w[0] = r * (float)Math.Sin(t);
				d.val.w[1] = r * (float)Math.Cos(t);
				d.label = new DataSet();
				d.label.predict = 0;
				data.Add(d);
			}
			for (var i = 0; i < 50; i++)
			{
				var r = Util.randf(3.0f, 5.0f);
				//var t = convnetjs.randf(0.0, 2*Math.PI);
				var t = 2 * (float)Math.PI * i / 50.0f;
				Data d = new Data();
				d.val = new Vol(1, 1, 2, 0);
				d.val.w[0] = r * (float)Math.Sin(t);
				d.val.w[1] = r * (float)Math.Cos(t);
				d.label = new DataSet();
				d.label.predict = 1;
				data.Add(d);
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			var ins = net.getInstance();
			for (int j = 0; j < 10000; j++)
			{
				int batchSize = 10;
				for (int i = 0; i < batchSize; i++)
				{
					int idx = (int)Util.randf(0, 100);
					net.train(ins, data[idx].val, data[idx].label);
				}
				net.endofBatch(new Net.Instance[] { ins }, batchSize);
				draw();
				Application.DoEvents();
			}
		}

	}
}
