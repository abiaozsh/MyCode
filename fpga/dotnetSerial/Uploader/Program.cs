using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.IO;
using System.Globalization;
using System.Threading;

namespace Uploader
{
	class Program
	{
		static SerialPort port;
		static void Main(string[] args)
		{
			try
			{
				port = new SerialPort(args[0], 115200, Parity.None, 8, StopBits.One);
				port.Open();
			}
			catch
			{
				Console.WriteLine("Port open error：");
				var list = SerialPort.GetPortNames();
				foreach (var item in list)
				{
					Console.WriteLine(item);
				}
			}

			FileStream fs = new FileStream("log.txt", FileMode.Append, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);
			int cnt = 0;
			while (true)
			{
				cnt++;
				Console.WriteLine(cnt);
				int res = test();
				if (res == 0)
				{
					if ((cnt % 1000) == 0)
					{
						sw.WriteLine(cnt + "ok");
					}
				}
				else if (res == -1)
				{
					sw.WriteLine(cnt + "bad");
					Console.WriteLine(cnt + "bad");
				}
				else if (res == -2)
				{
					sw.WriteLine(cnt + "err");
					Console.WriteLine(cnt + "err");
				}

			}
		}

		static int test()
		{
			try
			{

				Random r = new Random();
				string sdata = "";
				for (int i = 0; i < 32; i++)
				{
					sdata += getHex2(r.Next(256));
				}

				//check signature
				{
					portWrite("st");//st Start
					Thread.Sleep(10);
					checkOK();
					portWrite("si");
					string sig = readFromPort(6);
					//1E910B
					portWrite("ed");//ed End
					checkOK();
					if (sig != "1E910B")
					{
						return -2;
					}
				}

				//write
				{
					//1. Load Command “Write Flash” (see Table 19-16 on page 171).
					//2. Load Flash Page Buffer.
					//3. Load Flash High Address and Program Page. Wait after Instr. 3 until SDO goes high for the “Page Programming” cycle to finish.
					//4. Repeat 2 through 3 until the entire Flash is programmed or until all data has been
					//programmed.
					//5. End Page Programming by Loading Command “No Operation”.
					portWrite("st");//st Start
					Thread.Sleep(10);
					checkOK();
					portWrite("er");//st Start
					checkOK();
					portWrite("wf");//wf WriteFlash
					checkOK();

					//16 words attiny24   total 1024word
					int i;
					for (i = 0; i < sdata.Length / 4; i++)
					{
						int addr = (1024 - 16) + i;
						//uint8_t valal = GetByte();
						//uint8_t valdl = GetByte();
						//uint8_t valdh = GetByte();
						//Repeat after Instr. 1 - 7until the entire page buffer is filled or until all data within the page is filled.(2)
						portWrite("pb" + getHex2(addr) + sdata.Substring(i * 4, 4));//Load Flash Page Buffer//先低后高
						checkOK();
						if ((i + 1) % 16 == 0)//15 31 47
						{
							portWrite("ha" + getHex2(addr >> 8));
							checkOK();
						}
					}
					if (i % 16 != 0)//15 31 47
					{
						portWrite("ha" + getHex2(i >> 8));
						checkOK();
					}

					portWrite("no");//nop
					checkOK();
					portWrite("ed");//ed End
					checkOK();
				}

				//read
				StringBuilder sbresult = new StringBuilder();
				{
					portWrite("st");//st Start
					Thread.Sleep(10);
					checkOK();
					portWrite("rf");//rf ReadFlash
					checkOK();

					for (int i = 0; i < sdata.Length / 4; i++)
					{
						int addr = (1024 - 16) + i;
						portWrite("fb" + getHex4(addr));
						string sret = readFromPort(4);
						sbresult.Append(sret);//先低后高
					}

					portWrite("ed");//ed End
					checkOK();
				}

				if (sdata == sbresult.ToString())
				{
					return 0;
				}
				else
				{
					return -1;
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
				return -2;
			}
		}

		static void checkOK()
		{
			if (readFromPort(2) != "OK")
			{
				Console.WriteLine("error!!");
			}
		}

		static void portWrite(string val)
		{
			if (port != null)
			{
				port.Write(val);
			}
		}

		static string readFromPort(int count)
		{
			if (count <= 0) return "";
			StringBuilder _sb = new StringBuilder();
			while (true)
			{
				if (port.BytesToRead > 0)
				{
					_sb.Append((char)port.ReadChar());
					count--;
					if (count <= 0) return _sb.ToString();
				}
			}
		}
		static string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
		static string getHex4(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))] + convt[((val & 0xF000) >> 12)] + convt[((val & 0x0F00) >> 8)];
		}
		static string getHex2(int val)//"lh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
		}

	}
}
