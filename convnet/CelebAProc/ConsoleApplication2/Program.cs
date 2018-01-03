using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Drawing.Drawing2D;

namespace ConsoleApplication2
{
    class Program
    {
        static void Main(string[] args)
        {
            lowres_CelebaBetter();
            //PackAndLabel_CelebaBetter();
            //CelebaBetter();
            //CelebaAttr3();
            //NoFace();
        }

        static void lowres_CelebaBetter()
        {
            string ss1;

            StringBuilder sb = new StringBuilder();
            int OUTW = 192;
            int OUTH = 256;

            byte[] buff = new byte[4096 * OUTH * OUTW * 3];//603979776
            for (int fileindex = 0; fileindex < 31; fileindex++)
            {

                {//
                    FileStream _in = new FileStream("F:\\MNIST\\CelebABetter\\" + fileindex + ".bin", FileMode.Open, FileAccess.Read);
                    _in.Read(buff, 0, buff.Length);
                    _in.Close();
                }

                for (int packidx = 0; packidx < 4096; packidx++)
                {
                    for (int x = 0; x < OUTW; x += 4)
                    {
                        for (int y = 0; y < OUTH; y += 4)
                        {
                            int r = 0;
                            int g = 0;
                            int b = 0;

                            for (int i = 0; i < 4; i++)
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    r += buff[packidx * OUTW * OUTH * 3 + (y + j) * OUTW * 3 + (x + i) * 3 + 0];
                                    g += buff[packidx * OUTW * OUTH * 3 + (y + j) * OUTW * 3 + (x + i) * 3 + 1];
                                    b += buff[packidx * OUTW * OUTH * 3 + (y + j) * OUTW * 3 + (x + i) * 3 + 2];
                                }
                            }

                            r /= 16;
                            g /= 16;
                            b /= 16;

                            for (int i = 0; i < 4; i++)
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    buff[packidx * OUTW * OUTH * 3 + (y + j) * OUTW * 3 + (x + i) * 3 + 0] = (byte)r;
                                    buff[packidx * OUTW * OUTH * 3 + (y + j) * OUTW * 3 + (x + i) * 3 + 1] = (byte)g;
                                    buff[packidx * OUTW * OUTH * 3 + (y + j) * OUTW * 3 + (x + i) * 3 + 2] = (byte)b;

                                }
                            }

                        }
                    }
                }
                {//
                    FileStream _out = new FileStream("K:\\MNIST\\CelebABetterLowRes\\" + fileindex + ".bin", FileMode.Create, FileAccess.Write);
                    _out.Write(buff, 0, buff.Length);
                    _out.Close();
                }



                GC.Collect();
            }

        }

        static void PackAndLabel_CelebaBetter()
        {
            string ss1;

            StringBuilder sb = new StringBuilder();
            int OUTW = 192;
            int OUTH = 256;
            int fileindex = 0;
            int packidx = 0;
            byte[] buff = new byte[4096 * OUTH * OUTW * 3];//603979776
            for (int i = 1; i <= 202599; i++)
            {
                int fn = i;
                int folder = i / 10000;


                string filename = @"K:\MNIST\CelebA\Img\img_celeba.7z\img_celebaProc\" + folder + "\\" + string.Format("{0:000000}", i) + ".jpg";
                if (File.Exists(filename))
                {
                    sb.AppendLine(string.Format("{0:000000}", i) + ".jpg");
                    Bitmap bmp = new Bitmap(filename);
                    BitmapData bitmapData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, bmp.PixelFormat);

                    byte[] Pixels = new byte[bmp.Width * bmp.Height * 4];
                    Marshal.Copy(bitmapData.Scan0, Pixels, 0, Pixels.Length);
                    bmp.UnlockBits(bitmapData);

                    for (int x = 0; x < OUTW; x++)
                    {
                        for (int y = 0; y < OUTH; y++)
                        {
                            byte b = Pixels[y * bitmapData.Stride + x * 4 + 0];
                            byte g = Pixels[y * bitmapData.Stride + x * 4 + 1];
                            byte r = Pixels[y * bitmapData.Stride + x * 4 + 2];

                            buff[packidx * OUTW * OUTH * 3 + y * OUTW * 3 + x * 3 + 0] = r;
                            buff[packidx * OUTW * OUTH * 3 + y * OUTW * 3 + x * 3 + 1] = g;
                            buff[packidx * OUTW * OUTH * 3 + y * OUTW * 3 + x * 3 + 2] = b;

                        }
                    }
                    packidx++;
                    Console.WriteLine(packidx);
                    if (packidx == 4096)
                    {//
                        FileStream _out = new FileStream("F:\\MNIST\\CelebABetter\\" + fileindex + ".bin", FileMode.Create, FileAccess.Write);
                        _out.Write(buff, 0, buff.Length);
                        _out.Close();
                        fileindex++;
                        packidx = 0;
                    }

                }
                {
                    FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\BetterList.txt", FileMode.Create, FileAccess.Write);
                    StreamWriter sw = new StreamWriter(fs);
                    sw.Write(sb.ToString());
                    sw.Flush();
                    sw.Close();
                    fs.Close();
                }

                GC.Collect();
            }

        }

        static void CelebaAttr3()
        {
            string ss1;
            {
                FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\list_landmarks_celeba.txt", FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                ss1 = sr.ReadToEnd();
                sr.Close();
                fs.Close();
            }

            StringBuilder sb = new StringBuilder();

            for (int i = 0; i < 10; i++)
            {
                ss1 = ss1.Replace("  ", " ");
            }

            var s1 = ss1.Split('\n');

            for (int i = 0; i < s1.Length - 3; i++)//
            {
                string[] s2 = s1[i + 2].Split(' ');

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

                bool flg1 = centfy > func(centfx, lex, ley, lmx, lmy);
                bool flg2 = ny > func(nx, lex, ley, lmx, lmy);


                bool flg3 = centfy > func(centfx, rex, rey, rmx, rmy);
                bool flg4 = ny > func(nx, rex, rey, rmx, rmy);


                bool flg5 = (flg1 != flg2);
                bool flg6 = (flg3 != flg4);

                double dl = distance(lex, ley, lmx, lmy, nx, ny);

                double dr = distance(rex, rey, rmx, rmy, nx, ny);

                double flg = dl / (dl + dr) * 2 - 1;
                if (flg5)
                {
                    flg = -1;
                }
                if (flg6)
                {
                    flg = 1;
                }
                sb.AppendLine(s2[0] + " " + flg);

            }
            {
                FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\Attr3.txt", FileMode.Create, FileAccess.Write);
                StreamWriter sw = new StreamWriter(fs);
                sw.Write(sb.ToString());
                sw.Flush();
                sw.Close();
                fs.Close();
            }

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

        static void NoFace()
        {
            string ss1;
            string ss2;
            {
                FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\list_landmarks_celeba.txt", FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                ss1 = sr.ReadToEnd();
                sr.Close();
                fs.Close();
            }
            {
                FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\list_bbox_celeba.txt", FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                ss2 = sr.ReadToEnd();
                sr.Close();
                fs.Close();
            }

            for (int i = 0; i < 10; i++)
            {
                ss1 = ss1.Replace("  ", " ");
                ss2 = ss2.Replace("  ", " ");
            }

            var s1 = ss1.Split('\n');
            var sB = ss2.Split('\n');

            for (int i = 1; i < 20000; i++)
            {
                int fn = i;

                string[] s2 = s1[i + 1].Split(' ');
                string[] sb2 = sB[i + 1].Split(' ');

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
                    theta = Math.Atan((centex - centmx) / (centey - centmy));
                }
                else
                {
                    theta = 0;
                }

                if (centey > centmy)
                {
                    continue;
                }
                //整理歪脸度
                //白平衡 自动曝光

                //theta /= Math.PI / 2;
                //theta *= 90;
                Console.WriteLine((i + 1) + ":" + "\t" + theta);

                Bitmap bmp = new Bitmap(@"K:\MNIST\CelebA\Img\img_celeba.7z\img_celeba\" + s2[0]);
                Graphics graph;

                double bx1 = double.Parse(sb2[1]);
                double by1 = double.Parse(sb2[2]);
                double bx2 = double.Parse(sb2[3]);
                double by2 = double.Parse(sb2[4]);

                double h = by2 * 1.5;

                if (h < 200) continue;
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

                cut(bmp, 1, w, h, x1, y1, s2[0]);
                cut(bmp, 2, w, h, x1, y1, s2[0]);
                cut(bmp, 3, w, h, x1, y1, s2[0]);
                cut(bmp, 4, w, h, x1, y1, s2[0]);
                cut(bmp, 5, w, h, x1, y1, s2[0]);
                cut(bmp, 6, w, h, x1, y1, s2[0]);
                cut(bmp, 7, w, h, x1, y1, s2[0]);
                cut(bmp, 8, w, h, x1, y1, s2[0]);

                GC.Collect();
            }

        }

        public static void cut(Bitmap bmp, int pos, double w, double h, double x1, double y1, string fn)
        {
            Bitmap outbmp = new Bitmap((int)w, (int)h);
            Graphics graph = Graphics.FromImage(outbmp);
            graph.Clear(Color.FromArgb(128, 128, 128));

            if (pos == 1)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1 + w), -(int)(y1 + h)));
            }
            if (pos == 2)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1 + w), -(int)(y1)));
            }
            if (pos == 3)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1), -(int)(y1 + h)));
            }
            if (pos == 4)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1 - w), -(int)(y1 - h)));
            }
            if (pos == 5)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1 - w), -(int)(y1)));
            }
            if (pos == 6)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1), -(int)(y1 - h)));
            }
            if (pos == 7)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1 + w), -(int)(y1 - h)));
            }
            if (pos == 8)
            {
                graph.DrawImage(bmp, new Point(-(int)(x1 - w), -(int)(y1 + h)));
            }

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


            outbmp2.Save(@"K:\MNIST\CelebA\Img\img_celeba.7z\NoFace\" + pos + fn);

        }

        static void CelebaBetter()
        {
            string ss1;
            string ss2;
            string ss3;
            {
                FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\list_landmarks_celeba.txt", FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                ss1 = sr.ReadToEnd();
                sr.Close();
                fs.Close();
            }
            {
                FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\list_bbox_celeba.txt", FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                ss2 = sr.ReadToEnd();
                sr.Close();
                fs.Close();
            }
            {
                FileStream fs = new FileStream(@"K:\MNIST\CelebA\Anno\list_attr_celeba.txt", FileMode.Open, FileAccess.Read);
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

            for (int i = 041712; i < s1.Length - 2; i++)
            {
                int fn = i;

                string[] s2 = s1[i + 1].Split(' ');
                string[] sb2 = sB[i + 1].Split(' ');
                string[] sb3 = sB3[i + 1].Split(' ');
                string folder = "" + (i / 10000);

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
                    theta = Math.Atan((centex - centmx) / (centey - centmy));
                }
                else
                {
                    theta = 0;
                }

                if (sb3[11] == "1")
                {
                    continue;
                }
                if (centey > centmy)
                {
                    continue;
                }
                //整理歪脸度
                //白平衡 自动曝光

                //theta /= Math.PI / 2;
                //theta *= 90;

                Bitmap bmp = new Bitmap(@"K:\MNIST\CelebA\Img\img_celeba.7z\img_celeba\" + s2[0]);
                Graphics graph;

                //Graphics graph = Graphics.FromImage(bmp);
                //
                //Pen penw = new Pen(Color.White, 1.5f);
                //Pen penr = new Pen(Color.Red, 1.5f);
                //Pen penb = new Pen(Color.Blue, 1.5f);
                //Pen peng = new Pen(Color.Green, 1.5f);
                //
                //graph.DrawLine(penw, (float)lex, (float)ley, (float)rex, (float)rey);
                //graph.DrawLine(penr, (float)centex, (float)centey, (float)nx, (float)ny);
                //graph.DrawLine(penb, (float)lmx, (float)lmy, (float)rmx, (float)rmy);
                //if (flg5)
                //{
                //    graph.DrawLine(peng, (float)lex, (float)ley, (float)lmx, (float)lmy);
                //}
                //else
                //{
                //    graph.DrawLine(penr, (float)lex, (float)ley, (float)lmx, (float)lmy);
                //}
                //
                //if (flg6)
                //{
                //    graph.DrawLine(peng, (float)rex, (float)rey, (float)rmx, (float)rmy);
                //}
                //else
                //{
                //    graph.DrawLine(penr, (float)rex, (float)rey, (float)rmx, (float)rmy);
                //}
                //
                //graph.Flush();
                //graph.Dispose();

                bmp = GetRotateImage(bmp, theta, centfx, centfy);

                //double h = Math.Sqrt((centex - centmx) * (centex - centmx) + (centey - centmy) * (centey - centmy))*5;
                //double w = Math.Sqrt((lex - rex) * (lex - rex) + (ley - rey) * (ley - rey)) * 5;
                //w = h * 0.75;

                double bx1 = double.Parse(sb2[1]);
                double by1 = double.Parse(sb2[2]);
                double bx2 = double.Parse(sb2[3]);
                double by2 = double.Parse(sb2[4]);

                double h = by2 * 1.5;

                if (h < 180)
                {
                    continue;
                }
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


                Console.WriteLine((i + 1) + ":" + "\t" + folder);

                outbmp2.Save(@"K:\MNIST\CelebA\Img\img_celeba.7z\img_celebaProc\" + folder + "\\" + s2[0], ImageFormat.Bmp);
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

        /*
        static void CelebaHI()
        {
            FileStream fs = new FileStream(@"E:\MNIST\CelebA\Anno\list_attr_celeba.txt", FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);
            string ss = sr.ReadToEnd();
            sr.Close();
            fs.Close();
            ss = ss.Replace("  ", " ");
            ss = ss.Replace("  ", " ");
            ss = ss.Replace(" ", ",");

            var s1 = ss.Split('\n');
            int s1Index = 2;

            for (int i = 0; i < 49; i++)
            {
                s1Index = proc(s1, s1Index, i);
            }
        }

        static int proc(string[] s1, int s1Index, int num)
        {

            int OUTW = 160;
            int OUTH = 208;

            FileStream _out = new FileStream("K:\\MNIST\\celebaHI\\" + num + ".bin", FileMode.Create, FileAccess.Write);
            for (int i = 0; i < 4096; i++)//256*16
            {
                var s = s1[s1Index++];
                string filename = s.Split(',')[0];

                Bitmap bmp = (Bitmap)Bitmap.FromFile(@"E:\MNIST\img_align_celeba_total\" + filename);
                BitmapData bitmapData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, bmp.PixelFormat);

                byte[] Pixels = new byte[bmp.Width * bmp.Height * 3];
                Marshal.Copy(bitmapData.Scan0, Pixels, 0, Pixels.Length);
                bmp.UnlockBits(bitmapData);

                //Bitmap b2 = new Bitmap(OUTY,OUTX);

                int j = 0;//(bmp.Height - OUTY) / 2;
                int k = 0;//(bmp.Width - OUTX) / 2;

                byte[] buff = new byte[OUTW * OUTH * 3];

                for (int x = 0; x < OUTW; x++)
                {
                    for (int y = 0; y < OUTH; y++)
                    {
                        int datax = k + y;
                        int datay = j + x;
                        byte b = Pixels[datax * bitmapData.Stride + datay * 3 + 0];
                        byte g = Pixels[datax * bitmapData.Stride + datay * 3 + 1];
                        byte r = Pixels[datax * bitmapData.Stride + datay * 3 + 2];

                        buff[y * OUTW * 3 + x * 3 + 0] = r;
                        buff[y * OUTW * 3 + x * 3 + 1] = g;
                        buff[y * OUTW * 3 + x * 3 + 2] = b;

                        //b2.SetPixel(y, x, Color.FromArgb(b, g, r));
                    }
                }
                _out.Write(buff, 0, OUTH * OUTW * 3);
                _out.Flush();


                if (i % 64 == 0)
                {
                    Console.WriteLine(num + "," + i);
                }
                //b2.Save("e:\\MNIST\\fdsa"+i+".bmp");
            }
            _out.Close();
            return s1Index;
        }
         */
    }
}
