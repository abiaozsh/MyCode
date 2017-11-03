using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;
using Lib;

namespace GUI
{
	public partial class OpenCLTest : Form
	{
		public OpenCLTest()
		{
			InitializeComponent();
		}

		private void OpenCLTest_Load(object sender, EventArgs e)
		{

		}

		MyFloat my;
		private void button1_Click(object sender, EventArgs e)
		{
			my = new MyFloat(100 * 1024 * 1024);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			my = null;

			GC.Collect();
		}

		private void button3_Click(object sender, EventArgs e)
		{
			textBox1.Text = "";
			textBox1.Text += OpenCL.getPlatforms() + "\r\n";
			textBox1.Text += OpenCL.getDevices(0) + "\r\n";

			IntPtr openclp = OpenCL.init(0, 1);
			OpenCL.oclobjects = openclp;

			MyFloat f1 = new MyFloat(64);
			MyFloat f2 = new MyFloat(64);
			f1[0] = 1.1f;
			f1[1] = 2.2f;
			f1.copyToCLMEM();

			f2[0] = 3.1f;
			f2[1] = 4.2f;
			f2.copyToCLMEM();


			var kernel = OpenCL.getKernel(OpenCL.oclobjects, "ConvNet.cl", "test1");
			int[] param = new int[1];
			param[0] = 3;
			int ret = 0;
			Stopwatch sw = new Stopwatch();

			sw.Start();
			for (int i = 0; i < 1000; i++)
			{
				//public static extern void runKernel(IntPtr oclobjects, IntPtr kernel, int threads, IntPtr p_cl_mem1, IntPtr p_cl_mem2, int param1, float param2);
				 ret = OpenCL.runKernel(OpenCL.oclobjects, kernel, 5, f1.p_cl_mem, f2.p_cl_mem, 1, 1.1f);
			}
			sw.Stop();

			Console.WriteLine(sw.ElapsedMilliseconds);
			Console.WriteLine(ret);
			f1.copyFromCLMEM();
			f2.copyFromCLMEM();

			Console.WriteLine(f1[0]);
			//Console.WriteLine(f1[1]);
			//Console.WriteLine(f[2]);
			//Console.WriteLine(f[3]);
			//Console.WriteLine(f[4]);
			//Console.WriteLine(f[5]);
			//Console.WriteLine(f[6]);
			//Console.WriteLine(f[7]);
			//Console.WriteLine(f[8]);
			//Console.WriteLine(f[9]);
			//Console.WriteLine(f[10]);
			//Console.WriteLine(f[11]);
			//Console.WriteLine(f[12]);

		}
	}
}
