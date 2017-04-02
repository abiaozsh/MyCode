using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;

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
            string dir = @"d:\data\";
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
            //fs.ReadByte();
            while (true)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                if (line.StartsWith("\0"))
                {
                    line = line.Substring(1);
                }
                long val = long.Parse(line.Trim());

                Data d;
                d.t = longToDate(val);
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
            if (isShiftDown())
            {
                double ms = end - start;
                double oldms = start + ms * oldx / 1024.0;
                double currentms = start + ms * e.X / 1024.0;

                double power = calcPower(oldms, currentms);
                double kwh = power * (currentms - oldms) / 3600000000;
                textBox1.Text = power.ToString("0.00") + "w  " + kwh.ToString("0.00") + "kwh";
            }
            else
            {
                double ms = end - start;
                ms = ms * (oldx - e.X) / 1024;
                start = start + ms;
                end = end + ms;
                Draw();
            }
        }

        int oldx;
        void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            oldx = e.X;
            double ms = end - start;
            double currentms = start + ms * e.X / 1024.0;
            DateTime current = longToDate(datas[FindTop(currentms)].raw);
            textBox1.Text = current.ToString("MM-dd HH:mm:ss fff") + "  " + datas[FindTop(currentms)].raw.ToString();
        }

        void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            double ms = end - start;
            double currentms = start + ms * e.X / 1024.0;
            DateTime current = longToDate(currentms);
            label1.Text = current.ToString("MM-dd HH:mm:ss fff") + "\r\n" + tempData[e.X].ToString();
            label1.Top = e.Y;
            label1.Left = e.X;
        }
        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        [DllImport("user32.dll", EntryPoint = "GetKeyboardState")]
        public static extern int GetKeyboardState(byte[] pbKeyState);

        public bool isShiftDown()
        {
            byte[] buf = new byte[256];
            GetKeyboardState(buf);
            return (buf[160] & 128) == 128;
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


        private void textBox2_TextChanged(object sender, EventArgs e)
        {


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
                    d.t = longToDate(val);
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

        private void button3_Click(object sender, EventArgs e)
        {
            string dir = @"d:\data\temp\";
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

            SaveData(@"D:\data\201607.txt");
        }

        private void SaveData(string file)
        {
            FileStream fs = new FileStream(file, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);
            foreach (var item in datas)
            {
                sw.WriteLine(item.raw);
            }
            sw.Flush();
            fs.Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
        }

        private void button4_Click(object sender, EventArgs e)
        {
            long start = 1462065981357;//04-17 09:22:27 337   src
            long end = 1462066260144;//04-17 09:24:38 385

            long start1 = 1462066392724;//04-17 09:24:56 202  dst
            long end1 = 1462066631949;//04-17 09:25:41 316
            FileStream fs = new FileStream(@"F:\data\2016_05_01.txt", FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            while (true)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                long val = long.Parse(line);

                Data d;
                d.t = longToDate(val);
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

            double power = calcPower(start, end);

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

        public double calcPower(double start, double end)
        {
            int idxs = FindTop(start);
            int idxe = FindBottom(end);

            double power = 0;
            if (idxs == -1)
            {
                idxs = 0;
            }
            if (idxe == -1)
            {
                idxe = datas.Count - 1;
            }
            long diff = datas[idxe].raw - datas[idxs].raw;
            power = 2250000.0 / (diff / (idxe - idxs));
            return power;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {

        }

        public DateTime longToDate(long val)
        {
            return new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(val);
        }
        public DateTime longToDate(double val)
        {
            return new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(val);
        }

        public long DateToLong(DateTime date)
        {
            return (long)((date - new DateTime(1970, 1, 1).AddHours(8)).TotalMilliseconds);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            //report(@"d:\data\201605.txt", @"d:\data\report5.txt", "2016-05-01", "2016-06-01");
            //report(@"d:\data\201606.txt", @"d:\data\report6.txt", "2016-06-01", "2016-07-01");
            report(@"d:\data\201607.txt", @"d:\data\report7.txt", "2016-07-01", "2016-08-01");
        }

        private void report(string infile, string outfile, string mstart, string mend)
        {
            long val = DateToLong(longToDate(1462032074733));

            LoadData(infile);

            DateTime startdate = DateTime.Parse(mstart);
            DateTime enddate = DateTime.Parse(mend);
            double price = 0;
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("date\tw\tkwh\tpriceL\tpriceH");
            for (DateTime d = startdate; d < enddate; d = d.AddDays(1))
            {
                double pricel = 0;
                double priceh = 0;
                {//0~6
                    double oldms = DateToLong(d);
                    double currentms = DateToLong(d.AddHours(6));

                    double power = calcPower(oldms, currentms);
                    double kwh = power * (currentms - oldms) / 3600000000;
                    pricel += kwh * 0.3;
                }
                {//6~22
                    double oldms = DateToLong(d.AddHours(6));
                    double currentms = DateToLong(d.AddHours(22));

                    double power = calcPower(oldms, currentms);
                    double kwh = power * (currentms - oldms) / 3600000000;
                    priceh += kwh * 0.61;
                }
                {//22~24
                    double oldms = DateToLong(d.AddHours(22));
                    double currentms = DateToLong(d.AddDays(1));

                    double power = calcPower(oldms, currentms);
                    double kwh = power * (currentms - oldms) / 3600000000;
                    pricel += kwh * 0.3;
                }
                {
                    double oldms = DateToLong(d);
                    double currentms = DateToLong(d.AddDays(1));

                    double power = calcPower(oldms, currentms);
                    double kwh = power * (currentms - oldms) / 3600000000;
                    sb.AppendLine(d.ToString("MM-dd") + "\t" + power.ToString("0.00") + "\t" + kwh.ToString("0.00") + "\t" + pricel.ToString("0.00") + "\t" + priceh.ToString("0.00"));
                }
                price += priceh + pricel;
            }

            {
                double oldms = DateToLong(startdate);
                double currentms = DateToLong(enddate);

                double power = calcPower(oldms, currentms);
                double kwh = power * (currentms - oldms) / 3600000000;
                sb.AppendLine(startdate.ToString() + "\t" + power.ToString("0.00") + "\t" + kwh.ToString("0.00") + "\t" + price);
            }

            FileStream fs = new FileStream(outfile, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);

            sw.Write(sb.ToString());
            sw.Flush();
            sw.Close();

        }

    }
}
