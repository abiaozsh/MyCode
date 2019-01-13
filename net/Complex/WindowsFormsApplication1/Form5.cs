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
	public partial class Form5 : Form
	{
		public Form5()
		{
			InitializeComponent();
		}
		Img img1;
		Img img2;
		Img img3;
		Img img4;
		Img img5;
		Img img6;
		Img img7;
		Img img8;

		private void Form5_Load(object sender, EventArgs e)
		{
			img1 = new Img(pictureBox1, z => { return z; }, true);
			img2 = new Img(pictureBox2, z => { return z; }, true);
			img3 = new Img(pictureBox3, z => { return z; }, true);
			img4 = new Img(pictureBox4, z => { return z; }, true);

			img1.hRefresh += () => { img5.refresh(); img6.refresh(); img7.refresh(); img8.refresh(); };
			img2.hRefresh += () => { img5.refresh(); img6.refresh(); img7.refresh(); img8.refresh(); };
			img3.hRefresh += () => { img5.refresh(); img6.refresh(); img7.refresh(); img8.refresh(); };
			img4.hRefresh += () => { img5.refresh(); img6.refresh(); img7.refresh(); img8.refresh(); };

			img5 = new Img(pictureBox5, z => { return img1.vect; });
			img6 = new Img(pictureBox6, z => { return img1.vect + img2.vect * z; });
			img7 = new Img(pictureBox7, z => { return img1.vect + img2.vect * z + (img3.vect * z * z); });
			img8 = new Img(pictureBox8, z => { return img1.vect + img2.vect * z + (img3.vect * z * z) + (img4.vect * z * z * z); });

			img1.refresh();
			img2.refresh();
			img3.refresh();
			img4.refresh();
			img5.refresh();
			img6.refresh();
			img7.refresh();
			img8.refresh();

		}
	}
}
