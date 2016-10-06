using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using myd3d;
using Microsoft.DirectX;

namespace Gravity
{
	[Serializable]
	public struct DVector3
	{
		public double X;
		public double Y;
		public double Z;

		public Vector3 getVect()
		{
			return new Vector3((float)X, (float)Y, (float)Z);
		}
	}

	[Serializable]
	public class Trace
	{
		public DVector3 pos;
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

		public DVector3 pos;

		public DVector3 heading;

		private double _mass;
		public double qmass;
		public double mass
		{
			set
			{
				_mass = value;
				qmass = Math.Pow(_mass, 0.33333);
			}
			get
			{
				return _mass;
			}
		}

		[DllImport("cppDll.dll")]
		private static extern void ProcessGravity(
			double[] posx,
			double[] posy,
			double[] posz,
			double[] headingx,
			double[] headingy,
			double[] headingz,
			double[] qmass,
			double[] mass,
			int[] idx,
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


			double[] posx = new double[length];
			double[] posy = new double[length];
			double[] posz = new double[length];
			double[] headingx = new double[length];
			double[] headingy = new double[length];
			double[] headingz = new double[length];
			double[] qmass = new double[length];
			double[] mass = new double[length];
			int[] idx = new int[length];

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
				idx[i] = i;
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
				idx,
				length
			);

			for (int i = 0; i < length; i++)
			{
				int index = idx[i];
				spotList[index].pos.X = posx[i];
				spotList[index].pos.Y = posy[i];
				spotList[index].pos.Z = posz[i];

				spotList[index].heading.X = headingx[i];
				spotList[index].heading.Y = headingy[i];
				spotList[index].heading.Z = headingz[i];

				spotList[index].mass = mass[i];
			}

		}
	}
}
