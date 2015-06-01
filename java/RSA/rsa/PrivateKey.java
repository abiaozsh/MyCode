package rsa;

public class PrivateKey
{
	protected int length;
	protected byte[] n;
	protected byte[] key;
	
	protected PrivateKey()
	{
	} 

	public byte[] toByteArray()
	{
		byte[] b = new byte[n.length+key.length+4];
		b[0] = (byte)(length & 0xFF);
		b[1] = (byte)((length>>>8) & 0xFF);
		b[2] = (byte)((length>>>16) & 0xFF);
		b[3] = (byte)((length>>>24) & 0xFF);
		System.arraycopy(n,0,b,4,n.length);
		System.arraycopy(n,0,b,4,n.length-4);
		return b;
	}

	public PrivateKey(byte[] b)
	{
		length = (b[0]&0xFF)+((b[1]&0xFF)<<8)+((b[2]&0xFF)<<16)+((b[3]&0xFF)<<24);
		System.arraycopy(b,4,n,0,b.length-4);
 	}
}
