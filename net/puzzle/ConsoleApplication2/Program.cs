using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using ConsoleApplication1;

namespace ConsoleApplication2
{
	class Board
	{
		public string serial;
		public int[] dots = new int[100];
		//public int[] flipdots = new int[100];
		public Board pair;
	}

	class Program
	{
		static List<ulong[]> Data;

		static void Main(string[] args)
		{
			Data = new List<ulong[]>();

			Data.Add(CData.proc(CData.d0));
			Data.Add(CData.proc(CData.d1));
			Data.Add(CData.proc(CData.d2));
			Data.Add(CData.proc(CData.d3));
			Data.Add(CData.proc(CData.d4));
			Data.Add(CData.proc(CData.d5));
			Data.Add(CData.proc(CData.d6));
			Data.Add(CData.proc(CData.d7));
			Data.Add(CData.proc(CData.d8));
			Data.Add(CData.proc(CData.d9));
			Data.Add(CData.proc(CData.d10));
			Data.Add(CData.proc(CData.d11));


			string f1 = @"e:\out.txt";
			string f = @"e:\all.txt";
			FileStream fs1 = new FileStream(f1, FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs1);

			FileStream fs = new FileStream(f, FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			List<Board> boards = new List<Board>();
			while (true)
			{
				string line = sr.ReadLine();
				if (string.IsNullOrEmpty(line))
				{
					break;
				}
				int[] Idx = new int[12];
				for (int i = 0; i < 12; i++)
				{
					Idx[i] = int.Parse(line.Split(',')[i]);
				}

				Board board = new Board();
				board.serial = line;
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						board.dots[i * 10 + j] = -1;
					}
				}

				for (int i = 0; i < 12; i++)
				{
					ulong mask = Data[i][Idx[i]];
					draw(board.dots, mask, i);
				}

				flip(board);

				boards.Add(board);
			}
			List<Board> boards2 = new List<Board>();
			Dictionary<string, Board> boardsd = new Dictionary<string, Board>();
			foreach (var item1 in boards)
			{
				foreach (var item2 in boards)
				{
					bool isSame = true;
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 10; j++)
						{
							if (item1.dots[i * 10 + j] != item2.dots[j * 10 + i])
							{
								isSame = false;
							}
						}
					}
					if (isSame)
					{
						item1.pair = item2;
						item2.pair = item1;
						if (!boardsd.ContainsKey(item1.serial) && !boardsd.ContainsKey(item2.serial))
						{
							boardsd.Add(item1.serial, item1);
							boardsd.Add(item2.serial, item2);
							boards2.Add(item1);
						}
					}
				}
			}
			foreach (var item in boards)
			{
				if (item.pair != null)
				{
				}
				else
				{
					sw.WriteLine(item.serial);
				}
			}
			foreach (var item in boards2)
			{
				sw.WriteLine(item.serial + "\t" + item.pair.serial);
			}

			fs.Close();
			sw.Flush();
			fs1.Close();
		}

		static void flip(Board board)
		{
			//for (int i = 0; i < 10; i++)
			//{
			//	for (int j = 0; j < 10; j++)
			//	{
			//		board.flipdots[j * 10 + i] = board.dots[i * 10 + j];
			//	}
			//}
		}

		static void draw(int[] dots, ulong mask, int idx)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (CData.conv[i + j * 10] != -1)
					{
						if ((mask & ((ulong)1 << CData.conv[i + j * 10])) != 0)
						{
							dots[i * 10 + j] = idx;
						}
					}
				}
			}
		}

	}
}
