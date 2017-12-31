using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GUI
{
	static class Program
	{

		[STAThread]
		static void Main()
		{
			MNISTData.makeBoundary();
			MNISTData.loadBoundary();
			MNISTData.makeTrainData();

		}
	}
}
