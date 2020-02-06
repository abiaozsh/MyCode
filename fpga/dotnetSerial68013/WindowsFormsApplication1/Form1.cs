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

            //buffs1 = loadimg(new Bitmap("e:\\z051.jpg"),"",out dummy);
            //buffs2 = loadimg(new Bitmap("e:\\z043.jpg"), "");

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



        void textBox3_DragDrop(object sender, DragEventArgs e)
        {
            throw new NotImplementedException();
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
                outEndpoint.TimeOut = 1000;
                inEndpoint.TimeOut = 1000;
                outCmdEndpoint.TimeOut = 1000;
                inCmdEndpoint.TimeOut = 1000;
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



        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }


        private void button5_Click(object sender, EventArgs e)
        {
            textBox3.Text = "";
        }

        byte[] sendbuff = new byte[1024];
        private int send(byte[] data, int start)
        {
            Array.Copy(data, start, sendbuff, 0, 1024);
            int xferLen = 1024;
            bool bResult = outEndpoint.XferData(ref sendbuff, ref xferLen);
            return xferLen;
        }

        byte[] receivebuff = new byte[1024];
        private int receive(byte[] data, int start)
        {
            int xferLen = 1024;
            bool bResult = inEndpoint.XferData(ref receivebuff, ref xferLen);
            Array.Copy(receivebuff, 0, data, start, 1024);
            return xferLen;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 10; i++)
            {
                check();
            }
        }
        private void check()
        {
            int size = 16 * 1024 * 1024;//16 * 1024
            if (is2M)
            {
                size = 1024 * 1024;//* 1024
            }

            byte[] buff = new byte[size * 2];
            byte[] buff2 = new byte[size * 2];
            Random r = new Random();
            r.NextBytes(buff);

            bool first;

            first = true;
            for (int k = 0; k < size; k += 0x200)//1kword
            {
                send(buff, k << 1);

                if (!first)
                {
                    recAck(0x3412);
                }
                else
                {
                    first = false;
                }
                sendCmd(0x012, k & 0xFF);
                sendCmd(0x013, (k >> 8) & 0xFF);
                sendCmd(0x014, (k >> 16) & 0xFF);

                sendCmd((is2M ? 0x0B0 : 0xA0) + 2, 0);

                this.Text = "" + (k * 100 / size);
                Application.DoEvents();
            }
            recAck(0x3412);

            Thread.Sleep(1000);

            //recAck(0);
            first = true;
            int lastk = 0;
            for (int k = 0; k < size; k += 0x200)//1kword
            {
                if (!first)
                {
                    receive(buff2, lastk << 1);
                }
                else
                {
                    first = false;
                }


                sendCmd(0x012, k & 0xFF);
                sendCmd(0x013, (k >> 8) & 0xFF);
                sendCmd(0x014, (k >> 16) & 0xFF);
                sendCmd((is2M ? 0x0B0 : 0xA0) + 3, 0);

                recAck(0);

                lastk = k;
                this.Text = "" + (k * 100 / size);
                Application.DoEvents();
            }
            receive(buff2, lastk << 1);

            if (is2M)
            {
                Bitmap errmap = new Bitmap(1024, 1024);
                int err = 0;
                for (int i = 0; i < buff.Length; i += 2)
                {
                    if (buff[i] != buff2[i] || buff[i + 1] != buff2[i + 1])
                    {
                        errmap.SetPixel((i >> 1) & 1023, (i >> 1) >> 10, Color.Red);
                        err++;
                    }
                    else
                    {
                        errmap.SetPixel((i >> 1) & 1023, (i >> 1) >> 10, Color.LightGreen);
                    }
                }
                Form2 form2 = new Form2();
                form2.bitmap = errmap;
                form2.Show();
                textBox3.Text += ("done [" + err + " " + (err * 1.0 / size) + "] ") + "\r\n";
            }
            else
            {
                Bitmap errmap = new Bitmap(1024, 1024);
                for (int i = 0; i < buff.Length; i += 2 * 16)
                {
                    bool gerr = false;
                    for (int j = 0; j < 2 * 16; j += 2)
                    {
                        if (buff[i + j] != buff2[i + j] || buff[i + j + 1] != buff2[i + j + 1])
                        {
                            gerr = true;
                        }
                    }
                    if (gerr)
                    {
                        errmap.SetPixel((i >> 5) & 1023, (i >> 5) >> 10, Color.Red);
                    }
                    else
                    {
                        errmap.SetPixel((i >> 5) & 1023, (i >> 5) >> 10, Color.LightGreen);
                    }
                }
                Form2 form2 = new Form2();
                form2.bitmap = errmap;
                form2.Show();


                int err = 0;
                for (int i = 0; i < buff.Length; i++)
                {

                    if (buff[i] != buff2[i])
                    {
                        err++;
                    }

                }

                textBox3.Text += ("done [" + err + " " + (err * 1.0 / size) + "]  ") + "\r\n";
            }

        }
        private string receivePage(byte[] buff)
        {
            //byte[] buff = new byte[512 * 2];
            //readFromPort(1024, buff, 0);

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

        private void button7_Click(object sender, EventArgs e)
        {
        }

        private void button9_Click(object sender, EventArgs e)
        {
            sendCmd(0x021, 0);

        }

        private void button10_Click(object sender, EventArgs e)
        {
            sendCmd(0x022, 0);

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
        }
        private int recAck(int val)
        {
            int size = 2;
            bool bResult;
            byte[] inData = new byte[2];
            int xferLen = size;
            bResult = inCmdEndpoint.XferData(ref inData, ref xferLen);
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

        bool is2M = false;

        private void button12_Click(object sender, EventArgs e)
        {
            int addr = Convert.ToInt32(textBox1.Text, 16);
            for (int i = 0; i < (doublePage.Checked ? 2 : 1); i++)
            {
                sendCmd(0x012, addr & 0xFF);
                sendCmd(0x013, (addr >> 8) & 0xFF);
                sendCmd(0x014, (addr >> 16) & 0xFF);
                addr += 0x200;
                sendCmd((is2M ? 0x0B0 : 0xA0) + 3, 0);

                recAck(0);

                int size = 1024;
                bool bResult;
                byte[] inData = new byte[size];
                int xferLen = size;
                bResult = inEndpoint.XferData(ref inData, ref xferLen);
                string s = receivePage(inData);
                textBox3.Text += xferLen + "\r\n";
                textBox3.Text += s;
            }
        }

        private byte[] ranArr(Random r, int size)
        {
            byte[] outData = new byte[size];
            r.NextBytes(outData);
            return outData;
        }

        private void button13_Click(object sender, EventArgs e)
        {
            int addr = Convert.ToInt32(textBox1.Text, 16);
            Random r = new Random();

            for (int i = 0; i < (doublePage.Checked ? 2 : 1); i++)
            {
                bool first = true;
                {
                    int size = 1024;
                    bool bResult;
                    byte[] outData = ranArr(r, size);// new byte[size];
                    string s = receivePage(outData);
                    textBox3.Text += s;
                    int xferLen = size;
                    bResult = outEndpoint.XferData(ref outData, ref xferLen);
                }
                if (!first)
                {
                    recAck(0x3412);
                }
                else
                {
                    first = false;
                }

                sendCmd(0x012, addr & 0xFF);
                sendCmd(0x013, (addr >> 8) & 0xFF);
                sendCmd(0x014, (addr >> 16) & 0xFF);
                addr += 0x200;

                sendCmd((is2M ? 0x0B0 : 0xA0) + 2, 0);


                recAck(0x3412);
            }

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            this.is2M = this.checkBox1.Checked;
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


            sendCmd((is2M ? 0x0B0 : 0xA0) + 0, 0);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int addr = Convert.ToInt32(textBox1.Text, 16);
            sendCmd(0x012, addr & 0xFF);
            sendCmd(0x013, (addr >> 8) & 0xFF);
            sendCmd(0x014, (addr >> 16) & 0xFF);

            sendCmd((is2M ? 0x0B0 : 0xA0) + 1, 0);

            int data = recAck(0);
            textBox2.Text = getHex4(data);
        }

        private void button3_Click(object sender, EventArgs e)
        {
        }

        private void button4_Click(object sender, EventArgs e)
        {
            int ack;

            this.Text = "";
            Application.DoEvents();

            int dma_src = 0;
            int dma_des = 0;

            sendCmd(0x017, dma_src & 0xFF);
            sendCmd(0x018, (dma_src >> 8) & 0xFF);
            sendCmd(0x019, dma_des & 0xFF);
            sendCmd(0x01A, (dma_des >> 8) & 0xFF);

            //BF) begin memcopy
            sendCmd(0xBF, 0);
            ack = recAck(0x3412);


        }
        byte[][] buffs1;
        byte[][] buffs2;

        private void button7_Click_1(object sender, EventArgs e)
        {
            transfer(0, buffs1);
        }
        private void button14_Click(object sender, EventArgs e)
        {
            transfer(0, buffs2);

        }
        static readonly int PAGESIZE = 1024;

        private byte[][] loadimg(Bitmap b, string name, out Bitmap b2)
        {
            b2 = new Bitmap(1024, 768);
            Graphics g = Graphics.FromImage(b2);
            g.DrawImage(b, new Rectangle(0, 0, 1024, 768), 0, 0, b.Width, b.Height, GraphicsUnit.Pixel, null);
            g.Dispose();

            //set transfer
            this.Text = "procimg";
            Application.DoEvents();
            byte[][] buffs = new byte[768 * 2][];

            for (int y = 0; y < 768; y++)
            {
                {
                    {
                        byte[] buff = new byte[PAGESIZE];
                        for (int x = 0; x < 512; x++)
                        {
                            var c = b2.GetPixel(x + 0, y);
                            int val = getpixel(c);
                            buff[(x << 1) + 0] = (byte)(val & 0xFF);
                            buff[(x << 1) + 1] = (byte)((val >> 8) & 0xFF);
                        }
                        buffs[(y << 1) + 0] = buff;
                    }
                    {
                        byte[] buff = new byte[PAGESIZE];
                        for (int x = 0; x < 512; x++)
                        {
                            var c = b2.GetPixel(x + 512, y);
                            int val = getpixel(c);
                            buff[(x << 1) + 0] = (byte)(val & 0xFF);
                            buff[(x << 1) + 1] = (byte)((val >> 8) & 0xFF);
                        }
                        buffs[(y << 1) + 1] = buff;
                    }
                }
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

            for (int y = 0; y < buffs.Length; y++)
            {
                int xferLen = PAGESIZE;
                bool bResult = outEndpoint.XferData(ref buffs[y], ref xferLen);
            }
            int ack;
            ack = recAck(0x3412);
            //release transfer
            sendCmd(0x061, 0);
        }


        private void setpixel(int x, int y, Color c)
        {
            int addr = y * 1024 + x;
            sendCmd(0x012, addr & 0xFF);
            sendCmd(0x013, (addr >> 8) & 0xFF);
            sendCmd(0x014, (addr >> 16) & 0xFF);

            int val = getpixel(c);
            sendCmd(0x010, val & 0xFF);
            sendCmd(0x011, (val >> 8) & 0xFF);
            sendCmd(0x0B0, 0);
        }

        private int getpixel(Color c)
        {
            int val = 0;
            val += (c.R >> 3) << (5 + 6);
            val += (c.G >> 2) << (5);
            val += (c.B >> 3);
            return val;
        }

        private void button8_Click(object sender, EventArgs e)
        {
            sendCmd(0x023, 0);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            sendCmd(0x024, 0);
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {



        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count <= 0) return;
            transfer(0, ((pack)(listView1.SelectedItems)[0].Tag).buffs);
            int ack;

            this.Text = "";
            Application.DoEvents();

            setaddress(0);


            //BF) begin memcopy
            sendCmd(0xBF, 0);
            ack = recAck(0x3412);

        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            int ack;

            this.Text = "";
            Application.DoEvents();

            int dma_src = trackBar1.Value*4;
            int dma_des = 0;

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



    }
}
