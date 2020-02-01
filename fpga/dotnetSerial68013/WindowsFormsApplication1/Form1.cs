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
        SerialPort port;
        public Form1()
        {
            InitializeComponent();

            this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);
        }

        void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }
        string portName;
        private void Form1_Load(object sender, EventArgs e)
        {
            String[] list = SerialPort.GetPortNames();
            foreach (String s in list)
            {
                this.comboBox1.Items.Add(s);
            }
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

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            portName = (string)this.comboBox1.SelectedItem;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (port == null)
            {
                //COM4为Arduino使用的串口号，需根据实际情况调整  115200
                port = new SerialPort(portName, 2000000, Parity.None, 8, StopBits.One);
                port.Open();
            }
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
                this.Text = "Bulkloop - no device";
            /*
    private void button1_Click(object sender, EventArgs e)
    {
        bool bResult;
        byte[] outData = new byte[1];
        byte[] inData = new byte[1];
        outData[0] = (byte)(0x50+int.Parse(StartValBox.Text));
        int xferLen = 1;
        int xferLenin = 1;
        bResult = outEndpoint.XferData(ref outData, ref xferLen);
        bResult = inEndpoint.XferData(ref inData, ref xferLenin);
        bResult = false;
    }
             */
        }


        byte[] readFromPort(int count)
        {
            if (count <= 0) return null;
            byte[] buff = new byte[count];
            int idx = 0;
            long t = DateTime.Now.Ticks;
            while (true)
            {
                if (port.BytesToRead > 0)
                {
                    buff[idx] = (byte)port.ReadByte();
                    idx++;
                    count--;
                    if (count <= 0) return buff;
                }
                var diff = DateTime.Now.Ticks - t;
                if (diff > 250000)
                {
                    throw new Exception();
                }
            }
        }
        bool readFromPort(int count, byte[] buff, int idx)
        {
            if (count <= 0) return false;
            if (port == null) return false;
            int pos = 0;
            long t = DateTime.Now.Ticks;
            while (true)
            {
                int num = port.BytesToRead;
                if (num > 0)
                {
                    if (num > count)
                    {
                        num = count;
                        count = 0;
                    }
                    else
                    {
                        count -= num;
                    }
                    port.Read(buff, idx + pos, num);
                    pos += num;
                    if (count <= 0) return true;
                }
                var diff = DateTime.Now.Ticks - t;
                if (diff > 250000)
                {
                    throw new Exception();
                }
            }
        }


        private void button2_Click(object sender, EventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
                port = null;
            }
        }
        void portWrite(byte val1, byte val2)
        {
            byte[] buff = new byte[2];
            buff[0] = val1;
            buff[1] = val2;
            if (port != null)
            {
                port.Write(buff, 0, 2);
            }
        }
        void portWrite(byte val1)
        {
            byte[] buff = new byte[1];
            buff[0] = val1;
            if (port != null)
            {
                port.Write(buff, 0, 1);
            }
        }

        void portWrite(byte val1, byte val2, Stream s)
        {
            s.WriteByte(val1);
            s.WriteByte(val2);

        }
        void portWrite(byte val1, Stream s)
        {
            s.WriteByte(val1);
        }

        void sendall(byte[] buff)
        {
            port.Write(buff, 0, buff.Length);
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



        private void button3_Click(object sender, EventArgs e)
        {
            //test uart
            Random r = new Random();
            bool err = false;
            int count = 0;
            for (int i = 0; i < 10; i++)
            {
                byte[] buff = new byte[1];
                r.NextBytes(buff);
                port.Write(buff, 0, 1);

                byte[] buff2 = readFromPort(1);

                if (buff[0] != buff2[0])
                {
                    err = true;
                    count = i;
                    break;
                }
            }
            this.Text = "error:" + count + "," + err.ToString();
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }


        private byte[] longread(int addr)
        {
            try
            {
                int addr0 = (addr) & 0xFF;
                int addr1 = (addr >> 8) & 0xFF;
                int addr2 = (addr >> 16) & 0xFF;

                //if (item.StartsWith("pr"))//put reg
                portWrite((byte)(0x40 + 2), (byte)addr0);
                portWrite((byte)(0x40 + 3), (byte)addr1);
                portWrite((byte)(0x40 + 4), (byte)addr2);

                portWrite((byte)(0xA3));

                byte[] buff = new byte[512 * 2];
                readFromPort(1024, buff, 0);
                byte[] buff2 = readFromPort(1);
                byte check = buff2[0];
                byte b = 0;
                for (int i = 0; i < 1024; i++)
                {
                    b += buff[i];
                }
                if (b != check)
                {
                    throw new Exception();
                }
                return buff;
            }
            catch
            {
                return null;
            }
        }


        private void initA(int offset)
        {
            byte[] buff = new byte[512];
            for (int i = 0; i < 256; i++)
            {
                buff[i] = (byte)i;
            }
            for (int i = 256; i < 512; i++)
            {
                buff[i] = (byte)(511 - i);
            }
            writeFast(buff, offset);
        }
        private void initB(int offset)//256 * n
        {
            byte[] buff = new byte[512];
            for (int i = 0; i < 256; i++)
            {
                buff[i] = (byte)0xFF;
            }
            for (int i = 256; i < 512; i++)
            {
                buff[i] = (byte)0xFF;
            }
            writeFast(buff, offset);
        }

        private void writeFast(byte[] buff, int offset)
        {
            int addr = offset;
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            MemoryStream s = new MemoryStream();
            portWrite((byte)(0x40 + 2), (byte)addr0, s);
            portWrite((byte)(0x40 + 3), (byte)addr1, s);
            portWrite((byte)(0x40 + 4), (byte)addr2, s);
            portWrite((byte)(0xA2), s);
            s.Flush();
            byte[] buff2 = s.ToArray();
            sendall(buff2);

            s = new MemoryStream();
            for (int i = 0; i < 256; i++)
            {
                portWrite((byte)buff[i * 2 + 0], s);
                portWrite((byte)buff[i * 2 + 1], s);
            }
            portWrite(0, 0, s);//end
            s.Flush();
            buff2 = s.ToArray();
            sendall(buff2);

            byte[] data = readFromPort(1);
            var aa = data[0];
        }


        private void button4_Click(object sender, EventArgs e)
        {
            var list = textBox4.Text.Split('\n');

            foreach (var temp in list)
            {
                var item = temp.Trim();
                var idx = item.IndexOf("//");
                if (idx >= 0)
                {
                    item = item.Substring(0, idx);
                }

                if (item == "#dly")
                {
                    Thread.Sleep(100);
                }
                else if (item == "#br")
                {
                    Thread.Sleep(10); this.textBox3.Text += "\r\n";
                }
                else if (item == "ch") //10：out_clk on
                {
                    portWrite((byte)0x10);
                }
                else if (item == "cl")//11：out_clk off
                {
                    portWrite((byte)0x11);
                }
                else if (item == "rh")//12：out_rst on
                {
                    portWrite((byte)0x12);
                }
                else if (item == "rl")//13：out_rst off
                {
                    portWrite((byte)0x13);
                }
                else if (item.StartsWith("in"))//in
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    portWrite((byte)(0x20 + reg));
                    this.textBox3.Text += this.getHex2(readFromPort(1)[0]);
                }
                else if (item.StartsWith("ou"))//out
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    int val = System.Convert.ToInt32(item.Substring(3).Trim(), 16);
                    portWrite((byte)(0x30 + reg), (byte)val);
                }
                else if (item.StartsWith("pr"))//put reg
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    int val = System.Convert.ToInt32(item.Substring(3).Trim(), 16);
                    portWrite((byte)(0x40 + reg), (byte)val);
                }
                else if (item.StartsWith("gr"))//get reg
                {
                    var reg = int.Parse(item.Substring(2, 1).Trim());
                    portWrite((byte)(0x50 + reg));
                    this.textBox3.Text += this.getHex2(readFromPort(1)[0]);
                }
                else if (item.StartsWith("sc"))//special command
                {
                    var val = System.Convert.ToInt32(item.Substring(2, 2).Trim(), 16);
                    portWrite((byte)(val));
                }

            }
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            //buffer.Clear();
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
            //Marshal.
            int size = 16 * 1024 * 1024;
            if (is2M)
            {
                size = 1 * 1024 * 1024;
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
                sendCmd(0x072, k & 0xFF);
                sendCmd(0x073, (k >> 8) & 0xFF);
                sendCmd(0x074, (k >> 16) & 0xFF);
                if (is2M)
                {
                    sendCmd(0x0C0, 0);
                }
                else
                {
                    sendCmd(0x0B0, 0);
                }
                this.Text = "" + (k * 100 / size);
                Application.DoEvents();
            }
            recAck(0x3412);


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


                sendCmd(0x072, k & 0xFF);
                sendCmd(0x073, (k >> 8) & 0xFF);
                sendCmd(0x074, (k >> 16) & 0xFF);
                if (is2M)
                {
                    sendCmd(0x0C1, 0);
                }
                else
                {
                    sendCmd(0x0B1, 0);
                }

                recAck(0);

                lastk = k;
                this.Text = "" + (k * 100 / size);
                Application.DoEvents();
            }
            receive(buff2, lastk << 1);


            int pos = 0;
            for (int i = 0; i < buff.Length; i++)
            {
                if (buff[i] != buff2[i])
                {
                    pos = i;
                    break;
                }
            }


            MessageBox.Show("done" + pos);

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (port != null)
            {
                StringBuilder sb = new StringBuilder();
                while (port.BytesToRead > 0)
                {
                    sb.Append(getHex2(port.ReadByte()) + " ");
                }
                textBox3.Text += sb.ToString();
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
            textBox3.Text = "";
            StringBuilder sb = new StringBuilder();
            //max 24bit
            // 0x1000000
            for (int addr = 0; addr < 0x10000; addr += 0x0200)
            {
                byte[] buff = null;
                while (buff == null)
                {
                    buff = longread(addr);
                }
                sb.Append(receivePage(buff));
                this.Text = (addr / 512).ToString();
                Application.DoEvents();
            }
            textBox3.Text += sb.ToString();

        }

        private void button9_Click(object sender, EventArgs e)
        {
            initB(0x0000);
            initB(0x0100);
            initB(0x0200);
            initB(0x0300);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            textBox4.Text = "scA4\r\n";
            button4_Click(null, null);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            initA(0x0000);
            initA(0x0100);
            initA(0x0200);
            initA(0x0300);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            FileStream fs = new FileStream("c:\\temp\\a.bin", FileMode.Create, FileAccess.Write);
            int count = 0;
            int prog = 0;
            for (int addr = 0; addr < 0x100000; addr += 0x0200)
            {
                byte[] buff = null;
                while (buff == null)
                {
                    buff = longread(addr);
                }
                fs.Write(buff, 0, 1024);
                fs.Flush();
                if (count == 16)
                {
                    count = 0;
                    this.Text = prog.ToString();
                }
                count++;
                prog++;
            }
            fs.Flush();
            fs.Close();
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

        bool is2M = true;

        private void button12_Click(object sender, EventArgs e)
        {
            int k = 0;

            recAck(0);

            sendCmd(0x072, k & 0xFF);
            sendCmd(0x073, (k >> 8) & 0xFF);
            sendCmd(0x074, (k >> 16) & 0xFF);
            if (is2M)
            {
                sendCmd(0x0C1, 0);
            }
            else
            {
                sendCmd(0x0B1, 0);
            }

            recAck(0);

            int size = 1024;
            bool bResult;
            byte[] inData = new byte[size];
            int xferLen = size;
            bResult = inEndpoint.XferData(ref inData, ref xferLen);
            string s = receivePage(inData);
            textBox3.Text += xferLen + "\r\n";
            textBox3.Text += s;

            MessageBox.Show("done");
        }

        private byte[] ranArr(int size)
        {
            Random r = new Random();
            byte[] outData = new byte[size];
            r.NextBytes(outData);
            return outData;
        }

        private void button13_Click(object sender, EventArgs e)
        {
            int ret;
            int val = 0;
            bool first = true;
            {
                int size = 1024;
                bool bResult;
                byte[] outData = ranArr(size);// new byte[size];
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
            int k = 0;
            sendCmd(0x072, k & 0xFF);
            sendCmd(0x073, (k >> 8) & 0xFF);
            sendCmd(0x074, (k >> 16) & 0xFF);
            if (is2M)
            {
                sendCmd(0x0C0, 0);
            }
            else
            {
                sendCmd(0x0B0, 0);
            }

            recAck(0x3412);


            MessageBox.Show("done");

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            this.is2M = this.checkBox1.Checked;
        }



    }
}
