﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace debugger
{
	public class CodeSym
	{
		public string name;
		public int pos;
	}
	public class DataSym
	{
		public string name;
		public int pos;
	}

	public class Config
	{
		public string text;
		public string scmd;
		public string op1;
		public string op2;
		public int textformat;
		public int insformat;
		public int cmd;
		public Config(string text)
		{
			this.text = text;
			string[] temparr;
			int pos1 = text.IndexOf(" ");
			if (pos1 == -1)
			{
				scmd = text;
				temparr = new string[0];
			}
			else
			{
				scmd = text.Substring(0, pos1);
				string temp = text.Substring(pos1);
				temparr = temp.Split(',');
			}
			if (temparr.Length == 2)
			{
				op1 = temparr[0].Trim();
				op2 = temparr[1].Trim();
			}
			else if (temparr.Length == 1)
			{
				op1 = temparr[0].Trim();
			}
		}
		public static List<Config> loadConfig(string path)
		{
			List<Config> cfgs = new List<Config>();
			{
				FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				var temp = sr.ReadToEnd().Split('\n');
				foreach (var txt in temp)
				{
					var item = txt.Trim();
					if (item.Length == 0 || item.StartsWith("#"))
					{
						continue;
					}
					var arr = item.Split('@');
					Config cfg = new Config(arr[0].Trim());
					cfg.textformat = int.Parse(arr[1]);
					cfg.insformat = int.Parse(arr[2]);
					cfg.cmd = int.Parse(arr[3]);
					cfgs.Add(cfg);
				}

				fs.Close();
			}
			return cfgs;
		}

		public static string dasm(List<Config> cfgs, List<CodeSym> syms, int code, int pc, ref bool extend)
		{
			if (extend)
			{
				extend = false;
				return "(" + code + ")";
			}
			int cmd = (code >> 24) & 0xFF;
			int reg1 = (code >> 20) & 0xF;
			int reg2 = (code >> 16) & 0xF;
			long temp = ((code) & 0xFFFF);
			if ((temp & 0x8000) != 0)
			{
				temp |= 0xFFFF0000L;
			}
			int ins10 = (int)temp;
			temp = ((code) & 0xFFFFFF);
			if ((temp & 0x800000) != 0)
			{
				temp |= 0xFF000000L;
			}
			int ins11 = (int)temp;

			Config found = null;
			foreach (var cfg in cfgs)
			{
				if (cfg.cmd == cmd)
				{
					found = cfg;
					break;
				}
			}
			string ret = "";
			if (found == null)
			{
				return "unknow cmd";
			}
			else
			{
				ret = found.scmd + " ";
				switch (found.textformat)
				{
					case 0://nop
						break;
					case 1://mov reg, ins
						extend = true;
						ret += getReg(reg1) + ", next line";
						break;
					case 2://mov eax, DWORD PTR [esp+28]
						ret += getReg(reg1) + ",DWORD PTR [" + getReg(reg2) + "+" + ins10 + "]";
						break;
					case 3://mov DWORD PTR [esp+28], eax
						ret += "DWORD PTR [" + getReg(reg2) + "+" + ins10 + "]," + getReg(reg1);
						break;
					case 8://mov BYTE PTR [eax], dl
						ret += "BYTE PTR [" + getReg(reg2) + "+" + ins10 + "]," + getReg8(reg1);
						break;
					case 9://lea eax, [eax + 4]
						ret += getReg(reg1) + ", [" + getReg(reg2) + "+" + ins10 + "]";
						break;
					case 10://cmp al, bl
						ret += getReg8(reg1) + "," + getReg8(reg2);
						break;
					case 11://movzx eax, BYTE PTR [esp+28]
						ret += getReg(reg1) + ", BYTE PTR [" + getReg(reg2) + "+" + ins10 + "]";
						break;
					case 4://push	ebp
						ret += getReg(reg1);
						break;
					case 5://mov	ebp, esp
						ret += getReg(reg1) + "," + getReg(reg2);
						break;
					case 6:
						ret += getReg(reg1) + "," + getReg8(reg2);
						break;
					case 7://call ___main
						int target = pc + ins11;
						string foundsym = getSym(target, syms);
						ret += ins11 + "(" + foundsym + ")";
						break;
					case 14://hlt ins
						ret += ins10;
						break;
				}

			}
			return ret;
		}

		public static string getSym(int pos, List<CodeSym> syms)
		{
			string foundsym = "";
			foreach (var sym in syms)
			{
				if (sym.pos == pos)
				{
					foundsym += sym.name + ",";
				}
			}
			return foundsym;
		}

		public static string getReg(int? val)
		{
			switch (val)
			{
				case 0: return "eax";
				case 1: return "ebx";
				case 2: return "ecx";
				case 3: return "edx";
				case 4: return "ebp";
				case 5: return "esp";
				case 6: return "esi";
				case 7: return "edi";
				case 8: return "ra";
				case 9: return "rb";
			}
			return "reg(" + val + ")";
		}
		public static string getReg8(int? val)
		{
			switch (val)
			{
				case 0: return "al";
				case 1: return "bl";
				case 2: return "cl";
				case 3: return "dl";
				case 8: return "ralowbyte";
				case 9: return "rblowbyte";
			}
			return "reg8(" + val + ")";
		}

		public string ToString()
		{
			return text;
		}
	}

}
