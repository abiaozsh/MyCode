using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace BigMathLib
{
	public class BigMath
	{

		public int Precision;
		public int Exponent; // 10000^Exponent
		public int[] Num_Array;
		public int Sign; // 1/-1
		public BigMath(int precision, int val)
		{
			Num_Array = new int[precision];
			for (int i = 0; i < precision; i++)
			{
				Num_Array[i] = 0;
			}
			Exponent = 1;
			Precision = precision;
			Num_Array[0] = val;
			Sign = 1;
		}

		public BigMath(int precision, Random r)
		{
			Num_Array = new int[precision];
			for (int i = 0; i < precision; i++)
			{
				Num_Array[i] = (int)r.Next();
			}
			Exponent = 0;
			Precision = precision;
			Sign = 1;
		}

		public BigMath(int precision, String valstr)
		{
			// init
			{
				Num_Array = new int[precision];
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
					for (int i = (Exponent < precision ? Exponent : precision) - 1; i >= 0; i--)
					{
						if (i + 1 < precision)
						{
							Num_Array[i + 1] = Num_Array[i];
						}
					}
					Exponent++;
					Num_Array[0] = (int)rem;
					if (exitF == 0) break;
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
					int carryHigh = carry;
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
					int carryLow = carry;
					uint result = (uint)(carryHigh << 16 | carryLow);

					if (result == 0 && start == 0)
					{
						Exponent--;
					}
					else
					{
						start = 1;
						Num_Array[idx++] = (int)result;
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
				uint[] IArray = new uint[Exponent];
				for (int i = 0; i < (Exponent < Precision ? Exponent : Precision); i++)
				{
					IArray[IArray.Length - 1 - i] = (uint)Num_Array[i];
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
				int _decimal = Precision - Exponent;
				if (limit != int.MaxValue)
				{
					if (_decimal > limit * 100 / 1900 + 1)
					{
						_decimal = limit * 100 / 1900 + 1;
					};
				}
				uint[] DArray = new uint[_decimal];
				if (Exponent > 0)
				{
					for (int i = 0; i < _decimal; i++)
					{
						DArray[i] = (uint)Num_Array[i + Exponent];
					}
				}
				else
				{
					for (int i = 0; i < _decimal + Exponent; i++)
					{
						DArray[i + _decimal - Precision] = (uint)Num_Array[i];
					}
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
			}
			else
			{
				ret = result1.ToString();
			}
			return Sign == 1 ? ret : "-" + ret;
		}

		[DllImport("BigMathDll.dll")]
		private static extern int N_Comp(int[] PSE, int[] Num_Array, int[] num_PSE, int[] num_Num_Array);
		public int Cmp(BigMath num)
		{

			int[] PSE = new int[3];
			int[] num_PSE = new int[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;
			num_PSE[0] = num.Precision;
			num_PSE[1] = num.Sign;
			num_PSE[2] = num.Exponent;

			if (Precision == num.Precision && this != num)
			{
				return N_Comp(PSE, Num_Array, num_PSE, num.Num_Array);
			}
			else
			{
				throw new Exception();
			}
		}


		[DllImport("BigMathDll.dll")]
		private static extern void N_Add(int[] PSE, int[] Num_Array, int[] num_PSE, int[] num_Num_Array);
		public void Add(BigMath num)
		{

			int[] PSE = new int[3];
			int[] num_PSE = new int[3];

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
			Sign = PSE[1];
			Exponent = PSE[2];
		}
		[DllImport("BigMathDll.dll")]
		private static extern void N_Mul(int[] PSE, int[] Num_Array, int[] num_PSE, int[] num_Num_Array);
		public void Mul(BigMath num)
		{
			int[] PSE = new int[3];
			int[] num_PSE = new int[3];

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
			Sign = PSE[1];
			Exponent = PSE[2];
		}

		[DllImport("BigMathDll.dll")]
		private static extern void N_Muln(int[] PSE, int[] Num_Array, uint n);
		public void Muln(uint n)
		{
			int[] PSE = new int[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_Muln(PSE, Num_Array, n);

			Sign = PSE[1];
			Exponent = PSE[2];
		}


		[DllImport("BigMathDll.dll")]
		private static extern void N_Div(int[] PSE, int[] Num_Array, int[] num_PSE, int[] num_Num_Array);
		public bool IsZero()
		{
			return this.Num_Array[0] == 0;
		}
		public void Div(BigMath num)
		{
			int[] PSE = new int[3];
			int[] num_PSE = new int[3];

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
			Sign = PSE[1];
			Exponent = PSE[2];
		}
		[DllImport("BigMathDll.dll")]
		private static extern void N_Sin(int[] PSE, int[] Num_Array);
		public void Sin()
		{
			int[] PSE = new int[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_Sin(PSE, Num_Array);

			Sign = PSE[1];
			Exponent = PSE[2];
		}

		public void Sin_Slow()
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
		[DllImport("BigMathDll.dll")]
		private static extern void N_Exp(int[] PSE, int[] Num_Array);
		void Exp()
		{
			int[] PSE = new int[3];

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
		private static extern void N_Ln(int[] PSE, int[] Num_Array);
		void Ln()
		{
			int[] PSE = new int[3];

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
		[DllImport("BigMathDll.dll")]
		private static extern void N_E(int[] PSE, int[] Num_Array);
		public void e()
		{

			int[] PSE = new int[3];

			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_E(PSE, Num_Array);

			Sign = PSE[1];
			Exponent = PSE[2];
		}

		[DllImport("BigMathDll.dll")]
		private static extern void N_Test(int[] PSE, int[] Num_Array);
		public void Test()
		{
			int[] PSE = new int[3];
			PSE[0] = Precision;
			PSE[1] = Sign;
			PSE[2] = Exponent;

			N_Test(PSE, Num_Array);

			Sign = PSE[1];
			Exponent = PSE[2];
		}

	}
}