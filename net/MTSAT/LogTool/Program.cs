﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace LogTool
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			System.Net.ServicePointManager.DefaultConnectionLimit = 512;
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
		}
	}
}
