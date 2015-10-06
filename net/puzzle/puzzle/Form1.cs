using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace puzzle
{
    public partial class Form1 : Form
    {
        Bitmap bmp = new Bitmap(200, 200);
        Graphics g;
        ulong data = 0;
        public Form1()
        {
            InitializeComponent();
            pictureBox1.Image = bmp;
            g = Graphics.FromImage(bmp);
            g.Clear(Color.Gray);
            Pen p = new Pen(Color.Black);
            for (int i = 1; i < 10; i++)
            {
                g.DrawLine(p, i * 20, 0, i * 20, 220 - i * 20);
            }
            for (int i = 1; i < 10; i++)
            {
                g.DrawLine(p, 0, i * 20, 220 - i * 20, i * 20);
            }
            g.Flush();
            pictureBox1.Refresh();
        }

        int[] conv = {
             0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            10,11,12,13,14,15,16,17,18,-1,
            19,20,21,22,23,24,25,26,-1,-1,
            27,28,29,30,31,32,33,-1,-1,-1,
            34,35,36,37,38,39,-1,-1,-1,-1,
            40,41,42,43,44,-1,-1,-1,-1,-1,
            45,46,47,48,-1,-1,-1,-1,-1,-1,
            49,50,51,-1,-1,-1,-1,-1,-1,-1,
            52,53,-1,-1,-1,-1,-1,-1,-1,-1,
            54,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        };
        void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            int x = e.X / 20;
            int y = e.Y / 20;

            if (conv[x + y * 10] != -1)
            {
                data = data ^ ((ulong)1 << conv[x + y * 10]);
            }
            refresh();

        }

        void refresh()
        {
            Pen p1 = new Pen(Color.Red);
            Pen p2 = new Pen(Color.Gray);
            Brush b1 = new SolidBrush(Color.Red);
            Brush b2 = new SolidBrush(Color.Gray);

            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (conv[i + j * 10] != -1)
                    {
                        if ((data & ((ulong)1 << conv[i + j * 10])) != 0)
                        {
                            g.FillEllipse(b1, i * 20, j * 20, 19, 19);
                        }
                        else
                        {
                            g.FillEllipse(b2, i * 20, j * 20, 19, 19);
                        }
                    }
                }
            }
            g.Flush();
            pictureBox1.Refresh();
            textBox1.Text = getHex4(data);
        }

        static string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
        static string getHex4(ulong val)//"ll"+"hh"
        {
            return
                convt[((val & 0xF000000000000000) >> 60)] +
                convt[((val & 0x0F00000000000000) >> 56)] +
                convt[((val & 0x00F0000000000000) >> 52)] +
                convt[((val & 0x000F000000000000) >> 48)] +
                convt[((val & 0x0000F00000000000) >> 44)] +
                convt[((val & 0x00000F0000000000) >> 40)] +
                convt[((val & 0x000000F000000000) >> 36)] +
                convt[((val & 0x0000000F00000000) >> 32)] +
                convt[((val & 0x00000000F0000000) >> 28)] +
                convt[((val & 0x000000000F000000) >> 24)] +
                convt[((val & 0x0000000000F00000) >> 20)] +
                convt[((val & 0x00000000000F0000) >> 16)] +
                convt[((val & 0x000000000000F000) >> 12)] +
                convt[((val & 0x0000000000000F00) >> 8)] +
                convt[((val & 0x00000000000000F0) >> 4)] +
                convt[((val & 0x000000000000000F) >> 0)]
                ;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            try
            {
                data = Convert.ToUInt64("0x" + textBox1.Text, 16);
            }
            catch
            {
            }
            refresh();
        }
    }
}
