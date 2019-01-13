using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
	public partial class Form6 : Form
	{
		public Form6()
		{
			InitializeComponent();
		}
		Img img1;
		Img img2;
		Img img3;
		Img img4;
		Img img5;

		private void Form6_Load(object sender, EventArgs e)
		{

			img1 = new Img(pictureBox1, z => { return 1 / (1 + (z * z)); });
			img2 = new Img(pictureBox2, z => { return 1 / (1 - (z * z)); });
			img3 = new Img(pictureBox3, z => { return Complex.sin(z); });
			img4 = new Img(pictureBox4, z => { return Complex.e(z); });
			img5 = new Img(pictureBox5, z => { return Complex.log(z); });

			img1.refresh();
			img2.refresh();
			img3.refresh();
			img4.refresh();
			img5.refresh();

		}
	}
}
