﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;

namespace WindowsFormsApplication1
{

	public partial class Form1 : Form
	{
		SerialPort port;
		private delegate void FlushClient(string s);
		public Form1()
		{
			InitializeComponent();
		}

		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
		}

		class FileItem
		{
			public string filename;
			public TimeSpan time;
		}
		List<FileItem> files;

		private void Form1_Load(object sender, EventArgs e)
		{

			this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);


			//fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
			//sw = new StreamWriter(fs);
			if (port == null)
			{
				try
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM5", 250000, Parity.None, 8, StopBits.One);
					port.Open();
				}
				catch
				{ }
			}

		}

		public void send(byte[] data)
		{
			try
			{
				port.Write(data, 0, 32);
			}
			catch
			{
				//this.Text = "error";
			}
		}

		DateTime cur;
		private void button1_Click(object sender, EventArgs e)
		{
			files = new List<FileItem>();
			string folder = "e:\\img\\";
			var _files = System.IO.Directory.GetFiles(folder);
			foreach (var file in _files)
			{
				FileItem fi = new FileItem();
				fi.filename = file;

				int m = int.Parse(file.Substring(folder.Length + 4, 2));
				int s = int.Parse(file.Substring(folder.Length + 6, 2));
				int ms = int.Parse(file.Substring(folder.Length + 9, 3));
				fi.time = new TimeSpan(0, 0, m, s, ms);
				files.Add(fi);
			}

			files.Sort(delegate(FileItem a, FileItem b)
			{
				return a.time.CompareTo(b.time);
			});
			cur = DateTime.Now;
			timer1.Enabled = true;
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			TimeSpan ts = DateTime.Now - cur;
			if (ts > files[0].time){
				proc(files[0].filename);
				files.RemoveAt(0);
			}
		}

		void proc(string file)
		{
			Bitmap bmp = (Bitmap)Image.FromFile(file);
			Bitmap bmp1 = new Bitmap(32, 24);
			Bitmap bmpout = new Bitmap(16, 16);
			pictureBox1.Image = bmp;
			this.Text = file;
			using (Graphics g = Graphics.FromImage(bmp1))
			{
				g.DrawImage(bmp, 0, 0, new Rectangle(0, 4, 32, 28), GraphicsUnit.Pixel);
			}
			using (Graphics g = Graphics.FromImage(bmpout))
			{
				g.DrawImage(bmp1, 0, 0, 16, 16);
			}

			pic2txt(bmpout, true);
		}
		void pic2txt(Bitmap bmp, bool flip)
		{
			byte[] data = new byte[32];
			String s = "";
			int idx = 0;
			for (int j = 0; j < 16; j++)
			{

				int v1 = 0;
				for (int i = 0; i < 8; i++)
				{
					if (flip)
					{
						v1 += ((bmp.GetPixel(i + 0, j).R < 128) ? 0 : (1 << i));
					}
					else
					{
						v1 += ((bmp.GetPixel(i + 0, j).R > 128) ? 0 : (1 << i));
					}
				}
				int v2 = 0;
				for (int i = 0; i < 8; i++)
				{
					if (flip)
					{
						v2 += ((bmp.GetPixel(i + 8, j).R < 128) ? 0 : (1 << i));
					}
					else
					{
						v2 += ((bmp.GetPixel(i + 8, j).R > 128) ? 0 : (1 << i));
					}
				}
				data[idx++] = (byte)v1;
				data[idx++] = (byte)v2;
			}

			send(data);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			timer1.Enabled = false;
			pic2txt((Bitmap)Image.FromFile("e:\\img\\lm\\l.png"), false);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			timer1.Enabled = false;
			pic2txt((Bitmap)Image.FromFile("e:\\img\\lm\\m.png"), false);
		}
	}
}


