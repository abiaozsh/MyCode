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


        public byte[] longread(int addr)
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


        public void initA(int offset)
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
        public void initB(int offset)//256 * n
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


        public void writeFast(byte[] buff, int offset)
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







        public uint readByte(int addr)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x30), (byte)0x00);

            portWrite((byte)(0x70), (byte)0x00);
            return (uint)(readFromPort(1)[0] & 0xFF);
        }
        public uint readWord(int addr)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x31), (byte)0x00);

            portWrite((byte)(0x70), (byte)0x00); uint byte0 = readFromPort(1)[0];
            portWrite((byte)(0x71), (byte)0x00); uint byte1 = readFromPort(1)[0];
            return (byte0 & 0xFF) | (byte1 << 8 & 0xFF00);
        }
        public uint readDWord(int addr)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x32), (byte)0x00);

            portWrite((byte)(0x70), (byte)0x00); uint byte0 = readFromPort(1)[0];
            portWrite((byte)(0x71), (byte)0x00); uint byte1 = readFromPort(1)[0];
            portWrite((byte)(0x72), (byte)0x00); uint byte2 = readFromPort(1)[0];
            portWrite((byte)(0x73), (byte)0x00); uint byte3 = readFromPort(1)[0];
            return (byte0 & 0xFF) | (byte1 << 8 & 0x0FF00) | (byte2 << 16 & 0x0FF0000) | (byte3 << 24 & 0xFF000000);
        }
        public byte[] read8Byte(int addr)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x33), (byte)0x00);

            byte[] data = new byte[8];
            portWrite((byte)(0x70), (byte)0x00); data[0] = readFromPort(1)[0];
            portWrite((byte)(0x71), (byte)0x00); data[1] = readFromPort(1)[0];
            portWrite((byte)(0x72), (byte)0x00); data[2] = readFromPort(1)[0];
            portWrite((byte)(0x73), (byte)0x00); data[3] = readFromPort(1)[0];
            portWrite((byte)(0x74), (byte)0x00); data[4] = readFromPort(1)[0];
            portWrite((byte)(0x75), (byte)0x00); data[5] = readFromPort(1)[0];
            portWrite((byte)(0x76), (byte)0x00); data[6] = readFromPort(1)[0];
            portWrite((byte)(0x77), (byte)0x00); data[7] = readFromPort(1)[0];
            return data;
        }
        public byte[] read16Byte(int addr)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x34), (byte)0x00);

            byte[] data = new byte[16];
            portWrite((byte)(0x70), (byte)0x00); data[0] = readFromPort(1)[0];
            portWrite((byte)(0x71), (byte)0x00); data[1] = readFromPort(1)[0];
            portWrite((byte)(0x72), (byte)0x00); data[2] = readFromPort(1)[0];
            portWrite((byte)(0x73), (byte)0x00); data[3] = readFromPort(1)[0];
            portWrite((byte)(0x74), (byte)0x00); data[4] = readFromPort(1)[0];
            portWrite((byte)(0x75), (byte)0x00); data[5] = readFromPort(1)[0];
            portWrite((byte)(0x76), (byte)0x00); data[6] = readFromPort(1)[0];
            portWrite((byte)(0x77), (byte)0x00); data[7] = readFromPort(1)[0];
            portWrite((byte)(0x78), (byte)0x00); data[8] = readFromPort(1)[0];
            portWrite((byte)(0x79), (byte)0x00); data[9] = readFromPort(1)[0];
            portWrite((byte)(0x7A), (byte)0x00); data[10] = readFromPort(1)[0];
            portWrite((byte)(0x7B), (byte)0x00); data[11] = readFromPort(1)[0];
            portWrite((byte)(0x7C), (byte)0x00); data[12] = readFromPort(1)[0];
            portWrite((byte)(0x7D), (byte)0x00); data[13] = readFromPort(1)[0];
            portWrite((byte)(0x7E), (byte)0x00); data[14] = readFromPort(1)[0];
            portWrite((byte)(0x7F), (byte)0x00); data[15] = readFromPort(1)[0];
            return data;
        }

        public void writeByte(int addr, int data)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            int data0 = (data) & 0xFF;
            portWrite((byte)(0x60), (byte)data0);

            portWrite((byte)(0x40), (byte)0x00);
        }
        public void writeWord(int addr, int data)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            int data0 = (data) & 0xFF;
            int data1 = (data >> 8) & 0xFF;
            portWrite((byte)(0x60), (byte)data0);
            portWrite((byte)(0x61), (byte)data1);

            portWrite((byte)(0x41), (byte)0x00);
        }
        public void writeWord(int addr, byte data0, byte data1)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x60), (byte)data0);
            portWrite((byte)(0x61), (byte)data1);

            portWrite((byte)(0x41), (byte)0x00);
        }
        public void writeDWord(int addr, uint data)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            uint data0 = (data) & 0xFF;
            uint data1 = (data >> 8) & 0xFF;
            uint data2 = (data >> 16) & 0xFF;
            uint data3 = (data >> 24) & 0xFF;
            portWrite((byte)(0x60), (byte)data0);
            portWrite((byte)(0x61), (byte)data1);
            portWrite((byte)(0x62), (byte)data2);
            portWrite((byte)(0x63), (byte)data3);

            portWrite((byte)(0x42), (byte)0x00);
        }
        public void writeDWord(int addr, byte data0, byte data1, byte data2, byte data3)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x60), (byte)data0);
            portWrite((byte)(0x61), (byte)data1);
            portWrite((byte)(0x62), (byte)data2);
            portWrite((byte)(0x63), (byte)data3);

            portWrite((byte)(0x42), (byte)0x00);
        }
        public void write8Byte(int addr, byte[] data)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x60), (byte)data[0]);
            portWrite((byte)(0x61), (byte)data[1]);
            portWrite((byte)(0x62), (byte)data[2]);
            portWrite((byte)(0x63), (byte)data[3]);
            portWrite((byte)(0x64), (byte)data[4]);
            portWrite((byte)(0x65), (byte)data[5]);
            portWrite((byte)(0x66), (byte)data[6]);
            portWrite((byte)(0x67), (byte)data[7]);


            portWrite((byte)(0x43), (byte)0x00);
        }
        public void write16Byte(int addr, byte[] data)
        {
            int addr0 = (addr) & 0xFF;
            int addr1 = (addr >> 8) & 0xFF;
            int addr2 = (addr >> 16) & 0xFF;
            int addr3 = (addr >> 24) & 0xFF;
            portWrite((byte)(0x10), (byte)addr0);
            portWrite((byte)(0x11), (byte)addr1);
            portWrite((byte)(0x12), (byte)addr2);
            portWrite((byte)(0x13), (byte)addr3);

            portWrite((byte)(0x60), (byte)data[0]);
            portWrite((byte)(0x61), (byte)data[1]);
            portWrite((byte)(0x62), (byte)data[2]);
            portWrite((byte)(0x63), (byte)data[3]);
            portWrite((byte)(0x64), (byte)data[4]);
            portWrite((byte)(0x65), (byte)data[5]);
            portWrite((byte)(0x66), (byte)data[6]);
            portWrite((byte)(0x67), (byte)data[7]);
            portWrite((byte)(0x68), (byte)data[8]);
            portWrite((byte)(0x69), (byte)data[9]);
            portWrite((byte)(0x6A), (byte)data[10]);
            portWrite((byte)(0x6B), (byte)data[11]);
            portWrite((byte)(0x6C), (byte)data[12]);
            portWrite((byte)(0x6D), (byte)data[13]);
            portWrite((byte)(0x6E), (byte)data[14]);
            portWrite((byte)(0x6F), (byte)data[15]);


            portWrite((byte)(0x44), (byte)0x00);
        }

        public bool compare(byte[] a1, byte[] a2)
        {
            for (int i = 0; i < a1.Length; i++)
            {
                if (a1[i] != a2[i]) return false;
            }
            return true;
        }
    }
}
