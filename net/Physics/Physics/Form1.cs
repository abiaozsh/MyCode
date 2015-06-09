using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace physics
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        List<PDot> dList = new List<PDot>();

        List<PLine> lineList = new List<PLine>();

        double gravity = -0.1;
        double airDrag = 0.99;

        private void button1_Click(object sender, EventArgs e)
        {

            lineList = new List<PLine>();
            PLine l;

            l = new PLine(); l.x1 = 1; l.y1 = 1; l.x2 = 798; l.y2 = 1; lineList.Add(l);
            l = new PLine(); l.x1 = 1; l.y1 = 1; l.x2 = 1; l.y2 = 598; lineList.Add(l);
            l = new PLine(); l.x1 = 1; l.y1 = 598; l.x2 = 798; l.y2 = 598; lineList.Add(l);
            l = new PLine(); l.x1 = 798; l.y1 = 1; l.x2 = 798; l.y2 = 598; lineList.Add(l);

            l = new PLine();
            l.x1 = 37;
            l.y1 = 68;
            l.x2 = 71;
            l.y2 = 29;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 236;
            l.y1 = 27;
            l.x2 = 276;
            l.y2 = 58;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 111;
            l.y1 = 159;
            l.x2 = 216;
            l.y2 = 160;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 56;
            l.y1 = 127;
            l.x2 = 92;
            l.y2 = 101;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 207;
            l.y1 = 86;
            l.x2 = 244;
            l.y2 = 130;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 152;
            l.y1 = 70;
            l.x2 = 150;
            l.y2 = 32;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 78;
            l.y1 = 79;
            l.x2 = 144;
            l.y2 = 109;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 175;
            l.y1 = 91;
            l.x2 = 211;
            l.y2 = 62;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 104;
            l.y1 = 41;
            l.x2 = 80;
            l.y2 = 17;
            lineList.Add(l);

            l = new PLine();
            l.x1 = 206;
            l.y1 = 39;
            l.x2 = 227;
            l.y2 = 14;
            lineList.Add(l);


        }
        int tmpx;
        int tmpy;
        void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            timer1.Stop();

            tmpx = e.X;
            tmpy = img.Height - e.Y;
        }

        void pictureBox1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {

            int x = e.X;
            int y = img.Height - e.Y;
            int headingx = (x - tmpx);
            int headingy = (y - tmpy);

            double dist = Math.Sqrt(headingx * headingx + headingy * headingy);
            double arc = Math.Asin((double)headingx * Math.Sqrt((double)1 / (headingx * headingx + headingy * headingy)));
            if (headingx > 0 && headingy < 0)
            {
                arc = 3.14159265 - arc;
            }
            else if (headingx < 0 && headingy > 0)
            {
                arc = 3.14159265 * 2 + arc;
            }
            else if (headingx < 0 && headingy < 0)
            {
                arc = 3.14159265 - arc;
            }

            dList = new List<PDot>();
            PDot d;

            for (double i = 0; i < 0.5; i += 0.01)
            {
                if (i == 0)
                {
                    d = new PDot();
                    d.posx = tmpx;
                    d.posy = tmpy;
                    d.headingx = Math.Sin(arc) * dist * 0.2;
                    d.headingy = Math.Cos(arc) * dist * 0.2;
                    dList.Add(d);
                }
                else
                {
                    d = new PDot();
                    d.posx = tmpx;
                    d.posy = tmpy;
                    d.headingx = Math.Sin(arc + i) * dist * 0.2;
                    d.headingy = Math.Cos(arc + i) * dist * 0.2;
                    dList.Add(d);

                    d = new PDot();
                    d.posx = tmpx;
                    d.posy = tmpy;
                    d.headingx = Math.Sin(arc - i) * dist * 0.2;
                    d.headingy = Math.Cos(arc - i) * dist * 0.2;
                    dList.Add(d);
                }
            }

            Graphics g = Graphics.FromImage(img);

            g.Clear(Color.White);

            timer1.Start();
        }

        Image img = new Bitmap(800, 600);

        private void timer1_Tick(object sender, EventArgs e)
        {
            Graphics g = Graphics.FromImage(img);

            g.Clear(Color.White);
            foreach (PLine l in lineList)
            {
                g.DrawLine(Pens.Black, (float)l.x1, img.Height - (float)l.y1, (float)l.x2, img.Height - (float)l.y2);
            }

            foreach (PDot d in dList)
            {
                if (!d.CheckTouch(lineList))
                {
                    d.headingx *= airDrag;
                    d.headingy *= airDrag;
                    d.headingx = d.headingx + 0;
                    d.headingy = d.headingy + gravity;
                    d.posx += d.headingx;
                    d.posy += d.headingy;
                }
                g.DrawRectangle(Pens.Red, (float)d.posx, img.Height - (float)d.posy, 1, 1);
            }
			g.Flush();
            pictureBox1.Image = img;

        }

        private void button3_Click(object sender, EventArgs e)
        {
            timer1.Stop();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            timer1.Start();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            timer1_Tick(null, null);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Form2 form2 = new Form2();
            form2.Show();
        }
    }

    class AMath
    {
        public static bool IsLineSegmentCross(PPoint pFirst1, PPoint pFirst2, PPoint pSecond1, PPoint pSecond2)
        {
            double Linep1;
            double Linep2;
            Linep1 = pFirst1.x * (pSecond1.y - pFirst2.y) + pFirst2.x * (pFirst1.y - pSecond1.y) + pSecond1.x * (pFirst2.y - pFirst1.y);
            Linep2 = pFirst1.x * (pSecond2.y - pFirst2.y) + pFirst2.x * (pFirst1.y - pSecond2.y) + pSecond2.x * (pFirst2.y - pFirst1.y);
            if (((Linep1 * Linep2) >= 0) && !(Linep1 == 0 && Linep2 == 0))
            {
                return false;
            }
            Linep1 = pSecond1.x * (pFirst1.y - pSecond2.y) + pSecond2.x * (pSecond1.y - pFirst1.y) + pFirst1.x * (pSecond2.y - pSecond1.y);
            Linep2 = pSecond1.x * (pFirst2.y - pSecond2.y) + pSecond2.x * (pSecond1.y - pFirst2.y) + pFirst2.x * (pSecond2.y - pSecond1.y);
            if (((Linep1 * Linep2) >= 0) && !(Linep1 == 0 && Linep2 == 0))
            {
                return false;
            }
            return true;
        }
        public static PPoint GetCrossPoint(PPoint p1, PPoint p2, PPoint q1, PPoint q2)
        {
            PPoint crossPoint;
            double tempLeft, tempRight;
            tempLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
            tempRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);
            crossPoint.x = tempRight / tempLeft;
            tempLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
            tempRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x - p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
            crossPoint.y = tempRight / tempLeft;
            return crossPoint;
        }

        public static bool IsLineSegmentCross(PVector v, PLine l)
        {
            if (Math.Min(l.x1, l.x2) <= v.x2 && v.x2 <= Math.Max(l.x1, l.x2) && Math.Min(l.y1, l.y2) <= v.y2 && v.y2 <= Math.Max(l.y1, l.y2))
            {
                double B;
                double A;
                double C;

                if (Math.Abs(l.x1 - l.x2) > Math.Abs(l.y1 - l.y2))
                {
                    B = 1;
                    A = -(l.y1 - l.y2) / (l.x1 - l.x2);
                    C = -(A * l.x1 + l.y1);
                }
                else
                {
                    A = 1;
                    B = -(l.x1 - l.x2) / (l.y1 - l.y2);
                    C = -(l.x1 + B * l.y1);
                }

                if (v.x2 * A + v.y2 * B + C == 0)
                {
                    return true;
                }
            }

            return IsLineSegmentCross(new PPoint(v.x1, v.y1), new PPoint(v.x2, v.y2), new PPoint(l.x1, l.y1), new PPoint(l.x2, l.y2));
        }

        public static double GetDistance(PPoint p1, PPoint p2)
        {
            return Math.Sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
        }


    }


    public class PDot
    {
        public double posx;
        public double posy;

        public double headingx;
        public double headingy;

        public bool CheckTouch(List<PLine> lineList)
        {
            PVector result;

            if (IsTouch(new PVector(posx, posy, posx + headingx, posy + headingy), lineList, out result, false, new PLine()))
            {
                posx = result.x2;
                posy = result.y2;

                return true;
            }

            return false;
        }

        public bool IsTouch(PVector v, List<PLine> lineList, out PVector result, bool isExcept, PLine lineExcept)
        {
            double compDist = double.MaxValue;
            PPoint compP = new PPoint();
            PLine compL = new PLine();

            foreach (PLine l in lineList)
            {
                if (!isExcept || !lineExcept.Equals(l))
                {
                    if (AMath.IsLineSegmentCross(v, l))
                    {
                        PPoint p = AMath.GetCrossPoint(new PPoint(v.x1, v.y1), new PPoint(v.x2, v.y2), new PPoint(l.x1, l.y1), new PPoint(l.x2, l.y2));
                        double distance = AMath.GetDistance(new PPoint(v.x1, v.y1), p);
                        if (distance < compDist)
                        {
                            compDist = distance;
                            compP = p;
                            compL = l;
                        }
                    }
                }
            }


            if (compDist != double.MaxValue)
            {
                PVector newv = new PVector();
                newv.x1 = compP.x;
                newv.y1 = compP.y;
                PPoint mirror = Mirror(new PPoint(v.x2, v.y2), compL);
                newv.x2 = mirror.x;
                newv.y2 = mirror.y;
                result = newv;

                PVector outResult;
                if (IsTouch(newv, lineList, out outResult, true, compL))
                {
                    result = outResult;
                }
                return true;
            }
            else
            {
                result = new PVector();
                return false;
            }
        }

        public PPoint Mirror(PPoint p, PLine l)
        {
            double B;
            double A;
            double C;

            if (Math.Abs(l.x1 - l.x2) > Math.Abs(l.y1 - l.y2))
            {
                B = 1;
                A = -(l.y1 - l.y2) / (l.x1 - l.x2);
                C = -(A * l.x1 + l.y1);
            }
            else
            {
                A = 1;
                B = -(l.x1 - l.x2) / (l.y1 - l.y2);
                C = -(l.x1 + B * l.y1);
            }

            double posx = p.x;
            double posy = p.y;
            PPoint ret = new PPoint();

            ret.x = posx - (2 * A * (A * posx + B * posy + C) / (A * A + B * B));
            ret.y = posy - (2 * B * (A * posx + B * posy + C) / (A * A + B * B));


            posx = headingx;
            posy = headingy;
            headingx = posx - (2 * A * (A * posx + B * posy) / (A * A + B * B));
            headingy = posy - (2 * B * (A * posx + B * posy) / (A * A + B * B));


            return ret;
        }

    }

    public struct PVector
    {
        public PVector(double x1, double y1, double x2, double y2)
        {
            this.x1 = x1;
            this.y1 = y1;
            this.x2 = x2;
            this.y2 = y2;
        }
        public double x1;
        public double y1;
        public double x2;
        public double y2;
    }


    public struct PPoint
    {
        public PPoint(double x, double y)
        {
            this.x = x;
            this.y = y;
        }
        public double x;
        public double y;
    };

    public struct PLine
    {
        public double x1;
        public double y1;
        public double x2;
        public double y2;
        public string name;

    }
}