using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using myd3d;
using Microsoft.DirectX;

namespace GravityD
{
	[Serializable]
	public struct Trace
	{
		public double X;
		public double Y;
		public double Z;
		public double mass;

		public Vector3 getVect()
		{
			return new Vector3((float)X, (float)Y, (float)Z);
		}
	}

	[Serializable]
	public class PSpot
	{
		[NonSerialized]
		public Model model;

		[NonSerialized]
		public Chain line;

		public int lineCount;

		public Trace[] trace;



	}
}

/*

namespace GravityS
{
	[Serializable]
	public class Trace
	{
		public Vector3 pos;
		public double mass;
	}

	[Serializable]
	public class PSpot
	{
		[NonSerialized]
		public Model model;

		[NonSerialized]
		public Chain line;
		public int lineCount;

		public Trace[] trace;

		public Vector3 pos;

		public Vector3 heading;
		private float _mass;
		public float qmass;
		public float mass
		{
			set
			{
				_mass = value;
				qmass = (float)Math.Pow(_mass, 0.33333);
			}
			get
			{
				return _mass;
			}
		}

		[DllImport("cppDll.dll")]
		private static extern void ProcessGravity(
			float[] posx,
			float[] posy,
			float[] posz,
			float[] headingx,
			float[] headingy,
			float[] headingz,
			float[] qmass,
			float[] mass,
			int length
		);

		public static void ProcessGravity(List<PSpot> spotList)
		{
			spotList.Sort(delegate(PSpot p1, PSpot p2)
					{
						if (p1.mass > p2.mass)
						{
							return -1;
						}
						else if (p1.mass < p2.mass)
						{
							return 1;
						}
						else
						{
							return 0;
						}
					});

			int length;
			for (length = spotList.Count - 1; length >= 0; length--)
			{
				if (spotList[length].mass > 0)
				{
					break;
				}
			}

			length++;


			float[] posx = new float[length];
			float[] posy = new float[length];
			float[] posz = new float[length];
			float[] headingx = new float[length];
			float[] headingy = new float[length];
			float[] headingz = new float[length];
			float[] qmass = new float[length];
			float[] mass = new float[length];

			for (int i = 0; i < length; i++)
			{
				posx[i] = spotList[i].pos.X;
				posy[i] = spotList[i].pos.Y;
				posz[i] = spotList[i].pos.Z;

				headingx[i] = spotList[i].heading.X;
				headingy[i] = spotList[i].heading.Y;
				headingz[i] = spotList[i].heading.Z;

				qmass[i] = spotList[i].qmass;
				mass[i] = spotList[i].mass;
			}

			ProcessGravity(
				posx,
				posy,
				posz,
				headingx,
				headingy,
				headingz,
				qmass,
				mass,
				length
			);

			for (int i = 0; i < length; i++)
			{
				spotList[i].pos.X = posx[i];
				spotList[i].pos.Y = posy[i];
				spotList[i].pos.Z = posz[i];

				spotList[i].heading.X = headingx[i];
				spotList[i].heading.Y = headingy[i];
				spotList[i].heading.Z = headingz[i];

				spotList[i].qmass = qmass[i];
				spotList[i].mass = mass[i];
			}

		}
	}
}
*/