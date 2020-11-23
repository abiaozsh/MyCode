using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using FileTool;

namespace filetool
{
	static class Program
	{

		[DllImport("kernel32.dll")]
		public static extern Boolean AllocConsole();
		[DllImport("kernel32.dll")]
		public static extern Boolean FreeConsole();
		/// <summary>
		/// 应用程序的主入口点。
		/// </summary>
		[STAThread]
		static void Main()
		{
			AllocConsole();
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
			FreeConsole();
		}
	}
}
