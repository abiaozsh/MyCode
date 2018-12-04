package webServer;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;

import sun.misc.BASE64Encoder;

final class Util
{
    static int strToInt(String sNum)
    {
        int iNum;
        try
        {
            iNum = Integer.parseInt(sNum);
        }
        catch (NumberFormatException e)
        {
            iNum = 0;
        }
        return iNum;
    }

    final static String[] week = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

    static String getGMTString(Calendar c)
    {
        SimpleDateFormat formatter;

        formatter = new SimpleDateFormat("dd MMM yyyy HH:mm:ss 'GMT'", Locale.US);

        formatter.setTimeZone(TimeZone.getTimeZone("GMT"));

        return week[c.get(Calendar.DAY_OF_WEEK) - 1] + " " + formatter.format(c.getTime());
    }

    static String getTime()
    {
        Date d = Calendar.getInstance().getTime();
        SimpleDateFormat formatter;
        formatter = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        return formatter.format(d);
    }

    static String MD5(String str)
    {
        try
        {
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            BASE64Encoder base64en = new BASE64Encoder();
            return base64en.encode(md5.digest(str.getBytes("utf-8")));
        }
        catch (NoSuchAlgorithmException e)
        {
            //
        }
        catch (UnsupportedEncodingException e)
        {
            //
        }
        return null;
    }
}
