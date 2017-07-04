using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace BigMathLib
{
	public class BigMath
	{
		//b=n*ln(4294967296)/ln(1000000000)
		public int Precision;
		public int Exponent; // 10000^Exponent
		public UInt64[] Num_Array;
		public int Sign; // 1/-1
		public BigMath(int precision, UInt64 val)
		{
			Num_Array = new UInt64[precision];
			for (int i = 0; i < precision; i++)
			{
				Num_Array[i] = 0;
			}
			Exponent = 1;
			Precision = precision;
			Num_Array[0] = val;
			Sign = 1;
		}

		//public BigMath(int precision, Random r)
		//{
		//    Num_Array = new int[precision];
		//    for (int i = 0; i < precision; i++)
		//    {
		//        Num_Array[i] = (int)r.Next();
		//    }
		//    Exponent = 0;
		//    Precision = precision;
		//    Sign = 1;
		//}

		public BigMath(int precision, String valstr)
		{
			// init
			{
				Num_Array = new UInt64[precision];
				for (int i = 0; i < precision; i++)
				{
					Num_Array[i] = 0;
				}
				Exponent = 0;
				Precision = precision;
			}

			string iStr;
			string dStr;
			if (valstr.Length == 0)
			{
				throw new Exception("format error");
			}

			{
				String val;
				if (valstr.StartsWith("-"))
				{
					Sign = -1;
					val = valstr.Substring(1);
				}
				else
				{
					Sign = 1;
					val = valstr;
				}

				// sep
				if (val.IndexOf(".") == -1)
				{
					iStr = val;
					dStr = "";
				}
				else
				{
					string[] s = val.Split("[.]".ToCharArray());
					iStr = s[0];
					dStr = s[1];
				}
			}
			iStr = iStr.TrimStart('0');
			int[] I1GArray = new int[iStr.Length % 9 == 0 ? iStr.Length / 9 : iStr.Length / 9 + 1];
			{
				int i = 0;
				while (iStr.Length > 0)
				{
					//999,999,999
					int n;
					if (iStr.Length >= 9)
					{
						n = int.Parse(iStr.Substring(iStr.Length - 9));
						iStr = iStr.Substring(0, iStr.Length - 9);
					}
					else
					{
						n = int.Parse(iStr);
						iStr = "";
					}
					I1GArray[i++] = n;
				}
			}

			if (I1GArray.Length > 0)
			{
				int start = I1GArray.Length - 1;
				UInt64 low = 0;
				int sw = 0;
				while (true)
				{
					int exitF = 0;
					uint rem = 0;
					int tempstart = -1;
					for (int i = start; i >= 0; i--)
					{
						long num1 = rem;
						num1 *= 1000000000;
						num1 += I1GArray[i];
						int val = (int)(num1 >> 32);
						I1GArray[i] = (int)val;
						exitF |= val;
						if (val != 0 && tempstart == -1)
						{
							tempstart = i;
						}
						rem = (uint)num1;
					}
					start = tempstart;
					if (sw == 0)
					{
						low = rem;
						if (exitF == 0)
						{
							for (int i = (Exponent < precision ? Exponent : precision) - 1; i >= 0; i--)
							{
								if (i + 1 < precision)
								{
									Num_Array[i + 1] = Num_Array[i];
								}
							}
							Exponent++;
							Num_Array[0] = low;
							break;
						}
						sw = 1;
					}
					else
					{
						UInt64 high = rem;
						for (int i = (Exponent < precision ? Exponent : precision) - 1; i >= 0; i--)
						{
							if (i + 1 < precision)
							{
								Num_Array[i + 1] = Num_Array[i];
							}
						}
						Exponent++;
						Num_Array[0] = (high << 32) | low;
						if (exitF == 0) break;
						sw = 0;
					}
				}
			}

			dStr = dStr.TrimEnd('0');
			int[] D1GArray = new int[dStr.Length % 9 == 0 ? dStr.Length / 9 : dStr.Length / 9 + 1];
			{
				int i = 0;
				while (dStr.Length > 0)
				{
					//999,999,999
					int n;
					if (dStr.Length >= 9)
					{
						n = int.Parse(dStr.Substring(0, 9));
						dStr = dStr.Substring(9);
					}
					else
					{
						n = int.Parse(dStr.PadRight(9, '0'));
						dStr = "";
					}
					D1GArray[i++] = n;
				}
			}

			if (D1GArray.Length > 0)
			{
				int start = Exponent;
				int idx = Exponent;
				while (idx < precision)
				{
					int carry = 0;
					for (int i = D1GArray.Length - 1; i >= 0; i--)
					{
						long num1 = D1GArray[i];
						num1 = num1 << 16;
						num1 += carry;
						long rem;
						num1 = Math.DivRem(num1, 1000000000, out rem);
						D1GArray[i] = (int)rem;
						carry = (int)num1;
					}
					UInt64 carryTop = (UInt64)(carry & 0xFFFF);

					carry = 0;
					for (int i = D1GArray.Length - 1; i >= 0; i--)
					{
						long num1 = D1GArray[i];
						num1 = num1 << 16;
						num1 += carry;
						long rem;
						num1 = Math.DivRem(num1, 1000000000, out rem);
						D1GArray[i] = (int)rem;
						carry = (int)num1;
					}
					UInt64 carryHigh = (UInt64)(carry & 0xFFFF);

					carry = 0;
					for (int i = D1GArray.Length - 1; i >= 0; i--)
					{
						long num1 = D1GArray[i];
						num1 = num1 << 16;
						num1 += carry;
						long rem;
						num1 = Math.DivRem(num1, 1000000000, out rem);
						D1GArray[i] = (int)rem;
						carry = (int)num1;
					}
					UInt64 carryLow = (UInt64)(carry & 0xFFFF);

					carry = 0;
					for (int i = D1GArray.Length - 1; i >= 0; i--)
					{
						long num1 = D1GArray[i];
						num1 = num1 << 16;
						num1 += carry;
						long rem;
						num1 = Math.DivRem(num1, 1000000000, out rem);
						D1GArray[i] = (int)rem;
						carry = (int)num1;
					}
					UInt64 carryBottom = (UInt64)(carry & 0xFFFF);

					UInt64 result = carryTop << 48 | carryHigh << 32 | carryLow << 16 | carryBottom;

					if (result == 0 && start == 0)
					{
						Exponent--;
					}
					else
					{
						start = 1;
						Num_Array[idx++] = result;
					}
				}
			}
		}

		public string toString()
		{
			return toString(-1);
		}
		public String toString(int limit)
		{
			if (limit == 0)
			{
				double v = Math.Log(4294967296) / Math.Log(1000000000);
				limit = (int)(v * Precision);// * 2;
			}
			else if (limit == -1)
			{
				limit = int.MaxValue;
			}

			StringBuilder result1 = new StringBuilder();
			String ret;

			if (Exponent > 0)
			{
				if (Exponent > limit) throw new Exception("OverFlow");
				UInt64[] LArray = new UInt64[Exponent];
				uint[] IArray = new uint[Exponent * 2];

				for (int i = 0; i < (Exponent < Precision ? Exponent : Precision); i++)
				{
					LArray[Exponent - 1 - i] = Num_Array[i];
				}

				for (int i = 0; i < LArray.Length; i++)
				{
					IArray[(i << 1) + 1] = (uint)((LArray[i] >> 32) & 0xFFFFFFFF);
					IArray[(i << 1)] = (uint)(LArray[i] & 0xFFFFFFFF);
				}

				{
					int start = IArray.Length - 1;
					while (true)
					{
						uint exitF = 0;
						uint rem = 0;
						int tempstart = -1;
						for (int i = start; i >= 0; i--)
						{
							long num1 = rem;
							num1 = num1 << 32;
							num1 += IArray[i];
							long remc;
							num1 = Math.DivRem(num1, 1000000000, out remc);
							IArray[i] = (uint)num1;
							exitF |= (uint)num1;
							if (num1 != 0 && tempstart == -1)
							{
								tempstart = i;
							}
							rem = (uint)remc;
						}
						start = tempstart;
						if (exitF == 0)
						{
							result1.Insert(0, rem.ToString());
							break;
						}
						else
						{
							result1.Insert(0, rem.ToString("000000000"));
						}
					}
				}
			}
			else
			{
				result1.Append("0");
			}

			if (Precision - Exponent > 0)
			{
				result1.Append(".");
				if (Precision - Exponent > limit) throw new Exception("OverFlow");
				UInt64[] LDArray = new UInt64[Precision - Exponent];
				if (Exponent > 0)
				{
					for (int i = 0; i < Precision - Exponent; i++)
					{
						LDArray[i] = Num_Array[i + Exponent];
					}
				}
				else
				{
					for (int i = 0; i < Precision; i++)
					{
						LDArray[i - Exponent] = Num_Array[i];
					}
				}

				uint[] DArray = new uint[LDArray.Length * 2];

				for (int i = 0; i < LDArray.Length; i++)
				{
					DArray[(i << 1)] = (uint)((LDArray[i] >> 32) & 0xFFFFFFFF);
					DArray[(i << 1) + 1] = (uint)(LDArray[i] & 0xFFFFFFFF);
				}

				//b=n*ln(4294967296)/ln(1000000000)
				int maxlen = (int)(1.0703288734719331385377382923542 * DArray.Length) + 1;
				DArray[DArray.Length - 1]++;

				for (int index = 0; index < maxlen; index++)
				{
					int carry = 0;
					for (int i = DArray.Length - 1; i >= 0; i--)
					{
						long num1 = DArray[i];
						num1 = num1 * 1000000000;
						num1 += carry;
						DArray[i] = (uint)num1;
						carry = (int)(num1 >> 32);
					}

					result1.Append(carry.ToString("000000000"));
				}
				ret = result1.ToString();
				ret = ret.Substring(0, ret.Length - 9).TrimEnd('0');
				if (ret.EndsWith("9") && ret.IndexOf('.') >= 0)
				{
					ret = ret.TrimEnd('9');
					ret = ret.Substring(0, ret.Length - 1) + (char)(ret[ret.Length - 1] + 1);
				}
			}
			else
			{
				ret = result1.ToString();
			}
			return Sign == 1 ? ret : "-" + ret;
		}
		[DllImport("BigMathDll.dll")]
		private static extern void N_Add(Int64[] PSE, UInt64[] Num_Array, Int64[] num_PSE, UInt64[] num_Num_Array);
		public void Add(BigMath num)
		{

			Int64[] PSE = new Int64[3];
			Int64[] num_PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;
			num_PSE[0] = num.Precision;
			num_PSE[1] = num.Sign;
			num_PSE[2] = num.Exponent;

			if (Precision == num.Precision && this != num)
			{
				N_Add(PSE, Num_Array, num_PSE, num.Num_Array);
			}
			else
			{
				throw new Exception();
			}
			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}

		[DllImport("BigMathDll.dll")]
		private static extern Int64 N_Comp(Int64[] PSE, UInt64[] Num_Array, Int64[] num_PSE, UInt64[] num_Num_Array);
		public int Cmp(BigMath num)
		{

			Int64[] PSE = new Int64[3];
			Int64[] num_PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;
			num_PSE[0] = num.Precision;
			num_PSE[1] = num.Sign;
			num_PSE[2] = num.Exponent;

			if (Precision == num.Precision && this != num)
			{
				return (int)N_Comp(PSE, Num_Array, num_PSE, num.Num_Array);
			}
			else
			{
				throw new Exception();
			}
		}



		[DllImport("BigMathDll.dll")]
		private static extern void N_Mul(Int64[] PSE, UInt64[] Num_Array, Int64[] num_PSE, UInt64[] num_Num_Array);
		public void Mul(BigMath num)
		{
			Int64[] PSE = new Int64[3];
			Int64[] num_PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;
			num_PSE[0] = num.Precision;
			num_PSE[1] = num.Sign;
			num_PSE[2] = num.Exponent;

			if (Precision == num.Precision && this != num)
			{
				N_Mul(PSE, Num_Array, num_PSE, num.Num_Array);
			}
			else
			{
				throw new Exception();
			}
			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}

		[DllImport("BigMathDll.dll")]
		private static extern void N_Muln(Int64[] PSE, UInt64[] Num_Array, UInt64 n);
		public void Muln(UInt64 n)
		{
			Int64[] PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_Muln(PSE, Num_Array, n);

			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}


		[DllImport("BigMathDll.dll")]
		private static extern void N_Div(Int64[] PSE, UInt64[] Num_Array, Int64[] num_PSE, UInt64[] num_Num_Array);
		public bool IsZero()
		{
			return this.Num_Array[0] == 0;
		}

		public void Div(BigMath num)
		{
			Int64[] PSE = new Int64[3];
			Int64[] num_PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;
			num_PSE[0] = num.Precision;
			num_PSE[1] = num.Sign;
			num_PSE[2] = num.Exponent;

			if (Precision == num.Precision && this != num && !num.IsZero())
			{
				N_Div(PSE, Num_Array, num_PSE, num.Num_Array);
			}
			else
			{
				throw new Exception();
			}
			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}


		[DllImport("BigMathDll.dll")]
		private static extern void N_Divn(Int64[] PSE, UInt64[] Num_Array, UInt64 n);
		public void Divn(UInt64 n)
		{
			Int64[] PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_Divn(PSE, Num_Array, n);

			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}


		[DllImport("BigMathDll.dll")]
		private static extern void N_Sin(Int64[] PSE, UInt64[] Num_Array, int threads);
		public void Sin()
		{
			Int64[] PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			if (Precision > 1000)
			{
				N_Sin(PSE, Num_Array, 4);
			}
			else if (Precision > 50)
			{
				N_Sin(PSE, Num_Array, 2);
			}
			else
			{
				N_Sin(PSE, Num_Array, 1);
			}

			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}

		[DllImport("BigMathDll.dll")]
		private static extern void N_Exp(Int64[] PSE, UInt64[] Num_Array);
		void Exp()
		{
			Int64[] PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_Exp(PSE, Num_Array);

			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}
		public void Exp(BigMath e)
		{
			//x = this
			BigMath adj = new BigMath(Precision, 1);
			BigMath one = new BigMath(Precision, 1);
			BigMath dive = null;
			BigMath minusone = new BigMath(Precision, 1);
			minusone.Sign = -1;
			e.Sign = -1;
			//while (x < -e)
			while (Cmp(e) < 0)
			{
				//x += one;
				Add(one);
				if (dive == null)
				{
					dive = new BigMath(Precision, 1);
					dive.Div(e);
				}
				//adj /= e;
				e.Sign = 1;
				adj.Mul(dive);
				e.Sign = -1;
			}
			e.Sign = 1;
			//while (x > e)
			while (Cmp(e) > 0)
			{
				//x += minusone;
				Add(minusone);
				//adj *= e;
				adj.Mul(e);
			}

			Exp();

			//return ret * adj;
			Mul(adj);
		}

		public static BigMath E(int precision)
		{
			BigMath ret = new BigMath(precision, 1);
			ret.Exp();
			return ret;
		}

		[DllImport("BigMathDll.dll")]
		private static extern void N_Ln(Int64[] PSE, UInt64[] Num_Array);
		void Ln()
		{
			Int64[] PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_Ln(PSE, Num_Array);

			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}
		public void Ln(BigMath e)
		{
			//x = this
			BigMath adj = new BigMath(Precision, 1);
			if (Sign < 0) return;
			BigMath one = new BigMath(Precision, 1);
			BigMath minusone = new BigMath(Precision, 1);
			minusone.Sign = -1;
			BigMath dive = null;

			BigMath op5 = new BigMath(Precision, "0.5");
			BigMath lp5 = new BigMath(Precision, "1.5");

			//while (x < 0.5)
			while (Cmp(op5) < 0)
			{
				//x *= e;
				Mul(e);
				//adj--;
				adj.Add(minusone);
			}
			//while (x > 1.5)
			while (Cmp(lp5) > 0)
			{
				//x /= e;
				if (dive == null)
				{
					dive = new BigMath(Precision, 1);
					dive.Div(e);
				}
				Mul(dive);
				adj.Add(one);
			}

			Add(minusone);
			Ln();
			Add(minusone);

			Add(adj);

		}



		public void Sin(int threads)
		{
			Int64[] PSE = new Int64[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			if (Precision > 1000)
			{
				N_Sin(PSE, Num_Array, threads >= 4 ? 4 : (threads >= 2 ? 2 : 1));
			}
			else if (Precision > 50)
			{
				N_Sin(PSE, Num_Array, threads >= 2 ? 2 : 1);
			}
			else
			{
				N_Sin(PSE, Num_Array, 1);
			}

			Sign = (int)PSE[1];
			Exponent = (int)PSE[2];
		}

		public void Sin_slow()
		{
			BigMath num = (BigMath)this.clone();
			BigMath num1 = new BigMath(num.Precision, 1);
			BigMath num_i = new BigMath(num.Precision, 1);
			BigMath num_a = (BigMath)num.clone();
			BigMath num_b = new BigMath(num.Precision, 1);
			BigMath num_x2 = (BigMath)num.clone();
			num_x2.Mul(num);
			BigMath num2 = new BigMath(num.Precision, 1);
			for (int i = 1; ; i++)
			{
				num_a.Mul(num_x2);
				num_i.Add(num1);
				num_b.Mul(num_i);
				num_i.Add(num1);
				num_b.Mul(num_i);
				num_a.CopyTo(num2);
				num2.Div(num_b);
				if (i % 2 == 1)
				{
					num2.Sign = -num2.Sign;
				}
				if (this.Exponent - num2.Exponent > (num.Precision - 2))
				{
					break;
				}
				Add(num2);
			}
		}

		public BigMath clone()
		{
			BigMath ret_num = new BigMath(Precision, 0);
			ret_num.Exponent = Exponent;
			ret_num.Sign = Sign;
			Array.Copy(Num_Array, ret_num.Num_Array, Precision);
			return ret_num;
		}
		public void CopyTo(BigMath target)
		{
			target.Exponent = Exponent;
			target.Sign = Sign;
			Array.Copy(Num_Array, target.Num_Array, target.Precision < Precision ? target.Precision : Precision);
		}
		//[DllImport("D:\\work\\bigMath\\release\\BigMathDll.dll")]
		//private static extern void N_E(int[] PSE, int[] Num_Array);
		//public void e()
		//{

		//    int[] PSE = new int[3];

		//    PSE[0] = Precision;
		//    PSE[1] = Sign;
		//    PSE[2] = Exponent;

		//    N_E(PSE, Num_Array);

		//    Sign = PSE[1];
		//    Exponent = PSE[2];
		//}

		//[DllImport("D:\\work\\bigMath\\release\\BigMathDll.dll")]
		//private static extern void N_Test(int[] PSE, int[] Num_Array);
		//public void Test()
		//{
		//    int[] PSE = new int[3];
		//    PSE[0] = Precision;
		//    PSE[1] = Sign;
		//    PSE[2] = Exponent;

		//    N_Test(PSE, Num_Array);

		//    Sign = PSE[1];
		//    Exponent = PSE[2];
		//}

	}
}