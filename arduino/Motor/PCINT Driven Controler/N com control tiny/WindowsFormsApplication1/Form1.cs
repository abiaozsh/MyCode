using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;

namespace WindowsFormsApplication1
{

	public partial class Form1 : Form
	{
		SerialPort port;
		FileStream fs;
		StreamWriter sw;
		private delegate void FlushClient(string s);
		public Form1()
		{
			InitializeComponent();

			this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

			//fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
			//sw = new StreamWriter(fs);
			if (port == null)
			{
				try
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM3", 115200, Parity.None, 5, StopBits.One);
					port.Open();
				}
				catch
				{
                    this.Text = "error";
                }
			}
		}

		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
		}

		private void Send(byte dataA1, byte dataA2, byte dataB1, byte dataB2, byte dataC1, byte dataC2, byte dataD1, byte dataD2)
		{
			byte[] d = new byte[20];

			//                                                                                                                                            0x00;
			d[0] = (byte)((((0x0001 & 0x01) >> 0) << 4) | (((0x0001 & 0x01) >> 0) << 3) | (((0x0001 & 0x01) >> 0) << 2) | (((0x0001 & 0x01) >> 0) << 1) | 0x01);//start bit
			d[1] = (byte)((((dataD1 & 0x80) >> 7) << 4) | (((dataC1 & 0x80) >> 7) << 3) | (((dataB1 & 0x80) >> 7) << 2) | (((dataA1 & 0x80) >> 7) << 1) | 0x00);
			d[2] = (byte)((((dataD1 & 0x40) >> 6) << 4) | (((dataC1 & 0x40) >> 6) << 3) | (((dataB1 & 0x40) >> 6) << 2) | (((dataA1 & 0x40) >> 6) << 1) | 0x01);
			d[3] = (byte)((((dataD1 & 0x20) >> 5) << 4) | (((dataC1 & 0x20) >> 5) << 3) | (((dataB1 & 0x20) >> 5) << 2) | (((dataA1 & 0x20) >> 5) << 1) | 0x00);
            d[4] = (byte)((((dataD1 & 0x10) >> 4) << 4) | (((dataC1 & 0x10) >> 4) << 3) | (((dataB1 & 0x10) >> 4) << 2) | (((dataA1 & 0x10) >> 4) << 1) | 0x01);
            d[5] = (byte)((((dataD1 & 0x08) >> 3) << 4) | (((dataC1 & 0x08) >> 3) << 3) | (((dataB1 & 0x08) >> 3) << 2) | (((dataA1 & 0x08) >> 3) << 1) | 0x00);
            d[6] = (byte)((((dataD1 & 0x04) >> 2) << 4) | (((dataC1 & 0x04) >> 2) << 3) | (((dataB1 & 0x04) >> 2) << 2) | (((dataA1 & 0x04) >> 2) << 1) | 0x01);
            d[7] = (byte)((((dataD1 & 0x02) >> 1) << 4) | (((dataC1 & 0x02) >> 1) << 3) | (((dataB1 & 0x02) >> 1) << 2) | (((dataA1 & 0x02) >> 1) << 1) | 0x00);
            d[8] = (byte)((((dataD1 & 0x01) >> 0) << 4) | (((dataC1 & 0x01) >> 0) << 3) | (((dataB1 & 0x01) >> 0) << 2) | (((dataA1 & 0x01) >> 0) << 1) | 0x01);
            d[9] = (byte)((((0x0000 & 0x01) >> 0) << 4) | (((0x0000 & 0x01) >> 0) << 3) | (((0x0000 & 0x01) >> 0) << 2) | (((0x0000 & 0x01) >> 0) << 1) | 0x00);//stop bit

			d[10] = (byte)((((0x0001 & 0x01) >> 0) << 4) | (((0x0001 & 0x01) >> 0) << 3) | (((0x0001 & 0x01) >> 0) << 2) | (((0x0001 & 0x01) >> 0) << 1) | 0x01);//start bit
			d[11] = (byte)((((dataD2 & 0x80) >> 7) << 4) | (((dataC2 & 0x80) >> 7) << 3) | (((dataB2 & 0x80) >> 7) << 2) | (((dataA2 & 0x80) >> 7) << 1) | 0x00);
            d[12] = (byte)((((dataD2 & 0x40) >> 6) << 4) | (((dataC2 & 0x40) >> 6) << 3) | (((dataB2 & 0x40) >> 6) << 2) | (((dataA2 & 0x40) >> 6) << 1) | 0x01);
            d[13] = (byte)((((dataD2 & 0x20) >> 5) << 4) | (((dataC2 & 0x20) >> 5) << 3) | (((dataB2 & 0x20) >> 5) << 2) | (((dataA2 & 0x20) >> 5) << 1) | 0x00);
            d[14] = (byte)((((dataD2 & 0x10) >> 4) << 4) | (((dataC2 & 0x10) >> 4) << 3) | (((dataB2 & 0x10) >> 4) << 2) | (((dataA2 & 0x10) >> 4) << 1) | 0x01);
            d[15] = (byte)((((dataD2 & 0x08) >> 3) << 4) | (((dataC2 & 0x08) >> 3) << 3) | (((dataB2 & 0x08) >> 3) << 2) | (((dataA2 & 0x08) >> 3) << 1) | 0x00);
            d[16] = (byte)((((dataD2 & 0x04) >> 2) << 4) | (((dataC2 & 0x04) >> 2) << 3) | (((dataB2 & 0x04) >> 2) << 2) | (((dataA2 & 0x04) >> 2) << 1) | 0x01);
            d[17] = (byte)((((dataD2 & 0x02) >> 1) << 4) | (((dataC2 & 0x02) >> 1) << 3) | (((dataB2 & 0x02) >> 1) << 2) | (((dataA2 & 0x02) >> 1) << 1) | 0x00);
            d[18] = (byte)((((dataD2 & 0x01) >> 0) << 4) | (((dataC2 & 0x01) >> 0) << 3) | (((dataB2 & 0x01) >> 0) << 2) | (((dataA2 & 0x01) >> 0) << 1) | 0x01);
            d[19] = (byte)((((0x0000 & 0x01) >> 0) << 4) | (((0x0000 & 0x01) >> 0) << 3) | (((0x0000 & 0x01) >> 0) << 2) | (((0x0000 & 0x01) >> 0) << 1) | 0x00);//stop bit

			if (port != null && port.IsOpen)
			{
				port.Write(d, 0, 20);
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		const byte CMD_START = 1;/*START          */
		const byte CMD_NOSTART = 2;/*START off     */
		const byte CMD_SETPWRSIMP = 3;/*set power simple */
		const byte CMD_PITCH = 4;/*PITCH         */
		const byte CMD_SENDDATA1Xa = 5;/*0~255       1x */
		const byte CMD_SENDDATA1Xb = 6;/*256~511     1x */
		const byte CMD_SENDDATA2X = 7;/*512~1023    2x */
		const byte CMD_SENDDATA4X = 8;/*1024~2047   4x */
		const byte CMD_SENDDATA8X = 9;/*2048~4095   8x */
		const byte CMD_SENDDATA16X = 10; /*4096~8191  16x */
		const byte CMD_SETMAXPWR = 11; /*set max power  */
		const byte CMD_SAVESET = 12; /*savesetting    */
		const byte CMD_REVERSE = 13; /*REVERSE       */
		const byte CMD_SETCPU = 14; /*SETCPU        */

		int targetSpeed;

		bool mousedown1 = false;
		bool mousedown2 = false;
		bool mousedown3 = false;
		bool mousedown4 = false;

		void button1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown1 = true;
			Send(CMD_START, 0, 0, 0, 0, 0, 0, 0);
		}
		void button1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown1 = false;
		    Send(CMD_NOSTART, 0, 0, 0, 0, 0, 0, 0);
		}

		void button2_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown2 = true;
			Send(0, 0, CMD_START, 0, 0, 0, 0, 0);
		}
		void button2_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown2 = false;
			Send(0, 0, CMD_NOSTART, 0, 0, 0, 0, 0);
		}

		void button3_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown3 = true;
			Send(0, 0, 0, 0, CMD_START, 0, 0, 0);
		}
		void button3_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown3 = false;
			Send(0, 0, 0, 0, CMD_NOSTART, 0, 0, 0);
		}

		void button4_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown4 = true;
			Send(0, 0, 0, 0, 0, 0, CMD_START, 0);
		}
		void button4_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mousedown4 = false;
			Send(0, 0, 0, 0, 0, 0, CMD_NOSTART, 0);
		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			Send(CMD_SETPWRSIMP, (byte)trackBar1.Value, 0, 0, 0, 0, 0, 0);
		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			Send(0, 0, CMD_SETPWRSIMP, (byte)trackBar2.Value, 0, 0, 0, 0);
		}

		private void trackBar3_Scroll(object sender, EventArgs e)
		{
			Send(0, 0, 0, 0, CMD_SETPWRSIMP, (byte)trackBar3.Value, 0, 0);
		}

		private void trackBar4_Scroll(object sender, EventArgs e)
		{
			Send(0, 0, 0, 0, 0, 0, CMD_SETPWRSIMP, (byte)trackBar4.Value);
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			if (mousedown1)
			{
                if (trackBar1.Value < 255) trackBar1.Value++;
				trackBar1_Scroll(null, null);
			}
			if (mousedown2)
			{
                if (trackBar2.Value < 255) trackBar2.Value++;
				trackBar2_Scroll(null, null);
			}
			if (mousedown3)
			{
                if (trackBar3.Value < 255) trackBar3.Value++;
				trackBar3_Scroll(null, null);
			}
			if (mousedown4)
			{
                if (trackBar4.Value < 255) trackBar4.Value++;
				trackBar4_Scroll(null, null);
			}

		}

        private void button1_Click(object sender, EventArgs e)
        {

        }


	}
}


