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


        static void Main(string[] args)
        {
            int n = Util.initGPU(0);

            FCFWD_Test.Test();

        }
    }
}
