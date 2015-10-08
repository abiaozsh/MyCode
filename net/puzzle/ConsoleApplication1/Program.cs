using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ConsoleApplication1
{
	class Program
	{
		static List<ulong[]> Data;

		static void init()
		{
			Data = new List<ulong[]>();

			Data.Add(proc(CData.d0));
			Data.Add(proc(CData.d1));
			Data.Add(proc(CData.d2));
			Data.Add(proc(CData.d3));
			Data.Add(proc(CData.d4));
			Data.Add(proc(CData.d5));
			Data.Add(proc(CData.d6));
			Data.Add(proc(CData.d7));
			Data.Add(proc(CData.d8));
			Data.Add(proc(CData.d9));
			Data.Add(proc(CData.d10));
			Data.Add(proc(CData.d11));

		}
		static ulong[] proc(string s)
		{
			int cnt = 0;
			foreach (string s1 in s.Split('\n'))
			{
				string[] s2 = s1.Split(',');
				if (s2[4] != "x")
				{
					cnt++;
				}
			}
			ulong[] dat = new ulong[cnt];
			cnt = 0;
			foreach (string s1 in s.Split('\n'))
			{
				string[] s2 = s1.Split(',');
				if (s2[4] != "x")
				{
					dat[cnt++] = Convert.ToUInt64("0x" + (s2[0].Substring(1)), 16);
				}
			}

			return dat;
		}


		static void Main(string[] args)
		{
			init();
			FileStream fs = new FileStream(args[2], FileMode.Append, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);

			int[] Idx = new int[12];
			{
				string s = args[0];
				for (int i = 0; i < 12; i++)
				{
					Idx[i] = int.Parse(s.Split(',')[i]);
				}
			}
			int[] Idxend = new int[12];
			{
				string s = args[1];
				for (int i = 0; i < 12; i++)
				{
					Idxend[i] = int.Parse(s.Split(',')[i]);
				}
			}

			while (true)
			{
				for (int times = 0; times < 100000000; times++)
				{
					ulong board = 0;
					int last = 0;
					int error = 0;
					for (int i = 0; i < 12; i++)
					{
						ulong mask = Data[i][Idx[i]];
						if ((board & mask) == 0)
						{
							board |= mask;
						}
						else
						{
							last = i;
							error = 1;
							break;
						}
					}
					if (error != 0)
					{
						if (last < 11)
						{
							Idx[last + 1] = 0;
						}
						for (int i = last; i >= 0; i--)
						{
							Idx[i]++;
							if (Idx[i] >= Data[i].Length)
							{
								Idx[i] = 0;
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						string s = "";
						for (int i = 0; i < 12; i++)
						{
							s += Idx[i] + ",";
						}
						Console.WriteLine("bingo:" + s);
						sw.WriteLine(s);
						sw.Flush();
						fs.Flush();
						for (int i = 11; i >= 0; i--)
						{
							Idx[i]++;
							if (Idx[i] >= Data[i].Length)
							{
								Idx[i] = 0;
							}
							else
							{
								break;
							}
						}
					}
				}
				{
					string s = "";
					for (int i = 0; i < 12; i++)
					{
						s += Idx[i] + ",";
					}
					Console.WriteLine(s);
				}
				bool isEnd = true;
				for (int i = 0; i < 12; i++)
				{
					if (Idx[i] < Idxend[i])
					{
						isEnd = false;
						break;
					}
					else if (Idx[i] > Idxend[i])
					{
						isEnd = true;
						break;
					}
				}
				if (isEnd)
				{
					return;
				}
			}

		}
	}
}
