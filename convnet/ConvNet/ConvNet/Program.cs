using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Lib;

namespace ConvNet
{
	class Program
	{

		static void Main(string[] args)
		{
			//int n = Util.initGPU(0);

			MyFloat mf = new MyFloat(1000);
			mf = null;
			FCFWD_Test.Test();
			FCBWD_Test.Test();

			GC.Collect();
			//CVFWD_Test.Test();

			Console.ReadLine();

		}
	}
}
