package Core;

import java.util.ArrayList;
import java.util.List;


public class PDot
{
    public double posx;
    public double posy;

    public double headingx;
    public double headingy;

    public boolean CheckTouch(ArrayList<PLine> lineList)
    {
        PVector[] result = new PVector[1];

        if (IsTouch(new PVector(posx, posy, posx + headingx, posy + headingy), lineList, result, false, new PLine()))
        {
            posx = result[0].x2;
            posy = result[0].y2;

            return true;
        }

        return false;
    }

    public boolean IsTouch(PVector v, List<PLine> lineList, PVector[] result, boolean isExcept, PLine lineExcept)
    {
        double compDist = Double.MAX_VALUE;
        PPoint compP = new PPoint();
        PLine compL = new PLine();

        for (PLine l : lineList)
        {
            if (!isExcept || !lineExcept.equals(l))
            {
                if (IsLineSegmentCross(v, l))
                {
                    PPoint p = GetCrossPoint(new PPoint(v.x1, v.y1), new PPoint(v.x2, v.y2), new PPoint(l.x1, l.y1), new PPoint(l.x2, l.y2));
                    double distance = GetDistance(new PPoint(v.x1, v.y1), p);
                    if (distance < compDist)
                    {
                        compDist = distance;
                        compP = p;
                        compL = l;
                    }
                }
            }
        }


        if (compDist != Double.MAX_VALUE)
        {
            PVector newv = new PVector();
            newv.x1 = compP.x;
            newv.y1 = compP.y;
            PPoint mirror = Mirror(new PPoint(v.x2, v.y2), compL);
            newv.x2 = mirror.x;
            newv.y2 = mirror.y;
            result[0] = newv;

            PVector[] outResult = new PVector[1];
            if (IsTouch(newv, lineList, outResult, true, compL))
            {
                result[0] = outResult[0];
            }
            return true;
        }
        else
        {
            result[0] = new PVector();
            return false;
        }
    }

    public PPoint Mirror(PPoint p, PLine l)
    {
        double B;
        double A;
        double C;

        if (Math.abs(l.x1 - l.x2) > Math.abs(l.y1 - l.y2))
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

    
    
    public static boolean IsLineSegmentCross(PPoint pFirst1, PPoint pFirst2, PPoint pSecond1, PPoint pSecond2)
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
        PPoint crossPoint = new PPoint();
        double tempLeft, tempRight;
        tempLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
        tempRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);
        crossPoint.x = tempRight / tempLeft;
        tempLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
        tempRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x - p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
        crossPoint.y = tempRight / tempLeft;
        return crossPoint;
    }

    public static boolean IsLineSegmentCross(PVector v, PLine l)
    {
        if (Math.min(l.x1, l.x2) <= v.x2 && v.x2 <= Math.max(l.x1, l.x2) && Math.min(l.y1, l.y2) <= v.y2 && v.y2 <= Math.max(l.y1, l.y2))
        {
            double B;
            double A;
            double C;

            if (Math.abs(l.x1 - l.x2) > Math.abs(l.y1 - l.y2))
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
        return Math.sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }

}
