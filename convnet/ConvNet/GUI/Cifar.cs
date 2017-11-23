using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ConvNet;

namespace GUI
{
	class Cifar
	{
		public static void ToHWC()
		{

			string[] files = {
				@"E:\MNIST\cifar-10-batches-bin\data_batch_1.bin",
				@"E:\MNIST\cifar-10-batches-bin\data_batch_2.bin",
				@"E:\MNIST\cifar-10-batches-bin\data_batch_3.bin",
				@"E:\MNIST\cifar-10-batches-bin\data_batch_4.bin",
				@"E:\MNIST\cifar-10-batches-bin\data_batch_5.bin",
				@"E:\MNIST\cifar-10-batches-bin\test_batch.bin" };

			FileStream fs = new FileStream(@"E:\MNIST\cifar-10-batches-bin\HWC.bin", FileMode.Create, FileAccess.Write);

			foreach (var file in files)
			{

				load(file, 0);

				byte[] buff = new byte[32 * 32 * 3];

				for (int imgI = 0; imgI < 10000; imgI++)
				{
					for (int i = 0; i < 32; i++)
					{
						for (int j = 0; j < 32; j++)
						{
							byte r = imgData[imgI][32 * 32 * 0 + i * 32 + j];
							byte g = imgData[imgI][32 * 32 * 1 + i * 32 + j];
							byte b = imgData[imgI][32 * 32 * 2 + i * 32 + j];

							buff[i * 32 * 3 + j * 3 + 0] = r;
							buff[i * 32 * 3 + j * 3 + 1] = g;
							buff[i * 32 * 3 + j * 3 + 2] = b;
						}
					}

					fs.Write(buff, 0, 32 * 32 * 3);
				}

			}

			fs.Close();
		}



		static byte[][] imgData = new byte[60000][];
		static byte[] lblData = new byte[60000];

		public static int Count = 60000;

		static Cifar()
		{
			//int idx = 0;

			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_1.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_2.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_3.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_4.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_5.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\test_batch.bin", idx);
			load(@"E:\MNIST\cifar-10-batches-bin\32x32.bin");

		}


		//static void save() {
		//
		//	FileStream fsimg1 = new FileStream(@"E:\MNIST\cifar-10-batches-bin\32x32.bin", FileMode.Create, FileAccess.Write);
		//
		//	for (int i = 0; i < 60000; i++)
		//	{
		//		fsimg1.Write(imgData[i], 0, 3072);
		//	}
		//
		//	fsimg1.Flush();
		//	fsimg1.Close();
		//}
		static int load(string filename, int idx)
		{
			FileStream fsimg1 = new FileStream(filename, FileMode.Open, FileAccess.Read);

			for (int i = 0; i < 10000; i++)
			{
				fsimg1.Read(lblData, idx, 1);
				byte[] img = new byte[3072];
				fsimg1.Read(img, 0, 3072);
				imgData[idx] = img;
				idx++;
			}
			fsimg1.Close();
			return idx;
		}
		static void load(string filename)
		{
			FileStream fsimg1 = new FileStream(filename, FileMode.Open, FileAccess.Read);

			for (int i = 0; i < 60000; i++)
			{
				byte[] img = new byte[3072];
				fsimg1.Read(img, 0, 3072);
				imgData[i] = img;
			}

			fsimg1.Close();
		}

		public static void getImg(Vol v, int idx)
		{
			for (int i = 0; i < 32; i++)
			{
				int linepos = i * 32;
				for (int j = 0; j < 32; j++)
				{
					int r = imgData[idx][linepos + j];
					int g = imgData[idx][linepos + j + 1024];
					int b = imgData[idx][linepos + j + 2048];
					v.set(i, j, 0, r / 255.0f - 0.5f);
					v.set(i, j, 1, g / 255.0f - 0.5f);
					v.set(i, j, 2, b / 255.0f - 0.5f);
				}
			}
		}

		public static Bitmap getBmp(int idx)
		{
			Bitmap bmp = new Bitmap(32, 32);
			for (int i = 0; i < 32; i++)
			{
				int linepos = i * 32;
				for (int j = 0; j < 32; j++)
				{
					int r = imgData[idx][linepos + j];
					int g = imgData[idx][linepos + j + 1024];
					int b = imgData[idx][linepos + j + 2048];
					bmp.SetPixel(j, i, Color.FromArgb(r, g, b));
				}
			}
			return bmp;
		}

		public int getLbl(int idx)
		{

			return lblData[idx];
		}

		public void init()
		{
		}


	}
}
