using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ICSharpCode.SharpZipLib.Zip;
using System.IO;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    class Program
    {
        static System.Object Lock = new System.Object();

        static string root = @"O:\MTSAT\";
        static string outdir = @"E:\MTSAT\";

        public static void proc(DateTime date, int x1, int y1, int x2, int y2, double resize)
        {
            if (root == "") return;

            Image img = null;

            int w = x2 - x1;
            int h = y2 - y1;
            int w2 = (int)((double)w * resize);
            int h2 = (int)((double)h * resize);

            //hima820180101
            try
            {
                //lock (Lock)
                {
                    using (var input = new FileStream(root + "hima8" + date.ToString("yyyyMMdd") + "\\hima8" + date.ToString("yyyyMMddHH") + ".zip", FileMode.Open, FileAccess.Read))
                    {
                        using (ZipInputStream zip = new ZipInputStream(input))
                        {
                            while (true)
                            {
                                ZipEntry entry = zip.GetNextEntry();
                                if (entry == null) break;
                                if (entry.Name.StartsWith("hima8" + date.ToString("yyyyMMddHHmm")))
                                {
                                    img = Bitmap.FromStream(zip);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
            Graphics graph;
            Bitmap outbmp2 = new Bitmap(w2, h2);
            if (img != null)
            {
                graph = Graphics.FromImage(outbmp2);
                graph.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.High;
                graph.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
                graph.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
                graph.SmoothingMode = SmoothingMode.AntiAlias;

                Rectangle src = new Rectangle(x1, y1, w, h);
                Rectangle dest = new Rectangle(0, 0, w2, h2);
                graph.DrawImage(img, dest, src, GraphicsUnit.Pixel);

                graph.Flush();
                graph.Dispose();
            }

            Console.WriteLine("hima8" + date.ToString("yyyyMMddHHmm") + ".png");
            outbmp2.Save(outdir + "hima8" + date.ToString("yyyyMMddHHmm") + ".png", ImageFormat.Png);
            outbmp2.Dispose();
        }
        static void Main(string[] args)
        {
            try
            {
                List<DateTime> list = new List<DateTime>();

                for (DateTime d = DateTime.Parse("2018/01/01 00:00:00"); d < DateTime.Parse("2018/01/05 00:00:00"); d = d.AddMinutes(10))
                {
                    list.Add(d);
                }

                //Parallel.ForEach(list, d =>
                foreach (var d in list)
                {
                    //proc(d, 5140, 10720, 5620, 10940, 1);
                    proc(d, 0, 0, 11000, 11000, 0.125);
                }//);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

            Console.WriteLine("done");
            Console.ReadLine();

        }



    }
}
