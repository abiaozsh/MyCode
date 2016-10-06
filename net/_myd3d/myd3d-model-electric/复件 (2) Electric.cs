using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using myd3d;
using Microsoft.DirectX;

namespace Electric
{
	public class Bound
	{
		PSpot s1;

		PSpot s2;
	
	}

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

		public float tempDistance;

		public PSpot bounded;

		public override string ToString()
		{
			return sname + " charge:" + charge.ToString();
		}

		public static void Process(List<PSpot> spotList, bool slowdown, float temperature)
		{
			Random rnd = new Random();

			foreach (PSpot s in spotList)
			{
				s.bounded = null;
			}

			foreach (PSpot _this in spotList)
			{
				if (_this.charge < 0 && _this.bounded == null)
				{
					//Sort by distance
					foreach (PSpot s in spotList)
					{
						Vector3 diff = s.pos;
						diff.Subtract(_this.pos);
						s.tempDistance = diff.Length();
					}

					spotList.Sort(delegate(PSpot r1, PSpot r2)
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

					foreach (PSpot s in spotList)
					{
						if (s != _this && s.charge < 0 && s.bounded == null)
						{
							s.bounded = _this;
							_this.bounded = s;
							break;
						}
					}
				}
			}

			foreach (PSpot s in spotList)
			{
				s.Calc(spotList, rnd, slowdown, temperature);
			}
		}

		static float E = 0.001f;


		public void Calc(List<PSpot> allSpot, Random rnd, bool slowdown, float temperature)
		{
			if (this._static)
			{
				return;
			}

			Vector3 force = new Vector3(0, 0, 0);
			double reduce = 0;

			int eCount = 0;
			for (int i = 0; i < allSpot.Count; i++)
			{
				PSpot s = allSpot[i];
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
						r2 = 0.01f;
					}
					reduce += 1 / r;

					float f;

					if (charge > 0)
					{
						if (s.charge > 0)
						{
							float d = 2;//midpoint
							float b = 30+1;//force
							float a = d * b / 2;
							f = -E * e * a / r2;
						}
						else
						{
							eCount++;

							if (eCount <= 8)
							{
								float d = 1.7f;//midpoint
								float b = 80;//force
								float a = d * b / 2;
								if (r < d)
								{
									float n = -a / r + b;
									f = -E * e * n / r;
								}
								else
								{
									f = -E * e * a / r2;
								}
							}
							else
							{
								float d = 1.8f;//midpoint
								float b = 80;//force
								float a = d * b / 2;
								if (r < d)
								{
									float n = -a / r + b;
									f = -E * e * n / r;
								}
								else
								{
									f = -E * e * a / r2;
								}
							}
						}
					}
					else
					{
						if (s.charge < 0)
						{
							if (bounded == s)
							{
								float d = 0.1f;//midpoint
								float b = 3;//force
								float a = d * b / 2;
								f = -E * e * a / r2;
							}
							else
							{
								float d = 2;//midpoint
								float b = 30-1;//force
								float a = d * b / 2;
								f = -E * e * a / r2;
							}
						}
						else
						{
							float d = 2;//midpoint
							float b = 30;//force
							float a = d * b / 2;
							if (r < d)
							{
								float n = -a / r + b;
								f = -E * e * n / r;
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
