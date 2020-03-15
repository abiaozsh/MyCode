using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public class Base : Form
    {
        public SerialPort port;

        public void connect(string portName)
        {
            if (port == null)
            {
                //COM4为Arduino使用的串口号，需根据实际情况调整  115200
                port = new SerialPort(portName, 2000000, Parity.None, 8, StopBits.One);
                port.Open();
            }
        }
        public byte[] readFromPort(int count)
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
                    return buff;
                    //throw new Exception();
                }
            }
        }
        public bool readFromPort(int count, byte[] buff, int idx)
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

        public void portWrite(byte val1, byte val2)
        {
            byte[] buff = new byte[2];
            buff[0] = val1;
            buff[1] = val2;
            if (port != null)
            {
                port.Write(buff, 0, 2);
            }
        }
        public void portWrite(byte val1)
        {
            byte[] buff = new byte[1];
            buff[0] = val1;
            if (port != null)
            {
                port.Write(buff, 0, 1);
            }
        }

        public void portWrite(byte val1, byte val2, Stream s)
        {
            s.WriteByte(val1);
            s.WriteByte(val2);

        }
        public void portWrite(byte val1, Stream s)
        {
            s.WriteByte(val1);
        }

        public void sendall(byte[] buff)
        {
            port.Write(buff, 0, buff.Length);
        }


        string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
        public string getHex4(int val)//"ll"+"hh"
        {
            return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))] + convt[((val & 0xF000) >> 12)] + convt[((val & 0x0F00) >> 8)];
        }
        public string getHex2(int val)//"ll"+"hh"
        {
            return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
        }
        public string getHex(byte[] buff, int idx, int len)//"ll"+"hh"
        {
            StringBuilder sb = new StringBuilder();
            for (int i = idx; i < idx + len; i++)
            {
                byte val = buff[i];
                sb.Append(convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))]);
            }
            return sb.ToString();
        }



    }
}
