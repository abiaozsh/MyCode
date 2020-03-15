using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.IO;
using System.Globalization;
using System.Threading;
using System.Text.RegularExpressions;

namespace Assembler
{
    class Program : Base
    {

        public static void getElem(string line, out string sym, out int? ins1, out int? ins2, out int? reg1, out int? reg2)
        {
            sym = null;
            ins1 = null;
            ins2 = null;
            reg1 = null;
            reg2 = null;
            Match match;
            {
                match = Regex.Match(line, regCmd);
                string temp = line.Substring(match.Length);
                match = Regex.Match(temp, regSym);
                if (match.Success)
                {
                    sym = temp.Substring(match.Index, match.Length);
                }
            }
            match = Regex.Match(line, regIns);
            if (match.Success)
            {
                ins1 = int.Parse(line.Substring(match.Index, match.Length));
            }
            match = match.NextMatch();
            if (match.Success)
            {
                ins2 = int.Parse(line.Substring(match.Index, match.Length));
            }

            match = Regex.Match(line, regReg);
            if (match.Success)
            {
                reg1 = getReg(line.Substring(match.Index, match.Length));
            }
            match = match.NextMatch();
            if (match.Success)
            {
                reg2 = getReg(line.Substring(match.Index, match.Length));
            }
        }

        public static string processRegex(string reg)
        {
            reg = reg.Trim();

            reg = reg.Replace("!sym", regSym);
            reg = reg.Replace("!ins", regIns);
            reg = reg.Replace("!reg", regReg);
            reg = "^" + reg + "$";
            return reg;
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
                    Config cfg = new Config();
                    var arr = item.Split('@');
                    cfg.regex = processRegex(arr[0]);
                    cfg.format = int.Parse(arr[1]);
                    cfg.insformat = int.Parse(arr[2]);
                    cfg.cmd = int.Parse(arr[3]);

                    cfgs.Add(cfg);
                }

                fs.Close();
            }
            return cfgs;
        }

        //public static void procIns2(int? ins2, List<string> lines)
        //{
        //	string sins2 = null;
        //	if (ins2 != null)
        //	{
        //		sins2 = (ins2 >= 0 ? "+" : "") + ins2;
        //	}
        //
        //	if (ins2 > 30000 || ins2 < -30000)
        //	{
        //		lines.Add(@"movhighpart ra, " + ((ins2 >> 16) & 0xFFFF));
        //		lines.Add(@"movlowpart ra, " + ((ins2) & 0xFFFF));
        //	}
        //	else
        //	{
        //		lines.Add(@"movextendins ra, " + ((ins2) & 0xFFFF));
        //	}
        //}

        static void Main(string[] args)
        {

            string filein;
            string fileout;
            string filetemp;
            if (args.Length > 0 && !String.IsNullOrEmpty(args[0]))
            {
                filein = args[0];
            }
            else
            {
                filein = "a.s";
            }
            if (args.Length > 1 && !String.IsNullOrEmpty(args[1]))
            {
                fileout = args[1];
            }
            else
            {
                fileout = "a.hex";
            }
            if (args.Length > 2 && !String.IsNullOrEmpty(args[2]))
            {
                filetemp = args[2];
            }
            else
            {
                filetemp = "a.temp.s";
            }
            Console.WriteLine(filein);
            Console.WriteLine(fileout);
            Console.WriteLine(filetemp);

            try
            {
                compile(filein, fileout, filetemp);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }

            Console.ReadLine();
        }
        public static void compile(string filein, string fileout, string filetemp)
        {
            List<Config> cfgs = loadConfig(@"assembler\config.txt");
            string[] linesraw = null;
            {
                FileStream fs = new FileStream(filein, FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                var temp = sr.ReadToEnd().Replace('\t', ' ');
                while (true)
                {
                    if (temp.Contains("  "))
                    {
                        temp = temp.Replace("  ", " ");
                    }
                    else
                    {
                        break;
                    }
                }
                linesraw = temp.Split('\n');
                fs.Close();
            }

            List<string> lines = new List<string>();

            bool standalone = true;
            if (standalone)
            {
                lines.Add("jmp _main");
            }
            //pre process
            foreach (var item in linesraw)
            {
                var line = item.Trim();
                string sym;
                int? ins1;
                int? ins2;
                int? reg1;
                int? reg2;
                getElem(line, out sym, out ins1, out ins2, out reg1, out reg2);
                string sins1 = null;
                if (ins1 != null)
                {
                    sins1 = (ins1 >= 0 ? "+" : "") + ins1;
                }
                string sins2 = null;
                if (ins2 != null)
                {
                    sins2 = (ins2 >= 0 ? "+" : "") + ins2;
                }

                if (Regex.IsMatch(line, processRegex("je !sym")))
                {
                    string last = lines[lines.Count - 1];
                    if (!last.StartsWith("cmp"))
                    {
                        throw new Exception();
                    }
                }


                if (Regex.IsMatch(line, processRegex("leave")))
                {
                    //leave :   Set ESP to EBP, then pop EBP
                    lines.Add("mov ebp, esp");
                    lines.Add("pop ebp");
                }
                else if (Regex.IsMatch(line, processRegex(@"add DWORD PTR \[!reg!ins\], !ins")))
                {
                    lines.Add(@"mov rb, DWORD PTR [" + getReg(reg1) + sins1 + @"]");
                    lines.Add(@"mov ra, " + sins2);
                    lines.Add(@"add rb, ra");
                    lines.Add(@"mov DWORD PTR [" + getReg(reg1) + sins1 + @"], rb");
                }
                else if (Regex.IsMatch(line, processRegex(@"cmp !reg, DWORD PTR \[!reg!ins\]")))
                {
                    lines.Add(@"mov ra, DWORD PTR [" + getReg(reg2) + sins1 + @"]");
                    lines.Add(@"cmp " + getReg(reg1) + ", ra");
                }
                else if (Regex.IsMatch(line, processRegex(@"mov DWORD PTR \[!reg!ins\], !ins")))
                {
                    lines.Add(@"mov ra, " + sins2);
                    lines.Add(@"mov DWORD PTR [" + getReg(reg1) + sins1 + @"], ra");
                }
                else if (Regex.IsMatch(line, processRegex(@"add !reg, !ins")))
                {
                    lines.Add(@"mov ra, " + sins1);
                    lines.Add(@"add " + getReg(reg1) + ", ra");
                }
                else if (Regex.IsMatch(line, processRegex(@"and !reg, !ins")))
                {
                    lines.Add(@"mov ra, " + sins1);
                    lines.Add(@"and " + getReg(reg1) + ", ra");
                }
                else if (Regex.IsMatch(line, processRegex(@"sub !reg, !ins")))
                {
                    lines.Add(@"mov ra, " + sins1);
                    lines.Add(@"sub " + getReg(reg1) + ", ra");
                }

                else
                {
                    lines.Add(line);
                }
            }


            {
                FileStream fs2 = new FileStream(filetemp, FileMode.Create, FileAccess.Write);
                StreamWriter sw = new StreamWriter(fs2);
                foreach (var item in lines)
                {
                    sw.WriteLine(item);
                }
                sw.Flush();
                fs2.Close();
            }

            List<DataSym> datasyms = new List<DataSym>();
            int pos = 0;
            {
                foreach (var item in lines)
                {
                    var line = item.Trim();
                    if (line.StartsWith(".comm"))
                    {
                        //	.comm	_d, 16	 # 4
                        string[] temp = line.Split(' ');

                        int len = int.Parse(temp[4]);
                        if (len < 4)
                        {
                            len = 4;
                        }
                        string name = temp[1].TrimEnd(',');
                        DataSym datasym = new DataSym();
                        datasym.name = name;
                        datasym.pos = pos;
                        pos += len;
                        datasyms.Add(datasym);
                    }
                }
            }

            List<Ins> insList = new List<Ins>();
            pos = 0;
            List<CodeSym> codeSyms = new List<CodeSym>();
            {
                foreach (var item in lines)
                {
                    var line = item;
                    if (line.Length == 0 || line.StartsWith("."))
                    {
                    }
                    else if (Regex.IsMatch(line, @".*:$"))
                    {
                        CodeSym codeSym = new CodeSym();
                        codeSym.name = line.TrimEnd(':');
                        codeSym.pos = pos;
                        codeSyms.Add(codeSym);
                    }
                    else
                    {
                        Ins ins = new Ins();
                        Config matchCfg = null;
                        foreach (var cfg in cfgs)
                        {
                            if (Regex.IsMatch(line, cfg.regex))
                            {
                                matchCfg = cfg;
                                break;
                            }
                        }
                        if (matchCfg == null)
                        {
                            throw new Exception("unknow cmd:" + line);
                        }
                        else
                        {
                            string sym;
                            int? ins1;
                            int? ins2;
                            int? reg1;
                            int? reg2;
                            getElem(line, out sym, out ins1, out ins2, out reg1, out reg2);

                            switch (matchCfg.format)
                            {
                                case 0://nop
                                    break;
                                case 1:
                                    ins.bitreg1 = check(reg1);
                                    ins.bitins2 = ins1.Value;
                                    break;
                                case 2://mov DWORD PTR [esp+28], eax
                                    ins.bitreg1 = check(reg1);
                                    ins.bitreg2 = check(reg2);
                                    if (ins1 != null)
                                    {
                                        ins.bitins1 = ins1.Value;
                                    }
                                    else
                                    {
                                        ins.bitins1 = 0;
                                    }
                                    break;
                                case 4://push	ebp
                                    ins.bitreg1 = check(reg1);
                                    break;
                                case 5://mov	ebp, esp
                                    ins.bitreg1 = check(reg1);
                                    ins.bitreg2 = check(reg2);
                                    break;
                                case 6:
                                    break;
                                case 7://call ___main
                                    ins.sym = check(sym);
                                    break;
                            }
                            ins.line = line;
                            ins.format = matchCfg.insformat;
                            ins.bitcmd = matchCfg.cmd;
                            ins.pos = pos;
                            insList.Add(ins);
                            if (matchCfg.insformat == 2)
                            {
                                pos += 8;
                            }
                            else
                            {
                                pos += 4;
                            }
                        }
                    }

                    //
                    //ins.len = 8;
                    //ins.bit31 = 1;

                }
            }

            foreach (var ins in insList)
            {
                if (ins.sym != null)
                {
                    CodeSym found = null;
                    foreach (var sym in codeSyms)
                    {
                        if (ins.sym == sym.name)
                        {
                            found = sym; break;
                        }
                    }
                    if (found == null)
                    {
                        throw new Exception("not found:" + ins.sym);
                    }
                    ins.bitins1 = found.pos - ins.pos;
                }
            }

            {
                int entry = 0;
                int len = pos;
                //FileStream fs = new FileStream(@"..\..\..\a.bin", FileMode.Create, FileAccess.Write);
                FileStream fs2 = new FileStream(fileout, FileMode.Create, FileAccess.Write);

                //BinaryWriter bw = new BinaryWriter(fs);
                StreamWriter sw = new StreamWriter(fs2);
                //bw.Write(len);
                //bw.Write(entry);

                int posx = 0;
                foreach (var ins in insList)
                {
                    //[31]1bit:4 byte/8 byte
                    //[30:24]7bit:cmd
                    //[23:20]4bit:reg1
                    //[19:16]4bit:reg2
                    //[15:0]16bit:ins1
                    //[31:0]32bit:ins2

                    if (ins.bitins1 > 30000 || ins.bitins1 < -30000)
                    {
                        throw new Exception();
                    }

                    if (ins.format == 0)
                    {
                        int insbin = (ins.bitcmd << 24) | (ins.bitreg1 << 20) | (ins.bitreg2 << 16) | (ins.bitins1 & 0x0000FFFF);
                        //bw.Write(insbin);
                        writehex(insbin, posx++, sw);
                    }
                    else if (ins.format == 1)
                    {
                        int insbin = (ins.bitcmd << 24) | (ins.bitreg1 << 20) | (ins.bitreg2 << 16) | (ins.bitins1 & 0x00FFFFFF);
                        //bw.Write(insbin);
                        writehex(insbin, posx++, sw);
                    }
                    else if (ins.format == 2)
                    {
                        int insbin = (ins.bitcmd << 24) | (ins.bitreg1 << 20) | (ins.bitreg2 << 16) | (ins.bitins1 & 0x0000FFFF);

                        //bw.Write(insbin);
                        writehex(insbin, posx++, sw);

                        if ((insbin & 0x80000000) != 0)
                        {
                            //bw.Write(ins.bitins2);
                            writehex(ins.bitins2, posx++, sw);
                        }
                    }
                }

                sw.WriteLine(":00000001FF");

                sw.Flush();
                fs2.Close();

                //bw.Flush();
                //fs.Close();

            }
            //Console.ReadLine();
        }


        public static void writehex(int data, int posx, StreamWriter sw)
        {
            //:04 0000 00 0302 0100 51
            int sum = 4;
            sw.Write(":04" + getHex4(posx, ref sum) + "00" + getHex8(data, ref sum));
            sw.Write(getHex2(0x100 - (sum & 0xFF)));
            sw.WriteLine();
        }
    }
}
