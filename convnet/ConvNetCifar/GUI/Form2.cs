using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;
using Lib;

namespace GUI
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}
		Net net;
		Trainer trainer;
		List<Data> data = new List<Data>();

		private void Form2_Load(object sender, EventArgs e)
		{

			//layer_defs = [];
			//layer_defs.push({type:'input', out_sx:1, out_sy:1, out_depth:2});
			//layer_defs.push({type:'fc', num_neurons:4
			//, activation: 'relu'});
			//layer_defs.push({type:'fc', num_neurons:2, activation: 'relu'});
			//layer_defs.push({type:'softmax', num_classes:2});
			//
			//net = new convnetjs.Net();
			//net.makeLayers(layer_defs);
			//
			//trainer = new convnetjs.SGDTrainer(net, {learning_rate:0.01, momentum:0.1, batch_size:10, l2_decay:0.001});

//			List<Def> layer_defs = new List<Def>();
//			layer_defs.Add(new Def { type = "input", out_sx = 1, out_sy = 1, out_depth = 2 }); // 2 inputs= x, y \n\
//			layer_defs.Add(new Def { type = "fc", num_neurons = 6, bias_pref = 0.1f });// relus like a bit of positive bias to get gradients early
//			layer_defs.Add(new Def { type = "relu" });
//
//			layer_defs.Add(new Def { type = "fc", num_neurons = 2, bias_pref = 0.1f });// relus like a bit of positive bias to get gradients early
//			layer_defs.Add(new Def { type = "relu" });
//
//			layer_defs.Add(new Def() { type = "softmax", num_classes = 2 });
//
//			net = new Net();
//			net.makeLayers(layer_defs);

			trainer = new Trainer(net, new Trainer.Option() { learning_rate = 0.01f, momentum = 0.1f, batch_size = 10, l2_decay = 0.001f });//0.001f

			circle_data();
		}

		class Data
		{
			public Vol val;
			public DataSet label;
		}

		private void draw()
		{
			Vol v = new Vol(1, 1, 2, 0.0f);
			Bitmap b = new Bitmap(100, 100);
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					v.w[0] = i / 10.0f - 5.0f;
					v.w[1] = j / 10.0f - 5.0f;
					Vol _out = net.forward(v);
					Color c = _out.w[1] > 0.5f ? Color.PaleVioletRed : Color.Green;
					b.SetPixel(i, j, c);
				}

			}

			Graphics g = Graphics.FromImage(b);

			//g.

			g.Flush();
			g.Dispose();

			pictureBox1.Image = b;
		}

		private void circle_data()
		{
			for (var i = 0; i < 50; i++)
			{
				var r = RNN.randf(0.0f, 2.0f);
				var t = RNN.randf(0.0f, 2 * (float)Math.PI);
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
				var r = RNN.randf(3.0f, 5.0f);
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
			for (int i = 0; i < 1000; i++)
			{
				int idx = (int)RNN.randf(0, 100);
				trainer.train(data[idx].val, data[idx].label);
			}
			draw();
		}

	}
}
