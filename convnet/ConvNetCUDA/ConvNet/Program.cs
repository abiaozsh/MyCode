using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Lib;

namespace ConvNet
{
    class Program
    {

        [DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int init(int device);

        static void Main(string[] args)
        {
            int n = init(0);

            FCFWD_Test.Test();

        }
    }
}
