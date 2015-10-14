using System;
using System.Collections.Generic;
using System.Windows.Forms;
using ConsoleApplication1;
using System.IO;
using BigMathLib;

namespace BigMathSample
{
	class Program
	{
		static Int64[] Data;
		static Int64[] Length;
		static Int64[] Idx = new Int64[12];
		static Int64[] Idxend = new Int64[12];

		static void init()
		{
			Data = new Int64[512 * 12];
			Length = new Int64[12];
			proc();

		}
		static void proc()
		{
			int maxcnt = 0;
			for (int i = 0; i < 12; i++)
			{
				int cnt = 0;
				foreach (string s1 in CData.d.Split('\n'))
				{
					string[] s2 = s1.Split('\t');
					if (s2[1] == i.ToString())
					{
						cnt++;
					}
				}
				if (maxcnt < cnt)
				{
					maxcnt = cnt;
				}
				Length[i] = cnt;
				cnt = 0;
				foreach (string s1 in CData.d.Split('\n'))
				{
					string[] s2 = s1.Split('\t');
					{
						if (s2[1] == i.ToString())
						{
							Data[i * 512 + cnt++] = Convert.ToInt64("0x" + (s2[0].Substring(1)), 16);
						}
					}
				}
			}
		}


		static void Main(string[] args)
		{
			init();
			FileStream fs = new FileStream(args[2], FileMode.Append, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);

			{
				string s = args[0];
				for (int i = 0; i < 12; i++)
				{
					Idx[i] = int.Parse(s.Split(',')[i]);
				}
			}
			{
				string s = args[1];
				for (int i = 0; i < 12; i++)
				{
					Idxend[i] = int.Parse(s.Split(',')[i]);
				}
			}

			while (true)
			{
				Int64[] result = new Int64[1];

				BigMath.Add(Data, Length, Idx, result);
				if (result[0] == 1)
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
						if (Idx[i] >= Length[i])
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
