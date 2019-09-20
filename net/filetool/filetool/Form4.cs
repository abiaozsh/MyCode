using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FileTool
{
	public partial class Form4 : Form
	{
		public Form4()
		{
			InitializeComponent();
		}

		public string path;
		private void button2_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			path = this.textBox1.Text;
			this.Close();
		}

		private void Form4_Load(object sender, EventArgs e)
		{
			this.AcceptButton = this.button1;
			this.CancelButton = this.button2;
			this.textBox1.Text = path;
			this.textBox1.Focus();
		}
	}
}
