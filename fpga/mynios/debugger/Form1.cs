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
using debugger;
using SDcard;

namespace WindowsFormsApplication1
{

	public sealed partial class Form1 : Base
	{
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
			this.comboBox2.SelectedIndex = 0;

			foreach (var item in this.Controls)
			{
				(item as Control).Enabled = false;
			}
			this.comboBox1.Enabled = true;
			this.button1.Enabled = true;
			this.button10.Enabled = true;
			this.textBox3.Enabled = true;
			loadSym();
		}
		private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			portName = (string)this.comboBox1.SelectedItem;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			connect(portName);
			foreach (var item in this.Controls)
			{
				(item as Control).Enabled = true;
			}
		}

		List<CodeSym> syms;

		private void button2_Click(object sender, EventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
				port = null;
			}
		}

		private void button3_Click(object sender, EventArgs e)
		{
			//test uart
			Random r = new Random();
			bool err = false;
			int count = 0;
			for (int i = 0; i < 100000; i++)
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



		private void button4_Click(object sender, EventArgs e)
		{
			getstatus();
		}
		public void getstatus()
		{
			portWrite((byte)(0x00), (byte)(0x00));
			int baseaddr = 0;

			StringBuilder sb = new StringBuilder();
			sb.Append("pc:");
			int pc = getreg(0x43, 0x00, sb);
			byte[] temp;
			sb.Append("  last cmd:");
			portWrite((byte)(0x18), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));
			sb.Append("  Rtype:");
			portWrite((byte)(0x19), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0]));

			int halt_cpu;
			sb.Append("  halt_cpu:");
			portWrite((byte)(0x14), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0])); halt_cpu = temp[0];
			int halt_uart;
			sb.Append("  halt_uart:");
			portWrite((byte)(0x15), (byte)0x00); temp = readFromPort(1); sb.Append(Util.getHex2(temp[0])); halt_uart = temp[0];
			sb.AppendLine();

			if (halt_cpu != 0 || halt_uart != 0)
			{

				sb.Append("r0:");
				getreg(0x47, 0x00, sb);
				sb.Append(" r1:");
				getreg(0x47, 0x01, sb);

				sb.Append("   r2:");
				getreg(0x47, 0x02, sb);
				sb.Append(" r3:");
				getreg(0x47, 0x03, sb);
				sb.AppendLine();

				for (int i = 4; i < 8; i++)
				{
					sb.Append(" r" + i + ":");
					getreg(0x47, (byte)i, sb);
				}
				sb.AppendLine();

				for (int i = 8; i < 16; i++)
				{
					sb.Append(" r" + i + ":");
					getreg(0x47, (byte)i, sb);
				}
				sb.AppendLine();

				for (int i = 16; i < 24; i++)
				{
					sb.Append(" r" + i + ":");
					getreg(0x47, (byte)i, sb);
				}
				sb.AppendLine();

				sb.Append("sp:");
				int sp = getreg(0x47, 27, sb);

				sb.Append("    fp:");
				getreg(0x47, 28, sb);
				sb.AppendLine();

				sb.Append("cs:");
				int cs = getreg(0x44, 0, sb);
				sb.Append("   ds:");
				int ds = getreg(0x45, 0, sb);
				sb.AppendLine();

				sb.Append(" ra:");
				getreg(0x47, 31, sb);
				sb.AppendLine();


				sb.AppendLine("stack: ");
				baseaddr = sp + ds;
				for (int i = -Convert.ToInt32(this.comboBox2.Text) * 4; i < Convert.ToInt32(this.comboBox2.Text) * 4; i += 4)
				{
					sb.Append((i == 0 ? "*" : " ") + Util.getHex8((uint)(i + baseaddr)) + ":"); getmem(i + baseaddr, sb); sb.AppendLine();
				}

				baseaddr = 0;
				sb.AppendLine("code: ");
				for (int i = -32; i < 32; i += 4)
				{
					StringBuilder sb2 = new StringBuilder();
					int code = getmem(pc + cs + i, sb2);
					int target = pc + i;
					string foundsym = Config.getSym(target, syms);
					string scode = Config.dasm(syms, (uint)code, pc + i, baseaddr);
					if (i == 0)
					{
						sb.AppendLine("----------------------------------------------------------------------------------");
					}
					sb.AppendLine((i == 0 ? "*" : " ") + Util.getHex8((uint)(pc + cs + i)) + ":" + scode);
					if (i == 0)
					{
						sb.AppendLine("----------------------------------------------------------------------------------");
					}
				}

				sb.AppendLine("mem: ");
				baseaddr = Convert.ToInt32(textBox1.Text, 16);
				for (int i = 0; i < Convert.ToInt32(this.comboBox2.Text) * 4; i += 4)
				{
					sb.Append("" + Util.getHex8((uint)(i + baseaddr)) + ":"); getmem(i + baseaddr, sb); sb.AppendLine();
				}
			}

			this.textBox4.Text = sb.ToString();

		}

		private void textBox4_TextChanged(object sender, EventArgs e)
		{

		}
		public int getreg(byte cmd, byte reg, StringBuilder sb)
		{
			portWrite(cmd, reg);
			byte[] temp;
			int val = 0;
			portWrite((byte)(0x13), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 24; sb.Append(Util.getHex2(temp[0]));
			portWrite((byte)(0x12), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 16; sb.Append(Util.getHex2(temp[0]));
			portWrite((byte)(0x11), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 8; sb.Append(Util.getHex2(temp[0]));
			portWrite((byte)(0x10), (byte)0x00); temp = readFromPort(1); val |= temp[0] << 0; sb.Append(Util.getHex2(temp[0]));
			return val;
		}

		public int getmem(int addr, StringBuilder sb)
		{
			portWrite((byte)(0x20), (byte)((addr >> 0) & 0xFF));
			portWrite((byte)(0x21), (byte)((addr >> 8) & 0xFF));
			portWrite((byte)(0x22), (byte)((addr >> 16) & 0xFF));
			portWrite((byte)(0x23), (byte)((addr >> 24) & 0xFF));

			portWrite((byte)(0x30), 0);
			byte[] temp;
			int val = 0;
			portWrite((byte)(0x13), (byte)0x00);
			portWrite((byte)(0x12), (byte)0x00);
			portWrite((byte)(0x11), (byte)0x00);
			portWrite((byte)(0x10), (byte)0x00);
			temp = readFromPort(4);
			val |= temp[0] << 24;
			val |= temp[1] << 16;
			val |= temp[2] << 8;
			val |= temp[3] << 0;
			if (sb != null)
			{
				sb.Append(Util.getHex2(temp[0]));
				sb.Append(Util.getHex2(temp[1]));
				sb.Append(Util.getHex2(temp[2]));
				sb.Append(Util.getHex2(temp[3]));
			}
			return val;
		}
		public void setmem(int addr, int data)
		{
			portWrite((byte)(0x20), (byte)((addr >> 0) & 0xFF));
			portWrite((byte)(0x21), (byte)((addr >> 8) & 0xFF));
			portWrite((byte)(0x22), (byte)((addr >> 16) & 0xFF));
			portWrite((byte)(0x23), (byte)((addr >> 24) & 0xFF));

			portWrite((byte)(0x24), (byte)((data >> 0) & 0xFF));
			portWrite((byte)(0x25), (byte)((data >> 8) & 0xFF));
			portWrite((byte)(0x26), (byte)((data >> 16) & 0xFF));
			portWrite((byte)(0x27), (byte)((data >> 24) & 0xFF));

			portWrite((byte)(0x31), 0);
		}

		private void button2_Click_1(object sender, EventArgs e)
		{
			portWrite((byte)(0x00), (byte)(0x00));
			int addr = Convert.ToInt32(textBox1.Text, 16);
			StringBuilder sb = new StringBuilder();
			getmem(addr, sb);
			textBox2.Text = sb.ToString();

		}
		private void button6_Click(object sender, EventArgs e)
		{
			portWrite((byte)(0x00), (byte)(0x00));
			int addr = Convert.ToInt32(textBox1.Text, 16);
			setmem(addr, Convert.ToInt32(textBox2.Text, 16));
		}


		private void checkBox1_CheckedChanged(object sender, EventArgs e)
		{
			if (checkBox1.Checked)
			{
				portWrite((byte)(0x01), 1);
			}
			else
			{
				portWrite((byte)(0x01), 0);
			}

		}

		private void button3_Click_1(object sender, EventArgs e)
		{
			portWrite((byte)(0x03), 0);
			getstatus();
		}

		private void button5_Click(object sender, EventArgs e)
		{
			readFromPort(10);
		}

		private void button7_Click(object sender, EventArgs e)
		{
			portWrite((byte)(0x16), (byte)0x00); var temp = readFromPort(1);
			textBox2.Text += temp[0];
			portWrite((byte)(0x17), (byte)0x00); temp = readFromPort(1);
			textBox2.Text += temp[0];
		}

		private void loadSym()
		{
			syms = new List<CodeSym>();
			FileStream fs = new FileStream("temp.sym", FileMode.Open, FileAccess.Read);
			StreamReader sw = new StreamReader(fs);
			string s = sw.ReadToEnd();
			sw.Close();
			fs.Close();
			foreach (var line in s.Split('\n'))
			{
				if (line != "")
				{
					CodeSym sym = new CodeSym();
					sym.pos = int.Parse(line.Split(',')[0]);
					sym.name = line.Split(',')[1];
					syms.Add(sym);
				}
			}
		}


		private void button8_Click(object sender, EventArgs e)
		{
			portWrite((byte)(0x00), (byte)(0x00));


			loadSym();

			int baseAddr = 0x02000000;

			portWrite((byte)(0x01), 1);//halt_uart

			{
				FileStream fs = new FileStream("out.hex", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				String s = sr.ReadToEnd();
				sr.Close();
				fs.Close();

				int index = 0;
				foreach (var item in s.Split('\n'))
				{
					if (item.Length == ":040016001005003a97".Length + 1)
					{
						//:04001600 1005003a 97
						String data = item.Substring(9, 8);
						setmem(baseAddr + index, Convert.ToInt32(data, 16));
						index += 4;
						this.Text = index.ToString();
						Application.DoEvents();
					}
				}
			}


			//      end else if (command == 8'h02) begin debug_reset_n<=data[0]; command_done<=1;

			portWrite((byte)(0x02), 0);
			portWrite((byte)(0x02), 1);
			portWrite((byte)(0x01), 0);//halt_uart
		}

		private void button9_Click(object sender, EventArgs e)
		{
			portWrite((byte)(0x00), (byte)(0x00));
			
			portWrite((byte)(0x02), 0);
			portWrite((byte)(0x02), 1);

		}

		private void button10_Click(object sender, EventArgs e)
		{
			/*
struct bpb_t {//biosParmBlock
  char bytesPerSector_0;
  char bytesPerSector_1;
  char sectorsPerCluster;
  char reservedSectorCount_0;
  char reservedSectorCount_1;
  char fatCount; 
  char rootDirEntryCount_0;
  char rootDirEntryCount_1;
  char totalSectors16_0;
  char totalSectors16_1;

  char mediaType;

  char sectorsPerFat16_0;
  char sectorsPerFat16_1;
  char sectorsPerTrtack_0;
  char sectorsPerTrtack_1;
  char headCount_0;
  char headCount_1;
  char hidddenSectors_0;
  char hidddenSectors_1;
  char hidddenSectors_2;
  char hidddenSectors_3;

  char totalSectors32_0;
  char totalSectors32_1;
  char totalSectors32_2;
  char totalSectors32_3;
  char sectorsPerFat32_0;
  char sectorsPerFat32_1;
  char sectorsPerFat32_2;
  char sectorsPerFat32_3;
  char fat32Flags_0;
  char fat32Flags_1;

  char fat32Version_0;
  char fat32Version_1;
  char fat32RootCluster_0;
  char fat32RootCluster_1;
  char fat32RootCluster_2;
  char fat32RootCluster_3;
  char fat32FSInfo_0;
  char fat32FSInfo_1;
  char fat32BackBootBlock_0;
  char fat32BackBootBlock_1;

  char fat32Reserved[12];
};41+12

struct fbs_t {//fat32BootSector
  char jmpToBootCode[3]; 3
  char oemName[8]; 8 
  bpb_t bpb; 41+12

  char driveNumber;
  char reserved1;
  char bootSignature;
  char volumeSerialNumber_0;
  char volumeSerialNumber_1;
  char volumeSerialNumber_2;
  char volumeSerialNumber_3;  7

  char volumeLabel[11]; 11 
  char fileSystemType[8]; 8
  char bootCode[420];
  char bootSectorSig0;
  char bootSectorSig1;
};
struct dir_t {//directoryEntry
  char name[11];
  char attributes;
  char reservedNT;
  char creationTimeTenths;
  char creationTime_0;
  char creationTime_1;

  char creationDate_0;
  char creationDate_1;
  char lastAccessDate_0;
  char lastAccessDate_1;
  char firstClusterHigh_0; 20
  char firstClusterHigh_1;
  char lastWriteTime_0;
  char lastWriteTime_1;
  char lastWriteDate_0;
  char lastWriteDate_1;
  char firstClusterLow_0; 26
  char firstClusterLow_1;
  char fileSize_0;
  char fileSize_1;
  char fileSize_2;
  char fileSize_3;
};

			 */
			SDUtils sd = new SDUtils(this.textBox3.Text + ":");
			byte[] fbs = sd.ReadSector(0);
			int volumeStartBlock = 0;
			int reservedSectorCount = fbs[3 + 8 + 3 + 0] | (fbs[3 + 8 + 3 + 1] << 8);
			int fatStartBlock = volumeStartBlock + reservedSectorCount;

			int fatCount = fbs[3 + 8 + 5];
			int blocksPerFat = fbs[3 + 8 + 11 + 0] | (fbs[3 + 8 + 11 + 1] << 8);
			int rootDirStart = fatStartBlock + fatCount * blocksPerFat;

			int rootDirEntryCount = fbs[3 + 8 + 6 + 0] | (fbs[3 + 8 + 6 + 1] << 8);

			int dataStartBlock = rootDirStart + (((rootDirEntryCount << 5) + 511) >> 9);


			byte[] root = sd.ReadSector(rootDirStart);
			string name = "BOOT    BIN";
			//string name = "TEST1   TXT";
			int index = -1;
			for (int i = 0; i < 512; i += 32)
			{
				int match = 1;
				for (int j = 0; j < 11; j++)
				{
					if (root[i + j] != (byte)name[j])
					{
						match = 0;
						break;
					}
				}
				if (match == 1)
				{
					index = i;
				}
			}

			int firstCluster;
			int fileSize;
			if (index >= 0)
			{
				firstCluster = root[index + 26 + 0] | (root[index + 26 + 1] << 8) | (root[index + 20 + 0] << 16) | (root[index + 20 + 1] << 24);
				fileSize = root[index + 28 + 0] | (root[index + 28 + 1] << 8) | (root[index + 28 + 2] << 16) | (root[index + 28 + 3] << 24);
			}
			else
			{
				throw new Exception("not found boot.bin");
			}

			short[] fat = new short[blocksPerFat * 256];
			for (int i = 0; i < blocksPerFat; i++)
			{
				byte[] fattemp = sd.ReadSector(fatStartBlock + i);
				for (int j = 0; j < 256; j++)
				{
					fat[i * 256 + j] = (short)(fattemp[j * 2] + (fattemp[j * 2 + 1] << 8));
				}
			}

			int blocks = 0;
			int start = firstCluster;
			while (true)
			{
				int oldStart = start;
				start = fat[start];
				blocks++;
				if (start == -1)
				{
					break;
				}
				if (start != oldStart + 1)
				{
					throw new Exception("not continue block");
				}

			}

			int sectorsPerCluster = fbs[3 + 8 + 2];

			int blockOfCluster = 0;
			int firstBlock = dataStartBlock + (firstCluster - 2) * sectorsPerCluster + blockOfCluster;

			byte[] data = sd.ReadSector(firstBlock);


			int bootrec = 3 + 8 + 41 + 12 + 7 + 11 + 8;
			fbs[bootrec + 0] = 0x12;
			fbs[bootrec + 1] = 0x34;
			fbs[bootrec + 2] = 0x56;
			fbs[bootrec + 3] = 0x78;

			fbs[bootrec + 4] = (byte)((firstBlock >> 0) & 0xFF);
			fbs[bootrec + 5] = (byte)((firstBlock >> 8) & 0xFF);
			fbs[bootrec + 6] = (byte)((firstBlock >> 16) & 0xFF);
			fbs[bootrec + 7] = (byte)((firstBlock >> 24) & 0xFF);

			fbs[bootrec + 8] = (byte)((blocks >> 0) & 0xFF);
			fbs[bootrec + 9] = (byte)((blocks >> 8) & 0xFF);
			fbs[bootrec + 10] = (byte)((blocks >> 16) & 0xFF);
			fbs[bootrec + 11] = (byte)((blocks >> 24) & 0xFF);

			sd.WriteSector(fbs, 0);
			MessageBox.Show("ok");
			sd.Close();
		}

		private void button11_Click(object sender, EventArgs e)
		{
			portWrite((byte)(0x00), (byte)(0x00));


			loadSym();

			int baseAddr = 0x02000000;

			portWrite((byte)(0x01), 1);//halt_uart

			{
				setmem(0x02000000, Convert.ToInt32("0000000A", 16));
			}

			baseAddr = 0x00000000;

			{
				FileStream fs = new FileStream("out.hex", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				String s = sr.ReadToEnd();
				sr.Close();
				fs.Close();

				int index = 0;
				foreach (var item in s.Split('\n'))
				{
					if (item.Length == ":040016001005003a97".Length + 1)
					{
						//:04001600 1005003a 97
						String data = item.Substring(9, 8);
						setmem(baseAddr + index, Convert.ToInt32(data, 16));
						index += 4;
						if ((index & 0x0FF) == 0)
						{
							this.Text = index.ToString();
							Application.DoEvents();
						}
					}
				}
				index = 0;
				foreach (var item in s.Split('\n'))
				{
					if (item.Length == ":040016001005003a97".Length + 1)
					{
						//:04001600 1005003a 97
						String data = item.Substring(9, 8);
						int a = Convert.ToInt32(data, 16);
						int b = getmem(baseAddr + index, null);
						if (a != b)
						{
							MessageBox.Show("err," + a + "," + b);
							return;
						}
						index += 4;
						if ((index & 0x0FF) == 0)
						{
							this.Text = index.ToString();
							Application.DoEvents();
						}
					}
				}
			}


			//      end else if (command == 8'h02) begin debug_reset_n<=data[0]; command_done<=1;

			portWrite((byte)(0x02), 0);
			portWrite((byte)(0x02), 1);
			portWrite((byte)(0x01), 0);//halt_uart
		}


	}
}
