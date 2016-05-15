using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }

        private void Form3_Load(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < 12000; i++)
            {
                double temp = i;
                if (i != 0)
                {
                    temp = 3666666 / temp;
                }
                if (temp > 99999) temp = 99999;
                int val = (int)temp;

                int digin = (val % 10); val = val / 10;
                int digi3 = (val % 10); val = val / 10;
                int digi2 = (val % 10); val = val / 10;
                int digi1 = (val % 10); val = val / 10;
                int digi0 = (val % 10); val = val / 10;

                sb.Append("0x");
                sb.Append(digi0);
                sb.Append(digi1);
                sb.Append(",0x");
                sb.Append(digi2);
                sb.Append(digi3);
                sb.Append(",");
            
            }
            textBox1.Text = sb.ToString();
        }
    }
}
