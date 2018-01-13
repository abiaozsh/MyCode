using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

namespace FindEyeMouthNose
{
	class Program
	{
		static void Main(string[] args)
		{
			//lowres_CelebaBetter();
			//PackAndLabel_CelebaBetter();
			CelebaBetter(@"E:\MNIST\CelebA");
			//CelebaAttr3();
			//NoFace();
		}




		static double distance(double X1, double Y1, double X2, double Y2, double x0, double y0)
		{
			double A = Y2 - Y1;
			double B = X1 - X2;
			double C = X2 * Y1 - X1 * Y2;
			double a = A * x0 + B * y0 + C;
			double b = Math.Sqrt(A * A + B * B);
			double d = Math.Abs(a / b);
			return d;
		}



		static void CelebaBetter(string dir)//E:\MNIST\CelebA
		{
			string ss1;
			string ss2;
			string ss3;
			{
				FileStream fs = new FileStream(dir + @"\Anno\list_landmarks_celeba.txt", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				ss1 = sr.ReadToEnd();
				sr.Close();
				fs.Close();
			}
			{
				FileStream fs = new FileStream(dir + @"\Anno\list_bbox_celeba.txt", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				ss2 = sr.ReadToEnd();
				sr.Close();
				fs.Close();
			}
			{
				FileStream fs = new FileStream(dir + @"\Anno\list_attr_celeba.txt", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				ss3 = sr.ReadToEnd();
				sr.Close();
				fs.Close();
			}



			for (int i = 0; i < 10; i++)
			{
				ss1 = ss1.Replace("  ", " ");
				ss2 = ss2.Replace("  ", " ");
				ss3 = ss3.Replace("  ", " ");
			}

			var s1 = ss1.Split('\n');
			var sB = ss2.Split('\n');
			var sB3 = ss3.Split('\n');

			for (int i = 1; i < s1.Length - 2; i++)
			{
				int fn = i;

				string[] s2 = s1[i + 1].Split(' ');
				string[] sb2 = sB[i + 1].Split(' ');
				string[] sb3 = sB3[i + 1].Split(' ');
				string folder1 = "" + (i / 10000);
				string folder2 = "" + (i / 1000);
				string folder3 = "" + (i / 100);

				string fnn = s2[0].Substring(0, 7) + "png";

				string folder = dir + @"\Img\img_celeba.7z\img_celebaProc\" + folder1 + "\\" + folder2 + "\\" + folder3 + "\\";

				if (!Directory.Exists(folder))
				{
					Directory.CreateDirectory(folder);
				}

				//if (File.Exists(@"K:\MNIST\CelebA\Img\img_celeba.7z\img_celebaProc\" + folder + "\\" + s2[0]))
				//{
				//    continue;
				//}

				double lex = double.Parse(s2[1]);
				double ley = double.Parse(s2[2]);
				double rex = double.Parse(s2[3]);
				double rey = double.Parse(s2[4]);

				double lmx = double.Parse(s2[7]);
				double lmy = double.Parse(s2[8]);
				double rmx = double.Parse(s2[9]);
				double rmy = double.Parse(s2[10]);

				double distE = Math.Sqrt((lex - ley) * (lex - ley) + (rex - rey) * (rex - rey));

				double centex = (lex + rex) / 2;
				double centey = (ley + rey) / 2;

				double centmx = (lmx + rmx) / 2;
				double centmy = (lmy + rmy) / 2;

				double centfx = (centex + centmx) / 2;
				double centfy = (centey + centmy) / 2;

				double nx = double.Parse(s2[5]);
				double ny = double.Parse(s2[6]);

				double theta;
				if (centey - centmy != 0)
				{
					if (centey >= centmy)
					{
						theta = Math.Atan((centex - centmx) / (centey - centmy));
						theta = theta + Math.PI;
					}
					else
					{
						theta = Math.Atan((centex - centmx) / (centey - centmy));
					}
				}
				else
				{
					theta = 0;
				}

				//if (sb3[11] == "1")
				//{
				//	continue;
				//}
				//整理歪脸度
				//白平衡 自动曝光

				//theta /= Math.PI / 2;
				//theta *= 90;

				Bitmap bmp = new Bitmap(dir + @"\Img\img_celeba.7z\img_celeba\" + s2[0]);
				Graphics graph;

				double bx1 = double.Parse(sb2[1]);
				double by1 = double.Parse(sb2[2]);
				double bx2 = double.Parse(sb2[3]);
				double by2 = double.Parse(sb2[4]);

				bool mark = false;
				if (mark)
				{
					graph = Graphics.FromImage(bmp);

					Pen penw = new Pen(Color.White, 1.5f);
					Pen penr = new Pen(Color.Red, 1.5f);
					Pen penb = new Pen(Color.Blue, 1.5f);
					Pen peng = new Pen(Color.Green, 1.5f);

					graph.DrawLine(penw, (float)lex, (float)ley, (float)rex, (float)rey);
					graph.DrawLine(penr, (float)centex, (float)centey, (float)nx, (float)ny);
					graph.DrawLine(penb, (float)lmx, (float)lmy, (float)rmx, (float)rmy);
					graph.DrawLine(peng, (float)lex, (float)ley, (float)lmx, (float)lmy);
					graph.DrawLine(peng, (float)rex, (float)rey, (float)rmx, (float)rmy);

					graph.DrawRectangle(new Pen(Color.Black, 1.5f), (float)bx1, (float)by1, (float)bx2, (float)by2);
					graph.Flush();
					graph.Dispose();
					bmp.Save(folder + fnn, ImageFormat.Png);
					break;
				}


				bmp = GetRotateImage(bmp, theta, centfx, centfy);

				double h = Math.Sqrt((centex - centmx) * (centex - centmx) + (centey - centmy) * (centey - centmy)) * 5;
				//double w = Math.Sqrt((lex - rex) * (lex - rex) + (ley - rey) * (ley - rey)) * 5;
				//w = h * 0.75;



				//double h = by2 * 1.5;



				//if (h < 180)
				//{
				//	continue;
				//}
				double w = h * 0.75;


				double x1 = centfx - w / 2;
				double x2 = centfx + w / 2;
				double y1 = centfy - h * 0.55;
				double y2 = centfy + h / 2;

				//graph = Graphics.FromImage(bmp);
				//graph.DrawRectangle(penr, (float)x1, (float)y1, (float)w, (float)h);
				//
				//graph.Flush();
				//graph.Dispose();

				Bitmap outbmp = new Bitmap((int)w, (int)h);
				graph = Graphics.FromImage(outbmp);
				graph.Clear(Color.FromArgb(128, 128, 128));
				graph.DrawImage(bmp, new Point(-(int)x1, -(int)y1));

				graph.Flush();
				graph.Dispose();

				Bitmap outbmp2 = new Bitmap(192, 256);
				graph = Graphics.FromImage(outbmp2);
				graph.Clear(Color.FromArgb(128, 128, 128));
				//设置高质量插值法  
				graph.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.High;
				//设置高质量,低速度呈现平滑程度  
				graph.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
				graph.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
				//消除锯齿
				graph.SmoothingMode = SmoothingMode.AntiAlias;

				graph.DrawImage(outbmp, new Rectangle(0, 0, 192, 256));

				graph.Flush();
				graph.Dispose();


				Console.WriteLine((i) + ":" + "\t" + folder1);

				outbmp2.Save(folder + fnn, ImageFormat.Png);
				GC.Collect();
			}

		}

		public static double func(double X, double x1, double y1, double x2, double y2)
		{
			if (x2 == x1)
			{
				x2 += 0.01;
			}
			return (X - x1) / (x2 - x1) * (y2 - y1) + y1;
		}

		public static Rectangle GetRotateRectangle(int width, int height, double radian)
		{
			double cos = Math.Cos(radian);
			double sin = Math.Sin(radian);
			//只需要考虑到第四象限和第三象限的情况取大值(中间用绝对值就可以包括第一和第二象限)
			int newWidth = (int)(Math.Max(Math.Abs(width * cos - height * sin), Math.Abs(width * cos + height * sin)));
			int newHeight = (int)(Math.Max(Math.Abs(width * sin - height * cos), Math.Abs(width * sin + height * cos)));
			return new Rectangle(0, 0, newWidth, newHeight);
		}
		public static Bitmap GetRotateImage(Image srcImage, double radian, double centx, double centy)
		{
			//原图的宽和高
			int srcWidth = srcImage.Width;
			int srcHeight = srcImage.Height;
			//图像旋转之后所占区域宽和高
			//目标位图
			Bitmap destImage = null;
			Graphics graphics = null;
			try
			{
				//定义画布，宽高为图像旋转后的宽高
				destImage = new Bitmap(srcWidth, srcHeight);
				//graphics根据destImage创建，因此其原点此时在destImage左上角
				graphics = Graphics.FromImage(destImage);
				graphics.Clear(Color.FromArgb(128, 128, 128));
				//设置高质量插值法  
				graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.High;
				//设置高质量,低速度呈现平滑程度  
				graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
				graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
				//消除锯齿
				graphics.SmoothingMode = SmoothingMode.AntiAlias;

				//要让graphics围绕某矩形中心点旋转N度，分三步
				//第一步，将graphics坐标原点移到矩形中心点,假设其中点坐标（x,y）
				//第二步，graphics旋转相应的角度(沿当前原点)
				//第三步，移回（-x,-y）
				//获取画布中心点
				Point centerPoint = new Point((int)centx, (int)centy);
				//将graphics坐标原点移到中心点
				graphics.TranslateTransform(centerPoint.X, centerPoint.Y);
				//graphics旋转相应的角度(绕当前原点)
				graphics.RotateTransform((float)(radian / Math.PI * 180));
				//恢复graphics在水平和垂直方向的平移(沿当前原点)
				graphics.TranslateTransform(-centerPoint.X, -centerPoint.Y);
				//此时已经完成了graphics的旋转

				//计算:如果要将源图像画到画布上且中心与画布中心重合，需要的偏移量
				Point Offset = new Point(0, 0);
				//将源图片画到rect里（rotateRec的中心）
				graphics.DrawImage(srcImage, new Rectangle(Offset.X, Offset.Y, srcWidth, srcHeight));
				//重至绘图的所有变换
				graphics.ResetTransform();
				graphics.Save();
			}
			catch (Exception ex)
			{
				throw ex;
			}
			finally
			{
				if (graphics != null)
					graphics.Dispose();
			}
			return destImage;
		}
		public static Bitmap GetRotateImagex(Image srcImage, double radian)
		{
			//原图的宽和高
			int srcWidth = srcImage.Width;
			int srcHeight = srcImage.Height;
			//图像旋转之后所占区域宽和高
			Rectangle rotateRec = GetRotateRectangle(srcWidth, srcHeight, radian);
			int rotateWidth = rotateRec.Width;
			int rotateHeight = rotateRec.Height;
			//目标位图
			Bitmap destImage = null;
			Graphics graphics = null;
			try
			{
				//定义画布，宽高为图像旋转后的宽高
				destImage = new Bitmap(rotateWidth, rotateHeight);
				//graphics根据destImage创建，因此其原点此时在destImage左上角
				graphics = Graphics.FromImage(destImage);
				//要让graphics围绕某矩形中心点旋转N度，分三步
				//第一步，将graphics坐标原点移到矩形中心点,假设其中点坐标（x,y）
				//第二步，graphics旋转相应的角度(沿当前原点)
				//第三步，移回（-x,-y）
				//获取画布中心点
				Point centerPoint = new Point(rotateWidth / 2, rotateHeight / 2);
				//将graphics坐标原点移到中心点
				graphics.TranslateTransform(centerPoint.X, centerPoint.Y);
				//graphics旋转相应的角度(绕当前原点)
				graphics.RotateTransform((float)(radian / Math.PI * 180));
				//恢复graphics在水平和垂直方向的平移(沿当前原点)
				graphics.TranslateTransform(-centerPoint.X, -centerPoint.Y);
				//此时已经完成了graphics的旋转

				//计算:如果要将源图像画到画布上且中心与画布中心重合，需要的偏移量
				Point Offset = new Point((rotateWidth - srcWidth) / 2, (rotateHeight - srcHeight) / 2);
				//将源图片画到rect里（rotateRec的中心）
				graphics.DrawImage(srcImage, new Rectangle(Offset.X, Offset.Y, srcWidth, srcHeight));
				//重至绘图的所有变换
				graphics.ResetTransform();
				graphics.Save();
			}
			catch (Exception ex)
			{
				throw ex;
			}
			finally
			{
				if (graphics != null)
					graphics.Dispose();
			}
			return destImage;
		}

	}
}
