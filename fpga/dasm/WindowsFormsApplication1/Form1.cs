using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;
using System.Globalization;

namespace WindowsFormsApplication1
{

	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		private void button1_Click(object sender, EventArgs e)
		{
			FileStream fs = new FileStream(@"D:\MyCode.github\fpga\uart_adv_test\software\temp\temp.elf", FileMode.Open, FileAccess.Read);
			BinaryReader br = new BinaryReader(fs);
			//byte[] buff = new byte[fs.Length];
			//fs.Read(buff,0,(int)fs.Length);
			var head = ELF.ReadHead(br);
			fs.Close();

			this.textBox1.Text = head.pheads[0].dasm;

			this.textBox1.Text += head.pheads[1].dasm;


		}

	}
}
