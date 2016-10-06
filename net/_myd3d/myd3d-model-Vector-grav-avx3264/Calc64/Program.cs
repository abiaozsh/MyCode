using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using GravityD;
using System.Runtime.Serialization;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Calc64
{
	static class Program
	{



		[STAThread]
		static void Main()
		{
			try
			{
				calc c = new calc();
				c.init();
				c.proc();
				c.save();
			}
			catch (Exception e)
			{
				string s = e.ToString();
			}

		}
	}

	class calc
	{

		[DllImport("kernel32.dll")]
		static extern uint GetTickCount();

		int total = 100000;

		List<PSpot> traceList = new List<PSpot>();
		List<PSpot> spotList = new List<PSpot>();

		public void save()
		{
			IFormatter formatter = new BinaryFormatter();
			Stream fs = new FileStream("save.bin", FileMode.Create, FileAccess.Write, FileShare.None);
			BinaryWriter bw = new BinaryWriter(fs);
			bw.Write(traceList.Count);
			for (int i = 0; i < traceList.Count; i++)
			{
				PSpot ps = traceList[i];
				bw.Write(ps.trace.Length);
				for (int j = 0; j < ps.trace.Length; j++)
				{
					Trace t = ps.trace[j];
					bw.Write(t.X);
					bw.Write(t.Y);
					bw.Write(t.Z);
					bw.Write(t.mass);
				}
			}
			//formatter.Serialize(stream, traceList);
			bw.Flush();
			fs.Close();
		}

		public PSpot addSpot(double posx, double posy, double posz, double headingx, double headingy, double headingz, List<PSpot> spotList, List<PSpot> traceList, int total, double mass)
		{
			PSpot sp = new PSpot();
			sp.pos.X = posx;
			sp.pos.Y = posy;
			sp.pos.Z = posz;
			sp.heading.X = headingx;
			sp.heading.Y = headingy;
			sp.heading.Z = headingz;
			sp.mass = mass;
			sp.trace = new Trace[total];

			traceList.Add(sp);
			spotList.Add(sp);
			return sp;
		}

		public void init1()
		{
			Random rnd = new Random();

			{
				double m = 200;
				double G = 0.1;
				double x0 = 50;
				double y0 = 0;
				double z0 = 0;
				double mx0 = 0;
				double my0 = 0;
				double mz0 = -0.32;
				addSpot(x0, y0, z0, mx0, my0, mz0, spotList, traceList, total, m);

				for (int i = 0; i < 100; i++)
				{
					double ang = rnd.NextDouble() * Math.PI * 2;
					double dist = 10 + rnd.NextDouble() * 30;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = rnd.NextDouble() * 20 - 10;

					double speed2 = G * m / (Math.Sqrt(dist * dist + y * y));
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.1);

				}

			}

			{
				double m = 200;
				double G = 0.1;
				double x0 = -50;
				double y0 = 0;
				double z0 = 0;
				double mx0 = 0;
				double my0 = 0;
				double mz0 = 0.32;
				addSpot(x0, y0, z0, mx0, my0, mz0, spotList, traceList, total, m);

				for (int i = 0; i < 100; i++)
				{
					double ang = rnd.NextDouble() * Math.PI * 2;
					double dist = 10 + rnd.NextDouble() * 30;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = rnd.NextDouble() * 20 - 10;

					double speed2 = G * m / (Math.Sqrt(dist * dist + y * y));
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.1);

				}

			}

		}
		public void init()
		{
			Random rnd = new Random();

			{
				double m = 200;
				double G = 0.1;
				double x0 = 50;
				double y0 = 0;
				double z0 = 0;
				double mx0 = 0;
				double my0 = -0.32 * 1.1;
				double mz0 = 0;
				addSpot(x0, y0, z0, mx0, my0, mz0, spotList, traceList, total, m);

				for (int i = 0; i < 100; i++)
				{
					double ang = rnd.NextDouble() * Math.PI * 2;
					double dist = 10 + rnd.NextDouble() * 30;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = rnd.NextDouble() * 20 - 10;

					double speed2 = G * m / (Math.Sqrt(dist * dist + y * y));
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.1);

				}

			}

			{
				double m = 200;
				double G = 0.1;
				double x0 = -50;
				double y0 = 0;
				double z0 = 0;
				double mx0 = 0;
				double my0 = 0.32 * 1.1;
				double mz0 = 0;
				addSpot(x0, y0, z0, mx0, my0, mz0, spotList, traceList, total, m);

				for (int i = 0; i < 100; i++)
				{
					double ang = rnd.NextDouble() * Math.PI * 2;
					double dist = 10 + rnd.NextDouble() * 30;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = rnd.NextDouble() * 20 - 10;

					double speed2 = G * m / (Math.Sqrt(dist * dist + y * y));
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.1);

				}

			}

		}
		public void init2()
		{
			Random rnd = new Random();

			{
				double m = 200;
				double G = 0.1;
				double x0 = 0;
				double y0 = 0;
				double z0 = 0;
				double mx0 = 0;
				double my0 = 0;
				double mz0 = 0;
				addSpot(x0, y0, z0, mx0, my0, mz0, spotList, traceList, total, m);

				for (int i = 0; i < 100; i++)
				{
					double ang = rnd.NextDouble() * Math.PI * 2;
					double dist = 1 + rnd.NextDouble() * 100;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = rnd.NextDouble() * 20 - 10;

					double speed2 = G * m / (Math.Sqrt(dist * dist + y * y));
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.1);

				}

			}


		}

		public void proc()
		{

			double momentumx = 0;
			double momentumy = 0;
			double momentumz = 0;
			double mass = 0;
			foreach (PSpot s in traceList)
			{
				momentumx += s.heading.X * s.mass;
				momentumy += s.heading.Y * s.mass;
				momentumz += s.heading.Z * s.mass;
				mass += s.mass;
			}

			foreach (PSpot s in traceList)
			{
				s.heading.X -= momentumx / mass;
				s.heading.Y -= momentumy / mass;
				s.heading.Z -= momentumz / mass;
			}

			uint a = GetTickCount();
			for (int cnt = 0; cnt < total; cnt++)
			{

				#region proc
				PSpot.ProcessGravity(spotList);

				foreach (PSpot s in traceList)
				{
					//s.trace[cnt] = new Trace();
					s.trace[cnt].X = s.pos.X;
					s.trace[cnt].Y = s.pos.Y;
					s.trace[cnt].Z = s.pos.Z;
					s.trace[cnt].mass = s.mass;
				}
				#endregion
				//title = cnt.ToString();
				Console.WriteLine(cnt.ToString());
				//this.Text = title;
				Application.DoEvents();
			}
			uint b = GetTickCount();

			//Vcount++;
			//Vvalue += (b - a);
			//title += ":" + Vvalue;


		}

	}
}

namespace GravityD
{
	[Serializable]
	public struct DVector3
	{
		public double X;
		public double Y;
		public double Z;
	}

	[Serializable]
	public struct Trace
	{
		public double X;
		public double Y;
		public double Z;
		public double mass;
	}

	[Serializable]
	public class PSpot
	{
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

		[DllImport("GravDll.dll")]
		private static extern void ProcessGravityDouble(
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
		[DllImport("GravDll.dll")]
		private static extern void ProcessGravityAVX(
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

			//ProcessGravityAVX(
			ProcessGravityDouble(
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

				spotList[index].qmass = qmass[i];
				spotList[index].mass = mass[i];
			}

		}
	}
}

