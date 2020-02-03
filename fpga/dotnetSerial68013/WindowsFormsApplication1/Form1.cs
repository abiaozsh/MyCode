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
            int addr = Convert.ToInt32(textBox1.Text, 16);
            sendCmd(0x012, addr & 0xFF);
            sendCmd(0x013, (addr >> 8) & 0xFF);
            sendCmd(0x014, (addr >> 16) & 0xFF);

            sendCmd(0x0F1, 0); Thread.Sleep(10);
            sendCmd(0x0F2, 0); Thread.Sleep(10);

        }

        private void button4_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 1024; i++)
            {
                sendCmd(0x012, i & 0xFF);
                sendCmd(0x013, (i >> 8) & 0xFF);
                sendCmd(0x014, (i >> 16) & 0xFF);

                sendCmd(0x0F3, 0);
                sendCmd(0x0F4, 0);

                int data = recAck(0);
                if ((i & 15) == 0)
                {
                    textBox3.Text += "\r\n";
                }
                textBox3.Text += getHex4(data) + " ";
            }

        }

        private void button7_Click_1(object sender, EventArgs e)
        {
            Bitmap b = new Bitmap("e:\\z051.jpg");
            loadimg(b);
        }
        private void button14_Click(object sender, EventArgs e)
        {
            Bitmap b = new Bitmap("e:\\z043.jpg");
            loadimg(b);
        }

        private void loadimg2(Bitmap b)
        {

            bool first = true;
            for (int y = 0; y < 768; y++)
            {
                this.Text = "" + y;
                Application.DoEvents();
                {
                    {
                        int size = 1024;
                        bool bResult;
                        byte[] outData = new byte[size];
                        for (int x = 0; x < 512; x++)
                        {
                            var c = b.GetPixel(x, y);
                            int val = getpixel(c);
                            outData[x << 1] = (byte)(val & 0xFF);
                            outData[(x << 1) + 1] = (byte)((val >> 8) & 0xFF);
                        }
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

                    int addr = y * 1024;
                    sendCmd(0x012, addr & 0xFF);
                    sendCmd(0x013, (addr >> 8) & 0xFF);
                    sendCmd(0x014, (addr >> 16) & 0xFF);

                    sendCmd(0x0B0 + (this.doublePage.Checked ? 0x0F : 2), 0);
                }
                {
                    {
                        int size = 1024;
                        bool bResult;
                        byte[] outData = new byte[size];
                        for (int x = 0; x < 512; x++)
                        {
                            var c = b.GetPixel(x + 512, y);
                            int val = getpixel(c);
                            outData[x << 1] = (byte)(val & 0xFF);
                            outData[(x << 1) + 1] = (byte)((val >> 8) & 0xFF);
                        }
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

                    int addr = y * 1024 + 512;
                    sendCmd(0x012, addr & 0xFF);
                    sendCmd(0x013, (addr >> 8) & 0xFF);
                    sendCmd(0x014, (addr >> 16) & 0xFF);

                    sendCmd(0x0B0 + (this.doublePage.Checked ? 0x0F : 2), 0);
                }
            }
            recAck(0x3412);
        }

        private void loadimg(Bitmap b)
        {

            for (int y = 0; y < 768; y++)
            {
                this.Text = "" + y;
                Application.DoEvents();
                {
                    {
                        int size = 1024;
                        bool bResult;
                        byte[] outData = new byte[size];
                        for (int x = 0; x < 512; x++)
                        {
                            var c = b.GetPixel(x, y);
                            int val = getpixel(c);
                            outData[x << 1] = (byte)(val & 0xFF);
                            outData[(x << 1) + 1] = (byte)((val >> 8) & 0xFF);
                        }
                        int xferLen = size;
                        bResult = outEndpoint.XferData(ref outData, ref xferLen);
                    }

                    int addr = y * 1024;
                    sendCmd(0x012, addr & 0xFF);
                    sendCmd(0x013, (addr >> 8) & 0xFF);
                    sendCmd(0x014, (addr >> 16) & 0xFF);

                    sendCmd(0x0B0 + (this.doublePage.Checked?0x0F: 2), 0);

                    recAck(0x3412);
                }
                {
                    {
                        int size = 1024;
                        bool bResult;
                        byte[] outData = new byte[size];
                        for (int x = 0; x < 512; x++)
                        {
                            var c = b.GetPixel(x + 512, y);
                            int val = getpixel(c);
                            outData[x << 1] = (byte)(val & 0xFF);
                            outData[(x << 1) + 1] = (byte)((val >> 8) & 0xFF);
                        }
                        int xferLen = size;
                        bResult = outEndpoint.XferData(ref outData, ref xferLen);
                    }

                    int addr = y * 1024 + 512;
                    sendCmd(0x012, addr & 0xFF);
                    sendCmd(0x013, (addr >> 8) & 0xFF);
                    sendCmd(0x014, (addr >> 16) & 0xFF);

                    sendCmd(0x0B0 + (this.doublePage.Checked ? 0x0F : 2), 0);


                    recAck(0x3412);
                }
            }
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
            sendCmd((0x0B0 + 0), 0);
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



    }
}
