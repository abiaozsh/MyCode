using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using System.Threading;
using System.Net;
using System.Diagnostics;
using System.Data.Common;
using System.Xml.Serialization;

namespace LogTool
{

    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        Core c = new Core();

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {

            c.init();

        }

        List<Core.Job> list = new List<Core.Job>();

        private void button2_Click(object sender, EventArgs e)
        {
			DateTime start = DateTime.Parse(textBox1.Text);
			DateTime end = DateTime.Parse(textBox2.Text);

			for (DateTime date = start; date < end; date = date.AddMinutes(10))
            {
                Core.Job job = new Core.Job(c);
                job.target = date;
                list.Add(job);
            }
        }
        bool started = false;
        private void button3_Click(object sender, EventArgs e)
        {
            started = true;
        }

        int retry = 0;
        long sumSpeed = 0;

        long lastIndex = 0;
        int idx = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            long Index = 0;
            this.listBox1.Items.Clear();
            int totalActive = 0;

            Core.Job firstFree = null;
            foreach (var item in list)
            {
                if (item.status != 9)
                {
                    this.listBox1.Items.Add(item);
                }
                Index += item.index;
                if (item.status == 1)
                {
                    totalActive++;
                }
                if ((item.status == 0 || item.status == 2) && firstFree == null)
                {
                    firstFree = item;
                }
            }

            if (totalActive < 1 && started && firstFree != null)
            {
                if (firstFree.status == 2)
                {
                    retry++;
                }
                firstFree.start();
            }

            long speed = Index - lastIndex;

            lastIndex = Index;

            sumSpeed += speed;

            this.Text = speed.ToString("###,###");

            idx++;
            if (idx == 60)
            {
                idx = 0;
                FileStream fs = new FileStream("log.txt", FileMode.Append, FileAccess.Write);
                StreamWriter sw = new StreamWriter(fs);
                sw.WriteLine(DateTime.Now.ToString() + "\tr:" + retry + "\ts:" + (sumSpeed / 60));
                sw.Flush();
                fs.Flush();
                sw.Close();
                fs.Close();

                retry = 0;
                sumSpeed = 0;
            }

        }

		private void button4_Click(object sender, EventArgs e)
		{
			DateTime start = DateTime.Parse(textBox1.Text);
			DateTime end = DateTime.Parse(textBox2.Text);

			for (DateTime date = start; date < end; date = date.AddDays(1))
			{
				Core.Job job = new Core.Job(c);
				job.target = date;
				list.Add(job);
			}
		}



    }

}
