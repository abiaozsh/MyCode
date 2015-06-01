package rsa;

public class PublicKey
{
	protected int length;
	protected byte[] n;
	/*
	 * k = 65537;
	 */

	public PublicKey()
	{
		
	}
	
	public byte[] toByteArray()
	{
		byte[] b = new byte[n.length+4];
		b[0] = (byte)(length & 0xFF);
		b[1] = (byte)((length>>>8) & 0xFF);
		System.arraycopy(n,0,b,4,n.length-4);
		return b;
	}
	
	public PublicKey(byte[] b)
	{
		length = (b[0]&0xFF)+((b[1]&0xFF)<<8);
		System.arraycopy(b,4,n,0,b.length-4);
	}
}
