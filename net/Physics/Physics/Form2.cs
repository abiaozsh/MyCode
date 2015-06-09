using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace physics
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        float x1;
        float y1;

        void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            x1 = e.X;
            y1 = i.Height - e.Y;
        }
        Image i = new Bitmap(300, 200);

        void pictureBox1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {

            int x2 = e.X;
            int y2 = i.Height - e.Y;

            Graphics g = Graphics.FromImage(i);

            //g.Clear(Color.White);

            g.DrawLine(Pens.Black, x1, i.Height - y1, x2, i.Height - y2);

            textBox1.Text += "l = new PLine();\r\n" +
            "l.x1 = " + x1 + ";\r\n" +
            "l.y1 = " + y1 + ";\r\n" +
            "l.x2 = " + x2 + ";\r\n" +
            "l.y2 = " + y2 + ";\r\n" +
            "lineList.Add(l);\r\n\r\n";

            pictureBox1.Image = i;

        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

    }
}