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
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}

		Img img1;
		Img img2;

		private void Form2_Load(object sender, EventArgs e)
		{
			img1 = new Img(pictureBox1, z => { return z; }, true);
			img1.hRefresh += () => { img2.refresh(); };
			img2 = new Img(pictureBox2, z => { return z * img1.vect; });
			img1.refresh();
			img2.refresh();
		}
	}
}
