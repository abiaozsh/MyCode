package rsa;

import java.util.Random;

public class RSA
{
	public static KeyPair getKeyPair(int length)
	{
		BigInteger p;
		BigInteger q;
		BigInteger n;
		BigInteger m;
		BigInteger public_key;
		BigInteger private_key;
		Random r = new Random();
		p = BigInteger.probablePrime(length,r);
		q = BigInteger.probablePrime(length,r);
		n = p.multiply(q);
		m = (p.subtract(BigInteger.ONE)).multiply((q.subtract(BigInteger.ONE)));
		public_key = BigInteger.valueOf(65537);
		private_key = getPrivate_key(m,public_key);
		KeyPair kp = new KeyPair();
		kp.publicKey = new PublicKey();
		kp.publicKey.length = length;
		kp.publicKey.n = n.toByteArray();

		kp.privateKey = new PrivateKey();
		kp.privateKey.length = length;
		kp.privateKey.n = n.toByteArray();
		kp.privateKey.key = private_key.toByteArray();
		return kp;
	}

    private static BigInteger getPrivate_key(BigInteger m1, BigInteger pub)
    {
    	BigInteger value=BigInteger.ONE;
        for(BigInteger i = BigInteger.ONE;;i=i.add(BigInteger.ONE))
        {
            value=i.multiply(m1).add(BigInteger.ONE);
            BigInteger[] a = value.divideAndRemainder(pub);
            if((a[1].equals(BigInteger.ZERO)) && (a[0].compareTo(m1) < 0))
            {
                return a[0];
            }
        }
    }
}
