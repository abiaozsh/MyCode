using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;

namespace GUI
{
	public partial class MakeBlackList : Form
	{
		public MakeBlackList()
		{
			InitializeComponent();
		}
		MNIST mnist = new MNIST();
		MNIST.First3CVNet first3CVNet;
		private void MakeBlackList_Load(object sender, EventArgs e)
		{
			first3CVNet = new MNIST.First3CVNet();
			first3CVNet.init();
			//MNIST.loadBlackList();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			try
			{
				Util.load("net.txt", (s) =>
				{
					first3CVNet.load(s);
				});
			}
			catch (Exception ex)
			{
			}

			float accu = 0;
			for (int k = 0; k < 5; k++)
			{
				for (int i = 0; i < 50; i++)
				{
					first3CVNet.train();
					Text = k + "," + i + "," + accu;
					Application.DoEvents();
				}
				accu = first3CVNet.test();
				//for (int j = 0; j < 50; j++)
				//{
				//	mnist.mainNet.train();
				//	Text = k+" "+j;
				//	Application.DoEvents();
				//}
				Text = k + ",0," + accu;


				Util.save("net.txt", (sw) =>
				{
					first3CVNet.save(sw);
				});
				mnist.Init();

				//pictureBox3.Image = mnist.display();
				//FileStream fs = new FileStream("mnistnet" + textBox2.Text + k + ".txt", FileMode.Create, FileAccess.Write);
				//StreamWriter sw = new StreamWriter(fs);
				//mnist.mainNet.save(sw);
				//sw.Flush();
				//fs.Flush();
				//fs.Close();
			}
			//save("fist3l" + round + ".txt", (sw) =>
			//{
			//	mnist.first3CVNet.cv1.save(sw);
			//	mnist.first3CVNet.cv2.save(sw);
			//	mnist.first3CVNet.cv3.save(sw);
			//});
			//mnist.Init();


		}

		private void button3_Click(object sender, EventArgs e)
		{
			pictureBox4.Image = MNIST.getBmp(int.Parse(textBox1.Text));
		}

		private void button2_Click(object sender, EventArgs e)
		{
			StringBuilder sb = new StringBuilder();

			sb.AppendLine("<html>");
			sb.AppendLine("<table border = 1>");

			for (int j = int.Parse(textBox1.Text); j < int.Parse(textBox1.Text) + 1000; j++)//test
			{
				DataSet ds = new DataSet();
				ds.predict = MNIST.getLbl(j);
				var v = MNIST.getImg(j);
				var o = first3CVNet.forward(v);
				sb.AppendLine(report(o, j, ds.predict));
			}
			sb.AppendLine("</table>");
			sb.AppendLine("</html>");

			Util.save(@"d:\report.htm", (sr) =>
			{
				sr.Write(sb.ToString());
			});
		}
		public class Pack
		{
			public float predict;
			public int val;
		}
		public static string report(Vol v, int index, int correctVal)
		{
			List<Pack> list = new List<Pack>();
			for (int i = 0; i < 10; i++)
			{
				list.Add(new Pack() { predict = v.w[i], val = i });
			}

			list.Sort((a, b) => ((a.predict > b.predict) ? -1 : 1));
			string ret = "";

			ret += "<tr>";

			ret += "<td>" + index + "</td>";
			ret += "<td>" + correctVal + "</td>";
			ret += "<td>" + list[0].val + "</td>";
			if (correctVal != list[0].val)
			{
				ret += "<td bgcolor='red'>X</td>";
				ret += "<td>L</td>";
			}
			else
			{
				ret += "<td>O</td>";
				if (list[0].predict > 0.9f)
				{
					ret += "<td>H</td>";
				}
				else
				{
					ret += "<td>L</td>";
				}
			}

			for (int i = 0; i < 3; i++)
			{
				ret += "<td>" + list[i].val + ":" + list[i].predict + "</td>";
			}
			//ret += "<td><img src='E:\\MNIST\\imgs\\" + correctVal + "\\" + MNIST.blackList[index] + ".bmp'></td>";
			return ret;
		}

		private void button4_Click(object sender, EventArgs e)
		{
			MNIST.proc();
		}

		private void button5_Click(object sender, EventArgs e)
		{
			string todel = "449 1045 1044 1299 1393 1530 1901 1940 2130 2266 2291 2293 2654 2780 2995";
			string[] list = todel.Split(' ');
			foreach (string v in list)
			{
				//MNIST.removeData(int.Parse(v));
			}

		}

		private void button6_Click(object sender, EventArgs e)
		{
			//MNIST.saveBlackList();
		}
	}
}
