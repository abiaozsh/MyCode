using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using myd3d;
using Microsoft.DirectX;

namespace Electric
{
	public class test
	{
		public static void makedata()
		{
			double force;
			System.IO.FileStream fs1 = new System.IO.FileStream("d:\\_eeb.txt", System.IO.FileMode.Create);
			System.IO.StreamWriter sw1 = new System.IO.StreamWriter(fs1);

			System.IO.FileStream fs2 = new System.IO.FileStream("d:\\_ee.txt", System.IO.FileMode.Create);
			System.IO.StreamWriter sw2 = new System.IO.StreamWriter(fs2);

			System.IO.FileStream fs3 = new System.IO.FileStream("d:\\_pp.txt", System.IO.FileMode.Create);
			System.IO.StreamWriter sw3 = new System.IO.StreamWriter(fs3);

			System.IO.FileStream fs4 = new System.IO.FileStream("d:\\_peb.txt", System.IO.FileMode.Create);
			System.IO.StreamWriter sw4 = new System.IO.StreamWriter(fs4);

			System.IO.FileStream fs5 = new System.IO.FileStream("d:\\_pe.txt", System.IO.FileMode.Create);
			System.IO.StreamWriter sw5 = new System.IO.StreamWriter(fs5);


			for (int i = 0; i < 1000; i++)
			{
				//e-eb
				double r = ((double)i) / 100;
				double r2 = r * r;
				{
					double d = 1f;//midpoint
					double b = 0.1f;//force
					double a = d * b / 2;
					if (r < d)
					{
						double n = -a / r + b;
						force = -n / r;
					}
					else
					{
						force = -a / r2;
					}

					sw1.WriteLine(i + "," + force);
				}

				//e-e
				{
					double d = 1;//midpoint
					double b = 30;//force
					double a = d * b / 2;
					force = -a / r2;
					sw2.WriteLine(i + "," + force);
				}


				//p-p
				{
					double d = 1f;//midpoint
					double b = 30;//force
					double a = d * b / 2;
					force = -a / r2;
					sw3.WriteLine(i + "," + force);
				}


				//p-eb
				{
					double d = 1f;//midpoint
					double b = 30;//force
					double a = d * b / 2;
					if (r < d)
					{
						double n = -a / r + b;
						force = -n / r;
					}
					else
					{
						force = -a / r2;
					}
					sw4.WriteLine(i + "," + force);
				}

				//p-e
				{
					double d = 2f;//midpoint
					double b = 30;//force
					double a = d * b / 2;
					if (r < d)
					{
						double n = -a / r + b;
						force = -n / r;
					}
					else
					{
						force = -a / r2;
					}
					sw5.WriteLine(i + "," + force);
				}

			}
			sw1.Flush();
			fs1.Close();

			sw2.Flush();
			fs2.Close();

			sw3.Flush();
			fs3.Close();

			sw4.Flush();
			fs4.Close();

			sw5.Flush();
			fs5.Close();
		}
	}








	public class Bound
	{
		public PSpot sp1;

		public PSpot sp2;

		public float distance;

		// +/-
		public float force;
	}

	[Serializable]
	public class PSpot
	{
		[NonSerialized]
		public Model model;
		public Text name;
		public string sname;

		public Vector3 pos;

		public Vector3 heading;

		public float charge;

		public bool bounded_e;

		public bool bounded_p;

		List<Bound> bounds;

		List<Bound> bounds3;

		public override string ToString()
		{
			return sname + " charge:" + charge.ToString();
		}

		public static int compare(Bound r1, Bound r2)
		{
			if (r1.distance > r2.distance)
			{
				return 1;
			}
			else if (r1.distance < r2.distance)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}

		public static void Process(List<PSpot> spotList, bool slowdown, float temperature, float E)
		{
			Random rnd = new Random();

			// 1:e-e
			List<Bound> bounds1 = new List<Bound>();
			// 2:p-p
			List<Bound> bounds2 = new List<Bound>();

			foreach (PSpot s in spotList)
			{
				s.bounded_e = false;
				s.bounded_p = false;
				s.bounds = new List<Bound>();
				if (s.charge > 0)
				{
					// 3:p-e
					s.bounds3 = new List<Bound>();
				}
			}

			for (int i = 0; i < spotList.Count; i++)
			{
				for (int j = i + 1; j < spotList.Count; j++)
				{
					Bound bound = new Bound();
					Vector3 diff = spotList[i].pos;
					diff.Subtract(spotList[j].pos);
					bound.distance = diff.Length();

					if (spotList[i].charge < 0 && spotList[j].charge < 0)
					{
						bound.sp1 = spotList[i];
						bound.sp2 = spotList[j];
						spotList[i].bounds.Add(bound);
						spotList[j].bounds.Add(bound);
						bounds1.Add(bound);
					}
					if (spotList[i].charge > 0 && spotList[j].charge > 0)
					{
						bound.sp1 = spotList[i];
						bound.sp2 = spotList[j];
						spotList[i].bounds.Add(bound);
						spotList[j].bounds.Add(bound);
						bounds2.Add(bound);
					}

					if (spotList[i].charge < 0 && spotList[j].charge > 0)
					{
						bound.sp1 = spotList[j];
						bound.sp2 = spotList[i];
						spotList[i].bounds.Add(bound);
						spotList[j].bounds.Add(bound);
						spotList[j].bounds3.Add(bound);
					}
					if (spotList[i].charge > 0 && spotList[j].charge < 0)
					{
						bound.sp1 = spotList[i];
						bound.sp2 = spotList[j];
						spotList[i].bounds.Add(bound);
						spotList[j].bounds.Add(bound);
						spotList[i].bounds3.Add(bound);
					}
				}
			}

			bounds1.Sort(compare);

			//e-e
			foreach (Bound bound in bounds1)
			{
				float r = bound.distance;
				float r2 = r * r;
				float e = bound.sp1.charge * bound.sp2.charge;
				if (!bound.sp1.bounded_e && !bound.sp2.bounded_e)
				{
					bound.sp1.bounded_e = true;
					bound.sp2.bounded_e = true;

					float d = 1;//midpoint
					float b = 1;//force
					float a = d * b / 2;
					bound.force = -E * e * a / r2;
				}
				else
				{
					float d = 1;//midpoint
					float b = 30;//force
					float a = d * b / 2;
					bound.force = -E * e * a / r2;
				}
			}

			//p-p
			foreach (Bound bound in bounds2)
			{
				float r = bound.distance;
				float r2 = r * r;
				float e = bound.sp1.charge * bound.sp2.charge;
				float d = 1f;//midpoint
				float b = 30;//force
				float a = d * b / 2;
				bound.force = -E * e * a / r2;
			}

			//p-e
			foreach (PSpot s in spotList)
			{
				if (s.charge > 0)
				{
					s.bounds3.Sort(compare);
					int eCount = 0;
					foreach (Bound bound in s.bounds3)
					{
						float r = bound.distance;
						float r2 = r * r;
						float e = bound.sp1.charge * bound.sp2.charge;

						//if (!bound.sp2.bounded_p)
						{
							eCount++;
							int total;
							if (bound.sp1.charge >= 2)
							{
								total = 8;
							}
							else
							{
								total = 2;
							}
							if (eCount <= total)
							{
								//bound.sp2.bounded_p = true;
								float d = 1f;//midpoint
								float b = 30;//force
								float a = d * b / 2;
								if (r < d)
								{
									float n = -a / r + b;
									bound.force = -E * e * n / r;
								}
								else
								{
									bound.force = -E * e * a / r2;
								}
							}
							else
							{
								float d = 2f;//midpoint
								float b = 30;//force
								float a = d * b / 2;
								if (r < d)
								{
									float n = -a / r + b;
									bound.force = -E * e * n / r;
								}
								else
								{
									bound.force = -E * e * a / r2;
								}
							}
						}
					}
				}
			}

			foreach (PSpot s in spotList)
			{
				s.Calc(spotList, rnd, slowdown, temperature, E);
			}
		}

		public void Calc(List<PSpot> allSpot, Random rnd, bool slowdown, float temperature, float E)
		{

			Vector3 force = new Vector3(0, 0, 0);

			foreach (Bound bound in bounds)
			{

				Vector3 diff;

				if (bound.sp1 == this)
				{
					diff = bound.sp2.pos;
				}
				else
				{
					diff = bound.sp1.pos;
				}
				diff.Subtract(pos);

				diff.Multiply(bound.force / diff.Length());

				force.Add(diff);
			}
			if (force.Length() > 1f)
			{
				force.Multiply(1f / force.Length());
			}

			heading.Add(force);

			heading.Multiply(0.01f / E > 0.1f ? E : 0.1f);

			pos.Add(heading);

			//if (slowdown)
			//{
			//}

			pos.X += (float)(rnd.NextDouble() - 0.5) * temperature;
			pos.Y += (float)(rnd.NextDouble() - 0.5) * temperature;
			pos.Z += (float)(rnd.NextDouble() - 0.5) * temperature;
		}
	}
}
