package webServer;

import java.awt.image.BufferedImage;
import java.io.OutputStream;

public class ImgProc
{
	public static void Proc(OutputStream os, BufferedImage b, float q)
	{
		try
		{
			com.sun.image.codec.jpeg.JPEGImageEncoder encoder = com.sun.image.codec.jpeg.JPEGCodec.createJPEGEncoder(os);
			com.sun.image.codec.jpeg.JPEGEncodeParam param = com.sun.image.codec.jpeg.JPEGCodec.getDefaultJPEGEncodeParam(b);
			param.setQuality(q, false);
			encoder.setJPEGEncodeParam(param);
			encoder.encode(b);
		}
		catch (Throwable t)
		{
			t.printStackTrace();
		}
	}
}
