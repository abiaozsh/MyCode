using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using myd3d;
using Microsoft.DirectX;

namespace Electric
{

	[Serializable]
	public class PSpot
	{
		[NonSerialized]
		public Model model;
		public Text name;
		public string sname;

		public Vector3 pos;

		public bool _static;

		public Vector3 heading;

		public float charge;

		public float process;

		public float tempDistance;

		public override string ToString()
		{
			return sname + " charge:" + charge.ToString();
		}

		public static void ProcessGravity(List<PSpot> spotList, bool slowdown, float temperature)
		{
			Random rnd = new Random();
			while (true)
			{
				double process = 1;
				PSpot sp = null;
				foreach (PSpot s in spotList)
				{
					if (s.process < process)
					{
						process = s.process;
						sp = s;
					}
				}
				if (sp != null)
				{
					sp.CalcGravity(spotList, rnd, slowdown, temperature);
				}
				else
				{
					break;
				}
			}
		}

		static float E = 0.001f;


		public void CalcGravity(List<PSpot> allSpot, Random rnd, bool slowdown, float temperature)
		{
			if (this._static)
			{
				process = 1;
				return;
			}

			Vector3 force = new Vector3(0, 0, 0);
			double reduce = 0;

			//Sort by distance
			foreach (PSpot s in allSpot)
			{
				Vector3 diff = s.pos;
				diff.Subtract(pos);
				s.tempDistance = diff.Length();
			}

			allSpot.Sort(delegate(PSpot r1, PSpot r2)
			{
				if (r1.tempDistance > r2.tempDistance)
				{
					return 1;
				}
				else if (r1.tempDistance < r2.tempDistance)
				{
					return -1;
				}
				else
				{
					return 0;
				}
			});

			float d = 2;//midpoint
			float b = 30;//force
			float a = d * b / 2;
			foreach (PSpot s in allSpot)
			{
				if (s != this)
				{
					Vector3 diff = s.pos;
					diff.Subtract(pos);

					float r = diff.Length();
					float r2 = r * r;
					float e = this.charge * s.charge;

					if (r == 0)
					{
						r = 0.01f;
					}
					reduce += 1 / r;

					float f;
					if (charge > 0)
					{
						if (s.charge > 0)
						{
							f = -E * e * a / r2;
						}
						else
						{
							if (r < d)
							{
								float n = -a / r + b;
								float f1 = -E * e * n;
								f = f1 / r;
							}
							else
							{
								f = -E * e * a / r2;
							}
						}
					}
					else
					{
						if (s.charge < 0)
						{
							f = -E * e * a / r2;
						}
						else
						{
							if (r < d)
							{
								float n = -a / r + b;
								float f1 = -E * e * n;
								f = f1 / r;
							}
							else
							{
								f = -E * e * a / r2;
							}
						}
					}



					diff.Multiply(f);
					force.Add(diff);
				}
			}
			reduce /= allSpot.Count;

			float forcef = force.Length();


			process = 1;
			heading.Add(force);

			pos.Add(heading);

			if (slowdown)
			{
				heading.Multiply(0.5f);
			}

			heading.X += (float)(rnd.NextDouble() - 0.5) * temperature;
			heading.Y += (float)(rnd.NextDouble() - 0.5) * temperature;
			heading.Z += (float)(rnd.NextDouble() - 0.5) * temperature;
		}
	}
}
