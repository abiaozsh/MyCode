using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;
using Lib;

namespace GUI
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			//IntPtr openclp = OpenCL.init(0, 1);
			//OpenCL.oclobjects = openclp;

			Util.useSSE = true;

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
			//Application.Run(new Form2());
			//Application.Run(new Form3());
			//Application.Run(new Form34x4());
			//Application.Run(new Form4());
			//Application.Run(new MakeBlackList());
			//Application.Run(new OpenCLTest());
			//Application.Run(new RegMNISTLayer());
		}
	}
}
