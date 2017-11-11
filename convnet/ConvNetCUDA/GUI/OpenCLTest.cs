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

	}
}
