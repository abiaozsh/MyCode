package rsa;

import java.io.IOException;
import java.io.OutputStream;

public class RSAOutputStream extends OutputStream
{

	private int len;

	private byte[] buffer;

	private int IdxBuffer;

	private BigInteger public_key;

	private BigInteger n;

	private OutputStream os;

	public RSAOutputStream(OutputStream os, PublicKey pk)
	{
		len = (pk.length / 8 * 2) - 2;
		buffer = new byte[len];
		IdxBuffer = 0;
		public_key = BigInteger.valueOf(65537);
		n = new BigInteger(pk.n);
		this.os = os;
	}

	public void write(int b) throws IOException
	{
		buffer[IdxBuffer] = (byte) b;
		IdxBuffer++;
		flush();
	}

	public void flush() throws IOException
	{
		if (IdxBuffer == len)
		{
			byte[] num = new byte[len + 1];
			num[0] = 1;
			for (int i = 0; i < len; i++)
			{
				num[i+1] = buffer[i];
			}

			BigInteger v = new BigInteger(num);

			BigInteger coded = v.modPow(public_key, n);

			byte[] bb = coded.toByteArray();

			os.write(bb.length & 0xFF);
			os.write((bb.length >>> 8) & 0xFF);

			os.write(0x55);
			os.write(0x55);

			os.write(bb);

			IdxBuffer = 0;
		}
	}

	public void close() throws IOException
	{
		byte[] num = new byte[IdxBuffer + 1];
		num[0] = 1;
		for (int i = 0; i < IdxBuffer; i++)
		{
			num[i+1] = buffer[i];
		}

		BigInteger v = new BigInteger(num);

		BigInteger coded = v.modPow(public_key, n);

		byte[] bb = coded.toByteArray();

		os.write(bb.length & 0xFF);
		os.write((bb.length >>> 8) & 0xFF);

		os.write(IdxBuffer & 0xFF);
		os.write((IdxBuffer >>> 8) & 0xFF);

		os.write(bb);

		IdxBuffer = 0;

		os.flush();
		os.close();
	}
}
