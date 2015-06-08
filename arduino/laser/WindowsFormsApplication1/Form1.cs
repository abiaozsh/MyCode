using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        SerialPort port;

        public Form1()
        {
            InitializeComponent();

            this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

            //COM4为Arduino使用的串口号，需根据实际情况调整
            port = new SerialPort("COM8", 9600);
            port.Open();

        }

        void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //timer1.Enabled = true;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //string val = port.ReadLine();
            //textBox1.Text = val.ToString() + "\r\n" + textBox1.Text;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //const LASER_ON =    51;
            //const LASER_OFF =   52;
            //const A_ON_FW =     53;
            //const A_ON_BK =     54;
            //const A_STOP =      55;
            //const A_OFF =       56;
            //const B_ON_FW =     57;
            //const B_ON_BK =     58;
            //const B_STOP =      59;
            //const B_OFF =       60;
            //const FUN_WAIT =    61;
            //const DELAY =       62;
            /*
LASER_ON,5,0
LASER_OFF,5,0
LASER_ON,5,0
LASER_OFF,5,0
LASER_ON,5,0
LASER_OFF,5,0
LASER_ON,5,0
LASER_OFF,5,0
LASER_ON,5,0
LASER_OFF,5,0
LASER_ON,5,0
LASER_OFF,5,0
             
             */
            byte[] buff = new byte[10000];
            int idx = 0;

            foreach (var str1 in textBox1.Text.Split(new string[]{"\r\n"}, StringSplitOptions.None))
            {
                string[] str2 = str1.Split(',');
                {
                    try
                    {
                        byte val = 0;
                        if (str2[0] == "LASER_ON") val = 51;
                        if (str2[0] == "LASER_OFF") val = 52;
                        if (str2[0] == "A_ON_FW") val = 53;
                        if (str2[0] == "A_ON_BK") val = 54;
                        if (str2[0] == "A_STOP") val = 55;
                        if (str2[0] == "A_OFF") val = 56;
                        if (str2[0] == "B_ON_FW") val = 57;
                        if (str2[0] == "B_ON_BK") val = 58;
                        if (str2[0] == "B_STOP") val = 59;
                        if (str2[0] == "B_OFF") val = 60;
                        if (val > 0)
                        {
                            buff[idx++] = val;

                            buff[idx++] = byte.Parse(str2[1]);

                            buff[idx++] = byte.Parse(str2[2]);
                        }
                    }
                    catch { }
                }
            }


            port.Write(buff, 0, idx + 2);

        }
    }
}
