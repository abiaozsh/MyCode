﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using WindowsFormsApplication1;

namespace debugger
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        public Form1 mainForm;

        private void Form2_Load(object sender, EventArgs e)
        {
        }
        public class MemPage
        {
            public int page;
            public uint[] data = new uint[256];
        }

        MemPage[] pages = new MemPage[17];

        private void button1_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();


            for (int j = 0; j < pages.Length; j++)
            {
                pages[j] = new MemPage();
                bool dup = true;
                while (dup)
                {
                    pages[j].page = r.Next(0x02000000 >> 10);
                    bool flg = true;
                    for (int i = 0; i < j - 1; i++)
                    {
                        if (pages[j].page == pages[i].page)
                        {
                            flg = false;
                        }
                    }
                    if (flg)
                    {
                        dup = false;
                    }
                }
                for (int i = 0; i < 1024; i += 4)
                {
                    pages[j].data[i >> 2] = (uint)r.Next();
                }

            }

            this.textBox1.Text = "";
            for (int j = 0; j < pages.Length; j++)
            {
                int pageA = pages[j].page;
                uint[] dataA = pages[j].data;
                bool err = true;
                while (err)
                {
                    bool flg = true;
                    try
                    {
                        mainForm.portWrite((byte)(0x00), (byte)(0x00));
                        mainForm.clearBuff();

                        for (int i = 0; i < 1024; i += 4)
                        {
                            uint addr = (uint)((pageA << 10) + i);
                            mainForm.setmem(addr, dataA[i >> 2]);
                        }
                        for (int i = 0; i < 1024; i += 4)
                        {
                            uint addr = (uint)((pageA << 10) + i);
                            uint val = mainForm.getmem(addr, null);
                            if (val != dataA[i >> 2])
                            {
                                flg = false;
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        flg = false;
                    }
                    if (flg)
                    {
                        err = false;
                    }
                    else
                    {
                        this.textBox1.Text += "err";
                    }
                }
                this.textBox1.Text += "," + j;
                Application.DoEvents();
            }

            MessageBox.Show("ok");
        }

        private void confirm(MemPage page, int offset, StringBuilder sb)
        {
            int pageA = page.page;
            uint[] dataA = page.data;

            uint addr = (uint)((pageA << 10) + offset);
            uint val = mainForm.getmem(addr, null);
            if (sb != null)
            {
                sb.Append(" access time:");
                sb.Append(mainForm.getAccessTime());
                if (val != dataA[offset >> 2])
                {
                    sb.Append("addr:" + Util.getHex8(addr) + ",data:" + Util.getHex8(dataA[offset >> 2]) + ",mem:" + Util.getHex8(val));
                }
                sb.AppendLine();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();

            int j = r.Next(pages.Length);
            MemPage pageA = pages[j];

            int i = r.Next(128) << 3;
            confirm(pageA, i, sb);

            i += 4;
            confirm(pageA, i, sb);

            sb.AppendLine("end");
            this.textBox1.Text = sb.ToString();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();

            int j = r.Next(pages.Length);
            MemPage pageA = pages[j];

            int i = r.Next(128) << 3;
            i += 4;
            confirm(pageA, i, sb);

            i -= 4;
            confirm(pageA, i, sb);

            sb.AppendLine("end");
            this.textBox1.Text = sb.ToString();


        }

        private void button4_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();

            int[] points = new int[16];
            for (int i = 0; i < 16; i++)
            {
                points[i] = r.Next(256) << 2;
            }

            for (int i = 0; i < 16; i++)
            {
                confirm(pages[i], points[i], sb);
            }
            sb.AppendLine("------------1");
            {
                int j = 16;
                MemPage pageA = pages[j];

                for (int i = 0; i < 1024; i += 4)
                {
                    confirm(pageA, i, sb);
                }
            }
            sb.AppendLine("------------2");

            for (int i = 0; i < 16; i++)
            {
                int j = r.Next(256) << 2;
                confirm(pages[i], points[i], sb);
            }

            sb.AppendLine("------------3");

            for (int i = 0; i < 16; i++)
            {
                int j = r.Next(256) << 2;
                confirm(pages[i], 0, sb);
            }

            sb.AppendLine("end");
            this.textBox1.Text = sb.ToString();

        }

        private void wbprep(StringBuilder sb, Random r)
        {

            int[] points = new int[192];

            for (int j = 0; j < points.Length; j++)
            {
                bool dup = true;
                while (dup)
                {
                    points[j] = r.Next(256) << 2;
                    bool flg = true;
                    for (int i = 0; i < j - 1; i++)
                    {
                        if (points[j] == points[i])
                        {
                            flg = false;
                        }
                    }
                    if (flg)
                    {
                        dup = false;
                    }
                }
            }

            int[] bytePoses = new int[64];
            for (int j = 0; j < bytePoses.Length; j++)
            {
                bytePoses[j] = r.Next(4);
            }

            //targetpage,读取一部分，写入一部分   部分valid，部分dirty
            for (int i = 0; i < 32; i++)
            {
                confirm(pages[0], points[i], sb);
            }
            sb.AppendLine("------------p1");

            for (int i = 64; i < 128; i++)
            {
                int pageA = pages[0].page;
                uint[] dataA = pages[0].data;
                uint val = (uint)r.Next();
                int offset = points[i];
                uint addr = (uint)((pageA << 10) + offset);
                dataA[offset >> 2] = val;
                mainForm.setmem(addr, val);
                sb.AppendLine("write addr:" + Util.getHex8(addr) + ",data:" + Util.getHex8(dataA[offset >> 2]));

            }
            sb.AppendLine("------------p2");
            for (int i = 128; i < 192; i++)
            {
                if (i < 160)
                {
                    confirm(pages[0], points[i], sb);
                }
                int bytePos = bytePoses[i - 128];
                int pageA = pages[0].page;
                uint[] dataA = pages[0].data;
                int offset = points[i];
                uint addr = (uint)((pageA << 10) + offset);

                uint val = (uint)r.Next();

                mainForm.writeByteEnable(1 << bytePos);
                uint mask = ((uint)(0x0FF)) << (bytePos * 8);
                uint maskrev = ~mask;
                val &= mask;
                dataA[offset >> 2] &= maskrev;
                dataA[offset >> 2] |= val;
                mainForm.setmem(addr, val);
                sb.AppendLine("write addr:" + Util.getHex8(addr) + ",data:" + Util.getHex8(val));
            }
            mainForm.writeByteEnable(15);

            for (int i = 0; i < 256; i++)
            {
                int pageA = pages[0].page;
                uint[] dataA = pages[0].data;
                uint addr = 0;
                int offset = 0;
                bool found = false;
                for (int j = 0; j < points.Length; j++)
                {
                    if ((points[j] >> 2) == i)
                    {
                        offset = points[j];
                        addr = (uint)((pageA << 10) + offset);
                        found = true;
                    }
                }
                if (found)
                {
                    sb.AppendLine("writed addr:" + Util.getHex8(addr) + ",data:" + Util.getHex8(dataA[offset >> 2]));
                }
                else
                {
                    sb.AppendLine("writed addr:" + Util.getHex8(addr) + ",data:");
                }

            }

            sb.AppendLine("------------p3");
            for (int i = 64; i < 128; i++)
            {
                MemPage pageA = pages[0];
                confirm(pageA, points[i], sb);
            }
            sb.AppendLine("------------p4");
            for (int i = 128; i < 160; i++)
            {
                MemPage pageA = pages[0];
                confirm(pageA, points[i], sb);
            }
            sb.AppendLine("------------p5");
            for (int i = 160; i < 192; i++)
            {
                MemPage pageA = pages[0];
                confirm(pageA, points[i], sb);
            }
        }

        //写回：
        private void button5_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();

            wbprep(sb, r);

            //读其他15个cache
            for (int j = 0; j < 800; j++)
            {
                for (int i = 1; i < 16; i++)
                {
                    confirm(pages[i], 0, null);
                }
                this.Text = "" + j;
                Application.DoEvents();
            }

            //读otherpage
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();

            //确认life
            int life;
            int addr;
            string invalid;
            mainForm.getCacheDebugData(0, null, out  addr, out  life, out  invalid);
            if (life != 0)
            {
                MessageBox.Show("life!=0  :" + life);
            }

            sb.AppendLine("------------1");
            //读otherpage
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();

            for (int i = 1; i < 64; i++)
            {
                int offset = r.Next(256) << 2;
                confirm(pages[16], offset, sb);
            }

            //校验15个cache是否受影响
            for (int i = 1; i < 16; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    int offset = r.Next(256) << 2;
                    confirm(pages[i], offset, null);
                }
            }

            //切换，校验

            sb.AppendLine("switch back and check");
            this.textBox1.Text = sb.ToString();

        }
        //switch back and check
        private void button6_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();


            for (int i = 0; i < 256; i++)
            {
                confirm(pages[0], i << 2, sb);
            }

            sb.AppendLine("end");
            this.textBox1.Text = sb.ToString();

        }

        private void button7_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();

            wbprep(sb, r);

            int flushtime = mainForm.FlushCache((uint)(pages[0].page << 10));
            sb.Append(" flush time:");
            sb.Append(flushtime);

            //校验15个cache是否受影响
            for (int i = 1; i < 16; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    int offset = r.Next(256) << 2;
                    confirm(pages[i], offset, null);
                }
            }
            //切换，校验
            sb.AppendLine("switch back and check");
            this.textBox1.Text = sb.ToString();

        }

        private uint getAddr(uint debugAddrB, uint debugAddrA, uint addr)
        {
            return (debugAddrB << (4 + 8 + 2)) | (debugAddrA << (8 + 2)) | (addr << 2);
        }
        private uint CACHE = 1;
        private uint SDRAM = 0;
        private uint WRITEBACK = 2;

        private uint GETFLG = 1;
        private uint GETDATA = 0;
        private void button8_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            Random r = new Random();
            mainForm.portWrite((byte)(0x00), (byte)(0x00));
            mainForm.clearBuff();



            for (uint i = 0; i < 256; i++)
            {
                mainForm.setmem(getAddr(0, SDRAM, i), 0x11111100 | i);
                Thread.Sleep(1);
            }
            for (uint i = 0; i < 256; i++)
            {
                if (i >= 0)
                {
                    mainForm.setmem(getAddr(3, CACHE, i), 0x33333300 | i);
                }
                else
                {
                    mainForm.setmem(getAddr(2, CACHE, i), 0x22222200 | i);
                }
                Thread.Sleep(1);
            }

            for (uint i = 0; i < 256; i++)
            {
                mainForm.getmem(getAddr(GETFLG, CACHE, i), sb);
                sb.Append(" "); Thread.Sleep(1);
                mainForm.getmem(getAddr(GETDATA, CACHE, i), sb);
                sb.AppendLine(" " + Util.getHex8(i)); Thread.Sleep(1);
            }


            mainForm.setmem(getAddr(3, CACHE, 0), 0x33333312);

            if (this.checkBox1.Checked)
            {
                mainForm.getmem(getAddr(0, WRITEBACK, 0), null);
                Thread.Sleep(10);
            }

            for (uint i = 0; i < 256; i++)
            {
                mainForm.getmem(getAddr(0, SDRAM, i), sb);
                sb.AppendLine(" " + Util.getHex8(i)); Thread.Sleep(1);
            }

            sb.AppendLine("end");
            this.textBox1.Text = sb.ToString();

        }

        private int flushTimeTest(int count)
        {

            try
            {
                Random r = new Random();
                mainForm.portWrite((byte)(0x00), (byte)(0x00));
                mainForm.clearBuff();

                int[] points = new int[256];

                for (int j = 0; j < points.Length; j++)
                {
                    bool dup = true;
                    while (dup)
                    {
                        points[j] = r.Next(256) << 2;
                        bool flg = true;
                        for (int i = 0; i < j - 1; i++)
                        {
                            if (points[j] == points[i])
                            {
                                flg = false;
                            }
                        }
                        if (flg)
                        {
                            dup = false;
                        }
                    }
                }

                //for (int i = 0; i < 32; i++)
                //{
                //    mainForm.getmem((uint)(points[i]), null);
                //}

                for (int i = 0; i < count; i++)
                {
                    int offset = points[i];
                    uint addr = (uint)(offset);
                    mainForm.setmem(addr, 0);
                }

                int flushtime = mainForm.FlushCache(0);

                return flushtime;
            }
            catch
            {
                return -1;
            }
        }

        private void button9_Click(object sender, EventArgs e)
        {
            FileStream fs = new FileStream("d:\\flushtime.txt", FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);

            for (int count = 1; count < 256; count++)
            {
                int total = 0;
                for (int i = 0; i < 100; i++)
                {
                    int flushtime = flushTimeTest(count);
                    total += flushtime;
                    sw.Write(flushtime);
                    sw.Write(",");
                    this.Text = count + "," + i;
                    sw.Flush();
                }
                sw.WriteLine();
                sw.Write("total:" + count + "=" + total);
                sw.WriteLine();
                sw.Flush();
            }


        }

    }
}
