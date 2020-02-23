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
            FileStream fs = new FileStream(@"E:\MyCode.github\fpga\workspace\qsys\obj\default\hello_world.o", FileMode.Open, FileAccess.Read);
            //FileStream fs = new FileStream(@"E:\MyCode.github\fpga\workspace\qsys\qsys.elf", FileMode.Open, FileAccess.Read);
            BinaryReader br = new BinaryReader(fs);
            //byte[] buff = new byte[fs.Length];
            //fs.Read(buff,0,(int)fs.Length);
            var head = ELF.ReadHead(br);
            fs.Close();

            foreach (var item in head.pheads)
            {
                this.textBox1.Text += item.code + "\r\n\r\n";
            }

            foreach (var item in head.sheads)
            {
                this.textBox1.Text += item.code + "\r\n\r\n";
            }
            return;

            FileStream fs2 = new FileStream(@"E:\MyCode.github\fpga\workspace\qsys\qsys.mif", FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs2);
            sw.WriteLine("WIDTH = 32;");
            sw.WriteLine("DEPTH = 1024;");//4096byte
            sw.WriteLine("");
            sw.WriteLine("ADDRESS_RADIX = UNS;");
            sw.WriteLine("DATA_RADIX = HEX;");
            sw.WriteLine("");
            sw.WriteLine("CONTENT BEGIN");
            int idx = 0;
            {
                byte[] data = head.pheads[0].data;
                for (int i = 0; i < data.Length; i += 4)
                {
                    string hex = DASM.getHex2(data[i + 3]) + DASM.getHex2(data[i + 2]) + DASM.getHex2(data[i + 1]) + DASM.getHex2(data[i + 0]);
                    sw.WriteLine(" " + idx + ":" + hex);
                    idx++;
                }
            }
            {
                byte[] data = head.pheads[1].data;
                for (int i = 0; i < data.Length; i += 4)
                {
                    string hex = DASM.getHex2(data[i + 3]) + DASM.getHex2(data[i + 2]) + DASM.getHex2(data[i + 1]) + DASM.getHex2(data[i + 0]);
                    sw.WriteLine(" " + idx + ":" + hex);
                    idx++;
                }
            }
            for (int i = idx; i < 1024; i++)
            {
                sw.WriteLine(" " + i + ":00000000");
            }
            sw.WriteLine("END;");
            sw.Close();
            fs2.Close();
        }
        /*
-- 图片转MIF上位机软件
-- 图片分辨率100 x 100 x 16位

WIDTH = 16;
DEPTH = 10000;

ADDRESS_RADIX = UNS;
DATA_RADIX = HEX;

CONTENT BEGIN
    0      :  FFFF;
    1      :  FFFF;
    2      :  FFFF;
         * 
    9996   :  FFFF;
    9997   :  FFFF;
    9998   :  FFFF;
    9999   :  FFFF;
END;
         */
    }
}
