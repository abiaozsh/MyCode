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
	public partial class Form3 : Form
	{
		public Form3()
		{
			InitializeComponent();
		}
		public string text;
		private void Form3_Load(object sender, EventArgs e)
		{
		this.textBox1.Text = text;
		}
	}
}
