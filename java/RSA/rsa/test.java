package rsa;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.math.BigInteger;

public class test
{
	public static void main(String[] args) throws Exception
	{
		//a2();
		//if(true)return;
		KeyPair kp = RSA.getKeyPair(128);

		FileOutputStream fos = new FileOutputStream("d:\\coded.bin");
		RSAOutputStream ros = new RSAOutputStream(fos, kp.publicKey);
		byte[] data = "123456789012345678901234567890123".getBytes("utf-8");

		System.out.println(Util.toString(data));
		// byte[] data = {0,0};
		ros.write(data);
		ros.close();
		fos.close();

		FileInputStream fis = new FileInputStream("d:\\coded.bin");
		RSAInputStream ris = new RSAInputStream(fis, kp.privateKey);
		byte[] decodedData = new byte[1024];
		int len = ris.read(decodedData);
		System.out.println(Util.toString(decodedData));
		System.out.print(new String(decodedData, 0, len, "utf-8"));
	}

	static void a2()throws Exception
	{
		KeyPair kp = RSA.getKeyPair(128);
		BigInteger public_key = BigInteger.valueOf(65537);
		BigInteger n = new BigInteger(kp.publicKey.n);
		BigInteger private_key = new BigInteger(kp.privateKey.key);
		
		byte[] data = "a大家好".getBytes("utf-8");
		data[0] = 1;
		System.out.println(Util.toString(data));
		BigInteger v = new BigInteger(data);

		BigInteger coded = v.modPow(public_key, n);

		byte[] bb = coded.toByteArray();

		BigInteger coded2 = new BigInteger(bb);

		BigInteger decoded = coded2.modPow(private_key, n);

		//byte[] decodedData = new byte[1024];
		byte[] decodedData = decoded.toByteArray();

		System.out.println(Util.toString(decodedData));
		System.out.print(new String(decodedData, "utf-8"));
	}
}
