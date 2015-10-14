using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ConsoleApplication1
{
	class Program
	{
		static void Main(string[] args)
		{

			FileStream fs1 = new FileStream(@"e:\\in.txt", FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs1);
			FileStream fs2 = new FileStream(@"e:\\out.txt", FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs2);

			while (true)
			{
				ulong data = 0;
				string line = sr.ReadLine();
				if (string.IsNullOrEmpty(line))
				{
					break;
				}
				string s1 = line.Split('\t')[0];
				string s2 = line.Split('\t')[1];
				foreach (string s in s1.Split(','))
				{
					if (!string.IsNullOrEmpty(s))
					{
						data |= (ulong)1 << (int.Parse(s));
					}
				}
				string hex = CData.getHex4(data);
				sw.WriteLine('\'' + hex + '\t' + s2);
			}
			sw.Flush();
			fs2.Close();
		}
	}
}
