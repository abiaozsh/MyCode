using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;

namespace WindowsFormsApplication4
{
    public partial class Formana : Form
    {
        public Formana()
        {
            InitializeComponent();
        }



        static int T1 = 80;
        private static int pos = 0;
        private static int count8 = 0;
        private static int[] buff = new int[256];
        private static int sum8 = 0;
        private static int LOWSense = 0;
        private static int CoolDown = 0;

        private static void INIT()
        {
            for (int i = 0; i < 256; i++)
            {
                buff[i] = 512;
            }
        }
        private static int PROC(int val)
        {
            int ret = 0;
            sum8 += val;
            count8++;
            if (count8 == 8)
            {
                count8 = 0;
                buff[pos++] = sum8 / 8;
                pos &= 0x0FF;
                sum8 = 0;
                int total = 0;
                {
                    int idx = pos - 4;
                    idx &= 0x0FF;
                    for (int i = 0; i < 256 - 4; i++)
                    {
                        idx--;
                        idx &= 0x0FF;
                        total += buff[idx];
                    }
                    total /= 256 - 4;
                }

                {
                    int idx = pos;
                    int active = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        idx--;
                        idx &= 0x0FF;
                        if (buff[idx] - total > T1)
                            active++;
                    }
                    if (active >= 2)
                    {
                        LOWSense = 1;
                    }
                }

                if (LOWSense > 0)// && PEAKSense > 0
                {
                    if (CoolDown == 0)
                    {
                        CoolDown = 20;
                        ret = 1;
                    }
                }

                if (LOWSense > 0)
                    LOWSense--;
                if (CoolDown > 0)
                    CoolDown--;
            }
            return ret;
        }







        static int len = 2857502;
        int[] datas = new int[len];
        int[] phase = new int[len];

        double start = 0;
        double end = 2048;
        private void button1_Click(object sender, EventArgs e)
        {
            string file = @"f:\data.txt";
            INIT();

            LoadData(file);
            Draw();

            pictureBox1.MouseMove += new MouseEventHandler(pictureBox1_MouseMove);
            pictureBox1.MouseDown += new MouseEventHandler(pictureBox1_MouseDown);
            pictureBox1.MouseUp += new MouseEventHandler(pictureBox1_MouseUp);
            pictureBox1.MouseWheel += new MouseEventHandler(pictureBox1_MouseWheel);
            this.MouseWheel += new MouseEventHandler(Form2_MouseWheel);
        }

        void Form2_MouseWheel(object sender, MouseEventArgs e)
        {
            double ms = end - start;
            ms *= 0.1;
            if (e.Delta > 0)
            {
                start = start + ms;
                end = end - ms;
            }
            else
            {
                start = start - ms;
                end = end + ms;
            }
            Draw();

        }

        void LoadData(string file)
        {
            FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);
            int p = 1;
            int avg = 0;
            int lastavg = 0;
            for (int i = 0; i < len; i++)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                long val;
                long.TryParse(line, out val);

                int ret = 0;
                if (val <= 0 || val >= 1024)
                {
                }
                else
                {
                    ret = PROC((int)val);
                }
                avg += (int)val;
                if ((i & 7) == 0)
                {
                    lastavg = avg / 8;
                    avg = 0;
                }
                datas[i] = lastavg;
                if (ret > 0)
                {
                    p = -p;
                }
                phase[i] = p;
            }
            fs.Close();
        }

        void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
        {
        }

        void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            double ms = end - start;
            ms = ms * (oldx - e.X) / 1024;
            start = start + ms;
            end = end + ms;
            Draw();
        }

        int oldx;
        void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            oldx = e.X;

            double ms = end - start;
            double currentms = start + ms * e.X / 1024.0;
            if (currentms > 0 && currentms < len)
            {
                textBox1.Text = datas[(int)currentms].ToString() + "," + currentms;
            }
        }

        void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            double ms = end - start;
            double currentms = start + ms * e.X / 1024.0;
            if (currentms > 0 && currentms < len)
            {
                textBox1.Text = datas[(int)currentms].ToString() + "," + currentms; ;
            }
        }


        double[] tempData = new double[1024];

        void Form2_KeyPress(object sender, KeyPressEventArgs e)
        {
        }

        void Draw()
        {
            Bitmap b = new Bitmap(1024, 500);
            Graphics g = Graphics.FromImage(b);
            Pen p1 = new Pen(Color.Red);
            Pen p2 = new Pen(Color.Green);

            double ms = end - start;
            for (int i = 0; i < 1024; i++)
            {
                double currentms = ms * i / 1024.0;
                double nextms = ms * (i + 1) / 1024.0;

                int idx1 = (int)(start + currentms);
                int idx2 = (int)(start + nextms);
                if (idx1 > 0 && idx1 < len && idx2 > 0 && idx2 < len)
                {
                    int val = 0;
                    for (int j = idx1; j <= idx2; j++)
                    {
                        val += datas[j];
                    }
                    val /= idx2 - idx1 + 1;

                    if (phase[idx1] > 0)
                    {
                        g.DrawLine(p1, i, 500, i, val / 2);
                    }
                    else
                    {
                        g.DrawLine(p2, i, 500, i, val / 2);
                    }
                }
            }
            g.Flush();
            pictureBox1.Image = b;
        }



        private void Form2_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

    }
}
