using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WindowsFormsApplication4
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        struct Data
        {
            public DateTime t;
            public double p;
            public long raw;
            public long glitch;
            public long range;
        }

        List<Data> datas = new List<Data>();
        double start;
        double end;

        private void button1_Click(object sender, EventArgs e)
        {
            string dir = @"f:\data\";
            datas = new List<Data>();
            string[] files = Directory.GetFiles(dir);

            foreach (string file in files)
            {
                LoadData(file);
            }

            datas.Sort(delegate(Data a, Data b)
            {
                return a.raw.CompareTo(b.raw);
            });

            long lastval = 0;
            for (int i = 0; i < datas.Count; i++)
            {
                Data d = datas[i];
                d.p = 2250000.0 / (d.raw - lastval);// (w)
                datas[i] = d;
                lastval = d.raw;
            }




            start = datas[0].raw;
            end = datas[datas.Count - 1].raw;

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
            if (!file.EndsWith(".txt")) return;
            FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);
            fs.ReadByte();
            while (true)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                long val = long.Parse(line.Trim());

                Data d;
                d.t = new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(val);
                d.raw = val;
                d.p = 0;
                d.range = 0;
                d.glitch = 0;

                datas.Add(d);
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
            DateTime current = new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(datas[FindTop(currentms)].raw);
            textBox1.Text = current.ToString("MM-dd HH:mm:ss fff") + "  " + datas[FindTop(currentms)].raw.ToString();
        }

        void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            double ms = end - start;
            double currentms = start + ms * e.X / 1024.0;
            DateTime current = new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(currentms);
            label1.Text = current.ToString("MM-dd HH:mm:ss fff") + "\r\n" + tempData[e.X].ToString();
            label1.Top = e.Y;
            label1.Left = e.X;
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

                double current = start + currentms;

                double power = 0;
                int idxs = FindTop(start + currentms);
                int idxe = FindBottom(start + nextms);

                if (idxs != -1 && idxe != -1)
                {
                    long diff = datas[idxe].raw - datas[idxs].raw;
                    power = 2250000.0 / (diff / (idxe - idxs));
                }

                tempData[i] = power;

                if (((long)Math.Floor(((currentms + start) / 1000 / 3600 + 8) / 24)) % 2 == 0)
                {
                    g.DrawLine(p1, i, 500, i, (float)(500 - power / 10));
                }
                else
                {
                    g.DrawLine(p2, i, 500, i, (float)(500 - power / 10));
                }
            }
            g.Flush();
            pictureBox1.Image = b;
        }

        private int FindTop(double val)
        {
            int idx = datas.Count / 2;
            int max = datas.Count;
            int min = 0;
            while (true)
            {
                if (idx < 0 || idx + 1 >= datas.Count)
                {
                    return -1;
                }
                if (val > datas[idx].raw && val <= datas[idx + 1].raw)
                {
                    return idx;
                }
                else if (val <= datas[idx].raw)
                {
                    int tmp = (min + idx) / 2;
                    max = idx;
                    if (tmp == idx) return -1;
                    idx = tmp;
                }
                else if (val > datas[idx + 1].raw)
                {
                    int tmp = (max + idx) / 2;
                    min = idx;
                    if (tmp == idx) return -1;
                    idx = tmp;
                }
            }
        }
        private int FindBottom(double val)
        {
            int idx = datas.Count / 2;
            int max = datas.Count;
            int min = 0;
            while (true)
            {
                if (idx - 1 < 0 || idx >= datas.Count)
                {
                    return -1;
                }
                if (val < datas[idx].raw && val >= datas[idx - 1].raw)
                {
                    return idx;
                }
                else if (val >= datas[idx].raw)
                {
                    int tmp = (max + idx) / 2;
                    min = idx;
                    if (tmp == idx) return -1;
                    idx = tmp;
                }
                else if (val < datas[idx - 1].raw)
                {
                    int tmp = (min + idx) / 2;
                    max = idx;
                    if (tmp == idx) return -1;
                    idx = tmp;
                }
            }
        }



        private void Form2_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

            long start = 1460856147337;//04-17 09:22:27 337  
            long end = 1460856278385;//04-17 09:24:38 385

            long start1 = 1460856296202;//04-17 09:24:56 202
            long end1 = 1460856341316;//04-17 09:25:41 316
            FileStream fs = new FileStream(@"e:\data\0417.txt", FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            while (true)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                long val = long.Parse(line);

                Data d;
                d.t = new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(val);
                d.raw = val;
                d.p = 0;
                d.range = 0;
                d.glitch = 0;

                datas.Add(d);
            }
            fs.Close();
            datas.Sort(delegate(Data a, Data b)
            {
                return a.raw.CompareTo(b.raw);
            });

            long lastval = 0;
            for (int i = 0; i < datas.Count; i++)
            {
                Data d = datas[i];
                d.p = 2250000.0 / (d.raw - lastval);// (w)
                datas[i] = d;
                lastval = d.raw;
            }

            int idxs = FindTop(start);
            int idxe = FindBottom(end);

            double power = 0;
            if (idxs != -1 && idxe != -1)
            {
                long diff = datas[idxe].raw - datas[idxs].raw;
                power = 2250000.0 / (diff / (idxe - idxs));
            }

            double diff2 = 2250000.0 / power;

            long cnt = (long)((end1 - start1) / diff2);

            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < cnt; i++)
            {
                long val = start1 + ((end1 - start1) * i / cnt);
                sb.AppendLine(val.ToString());
            }
            textBox2.Text = sb.ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string[] data = Form1.DATA.Split('\n');
            datas = new List<Data>();
            foreach (string line in data)
            {
                long val;
                long.TryParse(line.Trim(), out val);
                if (val != 0)
                {
                    Data d;
                    d.t = new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(val);
                    d.raw = val;
                    d.p = 0;
                    d.range = 0;
                    d.glitch = 0;

                    datas.Add(d);
                }
            }

            datas.Sort(delegate(Data a, Data b)
            {
                return a.raw.CompareTo(b.raw);
            });

            long lastval = 0;
            for (int i = 0; i < datas.Count; i++)
            {
                Data d = datas[i];
                d.p = 2250000.0 / (d.raw - lastval);// (w)
                datas[i] = d;
                lastval = d.raw;
            }




            start = datas[0].raw;
            end = datas[datas.Count - 1].raw;

            Draw();

            pictureBox1.MouseMove += new MouseEventHandler(pictureBox1_MouseMove);
            pictureBox1.MouseDown += new MouseEventHandler(pictureBox1_MouseDown);
            pictureBox1.MouseUp += new MouseEventHandler(pictureBox1_MouseUp);
            pictureBox1.MouseWheel += new MouseEventHandler(pictureBox1_MouseWheel);
            this.MouseWheel += new MouseEventHandler(Form2_MouseWheel);

        }

    }
}
