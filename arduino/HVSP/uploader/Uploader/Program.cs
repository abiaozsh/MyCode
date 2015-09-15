using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.IO;
using System.Globalization;

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
				try
				{
				port.Open();
				}
				catch
				{
					Console.WriteLine("Port open error：");
					var list = SerialPort.GetPortNames();
					foreach(var item in list)
					{
						Console.WriteLine(item);
					}
					return;
				}
				FileStream fs = new FileStream(args[1], FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				StringBuilder sbdata = new StringBuilder();

				while (true)
				{
					string line = sr.ReadLine();
					if (line == null) break;

					int length = int.Parse(line.Substring(1, 2), NumberStyles.HexNumber);

					string data = line.Substring(9, length * 2);

					sbdata.Append(data);
				}
				fs.Close();

				string sdata = sbdata.ToString();

				if (sdata.Length == 0) return;

				//check signature
				{
					portWrite("st");//st Start
					checkOK();
					portWrite("si");
					string sig = readFromPort(6);
					Console.WriteLine(sig);
					//1E910B
					portWrite("ed");//ed End
					checkOK();
					if (sig != "1E910B")
					{
						Console.WriteLine("Signature error!!!");
						return;
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
					checkOK();
					portWrite("er");//st Start
					checkOK();
					portWrite("wf");//wf WriteFlash
					checkOK();

					//16 words attiny24
					int i;
					for (i = 0; i < sdata.Length / 4; i++)
					{
						//uint8_t valal = GetByte();
						//uint8_t valdl = GetByte();
						//uint8_t valdh = GetByte();
						//Repeat after Instr. 1 - 7until the entire page buffer is filled or until all data within the page is filled.(2)
						Console.Write(sdata.Substring(i * 4, 4));
						portWrite("pb" + getHex2(i) + sdata.Substring(i * 4, 4));//Load Flash Page Buffer//先低后高
						checkOK();
						if ((i + 1) % 16 == 0)//15 31 47
						{
							portWrite("ha" + getHex2(i >> 8));
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
				Console.WriteLine();

				//read
				StringBuilder sbresult = new StringBuilder();
				{
					portWrite("st");//st Start
					checkOK();
					portWrite("rf");//rf ReadFlash
					checkOK();

					for (int i = 0; i < sdata.Length / 4; i++)
					{
						portWrite("fb" + getHex4(i));
						string sret = readFromPort(4);
						sbresult.Append(sret);//先低后高
						Console.Write(sret);
					}

					portWrite("ed");//ed End
					checkOK();
				}
				Console.WriteLine();

				if (sdata != sbresult.ToString())
				{
					Console.WriteLine("verify error");
				}
				else
				{
					Console.WriteLine("verify ok");
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
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
		static string getHex2(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
		}

	}
}
