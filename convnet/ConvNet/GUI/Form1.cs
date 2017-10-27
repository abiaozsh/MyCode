using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;

namespace GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        MNIST mnist = new MNIST();
        MNIST._2D_ImgNet net2d2ImgTester;
        MNIST._2D_10Net net2d210Tester;
        MNIST.Img_2DNet img_2d;

        Bitmap baseimg;

        byte[] bigImg;
        int bigImgW;

        private void Form1_Load(object sender, EventArgs e)
        {

            label1.ForeColor = MNIST.cDict[0];
            label2.ForeColor = MNIST.cDict[1];
            label3.ForeColor = MNIST.cDict[2];
            label4.ForeColor = MNIST.cDict[3];
            label5.ForeColor = MNIST.cDict[4];
            label6.ForeColor = MNIST.cDict[5];
            label7.ForeColor = MNIST.cDict[6];
            label8.ForeColor = MNIST.cDict[7];
            label9.ForeColor = MNIST.cDict[8];
            label10.ForeColor = MNIST.cDict[9];


            Bitmap bmp = (Bitmap)Bitmap.FromFile(@"E:\MNIST\无标题.png");

            pictureBox2.Image = bmp;
            pictureBox2.Width = bmp.Width;
            pictureBox2.Height = bmp.Height;

            pictureBox2.MouseMove += pictureBox2_MouseMove;
            pictureBox3.MouseMove += pictureBox3_MouseMove;

            bigImg = new byte[bmp.Width * bmp.Height];
            bigImgW = bmp.Width;

            try
            {
                for (int j = 0; j < bmp.Height; j++)
                {
                    for (int i = 0; i < bmp.Width; i++)
                    {
                        bigImg[j * bigImgW + i] = bmp.GetPixel(i, j).R;
                    }
                }
            }
            catch (Exception ex)
            {
                ex.ToString();
            }


            img_2d = new MNIST.Img_2DNet();
            img_2d.init();

            net2d2ImgTester = new MNIST._2D_ImgNet();
            net2d2ImgTester.init();

            net2d210Tester = new MNIST._2D_10Net();
            net2d210Tester.init();

            baseimg = img_2d.display(5000);
            pictureBox3.Image = baseimg;
        }

        Vol v = new Vol(28, 28, 1, 0.0f);
        public Vol _getImg(int x, int y)
        {
            for (var i = 0; i < 28; i++)
            {
                for (var j = 0; j < 28; j++)
                {
                    int idx = (i + x) + (j + y) * bigImgW;
                    int val = 0;
                    if (idx < bigImg.Length)
                    {
                        val = bigImg[idx];
                    }
                    v.w[i + j * 28] = (float)val / 255.0f;
                }
            }
            return v;
        }
        public Bitmap _getBmp(int x, int y)
        {
            Bitmap b = new Bitmap(28, 28);

            for (int i = 0; i < 28; i++)
            {
                for (int j = 0; j < 28; j++)
                {
                    int idx = (i + x) + (j + y) * bigImgW;
                    int val = 0;
                    if (idx < bigImg.Length)
                    {
                        val = bigImg[idx];
                    }
                    b.SetPixel(i, j, Color.FromArgb(val, val, val));
                }
            }
            return b;
        }

        void pictureBox2_MouseMove(object sender, MouseEventArgs e)
        {

            pictureBox1.Image = _getBmp(e.X, e.Y);

            var img = _getImg(e.X, e.Y);

            var v2d = img_2d.forward(img);

            var vimg = net2d2ImgTester.forward(v2d);

            var v10 = net2d210Tester.forward(v2d);

            pictureBox4.Image = vimg.vis(0);

            String s = MNIST.report(v10, -1, -1);
            textBox2.Text = (s);

        }


        void pictureBox3_MouseMove(object sender, MouseEventArgs e)
        {
            pictureBox4.Image = net2d2ImgTester.proc(e.X, e.Y);

            textBox2.Text = net2d210Tester.proc(e.X, e.Y);
        }

        private void button1_Click(object sender, EventArgs e)
        {
        }

        private void button2_Click(object sender, EventArgs e)
        {
        }

        private void button3_Click(object sender, EventArgs e)
        {
            pictureBox3.Image = mnist.display();

            //{
            //	string[] files = Directory.GetFiles(@"D:\work\MyCode\convnet\ConvNet\Release");
            //	 Parallel.ForEach(files,(file) =>
            //	{
            //		var mnist = new MNIST();
            //		mnist.Init();
            //		if (file.EndsWith(".txt"))
            //		{
            //			FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read);
            //			StreamReader sr = new StreamReader(fs);
            //			mnist.net.load(sr);
            //			fs.Close();
            //			var b = mnist.display();
            //			b.Save(file + ".png", ImageFormat.Png);
            //		}
            //	});
            //}

            //int x = 96;
            //int y = 15;
            //
            //pictureBox1.Image = _getBmp(x, y);
            //var img = _getImg(x, y);
            //var v1 = mnist.net.forward(img);
            //String s = mnist.report(v1);
            //textBox1.Text = (s);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //for (int i = 0; i < 10; i++)
            //{
            //	Stopwatch sw = new Stopwatch();
            //	sw.Start();
            //	mnist.mainNet.train();
            //	sw.Stop();
            //	textBox1.Text += sw.ElapsedTicks + "\r\n";
            //
            //}


        }

        private void button5_Click(object sender, EventArgs e)
        {
            //Util.save("2Dnet.txt", (sw) =>
            //{
            //	mnist.classifierNet.fc144.save(sw);
            //	mnist.classifierNet.fc2.save(sw);
            //});
        }

    }
}
