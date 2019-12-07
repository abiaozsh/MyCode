using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace UploaderMega8
{
    class Program
    {
        static SerialPort port;

        static void Main(string[] args)
        {
            //if (test)
            {
                //args = new string[] { "COM4", @"Y:\atmega8\blink.hex", "true" };
                //args = new string[] { "COM4", @"Y:\atmega8\blinkserial\blink.hex", "true" };
                //args = new string[] { "COM4", @"D:\MyCode\arduino\atmega8\blink.hex", "true" };
            }

            try
            {
                {
                    int bps = 115200;
                    int.TryParse(args[3],out bps);
                    port = new SerialPort(args[0], bps, Parity.None, 8, StopBits.One);
                    try
                    {
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
                        return;
                    }
                }

                FileStream fs = new FileStream(args[1], FileMode.Open, FileAccess.Read);
                StreamReader sr = new StreamReader(fs);
                StringBuilder sbdata = new StringBuilder();
                bool verify = false;
                if (args.Length >= 3)
                {
                    bool.TryParse(args[2], out verify);
                }
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

                while (true)
                {
                    if (sdata.Length % (32 * 4) != 0)
                    {
                        sdata += "FFFF";
                    }
                    else
                    {
                        break;
                    }
                }

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
                    if (sig != "1E9307")
                    {
                        Console.WriteLine("Signature error!!!");
                        Console.Read();
                        return;
                    }
                }

                //write
                {
                    portWrite("st");//st Start
                    checkOK();
                    portWrite("er");//er Erase
                    checkOK();
                    portWrite("wf");//wf WriteFlash
                    checkOK();

                    int i;
                    //char/4 = word
                    for (i = 0; i < sdata.Length / 4; i += 32)
                    {
                        portWrite("pb" + getHex2(i));//Load Flash Page Buffer//先低后高
                        for (int j = i; j < i + 32; j++)
                        {
                            Console.Write(sdata.Substring(j * 4, 4));
                            portWrite(sdata.Substring(j * 4, 4));//Load Flash Page Buffer//先低后高
                        }
                        checkOK();
                        portWrite("ha" + getHex2(i >> 8));
                        checkOK();
                        Console.WriteLine();

                    }

                    portWrite("no");//nop
                    checkOK();
                    portWrite("ed");//ed End
                    checkOK();
                }
                Console.WriteLine();

                if (verify)
                {
                    //read
                    StringBuilder sbresult = new StringBuilder();
                    {
                        portWrite("st");//st Start
                        checkOK();
                        portWrite("rf");//rf ReadFlash
                        checkOK();

                        //char/4 = word
                        int i;
                        for (i = 0; i < sdata.Length / 4; i++)
                        {
                            portWrite("fb" + getHex4(i));
                            string sret = readFromPort(4);
                            sbresult.Append(sret);//先低后高
                            Console.Write(sret);
                            if ((i + 1) % 32 == 0)//31 63...
                            {
                                Console.WriteLine();
                            }
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
                else
                {
                    Console.WriteLine("end");
                }






            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }



            Console.Read();
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
