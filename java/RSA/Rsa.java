import java.util.Random;

public class Rsa {
	private int p = 0;

	private int q = 0;

	private long n = 0;

	private long m = 0;

	private long public_key = 0;// 公匙

	private long private_key = 0;// 密匙

	// 输入PQ
	public void inputPQ() throws Exception {
		int[] ar = getPRIME(1000);
		Random r = new Random();

		p = ar[(int) (r.nextDouble() * 30)];
		System.out.println(p);
		q = ar[(int) (r.nextDouble() * 30)];
		System.out.println(q);

		n = p * q;
		m = (p - 1) * (q - 1);
		System.out.println(n);
		System.out.println(m);

		public_key = 65537;

		System.out.println(public_key);

		private_key = getPrivate_key(m,public_key);
		
		System.out.println(private_key);

		for (long i = 1000; i < 1100; i++) {
			System.out.print(i + "\t");

			long eee = powmod(i, n, public_key);

			System.out.print(eee + "\t");

			long bbb = powmod(eee, n, private_key);

			System.out.print(bbb + "\n");
		}
	}
    public long getPrivate_key(long m1, long pub)
    {
        long value=1;
        for(long i=1;;i++)
        {
            value=i*m1+1;

            if((value%pub==0)&& (value/pub < m1))
            {
                return value/pub;
            }
        }
        
    }

	public long powmod(long text, long n, long key) {
		long mul = 1;
		long[] calcted = new long[50];
		long[] pow = new long[50];
		
		calcted[0] = text;
		pow[0] = 1;
		for(int i=1;i<50;i++)
		{
			calcted[i] = calcted[i-1] * calcted[i-1] % n;
			pow[i] = pow[i-1] * 2;
		}
		
		long ikey = key;
		while(true)
		{
			for(int i=50-1;i>=0;i--)
			{
				if(ikey>=pow[i])
				{
					mul = mul * calcted[i] % n;
					ikey-=pow[i];
				}
			}
			if(ikey==0)
			{
				break;
			}
		}
		return mul;
	}

	public static void main(String[] args) throws Exception {
		Rsa t = new Rsa();
		t.inputPQ();
	}

	private int[] getPRIME(int n) {
		int[] a = new int[n];
		int idx = 1;
		a[0] = 2;
		int v = 2;
		while (idx < n) {
			v++;
			int f = 0;
			for (int i = 0; i < idx; i++) {
				if (v % a[i] == 0) {
					f = 1;
					break;
				}
			}
			if (f == 1)
				continue;
			a[idx++] = v;
		}
		return a;
	}

}
