using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
	class Cifar
	{
		static byte[] buffData = new byte[1024 * 1024 * 200];
		static Cifar()
		{
			int idx = 0;

			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_1.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_2.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_3.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_4.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_5.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\test_batch.bin", idx);
		}

		public void init() { 
		
		
		
		}


		public Bitmap getBmp(int idx)
		{
			Bitmap bmp = new Bitmap(32, 32);
			int imgsize = 32 * 32 * 3 + 1;
			int pos = idx * imgsize;
			for (int i = 0; i < 32; i++)
			{
				int linepos = i * 32;
				for (int j = 0; j < 32; j++)
				{
					int r = buffData[pos + linepos + j + 1];
					int g = buffData[pos + linepos + j + 1+1024];
					int b = buffData[pos + linepos + j + 1+2048];
					bmp.SetPixel(j, i, Color.FromArgb(r, g, b));
				}
			}
			return bmp;
		}

		public int getLbl(int idx)
		{
			int imgsize = 32 * 32 * 3 + 1;

			return buffData[idx * imgsize];
		}

		static int load(string filename, int idx)
		{
			FileStream fsimg1 = new FileStream(filename, FileMode.Open, FileAccess.Read);

			int len = fsimg1.Read(buffData, idx, 1024 * 1024 * 40);

			fsimg1.Close();
			return len + idx;
		}
	}
}
