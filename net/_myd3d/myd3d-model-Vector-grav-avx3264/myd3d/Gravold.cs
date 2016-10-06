using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using myd3d;

namespace Gravity
{
    public class Point3D
    {
        public float x;
        public float y;
        public float z;
    }

    public class Trace
    {
        public float x;
        public float y;
        public float z;
        public double mass;
        public List<Point3D> drawPoint;
    }

    public class PSpot
    {
        public Model model1;

        public Model model2;

        public Line line1;

        public Line line2;

        public Trace[] trace;

        public List<Point3D> drawPoint;

        public double posx;

        public double posy;

        public double posz;

        public double headingx;

        public double headingy;

        public double headingz;

        public double mass;

        public double process;

        public static void ProcessGravity(List<PSpot> spotList)
        {
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
                    sp.CalcGravity(spotList);
                }
                else
                {
                    break;
                }
            }
        }

        static double G = 0.01;

        public void CalcGravity(List<PSpot> allSpot)
        {
            double forceX = 0;
            double forceY = 0;
            double forceZ = 0;
            foreach (PSpot s in allSpot)
            {
                if (s != this)
                {
                    double diffx = s.posx - posx;
                    double diffy = s.posy - posy;
                    double diffz = s.posz - posz;
                    double r2 = diffx * diffx + diffy * diffy + diffz * diffz;
                    double r = Math.Sqrt(r2);
                    double m = this.mass * s.mass;
                    double f = G * m / r2;

                    if (r < 0.1)
                    {
                        if (s.mass > this.mass)
                        {
                            s.headingx += this.headingx * (this.mass / s.mass);
                            s.headingy += this.headingy * (this.mass / s.mass);
                            s.headingz += this.headingz * (this.mass / s.mass);
                            s.mass += this.mass;
                            this.mass = 0;
                            allSpot.Remove(this);
                        }
                        else
                        {
                            this.headingx += s.headingx * (s.mass / this.mass);
                            this.headingy += s.headingy * (s.mass / this.mass);
                            this.headingz += s.headingz * (s.mass / this.mass);
                            this.mass += s.mass;
                            s.mass = 0;
                            allSpot.Remove(s);
                        }
                        return;
                    }
                    double tempv = f / r;
                    forceX += tempv * diffx;
                    forceY += tempv * diffy;
                    forceZ += tempv * diffz;
                }
            }
            forceX /= mass;
            forceY /= mass;
            forceZ /= mass;

            double force = Math.Sqrt(forceX * forceX + forceY * forceY + forceZ * forceZ);

            if (force > 0)
            {
                double gate = 0.0001;
                double step = gate / force;

                if (process + step < 1)
                {
                    process += step;
                }
                else
                {
                    step = 1 - process;
                    process = 1;
                }

                headingx += forceX * step;
                headingy += forceY * step;
                headingz += forceZ * step;
                posx += headingx * step;
                posy += headingy * step;
                posz += headingz * step;

                Point3D point = drawPoint[drawPoint.Count - 1];
                if (
                    Math.Abs(point.x - posx) > 0.1 ||
                    Math.Abs(point.y - posy) > 0.1 ||
                    Math.Abs(point.z - posz) > 0.1
                    )
                {
                    point = new Point3D();
                    point.x = (float)posx;
                    point.y = (float)posy;
                    point.z = (float)posz;
                    drawPoint.Add(point);
                }
            }
            else
            {
                process = 1;
            }
        }
    }
}