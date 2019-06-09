using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Stack
{
	public partial class Form1 : Form
	{

		public Form1()
		{
			InitializeComponent();
			this.textBox1.DragDrop += new DragEventHandler(Form1_DragDrop);
			this.textBox1.DragEnter += new DragEventHandler(Form1_DragEnter);
			this.textBox1.MouseWheel += textBox1_MouseWheel;
			this.vScrollBar1.ValueChanged += vScrollBar1_ValueChanged;
		}

		void vScrollBar1_ValueChanged(object sender, EventArgs e)
		{
			read(((long)this.vScrollBar1.Value) * 1024);
		}

		void textBox1_MouseWheel(object sender, MouseEventArgs e)
		{
			try
			{
				if (e.Delta > 0)
				{
					this.vScrollBar1.Value--;
				}
				else
				{
					this.vScrollBar1.Value++;
				}
			}
			catch { }
		}

		FileStream fs;

		private void Form1_Load(object sender, EventArgs e)
		{
			this.FormClosing += Form1_FormClosing;

		}
		void Form1_DragEnter(object sender, DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}
		void Form1_DragDrop(object sender, DragEventArgs e)
		{
			string[] files = (string[])e.Data.GetData("FileDrop");
			open(files[0]);
		}

		private void open(string file)
		{
			if (fs != null)
			{
				fs.Close();
			}
			fs = new FileStream(file, FileMode.Open, FileAccess.Read);
			this.vScrollBar1.Maximum = (int)(fs.Length / 1024 + 1);
			this.vScrollBar1.LargeChange = 1;
			this.vScrollBar1.SmallChange = 1;
			read(0);
		}

		void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (fs != null)
			{
				fs.Close();
			}
		}

		public void read(long pos)
		{
			label1.Text = vScrollBar1.Value + "/" + vScrollBar1.Maximum;
			if (fs != null)
			{
				fs.Seek(pos, SeekOrigin.Begin);

				byte[] buff = new byte[1024];

				fs.Read(buff, 0, 1024);
				StringBuilder sb = new StringBuilder();
				sb.Append("                 ");
				for (int j = 0; j < 16; j++)
				{
					sb.Append(String.Format("{0:X2}", j) + " ");
				}
				sb.Append("\r\n");
				for (long i = 0; i < 64; i++)
				{
					sb.Append(String.Format("{0:X16}", (pos + i * 16)) + " ");
					for (long j = 0; j < 16; j++)
					{
						int v = buff[i * 16 + j];
						sb.Append(String.Format("{0:X2}", v) + " ");
					}
					List<byte> temp = new List<byte>();
					for (long j = 0; j < 16; j++)
					{
						byte v = buff[i * 16 + j];
						if (v != 0 && v != 10 && v != 13)
						{
							temp.Add(v);
						}
					}
					sb.Append(Encoding.ASCII.GetString(temp.ToArray(), 0, temp.Count));
					sb.Append(Encoding.Default.GetString(temp.ToArray(), 0, temp.Count));
					sb.Append(Encoding.UTF8.GetString(temp.ToArray(), 0, temp.Count));
					sb.Append("\r\n");
				}

				this.textBox1.Text = sb.ToString();
			}
		}


		private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
		{
		}

		private void button1_Click(object sender, EventArgs e)
		{
			OpenFileDialog d = new OpenFileDialog();
			d.ShowDialog();
			if (d.FileNames.Length > 0)
			{
				open(d.FileNames[0]);
			}
		}
	}
}
