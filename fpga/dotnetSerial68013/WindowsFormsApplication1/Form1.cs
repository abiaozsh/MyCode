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
using CyUSB;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Drawing.Imaging;

namespace WindowsFormsApplication1
{

	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();

			this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);
		}

		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
		}
		string portName;
		private void Form1_Load(object sender, EventArgs e)
		{
			// Create a list of CYUSB devices
			usbDevices = new USBDeviceList(CyConst.DEVICES_CYUSB);

			//Adding event handlers for device attachment and device removal
			usbDevices.DeviceAttached += new EventHandler(usbDevices_DeviceAttached);
			usbDevices.DeviceRemoved += new EventHandler(usbDevices_DeviceRemoved);

			//The below function sets the device with particular VID and PId and searches for the device with the same VID and PID.
			setDevice();
			try
			{
				string[] list = System.IO.Directory.GetFiles(@"E:\test\");

				listView1.View = View.LargeIcon;
				listView1.TileSize = new System.Drawing.Size(160, 160);
				listView1.LargeImageList = this.imageList1;
				imageList1.ImageSize = new Size(160, 120);
				imageList1.ColorDepth = ColorDepth.Depth32Bit;
				this.listView1.BeginUpdate();
				int idx = 0;
				foreach (var item in list)
				{
					pack p = new pack();
					try
					{
						Bitmap b2;
						p.buffs = loadimg(new Bitmap(item), item, out b2);
						this.imageList1.Images.Add(b2);

						p.name = item;

						ListViewItem vi = new ListViewItem();
						vi.Tag = p;
						vi.Text = item;
						vi.ImageIndex = idx;
						idx++;
						this.listView1.Items.Add(vi);
					}
					catch (Exception ex)
					{
					}
				}
				this.listView1.EndUpdate();
			}
			catch { }
		}

		public class pack
		{
			public byte[][] buffs;
			public string name;
			override
			public string ToString()
			{
				return name;
			}
		}

		void usbDevices_DeviceRemoved(object sender, EventArgs e)
		{
			setDevice();
		}
		void usbDevices_DeviceAttached(object sender, EventArgs e)
		{
			setDevice();
		}

		USBDeviceList usbDevices = null;
		CyUSBDevice loopDevice = null;
		CyBulkEndPoint inEndpoint = null;
		CyBulkEndPoint outEndpoint = null;
		CyBulkEndPoint inCmdEndpoint = null;
		CyBulkEndPoint outCmdEndpoint = null;

		public void setDevice()
		{
			loopDevice = usbDevices[0x04b4, 0x1004] as CyUSBDevice;
			CyFX2Device progDevice = usbDevices[0x04b4, 0x8613] as CyFX2Device;
			if (loopDevice == null && progDevice != null)
			{
				//progDevice.LoadExternalRam(@"D:\MyCode.github\cy7c68013a\Bulkloop.firm.my\bulkloop.hex");
				//progDevice.LoadExternalRam(@"D:\MyCode.github\cy7c68013a\test1004slavefifo\bulkloop.hex");
				progDevice.LoadExternalRam(@"..\..\..\..\..\cy7c68013a\test1004slavefifo\bulkloop.hex");
			}

			if (loopDevice != null)
			{
				this.Text = loopDevice.FriendlyName;
				//EndpointAddress 8:in from cy  0:out to cy
				outCmdEndpoint = loopDevice.EndPointOf(0x01) as CyBulkEndPoint;
				inCmdEndpoint = loopDevice.EndPointOf(0x81) as CyBulkEndPoint;
				outEndpoint = loopDevice.EndPointOf(0x02) as CyBulkEndPoint;
				inEndpoint = loopDevice.EndPointOf(0x86) as CyBulkEndPoint;
				if (outEndpoint.MaxPktSize != 512)
				{
					MessageBox.Show("MaxPktSize != 512");
				}
				outEndpoint.TimeOut = 100;
				inEndpoint.TimeOut = 100;
				outCmdEndpoint.TimeOut = 100;
				inCmdEndpoint.TimeOut = 100;
			}
			else
			{
				this.Text = "Bulkloop - no device";
			}
		}

		string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
		private string getHex4(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))] + convt[((val & 0xF000) >> 12)] + convt[((val & 0x0F00) >> 8)];
		}
		private string getHex2(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
		}
		private string getHex(byte[] buff, int idx, int len)//"ll"+"hh"
		{
			StringBuilder sb = new StringBuilder();
			for (int i = idx; i < idx + len; i++)
			{
				byte val = buff[i];
				sb.Append(convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))]);
			}
			return sb.ToString();
		}

		private void button5_Click(object sender, EventArgs e)
		{
			textBox3.Text = "";
		}

		private void button6_Click(object sender, EventArgs e)
		{
			check();
		}
		private void check()
		{
			int pages = 32 * 1024;//16 * 1024 * 1024 words

			int addr = Convert.ToInt32(textBox1.Text, 16);

			byte[][] buffs1 = new byte[pages][];
			byte[][] buffs2 = new byte[pages][];

			//send
			{
				Random r = new Random();

				for (int i = 0; i < pages; i++)
				{
					buffs1[i] = new byte[PAGESIZE];
					r.NextBytes(buffs1[i]);
				}

				transfer(addr, buffs1);
			}

			//back
			{
				for (int i = 0; i < pages; i++)
				{
					buffs2[i] = new byte[PAGESIZE];
				}
				transferBack(addr, buffs2);
			}

			int err = 0;
			Bitmap errmap = new Bitmap(256, 128);
			for (int i = 0; i < buffs1.Length; i++)
			{
				bool gerr = false;
				for (int j = 0; j < buffs1[i].Length; j++)
				{
					if (buffs1[i][j] != buffs2[i][j])
					{
						gerr = true;
						err++;
					}
				}
				if (gerr)
				{
					errmap.SetPixel(i & 255, (i >> 8), Color.Red);
				}
				else
				{
					errmap.SetPixel(i & 255, (i >> 8), Color.LightGreen);
				}
			}
			Form2 form2 = new Form2();
			form2.bitmap = errmap;
			form2.Show();


			textBox3.Text += ("done [" + err + " " + (err * 1.0 / (pages * PAGESIZE)) + "]  ") + "\r\n";
		}

		private string bin2str(byte[] buff)
		{
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < 32; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					sb.Append(getHex2(buff[(i * 16 + j) * 2 + 0]) + getHex2(buff[(i * 16 + j) * 2 + 1]) + " ");
				}
				sb.AppendLine();
			}
			sb.AppendLine();
			return sb.ToString();
		}

		private void button9_Click(object sender, EventArgs e)
		{
			sendCmd(0x021, 0);
		}

		private void button10_Click(object sender, EventArgs e)
		{
			sendCmd(0x022, 0);
		}

		private void button8_Click(object sender, EventArgs e)
		{
			sendCmd(0x023, 0);
			//this.timer1.Enabled = false;
		}

		private void button11_Click(object sender, EventArgs e)
		{
			sendCmd(0x024, 0);
			//this.timer1.Enabled = true;
		}

		private void sendCmd(int cmd, int dat)
		{
			int size = 2;
			bool bResult;
			byte[] outData = new byte[2];
			outData[0] = (byte)cmd;
			outData[1] = (byte)dat;
			int xferLen = size;
			bResult = outCmdEndpoint.XferData(ref outData, ref xferLen);
			if (!bResult)
			{
				throw new Exception();
			}
		}

		private int recAck(int val)
		{
			int size = 2;
			bool bResult;
			byte[] inData = new byte[2];
			int xferLen = size;
			bResult = inCmdEndpoint.XferData(ref inData, ref xferLen);
			if (!bResult)
			{
				throw new Exception();
			}
			int ret = 0;
			if (bResult)
			{
				ret = inData[0] | (((int)inData[1]) << 8);
			}
			if (ret != val)
			{
				//this.textBox3.Text += "rec:" + ret + "\r\n";
				//MessageBox.Show(ret + "");
			}

			return ret;
		}

		private void button12_Click(object sender, EventArgs e)
		{
			int addr = Convert.ToInt32(textBox1.Text, 16);

			byte[][] buffs = new byte[1][];
			buffs[0] = new byte[PAGESIZE];

			transferBack(addr, buffs);

			string s = bin2str(buffs[0]);
			textBox3.Text += s;

		}


		private void button13_Click(object sender, EventArgs e)
		{
			int addr = Convert.ToInt32(textBox1.Text, 16);

			Random r = new Random();

			byte[][] buffs = new byte[1][];
			buffs[0] = new byte[PAGESIZE];

			r.NextBytes(buffs[0]);

			string s = bin2str(buffs[0]);
			textBox3.Text += s;

			transfer(addr, buffs);

			dma();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			int addr = Convert.ToInt32(textBox1.Text, 16);
			sendCmd(0x012, addr & 0xFF);
			sendCmd(0x013, (addr >> 8) & 0xFF);
			sendCmd(0x014, (addr >> 16) & 0xFF);

			int val = Convert.ToInt32(textBox2.Text, 16);
			sendCmd(0x010, val & 0xFF);
			sendCmd(0x011, (val >> 8) & 0xFF);

			sendCmd((checkBox1.Checked ? 0x0B0 : 0xA0) + 0, 0);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			int addr = Convert.ToInt32(textBox1.Text, 16);
			sendCmd(0x012, addr & 0xFF);
			sendCmd(0x013, (addr >> 8) & 0xFF);
			sendCmd(0x014, (addr >> 16) & 0xFF);

			sendCmd((checkBox1.Checked ? 0x0B0 : 0xA0) + 1, 0);

			int data = recAck(0);
			textBox2.Text = getHex4(data);
		}


		static readonly int PAGESIZE = 1024;

		private byte[][] loadimg(Bitmap b, string name, out Bitmap b2)
		{
			b2 = new Bitmap(1024, 768);
			Graphics g = Graphics.FromImage(b2);
			g.DrawImage(b, new Rectangle(0, 0, 1024, 768), 0, 0, b.Width, b.Height, GraphicsUnit.Pixel, null);
			g.Dispose();


			//set transfer
			this.Text = "procimg " + name;
			Application.DoEvents();
			return img2buff(b2);
		}

		private byte[][] img2buff(Bitmap b2)
		{
			byte[][] buffs = new byte[768 * 2][];

			try
			{
				BitmapData bmpData = b2.LockBits(new Rectangle(0, 0, 1024, 768), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format16bppRgb565);
				IntPtr ptr = bmpData.Scan0;
				byte[] buff = new byte[768 * 2 * PAGESIZE];
				Marshal.Copy(ptr, buff, 0, 768 * 2 * PAGESIZE);

				for (int y = 0; y < 768 * 2; y++)
				{
					buffs[y] = new byte[PAGESIZE];
					Array.Copy(buff, y * PAGESIZE, buffs[y], 0, PAGESIZE);

				}
				b2.UnlockBits(bmpData);
			}
			catch (Exception ex)
			{
				ex.ToString();
			}
			return buffs;

		}

		void setaddress(int addr)
		{
			sendCmd(0x012, addr & 0xFF);
			sendCmd(0x013, (addr >> 8) & 0xFF);
			sendCmd(0x014, (addr >> 16) & 0xFF);
		}

		void transfer(int addr, byte[][] buffs)
		{
			setaddress(addr);

			int pages = buffs.Length;
			sendCmd(0x015, (pages) & 0xFF);
			sendCmd(0x016, (pages >> 8) & 0xFF);

			sendCmd(0x060, 0);
			sendCmd(0x062, 0);

			Stopwatch a;
			for (int y = 0; y < buffs.Length; y++)
			{
				if (buffs[y].Length != PAGESIZE)
				{
					throw new Exception();
				}
				int xferLen = PAGESIZE;
				bool bResult = outEndpoint.XferData(ref buffs[y], ref xferLen);
				if (!bResult)
				{
					throw new Exception();
				}
			}
			int ack;
			ack = recAck(0x3412);
			//release transfer
			sendCmd(0x061, 0);
		}

		void transferBack(int addr, byte[][] buffs)
		{

			setaddress(addr);

			int pages = buffs.Length;
			sendCmd(0x015, (pages) & 0xFF);
			sendCmd(0x016, (pages >> 8) & 0xFF);

			sendCmd(0x060, 0);
			sendCmd(0x063, 0);

			bool bResult;

			int xferLen;
			for (int y = 0; y < pages; y++)
			{
				xferLen = PAGESIZE;
				bResult = inEndpoint.XferData(ref buffs[y], ref xferLen);
				if (!bResult)
				{
					throw new Exception();
				}
			}
			int ack;
			ack = recAck(0x3412);
			//release transfer
			sendCmd(0x061, 0);
		}

		private int getpixel(Color c)
		{
			int val = 0;
			val += (c.R >> 3) << (5 + 6);
			val += (c.G >> 2) << (5);
			val += (c.B >> 3);
			return val;
		}


		private void listView1_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (listView1.SelectedItems.Count <= 0) return;
			sendImg(((pack)(listView1.SelectedItems)[0].Tag).buffs);
		}

		private void sendImg(byte[][] buffs)
		{
			transfer(0, buffs);
			int ack;

			setaddress(0);


			dma();

		}




		void dma()
		{


			int dma_src = trackBar1.Value * 4;//4page per line
			int dma_des = int.Parse(dmaBase.Text) * 4;//4page per line
			int ack;

			//transfer 256page ,64 line, 64*12=768line
			int page_len = 256;//max 4095 // 12'b0011_1111_1111
			sendCmd(0x01B, page_len & 0xFF);
			sendCmd(0x01C, (page_len >> 8) & 0xFF);

			for (int i = 0; i < 12; i++)//1024*768
			{
				sendCmd(0x017, dma_src & 0xFF);
				sendCmd(0x018, (dma_src >> 8) & 0xFF);
				sendCmd(0x019, dma_des & 0xFF);
				sendCmd(0x01A, (dma_des >> 8) & 0xFF);
				dma_src += 256;
				dma_des += 256;

				//BF) begin memcopy
				sendCmd(0xBF, 0);
				ack = recAck(0x3412);
			}

		}
		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			dma();
		}


		private void timer1_Tick(object sender, EventArgs e)
		{
			int iWidth = Screen.PrimaryScreen.Bounds.Width;
			//屏幕高
			int iHeight = Screen.PrimaryScreen.Bounds.Height;
			//按照屏幕宽高创建位图
			Bitmap b2 = new Bitmap(1024, 768);
			//从一个继承自Image类的对象中创建Graphics对象
			Graphics gc = Graphics.FromImage(b2);
			//抓屏并拷贝到myimage里
			gc.CopyFromScreen(new Point(0, 0), new Point(0, 0), new Size(1024, 768));
			gc.Flush();
			gc.Dispose();


			byte[][] buffs = img2buff(b2);

			transfer(0, buffs);
			int ack;

			this.Text = "";

			setaddress(0);

			dma();

		}



		private void checkBox2_CheckedChanged(object sender, EventArgs e)
		{
			//end else if (command == 8'h40) begin switch1<=0; command_done<=1;
			//end else if (command == 8'h41) begin switch1<=1; command_done<=1;
			if (!checkBox2.Checked)
			{
				sendCmd(0x40, 0);
			}
			else
			{
				sendCmd(0x41, 0);
			}
		}

		private void checkBox3_CheckedChanged(object sender, EventArgs e)
		{
			//end else if (command == 8'h44) begin value1<=0; command_done<=1;
			//end else if (command == 8'h45) begin value1<=1; command_done<=1;
			if (!checkBox3.Checked)
			{
				sendCmd(0x44, 0);
			}
			else
			{
				sendCmd(0x45, 0);
			}
		}

		private void checkBox5_CheckedChanged(object sender, EventArgs e)
		{
			//end else if (command == 8'h42) begin switch2<=0; command_done<=1;
			//end else if (command == 8'h43) begin switch2<=1; command_done<=1;
			if (!checkBox5.Checked)
			{
				sendCmd(0x42, 0);
			}
			else
			{
				sendCmd(0x43, 0);
			}

		}

		private void checkBox4_CheckedChanged(object sender, EventArgs e)
		{
			//end else if (command == 8'h46) begin value2<=0; command_done<=1;
			//end else if (command == 8'h47) begin value2<=1; command_done<=1;
			if (!checkBox4.Checked)
			{
				sendCmd(0x46, 0);
			}
			else
			{
				sendCmd(0x47, 0);
			}

		}

		private void button3_Click(object sender, EventArgs e)
		{
			Random r = new Random();
			byte[] b = new byte[1];
			for (int i = 0; i < 1000000; i++)
			{
				r.NextBytes(b);
				sendCmd(0x30, b[0]);
				int ack = recAck(0);
				if (ack != ((b[0] << 8) + 0x12))
				{
					textBox3.Text += ack + "\r\n";
				}
			}
		}

		private void checkBox6_CheckedChanged(object sender, EventArgs e)
		{
			this.timer1.Enabled = checkBox6.Checked;
		}

		private void button4_Click(object sender, EventArgs e)
		{
			int iWidth = Screen.PrimaryScreen.Bounds.Width;
			//屏幕高
			int iHeight = Screen.PrimaryScreen.Bounds.Height;
			//按照屏幕宽高创建位图
			Bitmap b2 = new Bitmap(1024, 768);
			//从一个继承自Image类的对象中创建Graphics对象
			Graphics gc = Graphics.FromImage(b2);
			//抓屏并拷贝到myimage里
			gc.CopyFromScreen(new Point(0, 0), new Point(0, 0), new Size(1024, 768));
			gc.Flush();
			gc.Dispose();

			byte[][] buffs = img2buff(b2);
			//
			sendImg(buffs);
			//sendImg(((pack)(listView1.SelectedItems)[0].Tag).buffs);
		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			int vgaBase = trackBar2.Value;

			sendCmd(0x01D, vgaBase & 0xFF);
			sendCmd(0x01E, (vgaBase >> 8) & 0xFF);

		}


	}
}
