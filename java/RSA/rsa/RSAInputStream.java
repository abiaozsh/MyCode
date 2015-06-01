package rsa;

import java.io.IOException;
import java.io.InputStream;

public class RSAInputStream extends InputStream
{

	private int len;

	private byte[] buffer;

	private int IdxBuffer;

	private BigInteger private_key;

	private BigInteger n;

	private InputStream is;

	private int endFlg = 0;

	public RSAInputStream(InputStream is, PrivateKey pk)
	{
		len = (pk.length / 8 * 2) - 2;
		buffer = new byte[len];
		IdxBuffer = len;
		n = new BigInteger(pk.n);
		private_key = new BigInteger(pk.key);
		this.is = is;
	}

	public int read() throws IOException
	{
		if(endFlg!=0 && IdxBuffer == endFlg)
		{
			return -1;
		}
		if(IdxBuffer == len)
		{
			int bb1 = is.read();
			int bb2 = is.read();
			int bblen = bb1 + (bb2<<8);

			int a1 = is.read();
			int a2 = is.read();
			int aalen = a1 + (a2<<8);

			if(a1==0 && a2==0)
			{
				endFlg = 1;
				IdxBuffer = 1;
				return -1;
			}
			else if(a1==0x55 && a2==0x55)
			{
				aalen = len;
			}
			else
			{
				endFlg = aalen;
			}

			byte[] in = new byte[bblen];

			is.read(in);

			BigInteger coded2 = new BigInteger(in);

			BigInteger decoded = coded2.modPow(private_key, n);

			byte[] buf2 = decoded.toByteArray();

			for(int i=0;i<aalen;i++)
			{
				buffer[i] = buf2[i+1];
			}

			IdxBuffer = 0;
		}
		return buffer[IdxBuffer++] & 0xFF;
	}
}
