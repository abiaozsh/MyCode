package rsa;

import java.util.Random;

public class BigInteger
{
	/**
	 * The signum of this BigInteger: -1 for negative, 0 for zero, or 1 for
	 * positive. Note that the BigInteger zero <i>must</i> have a signum of 0.
	 * This is necessary to ensures that there is exactly one representation for
	 * each BigInteger value.
	 * 
	 * @serial
	 */
	int signum;

	/**
	 * The magnitude of this BigInteger, in <i>big-endian</i> order: the zeroth
	 * element of this array is the most-significant int of the magnitude. The
	 * magnitude must be "minimal" in that the most-significant int (<tt>mag[0]</tt>)
	 * must be non-zero. This is necessary to ensure that there is exactly one
	 * representation for each BigInteger value. Note that this implies that the
	 * BigInteger zero has a zero-length mag array.
	 */
	int[] mag;

	/**
	 * The bitLength of this BigInteger, as returned by bitLength(), or -1
	 * (either value is acceptable).
	 * 
	 * @serial
	 * @see #bitLength
	 */
	private int bitLength = -1;

	/**
	 * The lowest set bit of this BigInteger, as returned by getLowestSetBit(),
	 * or -2 (either value is acceptable).
	 * 
	 * @serial
	 * @see #getLowestSetBit
	 */
	private int lowestSetBit = -2;

	/**
	 * The index of the lowest-order int in the magnitude of this BigInteger
	 * that contains a nonzero int, or -2 (either value is acceptable). The
	 * least significant int has int-number 0, the next int in order of
	 * increasing significance has int-number 1, and so forth.
	 */
	private int firstNonzeroIntNum = -2;

	/**
	 * This mask is used to obtain the value of an int as if it were unsigned.
	 */
	private final static long LONG_MASK = 0xffffffffL;

	// Constructors

	/**
	 * Translates a byte array containing the two's-complement binary
	 * representation of a BigInteger into a BigInteger. The input array is
	 * assumed to be in <i>big-endian</i> byte-order: the most significant byte
	 * is in the zeroth element.
	 * 
	 * @param val
	 *            big-endian two's-complement binary representation of
	 *            BigInteger.
	 * @throws NumberFormatException
	 *             <tt>val</tt> is zero bytes long.
	 */
	public BigInteger(byte[] val)
	{
		if (val.length == 0)
			throw new NumberFormatException("Zero length BigInteger");

		if (val[0] < 0)
		{
			mag = makePositive(val);
			signum = -1;
		} else
		{
			mag = stripLeadingZeroBytes(val);
			signum = (mag.length == 0 ? 0 : 1);
		}
	}

	/**
	 * This private constructor translates an int array containing the
	 * two's-complement binary representation of a BigInteger into a BigInteger.
	 * The input array is assumed to be in <i>big-endian</i> int-order: the
	 * most significant int is in the zeroth element.
	 */
	private BigInteger(int[] val)
	{
		if (val.length == 0)
			throw new NumberFormatException("Zero length BigInteger");

		if (val[0] < 0)
		{
			mag = makePositive(val);
			signum = -1;
		} else
		{
			mag = trustedStripLeadingZeroInts(val);
			signum = (mag.length == 0 ? 0 : 1);
		}
	}

	/**
	 * Translates the sign-magnitude representation of a BigInteger into a
	 * BigInteger. The sign is represented as an integer signum value: -1 for
	 * negative, 0 for zero, or 1 for positive. The magnitude is a byte array in
	 * <i>big-endian</i> byte-order: the most significant byte is in the zeroth
	 * element. A zero-length magnitude array is permissible, and will result in
	 * in a BigInteger value of 0, whether signum is -1, 0 or 1.
	 * 
	 * @param signum
	 *            signum of the number (-1 for negative, 0 for zero, 1 for
	 *            positive).
	 * @param magnitude
	 *            big-endian binary representation of the magnitude of the
	 *            number.
	 * @throws NumberFormatException
	 *             <tt>signum</tt> is not one of the three legal values (-1,
	 *             0, and 1), or <tt>signum</tt> is 0 and <tt>magnitude</tt>
	 *             contains one or more non-zero bytes.
	 */
	private BigInteger(int signum, byte[] magnitude)
	{
		this.mag = stripLeadingZeroBytes(magnitude);

		if (signum < -1 || signum > 1)
			throw (new NumberFormatException("Invalid signum value"));

		if (this.mag.length == 0)
		{
			this.signum = 0;
		} else
		{
			if (signum == 0)
				throw (new NumberFormatException("signum-magnitude mismatch"));
			this.signum = signum;
		}
	}

	/**
	 * A constructor for internal use that translates the sign-magnitude
	 * representation of a BigInteger into a BigInteger. It checks the arguments
	 * and copies the magnitude so this constructor would be safe for external
	 * use.
	 */
	private BigInteger(int signum, int[] magnitude)
	{
		this.mag = stripLeadingZeroInts(magnitude);

		if (signum < -1 || signum > 1)
			throw (new NumberFormatException("Invalid signum value"));

		if (this.mag.length == 0)
		{
			this.signum = 0;
		} else
		{
			if (signum == 0)
				throw (new NumberFormatException("signum-magnitude mismatch"));
			this.signum = signum;
		}
	}

	/**
	 * Constructs a randomly generated BigInteger, uniformly distributed over
	 * the range <tt>0</tt> to <tt>(2<sup>numBits</sup> - 1)</tt>,
	 * inclusive. The uniformity of the distribution assumes that a fair source
	 * of random bits is provided in <tt>rnd</tt>. Note that this constructor
	 * always constructs a non-negative BigInteger.
	 * 
	 * @param numBits
	 *            maximum bitLength of the new BigInteger.
	 * @param rnd
	 *            source of randomness to be used in computing the new
	 *            BigInteger.
	 * @throws IllegalArgumentException
	 *             <tt>numBits</tt> is negative.
	 * @see #bitLength
	 */
	private BigInteger(int numBits, Random rnd)
	{
		this(1, randomBits(numBits, rnd));
	}

	private static byte[] randomBits(int numBits, Random rnd)
	{
		if (numBits < 0)
			throw new IllegalArgumentException("numBits must be non-negative");
		int numBytes = (numBits + 7) / 8;
		byte[] randomBits = new byte[numBytes];

		// Generate random bytes and mask out any excess bits
		if (numBytes > 0)
		{
			rnd.nextBytes(randomBits);
			int excessBits = 8 * numBytes - numBits;
			randomBits[0] &= (1 << (8 - excessBits)) - 1;
		}
		return randomBits;
	}

	/**
	 * Returns a positive BigInteger that is probably prime, with the specified
	 * bitLength. The probability that a BigInteger returned by this method is
	 * composite does not exceed 2<sup>-100</sup>.
	 * 
	 * @param bitLength
	 *            bitLength of the returned BigInteger.
	 * @param rnd
	 *            source of random bits used to select candidates to be tested
	 *            for primality.
	 * @return a BigInteger of <tt>bitLength</tt> bits that is probably prime
	 * @throws ArithmeticException
	 *             <tt>bitLength &lt; 2</tt>.
	 * @see #bitLength
	 */
	public static BigInteger probablePrime(int bitLength, Random rnd)
	{
		if (bitLength < 2)
			throw new ArithmeticException("bitLength < 2");

		// The cutoff of 95 was chosen empirically for best performance
		return largePrime(bitLength, 100, rnd);
	}

	/**
	 * Find a random number of the specified bitLength that is probably prime.
	 * This method is more appropriate for larger bitlengths since it uses a
	 * sieve to eliminate most composites before using a more expensive test.
	 */
	private static BigInteger largePrime(int bitLength, int certainty, Random rnd)
	{
		BigInteger p;
		p = new BigInteger(bitLength, rnd).setBit(bitLength - 1);
		p.mag[p.mag.length - 1] &= 0xfffffffe;

		// Use a sieve length likely to contain the next prime number
		int searchLen = (bitLength / 20) * 64;
		BitSieve searchSieve = new BitSieve(p, searchLen);
		BigInteger candidate = searchSieve.retrieve(p, certainty);

		while ((candidate == null) || (candidate.bitLength() != bitLength))
		{
			p = p.add(BigInteger.valueOf(2 * searchLen));
			if (p.bitLength() != bitLength)
				p = new BigInteger(bitLength, rnd).setBit(bitLength - 1);
			p.mag[p.mag.length - 1] &= 0xfffffffe;
			searchSieve = new BitSieve(p, searchLen);
			candidate = searchSieve.retrieve(p, certainty);
		}
		return candidate;
	}

	/**
	 * Returns <tt>true</tt> if this BigInteger is probably prime,
	 * <tt>false</tt> if it's definitely composite.
	 * 
	 * This method assumes bitLength > 2.
	 * 
	 * @param certainty
	 *            a measure of the uncertainty that the caller is willing to
	 *            tolerate: if the call returns <tt>true</tt> the probability
	 *            that this BigInteger is prime exceeds
	 *            <tt>(1 - 1/2<sup>certainty</sup>)</tt>. The execution
	 *            time of this method is proportional to the value of this
	 *            parameter.
	 * @return <tt>true</tt> if this BigInteger is probably prime,
	 *         <tt>false</tt> if it's definitely composite.
	 */
	protected boolean primeToCertainty(int certainty)
	{
		int rounds = 0;
		int n = (Math.min(certainty, Integer.MAX_VALUE - 1) + 1) / 2;

		// The relationship between the certainty and the number of rounds
		// we perform is given in the draft standard ANSI X9.80, "PRIME
		// NUMBER GENERATION, PRIMALITY TESTING, AND PRIMALITY CERTIFICATES".
		int sizeInBits = this.bitLength();
		if (sizeInBits < 100)
		{
			rounds = 50;
			rounds = n < rounds ? n : rounds;
			return passesMillerRabin(rounds);
		}

		if (sizeInBits < 256)
		{
			rounds = 27;
		} else if (sizeInBits < 512)
		{
			rounds = 15;
		} else if (sizeInBits < 768)
		{
			rounds = 8;
		} else if (sizeInBits < 1024)
		{
			rounds = 4;
		} else
		{
			rounds = 2;
		}
		rounds = n < rounds ? n : rounds;

		return passesMillerRabin(rounds) && passesLucasLehmer();
	}

	/**
	 * Returns true iff this BigInteger is a Lucas-Lehmer probable prime.
	 * 
	 * The following assumptions are made: This BigInteger is a positive, odd
	 * number.
	 */
	private boolean passesLucasLehmer()
	{
		BigInteger thisPlusOne = this.add(ONE);

		// Step 1
		int d = 5;
		while (jacobiSymbol(d, this) != -1)
		{
			// 5, -7, 9, -11, ...
			d = (d < 0) ? Math.abs(d) + 2 : -(d + 2);
		}

		// Step 2
		BigInteger u = lucasLehmerSequence(d, thisPlusOne, this);

		// Step 3
		return u.mod(this).equals(ZERO);
	}

	/**
	 * Computes Jacobi(p,n). Assumes n is positive, odd.
	 */
	private int jacobiSymbol(int p, BigInteger n)
	{
		if (p == 0)
			return 0;

		// Algorithm and comments adapted from Colin Plumb's C library.
		int j = 1;
		int u = n.mag[n.mag.length - 1];

		// Make p positive
		if (p < 0)
		{
			p = -p;
			int n8 = u & 7;
			if ((n8 == 3) || (n8 == 7))
				j = -j; // 3 (011) or 7 (111) mod 8
		}

		// Get rid of factors of 2 in p
		while ((p & 3) == 0)
			p >>= 2;
		if ((p & 1) == 0)
		{
			p >>= 1;
			if (((u ^ u >> 1) & 2) != 0)
				j = -j; // 3 (011) or 5 (101) mod 8
		}
		if (p == 1)
			return j;
		// Then, apply quadratic reciprocity
		if ((p & u & 2) != 0) // p = u = 3 (mod 4)?
			j = -j;
		// And reduce u mod p
		u = n.mod(BigInteger.valueOf(p)).intValue();

		// Now compute Jacobi(u,p), u < p
		while (u != 0)
		{
			while ((u & 3) == 0)
				u >>= 2;
			if ((u & 1) == 0)
			{
				u >>= 1;
				if (((p ^ p >> 1) & 2) != 0)
					j = -j; // 3 (011) or 5 (101) mod 8
			}
			if (u == 1)
				return j;
			// Now both u and p are odd, so use quadratic reciprocity
			if (u < p)
			{
				int t = u;
				u = p;
				p = t;
				if ((u & p & 2) != 0)// u = p = 3 (mod 4)?
					j = -j;
			}
			// Now u >= p, so it can be reduced
			u %= p;
		}
		return 0;
	}

	private static BigInteger lucasLehmerSequence(int z, BigInteger k, BigInteger n)
	{
		BigInteger d = BigInteger.valueOf(z);
		BigInteger u = ONE;
		BigInteger u2;
		BigInteger v = ONE;
		BigInteger v2;

		for (int i = k.bitLength() - 2; i >= 0; i--)
		{
			u2 = u.multiply(v).mod(n);

			v2 = v.square().add(d.multiply(u.square())).mod(n);
			if (v2.testBit(0))
			{
				v2 = n.subtract(v2);
				v2.signum = -v2.signum;
			}
			v2 = v2.shiftRight(1);

			u = u2;
			v = v2;
			if (k.testBit(i))
			{
				u2 = u.add(v).mod(n);
				if (u2.testBit(0))
				{
					u2 = n.subtract(u2);
					u2.signum = -u2.signum;
				}
				u2 = u2.shiftRight(1);

				v2 = v.add(d.multiply(u)).mod(n);
				if (v2.testBit(0))
				{
					v2 = n.subtract(v2);
					v2.signum = -v2.signum;
				}
				v2 = v2.shiftRight(1);

				u = u2;
				v = v2;
			}
		}
		return u;
	}

	/**
	 * Returns true iff this BigInteger passes the specified number of
	 * Miller-Rabin tests. This test is taken from the DSA spec (NIST FIPS
	 * 186-2).
	 * 
	 * The following assumptions are made: This BigInteger is a positive, odd
	 * number greater than 2. iterations<=50.
	 */
	private boolean passesMillerRabin(int iterations)
	{
		// Find a and m such that m is odd and this == 1 + 2**a * m
		BigInteger thisMinusOne = this.subtract(ONE);
		BigInteger m = thisMinusOne;
		int a = m.getLowestSetBit();
		m = m.shiftRight(a);

		// Do the tests
		Random rnd = new Random();
		for (int i = 0; i < iterations; i++)
		{
			// Generate a uniform random on (1, this)
			BigInteger b;
			do
			{
				b = new BigInteger(this.bitLength(), rnd);
			} while (b.compareTo(ONE) <= 0 || b.compareTo(this) >= 0);

			int j = 0;
			BigInteger z = b.modPow(m, this);
			while (!((j == 0 && z.equals(ONE)) || z.equals(thisMinusOne)))
			{
				if (j > 0 && z.equals(ONE) || ++j == a)
					return false;
				z = z.modPow(TWO, this);
			}
		}
		return true;
	}

	/**
	 * This private constructor differs from its public cousin with the
	 * arguments reversed in two ways: it assumes that its arguments are
	 * correct, and it doesn't copy the magnitude array.
	 */
	private BigInteger(int[] magnitude, int signum)
	{
		this.signum = (magnitude.length == 0 ? 0 : signum);
		this.mag = magnitude;
	}

	/**
	 * This private constructor is for internal use in converting from a
	 * MutableBigInteger object into a BigInteger.
	 */
	protected BigInteger(MutableBigInteger val, int sign)
	{
		if (val.offset > 0 || val.value.length != val.intLen)
		{
			mag = new int[val.intLen];
			for (int i = 0; i < val.intLen; i++)
				mag[i] = val.value[val.offset + i];
		} else
		{
			mag = val.value;
		}

		this.signum = (val.intLen == 0) ? 0 : sign;
	}

	// Static Factory Methods

	/**
	 * Returns a BigInteger whose value is equal to that of the specified
	 * <code>long</code>. This "static factory method" is provided in
	 * preference to a (<code>long</code>) constructor because it allows for
	 * reuse of frequently used BigIntegers.
	 * 
	 * @param val
	 *            value of the BigInteger to return.
	 * @return a BigInteger with the specified value.
	 */
	protected static BigInteger valueOf(long val)
	{
		// If -MAX_CONSTANT < val < MAX_CONSTANT, return stashed constant
		if (val == 0)
			return ZERO;
		if (val > 0 && val <= MAX_CONSTANT)
			return posConst[(int) val];
		else if (val < 0 && val >= -MAX_CONSTANT)
			return negConst[(int) -val];

		return new BigInteger(val);
	}

	/**
	 * Constructs a BigInteger with the specified value, which may not be zero.
	 */
	private BigInteger(long val)
	{
		if (val < 0)
		{
			signum = -1;
			val = -val;
		} else
		{
			signum = 1;
		}

		int highWord = (int) (val >>> 32);
		if (highWord == 0)
		{
			mag = new int[1];
			mag[0] = (int) val;
		} else
		{
			mag = new int[2];
			mag[0] = highWord;
			mag[1] = (int) val;
		}
	}

	/**
	 * Returns a BigInteger with the given two's complement representation.
	 * Assumes that the input array will not be modified (the returned
	 * BigInteger will reference the input array if feasible).
	 */
	private static BigInteger valueOf(int val[])
	{
		return (val[0] > 0 ? new BigInteger(val, 1) : new BigInteger(val));
	}

	// Constants

	/**
	 * Initialize static constant array when class is loaded.
	 */
	private final static int MAX_CONSTANT = 16;

	private static BigInteger posConst[] = new BigInteger[MAX_CONSTANT + 1];

	private static BigInteger negConst[] = new BigInteger[MAX_CONSTANT + 1];
	static
	{
		for (int i = 1; i <= MAX_CONSTANT; i++)
		{
			int[] magnitude = new int[1];
			magnitude[0] = (int) i;
			posConst[i] = new BigInteger(magnitude, 1);
			negConst[i] = new BigInteger(magnitude, -1);
		}
	}

	/**
	 * The BigInteger constant zero.
	 * 
	 * @since 1.2
	 */
	public static final BigInteger ZERO = new BigInteger(new int[0], 0);

	/**
	 * The BigInteger constant one.
	 * 
	 * @since 1.2
	 */
	public static final BigInteger ONE = valueOf(1);

	/**
	 * The BigInteger constant two. (Not exported.)
	 */
	private static final BigInteger TWO = valueOf(2);

	// Arithmetic Operations

	/**
	 * Returns a BigInteger whose value is <tt>(this + val)</tt>.
	 * 
	 * @param val
	 *            value to be added to this BigInteger.
	 * @return <tt>this + val</tt>
	 */
	public BigInteger add(BigInteger val)
	{
		int[] resultMag;
		if (val.signum == 0)
			return this;
		if (signum == 0)
			return val;
		if (val.signum == signum)
			return new BigInteger(add(mag, val.mag), signum);

		int cmp = intArrayCmp(mag, val.mag);
		if (cmp == 0)
			return ZERO;
		resultMag = (cmp > 0 ? subtract(mag, val.mag) : subtract(val.mag, mag));
		resultMag = trustedStripLeadingZeroInts(resultMag);

		return new BigInteger(resultMag, cmp * signum);
	}

	/**
	 * Adds the contents of the int arrays x and y. This method allocates a new
	 * int array to hold the answer and returns a reference to that array.
	 */
	private static int[] add(int[] x, int[] y)
	{
		// If x is shorter, swap the two arrays
		if (x.length < y.length)
		{
			int[] tmp = x;
			x = y;
			y = tmp;
		}

		int xIndex = x.length;
		int yIndex = y.length;
		int result[] = new int[xIndex];
		long sum = 0;

		// Add common parts of both numbers
		while (yIndex > 0)
		{
			sum = (x[--xIndex] & LONG_MASK) + (y[--yIndex] & LONG_MASK) + (sum >>> 32);
			result[xIndex] = (int) sum;
		}

		// Copy remainder of longer number while carry propagation is required
		boolean carry = (sum >>> 32 != 0);
		while (xIndex > 0 && carry)
			carry = ((result[--xIndex] = x[xIndex] + 1) == 0);

		// Copy remainder of longer number
		while (xIndex > 0)
			result[--xIndex] = x[xIndex];

		// Grow result if necessary
		if (carry)
		{
			int newLen = result.length + 1;
			int temp[] = new int[newLen];
			for (int i = 1; i < newLen; i++)
				temp[i] = result[i - 1];
			temp[0] = 0x01;
			result = temp;
		}
		return result;
	}

	/**
	 * Returns a BigInteger whose value is <tt>(this - val)</tt>.
	 * 
	 * @param val
	 *            value to be subtracted from this BigInteger.
	 * @return <tt>this - val</tt>
	 */
	public BigInteger subtract(BigInteger val)
	{
		int[] resultMag;
		if (val.signum == 0)
			return this;
		if (signum == 0)
			return val.negate();
		if (val.signum != signum)
			return new BigInteger(add(mag, val.mag), signum);

		int cmp = intArrayCmp(mag, val.mag);
		if (cmp == 0)
			return ZERO;
		resultMag = (cmp > 0 ? subtract(mag, val.mag) : subtract(val.mag, mag));
		resultMag = trustedStripLeadingZeroInts(resultMag);
		return new BigInteger(resultMag, cmp * signum);
	}

	/**
	 * Subtracts the contents of the second int arrays (little) from the first
	 * (big). The first int array (big) must represent a larger number than the
	 * second. This method allocates the space necessary to hold the answer.
	 */
	private static int[] subtract(int[] big, int[] little)
	{
		int bigIndex = big.length;
		int result[] = new int[bigIndex];
		int littleIndex = little.length;
		long difference = 0;

		// Subtract common parts of both numbers
		while (littleIndex > 0)
		{
			difference = (big[--bigIndex] & LONG_MASK) - (little[--littleIndex] & LONG_MASK) + (difference >> 32);
			result[bigIndex] = (int) difference;
		}

		// Subtract remainder of longer number while borrow propagates
		boolean borrow = (difference >> 32 != 0);
		while (bigIndex > 0 && borrow)
			borrow = ((result[--bigIndex] = big[bigIndex] - 1) == -1);

		// Copy remainder of longer number
		while (bigIndex > 0)
			result[--bigIndex] = big[bigIndex];

		return result;
	}

	/**
	 * Returns a BigInteger whose value is <tt>(this * val)</tt>.
	 * 
	 * @param val
	 *            value to be multiplied by this BigInteger.
	 * @return <tt>this * val</tt>
	 */
	public BigInteger multiply(BigInteger val)
	{
		if (signum == 0 || val.signum == 0)
			return ZERO;

		int[] result = multiplyToLen(mag, mag.length, val.mag, val.mag.length, null);
		result = trustedStripLeadingZeroInts(result);
		return new BigInteger(result, signum * val.signum);
	}

	/**
	 * Multiplies int arrays x and y to the specified lengths and places the
	 * result into z.
	 */
	private int[] multiplyToLen(int[] x, int xlen, int[] y, int ylen, int[] z)
	{
		int xstart = xlen - 1;
		int ystart = ylen - 1;

		if (z == null || z.length < (xlen + ylen))
			z = new int[xlen + ylen];

		long carry = 0;
		for (int j = ystart, k = ystart + 1 + xstart; j >= 0; j--, k--)
		{
			long product = (y[j] & LONG_MASK) * (x[xstart] & LONG_MASK) + carry;
			z[k] = (int) product;
			carry = product >>> 32;
		}
		z[xstart] = (int) carry;

		for (int i = xstart - 1; i >= 0; i--)
		{
			carry = 0;
			for (int j = ystart, k = ystart + 1 + i; j >= 0; j--, k--)
			{
				long product = (y[j] & LONG_MASK) * (x[i] & LONG_MASK) + (z[k] & LONG_MASK) + carry;
				z[k] = (int) product;
				carry = product >>> 32;
			}
			z[i] = (int) carry;
		}
		return z;
	}

	/**
	 * Returns a BigInteger whose value is <tt>(this<sup>2</sup>)</tt>.
	 * 
	 * @return <tt>this<sup>2</sup></tt>
	 */
	private BigInteger square()
	{
		if (signum == 0)
			return ZERO;
		int[] z = squareToLen(mag, mag.length, null);
		return new BigInteger(trustedStripLeadingZeroInts(z), 1);
	}

	/**
	 * Squares the contents of the int array x. The result is placed into the
	 * int array z. The contents of x are not changed.
	 */
	private static int[] squareToLen(int[] x, int len, int[] z)
	{
		/*
		 * The algorithm used here is adapted from Colin Plumb's C library.
		 * Technique: Consider the partial products in the multiplication of
		 * "abcde" by itself:
		 * 
		 * a b c d e * a b c d e ================== ae be ce de ee ad bd cd dd
		 * de ac bc cc cd ce ab bb bc bd be aa ab ac ad ae
		 * 
		 * Note that everything above the main diagonal: ae be ce de = (abcd) *
		 * e ad bd cd = (abc) * d ac bc = (ab) * c ab = (a) * b
		 * 
		 * is a copy of everything below the main diagonal: de cd ce bc bd be ab
		 * ac ad ae
		 * 
		 * Thus, the sum is 2 * (off the diagonal) + diagonal.
		 * 
		 * This is accumulated beginning with the diagonal (which consist of the
		 * squares of the digits of the input), which is then divided by two,
		 * the off-diagonal added, and multiplied by two again. The low bit is
		 * simply a copy of the low bit of the input, so it doesn't need special
		 * care.
		 */
		int zlen = len << 1;
		if (z == null || z.length < zlen)
			z = new int[zlen];

		// Store the squares, right shifted one bit (i.e., divided by 2)
		int lastProductLowWord = 0;
		for (int j = 0, i = 0; j < len; j++)
		{
			long piece = (x[j] & LONG_MASK);
			long product = piece * piece;
			z[i++] = (lastProductLowWord << 31) | (int) (product >>> 33);
			z[i++] = (int) (product >>> 1);
			lastProductLowWord = (int) product;
		}

		// Add in off-diagonal sums
		for (int i = len, offset = 1; i > 0; i--, offset += 2)
		{
			int t = x[i - 1];
			t = mulAdd(z, x, offset, i - 1, t);
			addOne(z, offset - 1, i, t);
		}

		// Shift back up and set low bit
		primitiveLeftShift(z, zlen, 1);
		z[zlen - 1] |= x[len - 1] & 1;

		return z;
	}

	/**
	 * Returns an array of two BigIntegers containing <tt>(this / val)</tt>
	 * followed by <tt>(this % val)</tt>.
	 * 
	 * @param val
	 *            value by which this BigInteger is to be divided, and the
	 *            remainder computed.
	 * @return an array of two BigIntegers: the quotient <tt>(this / val)</tt>
	 *         is the initial element, and the remainder <tt>(this % val)</tt>
	 *         is the final element.
	 * @throws ArithmeticException
	 *             <tt>val==0</tt>
	 */
	public BigInteger[] divideAndRemainder(BigInteger val)
	{
		BigInteger[] result = new BigInteger[2];
		MutableBigInteger q = new MutableBigInteger(), r = new MutableBigInteger(), a = new MutableBigInteger(this.mag), b = new MutableBigInteger(val.mag);
		a.divide(b, q, r);
		result[0] = new BigInteger(q, this.signum * val.signum);
		result[1] = new BigInteger(r, this.signum);
		return result;
	}

	/**
	 * Returns a BigInteger whose value is <tt>(this % val)</tt>.
	 * 
	 * @param val
	 *            value by which this BigInteger is to be divided, and the
	 *            remainder computed.
	 * @return <tt>this % val</tt>
	 * @throws ArithmeticException
	 *             <tt>val==0</tt>
	 */
	private BigInteger remainder(BigInteger val)
	{
		MutableBigInteger q = new MutableBigInteger(), r = new MutableBigInteger(), a = new MutableBigInteger(this.mag), b = new MutableBigInteger(val.mag);

		a.divide(b, q, r);
		return new BigInteger(r, this.signum);
	}

	/**
	 * Left shift int array a up to len by n bits. Returns the array that
	 * results from the shift since space may have to be reallocated.
	 */
	private static int[] leftShift(int[] a, int len, int n)
	{
		int nInts = n >>> 5;
		int nBits = n & 0x1F;
		int bitsInHighWord = bitLen(a[0]);

		// If shift can be done without recopy, do so
		if (n <= (32 - bitsInHighWord))
		{
			primitiveLeftShift(a, len, nBits);
			return a;
		} else
		{ // Array must be resized
			if (nBits <= (32 - bitsInHighWord))
			{
				int result[] = new int[nInts + len];
				for (int i = 0; i < len; i++)
					result[i] = a[i];
				primitiveLeftShift(result, result.length, nBits);
				return result;
			} else
			{
				int result[] = new int[nInts + len + 1];
				for (int i = 0; i < len; i++)
					result[i] = a[i];
				primitiveRightShift(result, result.length, 32 - nBits);
				return result;
			}
		}
	}

	// shifts a up to len right n bits assumes no leading zeros, 0<n<32
	private static void primitiveRightShift(int[] a, int len, int n)
	{
		int n2 = 32 - n;
		for (int i = len - 1, c = a[i]; i > 0; i--)
		{
			int b = c;
			c = a[i - 1];
			a[i] = (c << n2) | (b >>> n);
		}
		a[0] >>>= n;
	}

	// shifts a up to len left n bits assumes no leading zeros, 0<=n<32
	protected static void primitiveLeftShift(int[] a, int len, int n)
	{
		if (len == 0 || n == 0)
			return;

		int n2 = 32 - n;
		for (int i = 0, c = a[i], m = i + len - 1; i < m; i++)
		{
			int b = c;
			c = a[i + 1];
			a[i] = (b << n) | (c >>> n2);
		}
		a[len - 1] <<= n;
	}

	/**
	 * Calculate bitlength of contents of the first len elements an int array,
	 * assuming there are no leading zero ints.
	 */
	private static int bitLength(int[] val, int len)
	{
		if (len == 0)
			return 0;
		return ((len - 1) << 5) + bitLen(val[0]);
	}

	/**
	 * Returns a BigInteger whose value is <tt>(-this)</tt>.
	 * 
	 * @return <tt>-this</tt>
	 */
	private BigInteger negate()
	{
		return new BigInteger(this.mag, -this.signum);
	}

	// Modular Arithmetic Operations

	/**
	 * Returns a BigInteger whose value is <tt>(this mod m</tt>). This method
	 * differs from <tt>remainder</tt> in that it always returns a
	 * <i>non-negative</i> BigInteger.
	 * 
	 * @param m
	 *            the modulus.
	 * @return <tt>this mod m</tt>
	 * @throws ArithmeticException
	 *             <tt>m &lt;= 0</tt>
	 * @see #remainder
	 */
	private BigInteger mod(BigInteger m)
	{
		if (m.signum <= 0)
			throw new ArithmeticException("BigInteger: modulus not positive");

		BigInteger result = this.remainder(m);
		return (result.signum >= 0 ? result : result.add(m));
	}

	/**
	 * Returns a BigInteger whose value is
	 * <tt>(this<sup>exponent</sup> mod m)</tt>. (Unlike <tt>pow</tt>,
	 * this method permits negative exponents.)
	 * 
	 * @param exponent
	 *            the exponent.
	 * @param m
	 *            the modulus.
	 * @return <tt>this<sup>exponent</sup> mod m</tt>
	 * @throws ArithmeticException
	 *             <tt>m &lt;= 0</tt>
	 * @see #modInverse
	 */
	public BigInteger modPow(BigInteger exponent, BigInteger m)
	{
		if (m.signum <= 0)
			throw new ArithmeticException("BigInteger: modulus not positive");

		// Trivial cases
		if (exponent.signum == 0)
			return (m.equals(ONE) ? ZERO : ONE);

		if (this.equals(ONE))
			return (m.equals(ONE) ? ZERO : ONE);

		if (this.equals(ZERO) && exponent.signum >= 0)
			return ZERO;

		if (this.equals(negConst[1]) && (!exponent.testBit(0)))
			return (m.equals(ONE) ? ZERO : ONE);

		boolean invertResult;
		if ((invertResult = (exponent.signum < 0)))
			exponent = exponent.negate();

		BigInteger base = (this.signum < 0 || this.compareTo(m) >= 0 ? this.mod(m) : this);
		BigInteger result;
		if (m.testBit(0))
		{ // odd modulus
			result = base.oddModPow(exponent, m);
		} else
		{
			/*
			 * Even modulus. Tear it into an "odd part" (m1) and power of two
			 * (m2), exponentiate mod m1, manually exponentiate mod m2, and use
			 * Chinese Remainder Theorem to combine results.
			 */

			// Tear m apart into odd part (m1) and power of 2 (m2)
			int p = m.getLowestSetBit(); // Max pow of 2 that divides m

			BigInteger m1 = m.shiftRight(p); // m/2**p
			BigInteger m2 = ONE.shiftLeft(p); // 2**p

			// Calculate new base from m1
			BigInteger base2 = (this.signum < 0 || this.compareTo(m1) >= 0 ? this.mod(m1) : this);

			// Caculate (base ** exponent) mod m1.
			BigInteger a1 = (m1.equals(ONE) ? ZERO : base2.oddModPow(exponent, m1));

			// Calculate (this ** exponent) mod m2
			BigInteger a2 = base.modPow2(exponent, p);

			// Combine results using Chinese Remainder Theorem
			BigInteger y1 = m2.modInverse(m1);
			BigInteger y2 = m1.modInverse(m2);

			result = a1.multiply(m2).multiply(y1).add(a2.multiply(m1).multiply(y2)).mod(m);
		}

		return (invertResult ? result.modInverse(m) : result);
	}

	private static int[] bnExpModThreshTable = { 7, 25, 81, 241, 673, 1793, Integer.MAX_VALUE }; // Sentinel

	/**
	 * Returns a BigInteger whose value is x to the power of y mod z. Assumes: z
	 * is odd && x < z.
	 */
	private BigInteger oddModPow(BigInteger y, BigInteger z)
	{
		// Special case for exponent of one
		if (y.equals(ONE))
			return this;

		// Special case for base of zero
		if (signum == 0)
			return ZERO;

		int[] base = (int[]) mag.clone();
		int[] exp = y.mag;
		int[] mod = z.mag;
		int modLen = mod.length;

		// Select an appropriate window size
		int wbits = 0;
		int ebits = bitLength(exp, exp.length);
		while (ebits > bnExpModThreshTable[wbits])
			wbits++;

		// Calculate appropriate table size
		int tblmask = 1 << wbits;

		// Allocate table for precomputed odd powers of base in Montgomery form
		int[][] table = new int[tblmask][];
		for (int i = 0; i < tblmask; i++)
			table[i] = new int[modLen];

		// Compute the modular inverse
		int inv = -MutableBigInteger.inverseMod32(mod[modLen - 1]);

		// Convert base to Montgomery form
		int[] a = leftShift(base, base.length, modLen << 5);

		MutableBigInteger q = new MutableBigInteger();
		MutableBigInteger r = new MutableBigInteger();
		MutableBigInteger a2 = new MutableBigInteger(a);
		MutableBigInteger b2 = new MutableBigInteger(mod);

		a2.divide(b2, q, r);
		table[0] = r.toIntArray();

		// Pad table[0] with leading zeros so its length is at least modLen
		if (table[0].length < modLen)
		{
			int offset = modLen - table[0].length;
			int[] t2 = new int[modLen];
			for (int i = 0; i < table[0].length; i++)
				t2[i + offset] = table[0][i];
			table[0] = t2;
		}

		// Set b to the square of the base
		int[] b = squareToLen(table[0], modLen, null);
		b = montReduce(b, mod, modLen, inv);

		// Set t to high half of b
		int[] t = new int[modLen];
		for (int i = 0; i < modLen; i++)
			t[i] = b[i];

		// Fill in the table with odd powers of the base
		for (int i = 1; i < tblmask; i++)
		{
			int[] prod = multiplyToLen(t, modLen, table[i - 1], modLen, null);
			table[i] = montReduce(prod, mod, modLen, inv);
		}

		// Pre load the window that slides over the exponent
		int bitpos = 1 << ((ebits - 1) & (32 - 1));

		int buf = 0;
		int elen = exp.length;
		int eIndex = 0;
		for (int i = 0; i <= wbits; i++)
		{
			buf = (buf << 1) | (((exp[eIndex] & bitpos) != 0) ? 1 : 0);
			bitpos >>>= 1;
			if (bitpos == 0)
			{
				eIndex++;
				bitpos = 1 << (32 - 1);
				elen--;
			}
		}

		int multpos = ebits;

		// The first iteration, which is hoisted out of the main loop
		ebits--;
		boolean isone = true;

		multpos = ebits - wbits;
		while ((buf & 1) == 0)
		{
			buf >>>= 1;
			multpos++;
		}

		int[] mult = table[buf >>> 1];

		buf = 0;
		if (multpos == ebits)
			isone = false;

		// The main loop
		while (true)
		{
			ebits--;
			// Advance the window
			buf <<= 1;

			if (elen != 0)
			{
				buf |= ((exp[eIndex] & bitpos) != 0) ? 1 : 0;
				bitpos >>>= 1;
				if (bitpos == 0)
				{
					eIndex++;
					bitpos = 1 << (32 - 1);
					elen--;
				}
			}

			// Examine the window for pending multiplies
			if ((buf & tblmask) != 0)
			{
				multpos = ebits - wbits;
				while ((buf & 1) == 0)
				{
					buf >>>= 1;
					multpos++;
				}
				mult = table[buf >>> 1];
				buf = 0;
			}

			// Perform multiply
			if (ebits == multpos)
			{
				if (isone)
				{
					b = (int[]) mult.clone();
					isone = false;
				} else
				{
					t = b;
					a = multiplyToLen(t, modLen, mult, modLen, a);
					a = montReduce(a, mod, modLen, inv);
					t = a;
					a = b;
					b = t;
				}
			}

			// Check if done
			if (ebits == 0)
				break;

			// Square the input
			if (!isone)
			{
				t = b;
				a = squareToLen(t, modLen, a);
				a = montReduce(a, mod, modLen, inv);
				t = a;
				a = b;
				b = t;
			}
		}

		// Convert result out of Montgomery form and return
		int[] t2 = new int[2 * modLen];
		for (int i = 0; i < modLen; i++)
			t2[i + modLen] = b[i];

		b = montReduce(t2, mod, modLen, inv);

		t2 = new int[modLen];
		for (int i = 0; i < modLen; i++)
			t2[i] = b[i];

		return new BigInteger(1, t2);
	}

	/**
	 * Montgomery reduce n, modulo mod. This reduces modulo mod and divides by
	 * 2^(32*mlen). Adapted from Colin Plumb's C library.
	 */
	private static int[] montReduce(int[] n, int[] mod, int mlen, int inv)
	{
		int c = 0;
		int len = mlen;
		int offset = 0;

		do
		{
			int nEnd = n[n.length - 1 - offset];
			int carry = mulAdd(n, mod, offset, mlen, inv * nEnd);
			c += addOne(n, offset, mlen, carry);
			offset++;
		} while (--len > 0);

		while (c > 0)
			c += subN(n, mod, mlen);

		while (intArrayCmpToLen(n, mod, mlen) >= 0)
			subN(n, mod, mlen);

		return n;
	}

	/*
	 * Returns -1, 0 or +1 as big-endian unsigned int array arg1 is less than,
	 * equal to, or greater than arg2 up to length len.
	 */
	private static int intArrayCmpToLen(int[] arg1, int[] arg2, int len)
	{
		for (int i = 0; i < len; i++)
		{
			long b1 = arg1[i] & LONG_MASK;
			long b2 = arg2[i] & LONG_MASK;
			if (b1 < b2)
				return -1;
			if (b1 > b2)
				return 1;
		}
		return 0;
	}

	/**
	 * Subtracts two numbers of same length, returning borrow.
	 */
	private static int subN(int[] a, int[] b, int len)
	{
		long sum = 0;

		while (--len >= 0)
		{
			sum = (a[len] & LONG_MASK) - (b[len] & LONG_MASK) + (sum >> 32);
			a[len] = (int) sum;
		}

		return (int) (sum >> 32);
	}

	/**
	 * Multiply an array by one word k and add to result, return the carry
	 */
	private static int mulAdd(int[] out, int[] in, int offset, int len, int k)
	{
		long kLong = k & LONG_MASK;
		long carry = 0;

		offset = out.length - offset - 1;
		for (int j = len - 1; j >= 0; j--)
		{
			long product = (in[j] & LONG_MASK) * kLong + (out[offset] & LONG_MASK) + carry;
			out[offset--] = (int) product;
			carry = product >>> 32;
		}
		return (int) carry;
	}

	/**
	 * Add one word to the number a mlen words into a. Return the resulting
	 * carry.
	 */
	private static int addOne(int[] a, int offset, int mlen, int carry)
	{
		offset = a.length - 1 - mlen - offset;
		long t = (a[offset] & LONG_MASK) + (carry & LONG_MASK);

		a[offset] = (int) t;
		if ((t >>> 32) == 0)
			return 0;
		while (--mlen >= 0)
		{
			if (--offset < 0)
			{ // Carry out of number
				return 1;
			} else
			{
				a[offset]++;
				if (a[offset] != 0)
					return 0;
			}
		}
		return 1;
	}

	/**
	 * Returns a BigInteger whose value is (this ** exponent) mod (2**p)
	 */
	private BigInteger modPow2(BigInteger exponent, int p)
	{
		/*
		 * Perform exponentiation using repeated squaring trick, chopping off
		 * high order bits as indicated by modulus.
		 */
		BigInteger result = valueOf(1);
		BigInteger baseToPow2 = this.mod2(p);
		int expOffset = 0;

		int limit = exponent.bitLength();

		if (this.testBit(0))
			limit = (p - 1) < limit ? (p - 1) : limit;

		while (expOffset < limit)
		{
			if (exponent.testBit(expOffset))
				result = result.multiply(baseToPow2).mod2(p);
			expOffset++;
			if (expOffset < limit)
				baseToPow2 = baseToPow2.square().mod2(p);
		}

		return result;
	}

	/**
	 * Returns a BigInteger whose value is this mod(2**p). Assumes that this
	 * BigInteger &gt;= 0 and p &gt; 0.
	 */
	private BigInteger mod2(int p)
	{
		if (bitLength() <= p)
			return this;

		// Copy remaining ints of mag
		int numInts = (p + 31) / 32;
		int[] mag = new int[numInts];
		for (int i = 0; i < numInts; i++)
			mag[i] = this.mag[i + (this.mag.length - numInts)];

		// Mask out any excess bits
		int excessBits = (numInts << 5) - p;
		mag[0] &= (1L << (32 - excessBits)) - 1;

		return (mag[0] == 0 ? new BigInteger(1, mag) : new BigInteger(mag, 1));
	}

	/**
	 * Returns a BigInteger whose value is <tt>(this<sup>-1</sup> mod m)</tt>.
	 * 
	 * @param m
	 *            the modulus.
	 * @return <tt>this<sup>-1</sup> mod m</tt>.
	 * @throws ArithmeticException
	 *             <tt> m &lt;= 0</tt>, or this BigInteger has no
	 *             multiplicative inverse mod m (that is, this BigInteger is not
	 *             <i>relatively prime</i> to m).
	 */
	private BigInteger modInverse(BigInteger m)
	{
		if (m.signum != 1)
			throw new ArithmeticException("BigInteger: modulus not positive");

		if (m.equals(ONE))
			return ZERO;

		// Calculate (this mod m)
		BigInteger modVal = this;
		if (signum < 0 || (intArrayCmp(mag, m.mag) >= 0))
			modVal = this.mod(m);

		if (modVal.equals(ONE))
			return ONE;

		MutableBigInteger a = new MutableBigInteger(modVal);
		MutableBigInteger b = new MutableBigInteger(m);

		MutableBigInteger result = a.mutableModInverse(b);
		return new BigInteger(result, 1);
	}

	// Shift Operations

	/**
	 * Returns a BigInteger whose value is <tt>(this &lt;&lt; n)</tt>. The
	 * shift distance, <tt>n</tt>, may be negative, in which case this method
	 * performs a right shift. (Computes <tt>floor(this * 2<sup>n</sup>)</tt>.)
	 * 
	 * @param n
	 *            shift distance, in bits.
	 * @return <tt>this &lt;&lt; n</tt>
	 * @see #shiftRight
	 */
	private BigInteger shiftLeft(int n)
	{
		if (signum == 0)
			return ZERO;
		if (n == 0)
			return this;
		if (n < 0)
			return shiftRight(-n);

		int nInts = n >>> 5;
		int nBits = n & 0x1f;
		int magLen = mag.length;
		int newMag[] = null;

		if (nBits == 0)
		{
			newMag = new int[magLen + nInts];
			for (int i = 0; i < magLen; i++)
				newMag[i] = mag[i];
		} else
		{
			int i = 0;
			int nBits2 = 32 - nBits;
			int highBits = mag[0] >>> nBits2;
			if (highBits != 0)
			{
				newMag = new int[magLen + nInts + 1];
				newMag[i++] = highBits;
			} else
			{
				newMag = new int[magLen + nInts];
			}
			int j = 0;
			while (j < magLen - 1)
				newMag[i++] = mag[j++] << nBits | mag[j] >>> nBits2;
			newMag[i] = mag[j] << nBits;
		}

		return new BigInteger(newMag, signum);
	}

	/**
	 * Returns a BigInteger whose value is <tt>(this &gt;&gt; n)</tt>. Sign
	 * extension is performed. The shift distance, <tt>n</tt>, may be
	 * negative, in which case this method performs a left shift. (Computes
	 * <tt>floor(this / 2<sup>n</sup>)</tt>.)
	 * 
	 * @param n
	 *            shift distance, in bits.
	 * @return <tt>this &gt;&gt; n</tt>
	 * @see #shiftLeft
	 */
	private BigInteger shiftRight(int n)
	{
		if (n == 0)
			return this;
		if (n < 0)
			return shiftLeft(-n);

		int nInts = n >>> 5;
		int nBits = n & 0x1f;
		int magLen = mag.length;
		int newMag[] = null;

		// Special case: entire contents shifted off the end
		if (nInts >= magLen)
			return (signum >= 0 ? ZERO : negConst[1]);

		if (nBits == 0)
		{
			int newMagLen = magLen - nInts;
			newMag = new int[newMagLen];
			for (int i = 0; i < newMagLen; i++)
				newMag[i] = mag[i];
		}
		else
		{
			int i = 0;
			int highBits = mag[0] >>> nBits;
			if (highBits != 0)
			{
				newMag = new int[magLen - nInts];
				newMag[i++] = highBits;
			}
			else
			{
				newMag = new int[magLen - nInts - 1];
			}

			int nBits2 = 32 - nBits;
			int j = 0;
			while (j < magLen - nInts - 1)
				newMag[i++] = (mag[j++] << nBits2) | (mag[j] >>> nBits);
		}

		if (signum < 0)
		{
			// Find out whether any one-bits were shifted off the end.
			boolean onesLost = false;
			for (int i = magLen - 1, j = magLen - nInts; i >= j && !onesLost; i--)
				onesLost = (mag[i] != 0);
			if (!onesLost && nBits != 0)
				onesLost = (mag[magLen - nInts - 1] << (32 - nBits) != 0);

			if (onesLost)
				newMag = javaIncrement(newMag);
		}

		return new BigInteger(newMag, signum);
	}

	private int[] javaIncrement(int[] val)
	{
		int lastSum = 0;
		for (int i = val.length - 1; i >= 0 && lastSum == 0; i--)
			lastSum = (val[i] += 1);
		if (lastSum == 0)
		{
			val = new int[val.length + 1];
			val[0] = 1;
		}
		return val;
	}

	/**
	 * Returns <tt>true</tt> if and only if the designated bit is set.
	 * (Computes <tt>((this &amp; (1&lt;&lt;n)) != 0)</tt>.)
	 * 
	 * @param n
	 *            index of bit to test.
	 * @return <tt>true</tt> if and only if the designated bit is set.
	 * @throws ArithmeticException
	 *             <tt>n</tt> is negative.
	 */
	private boolean testBit(int n)
	{
		if (n < 0)
			throw new ArithmeticException("Negative bit address");

		return (getInt(n / 32) & (1 << (n % 32))) != 0;
	}

	/**
	 * Returns a BigInteger whose value is equivalent to this BigInteger with
	 * the designated bit set. (Computes <tt>(this | (1&lt;&lt;n))</tt>.)
	 * 
	 * @param n
	 *            index of bit to set.
	 * @return <tt>this | (1&lt;&lt;n)</tt>
	 * @throws ArithmeticException
	 *             <tt>n</tt> is negative.
	 */
	private BigInteger setBit(int n)
	{
		if (n < 0)
			throw new ArithmeticException("Negative bit address");

		int intNum = n / 32;
		int[] result = new int[Math.max(intLength(), intNum + 2)];

		for (int i = 0; i < result.length; i++)
			result[result.length - i - 1] = getInt(i);

		result[result.length - intNum - 1] |= (1 << (n % 32));

		return valueOf(result);
	}

	/**
	 * Returns the index of the rightmost (lowest-order) one bit in this
	 * BigInteger (the number of zero bits to the right of the rightmost one
	 * bit). Returns -1 if this BigInteger contains no one bits. (Computes
	 * <tt>(this==0? -1 : log<sub>2</sub>(this &amp; -this))</tt>.)
	 * 
	 * @return index of the rightmost one bit in this BigInteger.
	 */
	private int getLowestSetBit()
	{
		/*
		 * Initialize lowestSetBit field the first time this method is executed.
		 * This method depends on the atomicity of int modifies; without this
		 * guarantee, it would have to be synchronized.
		 */
		if (lowestSetBit == -2)
		{
			if (signum == 0)
			{
				lowestSetBit = -1;
			} else
			{
				// Search for lowest order nonzero int
				int i, b;
				for (i = 0; (b = getInt(i)) == 0; i++)
					;
				lowestSetBit = (i << 5) + trailingZeroCnt(b);
			}
		}
		return lowestSetBit;
	}

	// Miscellaneous Bit Operations

	/**
	 * Returns the number of bits in the minimal two's-complement representation
	 * of this BigInteger, <i>excluding</i> a sign bit. For positive
	 * BigIntegers, this is equivalent to the number of bits in the ordinary
	 * binary representation. (Computes
	 * <tt>(ceil(log<sub>2</sub>(this &lt; 0 ? -this : this+1)))</tt>.)
	 * 
	 * @return number of bits in the minimal two's-complement representation of
	 *         this BigInteger, <i>excluding</i> a sign bit.
	 */
	private int bitLength()
	{
		/*
		 * Initialize bitLength field the first time this method is executed.
		 * This method depends on the atomicity of int modifies; without this
		 * guarantee, it would have to be synchronized.
		 */
		if (bitLength == -1)
		{
			if (signum == 0)
			{
				bitLength = 0;
			} else
			{
				// Calculate the bit length of the magnitude
				int magBitLength = ((mag.length - 1) << 5) + bitLen(mag[0]);

				if (signum < 0)
				{
					// Check if magnitude is a power of two
					boolean pow2 = (bitCnt(mag[0]) == 1);
					for (int i = 1; i < mag.length && pow2; i++)
						pow2 = (mag[i] == 0);

					bitLength = (pow2 ? magBitLength - 1 : magBitLength);
				} else
				{
					bitLength = magBitLength;
				}
			}
		}
		return bitLength;
	}

	private static int bitCnt(int val)
	{
		val -= (0xaaaaaaaa & val) >>> 1;
		val = (val & 0x33333333) + ((val >>> 2) & 0x33333333);
		val = val + (val >>> 4) & 0x0f0f0f0f;
		val += val >>> 8;
		val += val >>> 16;
		return val & 0xff;
	}

	/**
	 * bitLen(val) is the number of bits in val.
	 */
	protected static int bitLen(int w)
	{
		// Binary search - decision tree (5 tests, rarely 6)
		return (w < 1 << 15 ? (w < 1 << 7 ? (w < 1 << 3 ? (w < 1 << 1 ? (w < 1 << 0 ? (w < 0 ? 32 : 0) : 1) : (w < 1 << 2 ? 2 : 3)) : (w < 1 << 5 ? (w < 1 << 4 ? 4 : 5) : (w < 1 << 6 ? 6 : 7))) : (w < 1 << 11 ? (w < 1 << 9 ? (w < 1 << 8 ? 8 : 9) : (w < 1 << 10 ? 10 : 11)) : (w < 1 << 13 ? (w < 1 << 12 ? 12 : 13) : (w < 1 << 14 ? 14 : 15)))) : (w < 1 << 23 ? (w < 1 << 19 ? (w < 1 << 17 ? (w < 1 << 16 ? 16 : 17) : (w < 1 << 18 ? 18 : 19)) : (w < 1 << 21 ? (w < 1 << 20 ? 20 : 21) : (w < 1 << 22 ? 22 : 23))) : (w < 1 << 27 ? (w < 1 << 25 ? (w < 1 << 24 ? 24 : 25) : (w < 1 << 26 ? 26 : 27)) : (w < 1 << 29 ? (w < 1 << 28 ? 28 : 29) : (w < 1 << 30 ? 30 : 31)))));
	}

	/*
	 * trailingZeroTable[i] is the number of trailing zero bits in the binary
	 * representaion of i.
	 */
	private final static byte trailingZeroTable[] = { -25, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0 };

	protected static int trailingZeroCnt(int val)
	{
		// Loop unrolled for performance
		int byteVal = val & 0xff;
		if (byteVal != 0)
			return trailingZeroTable[byteVal];

		byteVal = (val >>> 8) & 0xff;
		if (byteVal != 0)
			return trailingZeroTable[byteVal] + 8;

		byteVal = (val >>> 16) & 0xff;
		if (byteVal != 0)
			return trailingZeroTable[byteVal] + 16;

		byteVal = (val >>> 24) & 0xff;
		return trailingZeroTable[byteVal] + 24;
	}

	// Comparison Operations

	/**
	 * Compares this BigInteger with the specified BigInteger. This method is
	 * provided in preference to individual methods for each of the six boolean
	 * comparison operators (&lt;, ==, &gt;, &gt;=, !=, &lt;=). The suggested
	 * idiom for performing these comparisons is: <tt>(x.compareTo(y)</tt>
	 * &lt;<i>op</i>&gt; <tt>0)</tt>, where &lt;<i>op</i>&gt; is one of
	 * the six comparison operators.
	 * 
	 * @param val
	 *            BigInteger to which this BigInteger is to be compared.
	 * @return -1, 0 or 1 as this BigInteger is numerically less than, equal to,
	 *         or greater than <tt>val</tt>.
	 */
	public int compareTo(BigInteger val)
	{
		return (signum == val.signum ? signum * intArrayCmp(mag, val.mag) : (signum > val.signum ? 1 : -1));
	}

	/*
	 * Returns -1, 0 or +1 as big-endian unsigned int array arg1 is less than,
	 * equal to, or greater than arg2.
	 */
	private static int intArrayCmp(int[] arg1, int[] arg2)
	{
		if (arg1.length < arg2.length)
			return -1;
		if (arg1.length > arg2.length)
			return 1;

		// Argument lengths are equal; compare the values
		for (int i = 0; i < arg1.length; i++)
		{
			long b1 = arg1[i] & LONG_MASK;
			long b2 = arg2[i] & LONG_MASK;
			if (b1 < b2)
				return -1;
			if (b1 > b2)
				return 1;
		}
		return 0;
	}

	/**
	 * Compares this BigInteger with the specified Object for equality.
	 * 
	 * @param x
	 *            Object to which this BigInteger is to be compared.
	 * @return <tt>true</tt> if and only if the specified Object is a
	 *         BigInteger whose value is numerically equal to this BigInteger.
	 */
	public boolean equals(Object x)
	{
		// This test is just an optimization, which may or may not help
		if (x == this)
			return true;

		if (!(x instanceof BigInteger))
			return false;
		BigInteger xInt = (BigInteger) x;

		if (xInt.signum != signum || xInt.mag.length != mag.length)
			return false;

		for (int i = 0; i < mag.length; i++)
			if (xInt.mag[i] != mag[i])
				return false;

		return true;
	}

	/**
	 * Returns a byte array containing the two's-complement representation of
	 * this BigInteger. The byte array will be in <i>big-endian</i> byte-order:
	 * the most significant byte is in the zeroth element. The array will
	 * contain the minimum number of bytes required to represent this
	 * BigInteger, including at least one sign bit, which is
	 * <tt>(ceil((this.bitLength() +
	 * 1)/8))</tt>. (This representation is
	 * compatible with the {@link #BigInteger(byte[]) (byte[])} constructor.)
	 * 
	 * @return a byte array containing the two's-complement representation of
	 *         this BigInteger.
	 * @see #BigInteger(byte[])
	 */
	public byte[] toByteArray()
	{
		int byteLen = bitLength() / 8 + 1;
		byte[] byteArray = new byte[byteLen];

		for (int i = byteLen - 1, bytesCopied = 4, nextInt = 0, intIndex = 0; i >= 0; i--)
		{
			if (bytesCopied == 4)
			{
				nextInt = getInt(intIndex++);
				bytesCopied = 1;
			} else
			{
				nextInt >>>= 8;
				bytesCopied++;
			}
			byteArray[i] = (byte) nextInt;
		}
		return byteArray;
	}

	/**
	 * Converts this BigInteger to an <code>int</code>. This conversion is
	 * analogous to a <a
	 * href="http://java.sun.com/docs/books/jls/second_edition/html/conversions.doc.html#25363"><i>narrowing
	 * primitive conversion</i></a> from <code>long</code> to
	 * <code>int</code> as defined in the <a
	 * href="http://java.sun.com/docs/books/jls/html/">Java Language
	 * Specification</a>: if this BigInteger is too big to fit in an
	 * <code>int</code>, only the low-order 32 bits are returned. Note that
	 * this conversion can lose information about the overall magnitude of the
	 * BigInteger value as well as return a result with the opposite sign.
	 * 
	 * @return this BigInteger converted to an <code>int</code>.
	 */
	private int intValue()
	{
		int result = 0;
		result = getInt(0);
		return result;
	}

	/**
	 * Returns a copy of the input array stripped of any leading zero bytes.
	 */
	private static int[] stripLeadingZeroInts(int val[])
	{
		int keep;

		// Find first nonzero byte
		for (keep = 0; keep < val.length && val[keep] == 0; keep++)
			;

		int result[] = new int[val.length - keep];
		for (int i = 0; i < val.length - keep; i++)
			result[i] = val[keep + i];

		return result;
	}

	/**
	 * Returns the input array stripped of any leading zero bytes. Since the
	 * source is trusted the copying may be skipped.
	 */
	private static int[] trustedStripLeadingZeroInts(int val[])
	{
		int keep;

		// Find first nonzero byte
		for (keep = 0; keep < val.length && val[keep] == 0; keep++)
			;

		// Only perform copy if necessary
		if (keep > 0)
		{
			int result[] = new int[val.length - keep];
			for (int i = 0; i < val.length - keep; i++)
				result[i] = val[keep + i];
			return result;
		}
		return val;
	}

	/**
	 * Returns a copy of the input array stripped of any leading zero bytes.
	 */
	private static int[] stripLeadingZeroBytes(byte a[])
	{
		int byteLength = a.length;
		int keep;

		// Find first nonzero byte
		for (keep = 0; keep < a.length && a[keep] == 0; keep++)
			;

		// Allocate new array and copy relevant part of input array
		int intLength = ((byteLength - keep) + 3) / 4;
		int[] result = new int[intLength];
		int b = byteLength - 1;
		for (int i = intLength - 1; i >= 0; i--)
		{
			result[i] = a[b--] & 0xff;
			int bytesRemaining = b - keep + 1;
			int bytesToTransfer = Math.min(3, bytesRemaining);
			for (int j = 8; j <= 8 * bytesToTransfer; j += 8)
				result[i] |= ((a[b--] & 0xff) << j);
		}
		return result;
	}

	/**
	 * Takes an array a representing a negative 2's-complement number and
	 * returns the minimal (no leading zero bytes) unsigned whose value is -a.
	 */
	private static int[] makePositive(byte a[])
	{
		int keep, k;
		int byteLength = a.length;

		// Find first non-sign (0xff) byte of input
		for (keep = 0; keep < byteLength && a[keep] == -1; keep++);

		/*
		 * Allocate output array. If all non-sign bytes are 0x00, we must
		 * allocate space for one extra output byte.
		 */
		for (k = keep; k < byteLength && a[k] == 0; k++);

		int extraByte = (k == byteLength) ? 1 : 0;
		int intLength = ((byteLength - keep + extraByte) + 3) / 4;
		int result[] = new int[intLength];

		/*
		 * Copy one's complement of input into into output, leaving extra byte
		 * (if it exists) == 0x00
		 */
		int b = byteLength - 1;
		for (int i = intLength - 1; i >= 0; i--)
		{
			result[i] = a[b--] & 0xff;
			int numBytesToTransfer = Math.min(3, b - keep + 1);
			if (numBytesToTransfer < 0)
				numBytesToTransfer = 0;
			for (int j = 8; j <= 8 * numBytesToTransfer; j += 8)
				result[i] |= ((a[b--] & 0xff) << j);

			// Mask indicates which bits must be complemented
			int mask = -1 >>> (8 * (3 - numBytesToTransfer));
			result[i] = ~result[i] & mask;
		}

		// Add one to one's complement to generate two's complement
		for (int i = result.length - 1; i >= 0; i--)
		{
			result[i] = (int) ((result[i] & LONG_MASK) + 1);
			if (result[i] != 0)
				break;
		}

		return result;
	}

	/**
	 * Takes an array a representing a negative 2's-complement number and
	 * returns the minimal (no leading zero ints) unsigned whose value is -a.
	 */
	private static int[] makePositive(int a[])
	{
		int keep, j;

		// Find first non-sign (0xffffffff) int of input
		for (keep = 0; keep < a.length && a[keep] == -1; keep++);

		/*
		 * Allocate output array. If all non-sign ints are 0x00, we must
		 * allocate space for one extra output int.
		 */
		for (j = keep; j < a.length && a[j] == 0; j++);
		int extraInt = (j == a.length ? 1 : 0);
		int result[] = new int[a.length - keep + extraInt];

		/*
		 * Copy one's complement of input into into output, leaving extra int
		 * (if it exists) == 0x00
		 */
		for (int i = keep; i < a.length; i++)
			result[i - keep + extraInt] = ~a[i];

		// Add one to one's complement to generate two's complement
		for (int i = result.length - 1; ++result[i] == 0; i--)
			;

		return result;
	}

	/**
	 * These routines provide access to the two's complement representation of
	 * BigIntegers.
	 */

	/**
	 * Returns the length of the two's complement representation in ints,
	 * including space for at least one sign bit.
	 */
	private int intLength()
	{
		return bitLength() / 32 + 1;
	}

	/* Returns an int of sign bits */
	private int signInt()
	{
		return (int) (signum < 0 ? -1 : 0);
	}

	/**
	 * Returns the specified int of the little-endian two's complement
	 * representation (int 0 is the least significant). The int number can be
	 * arbitrarily high (values are logically preceded by infinitely many sign
	 * ints).
	 */
	private int getInt(int n)
	{
		if (n < 0)
			return 0;
		if (n >= mag.length)
			return signInt();

		int magInt = mag[mag.length - n - 1];

		return (int) (signum >= 0 ? magInt : (n <= firstNonzeroIntNum() ? -magInt : ~magInt));
	}

	/**
	 * Returns the index of the int that contains the first nonzero int in the
	 * little-endian binary representation of the magnitude (int 0 is the least
	 * significant). If the magnitude is zero, return value is undefined.
	 */
	private int firstNonzeroIntNum()
	{
		/*
		 * Initialize firstNonzeroIntNum field the first time this method is
		 * executed. This method depends on the atomicity of int modifies;
		 * without this guarantee, it would have to be synchronized.
		 */
		if (firstNonzeroIntNum == -2)
		{
			// Search for the first nonzero int
			int i;
			for (i = mag.length - 1; i >= 0 && mag[i] == 0; i--)
				;
			firstNonzeroIntNum = mag.length - i - 1;
		}
		return firstNonzeroIntNum;
	}
}

class BitSieve
{
	/**
	 * Stores the bits in this bitSieve.
	 */
	private long bits[];

	/**
	 * Length is how many bits this sieve holds.
	 */
	private int length;

	/**
	 * A small sieve used to filter out multiples of small primes in a search
	 * sieve.
	 */
	private static BitSieve smallSieve = new BitSieve();

	/**
	 * Construct a "small sieve" with a base of 0. This constructor is used
	 * internally to generate the set of "small primes" whose multiples are
	 * excluded from sieves generated by the main (package private) constructor,
	 * BitSieve(BigInteger base, int searchLen). The length of the sieve
	 * generated by this constructor was chosen for performance; it controls a
	 * tradeoff between how much time is spent constructing other sieves, and
	 * how much time is wasted testing composite candidates for primality. The
	 * length was chosen experimentally to yield good performance.
	 */
	private BitSieve()
	{
		length = 150 * 64;
		bits = new long[(unitIndex(length - 1) + 1)];

		// Mark 1 as composite
		set(0);
		int nextIndex = 1;
		int nextPrime = 3;

		// Find primes and remove their multiples from sieve
		do
		{
			sieveSingle(length, nextIndex + nextPrime, nextPrime);
			nextIndex = sieveSearch(length, nextIndex + 1);
			nextPrime = 2 * nextIndex + 1;
		} while ((nextIndex > 0) && (nextPrime < length));
	}

	/**
	 * Construct a bit sieve of searchLen bits used for finding prime number
	 * candidates. The new sieve begins at the specified base, which must be
	 * even.
	 */
	BitSieve(BigInteger base, int searchLen)
	{
		/*
		 * Candidates are indicated by clear bits in the sieve. As a candidates
		 * nonprimality is calculated, a bit is set in the sieve to eliminate
		 * it. To reduce storage space and increase efficiency, no even numbers
		 * are represented in the sieve (each bit in the sieve represents an odd
		 * number).
		 */
		bits = new long[(unitIndex(searchLen - 1) + 1)];
		length = searchLen;
		int start = 0;

		int step = smallSieve.sieveSearch(smallSieve.length, start);
		int convertedStep = (step * 2) + 1;

		// Construct the large sieve at an even offset specified by base
		MutableBigInteger r = new MutableBigInteger();
		MutableBigInteger q = new MutableBigInteger();
		do
		{
			// Calculate base mod convertedStep
			r.copyValue(base.mag);
			r.divideOneWord(convertedStep, q);
			start = r.value[r.offset];

			// Take each multiple of step out of sieve
			start = convertedStep - start;
			if (start % 2 == 0)
				start += convertedStep;
			sieveSingle(searchLen, (start - 1) / 2, convertedStep);

			// Find next prime from small sieve
			step = smallSieve.sieveSearch(smallSieve.length, step + 1);
			convertedStep = (step * 2) + 1;
		} while (step > 0);
	}

	/**
	 * Given a bit index return unit index containing it.
	 */
	private static int unitIndex(int bitIndex)
	{
		return bitIndex >>> 6;
	}

	/**
	 * Return a unit that masks the specified bit in its unit.
	 */
	private static long bit(int bitIndex)
	{
		return 1L << (bitIndex & ((1 << 6) - 1));
	}

	/**
	 * Get the value of the bit at the specified index.
	 */
	private boolean get(int bitIndex)
	{
		int unitIndex = unitIndex(bitIndex);
		return ((bits[unitIndex] & bit(bitIndex)) != 0);
	}

	/**
	 * Set the bit at the specified index.
	 */
	private void set(int bitIndex)
	{
		int unitIndex = unitIndex(bitIndex);
		bits[unitIndex] |= bit(bitIndex);
	}

	/**
	 * This method returns the index of the first clear bit in the search array
	 * that occurs at or after start. It will not search past the specified
	 * limit. It returns -1 if there is no such clear bit.
	 */
	private int sieveSearch(int limit, int start)
	{
		if (start >= limit)
			return -1;

		int index = start;
		do
		{
			if (!get(index))
				return index;
			index++;
		} while (index < limit - 1);
		return -1;
	}

	/**
	 * Sieve a single set of multiples out of the sieve. Begin to remove
	 * multiples of the specified step starting at the specified start index, up
	 * to the specified limit.
	 */
	private void sieveSingle(int limit, int start, int step)
	{
		while (start < limit)
		{
			set(start);
			start += step;
		}
	}

	/**
	 * Test probable primes in the sieve and return successful candidates.
	 */
	BigInteger retrieve(BigInteger initValue, int certainty)
	{
		// Examine the sieve one long at a time to find possible primes
		int offset = 1;
		for (int i = 0; i < bits.length; i++)
		{
			long nextLong = ~bits[i];
			for (int j = 0; j < 64; j++)
			{
				if ((nextLong & 1) == 1)
				{
					BigInteger candidate = initValue.add(BigInteger.valueOf(offset));
					if (candidate.primeToCertainty(certainty))
						return candidate;
				}
				nextLong >>>= 1;
				offset += 2;
			}
		}
		return null;
	}
}

class MutableBigInteger
{
	/**
	 * Holds the magnitude of this MutableBigInteger in big endian order. The
	 * magnitude may start at an offset into the value array, and it may end
	 * before the length of the value array.
	 */
	int[] value;

	/**
	 * The number of ints of the value array that are currently used to hold the
	 * magnitude of this MutableBigInteger. The magnitude starts at an offset
	 * and offset + intLen may be less than value.length.
	 */
	int intLen;

	/**
	 * The offset into the value array where the magnitude of this
	 * MutableBigInteger begins.
	 */
	int offset = 0;

	/**
	 * This mask is used to obtain the value of an int as if it were unsigned.
	 */
	private final static long LONG_MASK = 0xffffffffL;

	// Constructors

	/**
	 * The default constructor. An empty MutableBigInteger is created with a one
	 * word capacity.
	 */
	protected MutableBigInteger()
	{
		value = new int[1];
		intLen = 0;
	}

	/**
	 * Construct a new MutableBigInteger with a magnitude specified by the int
	 * val.
	 */
	protected MutableBigInteger(int val)
	{
		value = new int[1];
		intLen = 1;
		value[0] = val;
	}

	/**
	 * Construct a new MutableBigInteger with the specified value array up to
	 * the length of the array supplied.
	 */
	protected MutableBigInteger(int[] val)
	{
		value = val;
		intLen = val.length;
	}

	/**
	 * Construct a new MutableBigInteger with a magnitude equal to the specified
	 * BigInteger.
	 */
	protected MutableBigInteger(BigInteger b)
	{
		value = (int[]) b.mag.clone();
		intLen = value.length;
	}

	/**
	 * Construct a new MutableBigInteger with a magnitude equal to the specified
	 * MutableBigInteger.
	 */
	protected MutableBigInteger(MutableBigInteger val)
	{
		intLen = val.intLen;
		value = new int[intLen];

		for (int i = 0; i < intLen; i++)
			value[i] = val.value[val.offset + i];
	}

	/**
	 * Clear out a MutableBigInteger for reuse.
	 */
	private void clear()
	{
		offset = intLen = 0;
		for (int index = 0, n = value.length; index < n; index++)
			value[index] = 0;
	}

	/**
	 * Set a MutableBigInteger to zero, removing its offset.
	 */
	private void reset()
	{
		offset = intLen = 0;
	}

	/**
	 * Compare the magnitude of two MutableBigIntegers. Returns -1, 0 or 1 as
	 * this MutableBigInteger is numerically less than, equal to, or greater
	 * than <tt>b</tt>.
	 */
	private int compare(MutableBigInteger b)
	{
		if (intLen < b.intLen)
			return -1;
		if (intLen > b.intLen)
			return 1;

		for (int i = 0; i < intLen; i++)
		{
			int b1 = value[offset + i] + 0x80000000;
			int b2 = b.value[b.offset + i] + 0x80000000;
			if (b1 < b2)
				return -1;
			if (b1 > b2)
				return 1;
		}
		return 0;
	}

	/**
	 * Return the index of the lowest set bit in this MutableBigInteger. If the
	 * magnitude of this MutableBigInteger is zero, -1 is returned.
	 */
	private int getLowestSetBit()
	{
		if (intLen == 0)
			return -1;
		int j, b;
		for (j = intLen - 1; (j > 0) && (value[j + offset] == 0); j--)
			;
		b = value[j + offset];
		if (b == 0)
			return -1;
		return ((intLen - 1 - j) << 5) + BigInteger.trailingZeroCnt(b);
	}

	/**
	 * Ensure that the MutableBigInteger is in normal form, specifically making
	 * sure that there are no leading zeros, and that if the magnitude is zero,
	 * then intLen is zero.
	 */
	private void normalize()
	{
		if (intLen == 0)
		{
			offset = 0;
			return;
		}

		int index = offset;
		if (value[index] != 0)
			return;

		int indexBound = index + intLen;
		do
		{
			index++;
		} while (index < indexBound && value[index] == 0);

		int numZeros = index - offset;
		intLen -= numZeros;
		offset = (intLen == 0 ? 0 : offset + numZeros);
	}

	/**
	 * Convert this MutableBigInteger into an int array with no leading zeros,
	 * of a length that is equal to this MutableBigInteger's intLen.
	 */
	protected int[] toIntArray()
	{
		int[] result = new int[intLen];
		for (int i = 0; i < intLen; i++)
			result[i] = value[offset + i];
		return result;
	}

	/**
	 * Sets this MutableBigInteger's value array to the specified array. The
	 * intLen is set to the specified length.
	 */
	private void setValue(int[] val, int length)
	{
		value = val;
		intLen = length;
		offset = 0;
	}

	/**
	 * Sets this MutableBigInteger's value array to a copy of the specified
	 * array. The intLen is set to the length of the new array.
	 */
	private void copyValue(MutableBigInteger val)
	{
		int len = val.intLen;
		if (value.length < len)
			value = new int[len];

		for (int i = 0; i < len; i++)
			value[i] = val.value[val.offset + i];
		intLen = len;
		offset = 0;
	}

	/**
	 * Sets this MutableBigInteger's value array to a copy of the specified
	 * array. The intLen is set to the length of the specified array.
	 */
	protected void copyValue(int[] val)
	{
		int len = val.length;
		if (value.length < len)
			value = new int[len];
		for (int i = 0; i < len; i++)
			value[i] = val[i];
		intLen = len;
		offset = 0;
	}

	/**
	 * Returns true iff this MutableBigInteger has a value of one.
	 */
	private boolean isOne()
	{
		return (intLen == 1) && (value[offset] == 1);
	}

	/**
	 * Returns true iff this MutableBigInteger has a value of zero.
	 */
	private boolean isZero()
	{
		return (intLen == 0);
	}

	/**
	 * Returns true iff this MutableBigInteger is even.
	 */
	private boolean isEven()
	{
		return (intLen == 0) || ((value[offset + intLen - 1] & 1) == 0);
	}

	/**
	 * Returns true iff this MutableBigInteger is odd.
	 */
	private boolean isOdd()
	{
		return ((value[offset + intLen - 1] & 1) == 1);
	}

	/**
	 * Right shift this MutableBigInteger n bits. The MutableBigInteger is left
	 * in normal form.
	 */
	private void rightShift(int n)
	{
		if (intLen == 0)
			return;
		int nInts = n >>> 5;
		int nBits = n & 0x1F;
		this.intLen -= nInts;
		if (nBits == 0)
			return;
		int bitsInHighWord = BigInteger.bitLen(value[offset]);
		if (nBits >= bitsInHighWord)
		{
			this.primitiveLeftShift(32 - nBits);
			this.intLen--;
		} else
		{
			primitiveRightShift(nBits);
		}
	}

	/**
	 * Left shift this MutableBigInteger n bits.
	 */
	protected void leftShift(int n)
	{
		/*
		 * If there is enough storage space in this MutableBigInteger already
		 * the available space will be used. Space to the right of the used ints
		 * in the value array is faster to utilize, so the extra space will be
		 * taken from the right if possible.
		 */
		if (intLen == 0)
			return;
		int nInts = n >>> 5;
		int nBits = n & 0x1F;
		int bitsInHighWord = BigInteger.bitLen(value[offset]);

		// If shift can be done without moving words, do so
		if (n <= (32 - bitsInHighWord))
		{
			primitiveLeftShift(nBits);
			return;
		}

		int newLen = intLen + nInts + 1;
		if (nBits <= (32 - bitsInHighWord))
			newLen--;
		if (value.length < newLen)
		{
			// The array must grow
			int[] result = new int[newLen];
			for (int i = 0; i < intLen; i++)
				result[i] = value[offset + i];
			setValue(result, newLen);
		} else if (value.length - offset >= newLen)
		{
			// Use space on right
			for (int i = 0; i < newLen - intLen; i++)
				value[offset + intLen + i] = 0;
		} else
		{
			// Must use space on left
			for (int i = 0; i < intLen; i++)
				value[i] = value[offset + i];
			for (int i = intLen; i < newLen; i++)
				value[i] = 0;
			offset = 0;
		}
		intLen = newLen;
		if (nBits == 0)
			return;
		if (nBits <= (32 - bitsInHighWord))
			primitiveLeftShift(nBits);
		else
			primitiveRightShift(32 - nBits);
	}

	/**
	 * A primitive used for division. This method adds in one multiple of the
	 * divisor a back to the dividend result at a specified offset. It is used
	 * when qhat was estimated too large, and must be adjusted.
	 */
	private int divadd(int[] a, int[] result, int offset)
	{
		long carry = 0;

		for (int j = a.length - 1; j >= 0; j--)
		{
			long sum = (a[j] & LONG_MASK) + (result[j + offset] & LONG_MASK) + carry;
			result[j + offset] = (int) sum;
			carry = sum >>> 32;
		}
		return (int) carry;
	}

	/**
	 * This method is used for division. It multiplies an n word input a by one
	 * word input x, and subtracts the n word product from q. This is needed
	 * when subtracting qhat*divisor from dividend.
	 */
	private int mulsub(int[] q, int[] a, int x, int len, int offset)
	{
		long xLong = x & LONG_MASK;
		long carry = 0;
		offset += len;

		for (int j = len - 1; j >= 0; j--)
		{
			long product = (a[j] & LONG_MASK) * xLong + carry;
			long difference = q[offset] - product;
			q[offset--] = (int) difference;
			carry = (product >>> 32) + (((difference & LONG_MASK) > (((~(int) product) & LONG_MASK))) ? 1 : 0);
		}
		return (int) carry;
	}

	/**
	 * Right shift this MutableBigInteger n bits, where n is less than 32.
	 * Assumes that intLen > 0, n > 0 for speed
	 */
	private void primitiveRightShift(int n)
	{
		int[] val = value;
		int n2 = 32 - n;
		for (int i = offset + intLen - 1, c = val[i]; i > offset; i--)
		{
			int b = c;
			c = val[i - 1];
			val[i] = (c << n2) | (b >>> n);
		}
		val[offset] >>>= n;
	}

	/**
	 * Left shift this MutableBigInteger n bits, where n is less than 32.
	 * Assumes that intLen > 0, n > 0 for speed
	 */
	private void primitiveLeftShift(int n)
	{
		int[] val = value;
		int n2 = 32 - n;
		for (int i = offset, c = val[i], m = i + intLen - 1; i < m; i++)
		{
			int b = c;
			c = val[i + 1];
			val[i] = (b << n) | (c >>> n2);
		}
		val[offset + intLen - 1] <<= n;
	}

	/**
	 * Adds the contents of two MutableBigInteger objects.The result is placed
	 * within this MutableBigInteger. The contents of the addend are not
	 * changed.
	 */
	protected void add(MutableBigInteger addend)
	{
		int x = intLen;
		int y = addend.intLen;
		int resultLen = (intLen > addend.intLen ? intLen : addend.intLen);
		int[] result = (value.length < resultLen ? new int[resultLen] : value);

		int rstart = result.length - 1;
		long sum = 0;

		// Add common parts of both numbers
		while (x > 0 && y > 0)
		{
			x--;
			y--;
			sum = (value[x + offset] & LONG_MASK) + (addend.value[y + addend.offset] & LONG_MASK) + (sum >>> 32);
			result[rstart--] = (int) sum;
		}

		// Add remainder of the longer number
		while (x > 0)
		{
			x--;
			sum = (value[x + offset] & LONG_MASK) + (sum >>> 32);
			result[rstart--] = (int) sum;
		}
		while (y > 0)
		{
			y--;
			sum = (addend.value[y + addend.offset] & LONG_MASK) + (sum >>> 32);
			result[rstart--] = (int) sum;
		}

		if ((sum >>> 32) > 0)
		{ // Result must grow in length
			resultLen++;
			if (result.length < resultLen)
			{
				int temp[] = new int[resultLen];
				for (int i = resultLen - 1; i > 0; i--)
					temp[i] = result[i - 1];
				temp[0] = 1;
				result = temp;
			} else
			{
				result[rstart--] = 1;
			}
		}

		value = result;
		intLen = resultLen;
		offset = result.length - resultLen;
	}

	/**
	 * Subtracts the smaller of this and b from the larger and places the result
	 * into this MutableBigInteger.
	 */
	protected int subtract(MutableBigInteger b)
	{
		MutableBigInteger a = this;

		int[] result = value;
		int sign = a.compare(b);

		if (sign == 0)
		{
			reset();
			return 0;
		}
		if (sign < 0)
		{
			MutableBigInteger tmp = a;
			a = b;
			b = tmp;
		}

		int resultLen = a.intLen;
		if (result.length < resultLen)
			result = new int[resultLen];

		long diff = 0;
		int x = a.intLen;
		int y = b.intLen;
		int rstart = result.length - 1;

		// Subtract common parts of both numbers
		while (y > 0)
		{
			x--;
			y--;

			diff = (a.value[x + a.offset] & LONG_MASK) - (b.value[y + b.offset] & LONG_MASK) - ((int) -(diff >> 32));
			result[rstart--] = (int) diff;
		}
		// Subtract remainder of longer number
		while (x > 0)
		{
			x--;
			diff = (a.value[x + a.offset] & LONG_MASK) - ((int) -(diff >> 32));
			result[rstart--] = (int) diff;
		}

		value = result;
		intLen = resultLen;
		offset = value.length - resultLen;
		normalize();
		return sign;
	}

	/**
	 * Multiply the contents of two MutableBigInteger objects. The result is
	 * placed into MutableBigInteger z. The contents of y are not changed.
	 */
	private void multiply(MutableBigInteger y, MutableBigInteger z)
	{
		int xLen = intLen;
		int yLen = y.intLen;
		int newLen = xLen + yLen;

		// Put z into an appropriate state to receive product
		if (z.value.length < newLen)
			z.value = new int[newLen];
		z.offset = 0;
		z.intLen = newLen;

		// The first iteration is hoisted out of the loop to avoid extra add
		long carry = 0;
		for (int j = yLen - 1, k = yLen + xLen - 1; j >= 0; j--, k--)
		{
			long product = (y.value[j + y.offset] & LONG_MASK) * (value[xLen - 1 + offset] & LONG_MASK) + carry;
			z.value[k] = (int) product;
			carry = product >>> 32;
		}
		z.value[xLen - 1] = (int) carry;

		// Perform the multiplication word by word
		for (int i = xLen - 2; i >= 0; i--)
		{
			carry = 0;
			for (int j = yLen - 1, k = yLen + i; j >= 0; j--, k--)
			{
				long product = (y.value[j + y.offset] & LONG_MASK) * (value[i + offset] & LONG_MASK) + (z.value[k] & LONG_MASK) + carry;
				z.value[k] = (int) product;
				carry = product >>> 32;
			}
			z.value[i] = (int) carry;
		}

		// Remove leading zeros from product
		z.normalize();
	}

	/**
	 * Multiply the contents of this MutableBigInteger by the word y. The result
	 * is placed into z.
	 */
	private void mul(int y, MutableBigInteger z)
	{
		if (y == 1)
		{
			z.copyValue(this);
			return;
		}

		if (y == 0)
		{
			z.clear();
			return;
		}

		// Perform the multiplication word by word
		long ylong = y & LONG_MASK;
		int[] zval = (z.value.length < intLen + 1 ? new int[intLen + 1] : z.value);
		long carry = 0;
		for (int i = intLen - 1; i >= 0; i--)
		{
			long product = ylong * (value[i + offset] & LONG_MASK) + carry;
			zval[i + 1] = (int) product;
			carry = product >>> 32;
		}

		if (carry == 0)
		{
			z.offset = 1;
			z.intLen = intLen;
		} else
		{
			z.offset = 0;
			z.intLen = intLen + 1;
			zval[0] = (int) carry;
		}
		z.value = zval;
	}

	/**
	 * This method is used for division of an n word dividend by a one word
	 * divisor. The quotient is placed into quotient. The one word divisor is
	 * specified by divisor. The value of this MutableBigInteger is the dividend
	 * at invocation but is replaced by the remainder.
	 * 
	 * NOTE: The value of this MutableBigInteger is modified by this method.
	 */
	protected void divideOneWord(int divisor, MutableBigInteger quotient)
	{
		long divLong = divisor & LONG_MASK;

		// Special case of one word dividend
		if (intLen == 1)
		{
			long remValue = value[offset] & LONG_MASK;
			quotient.value[0] = (int) (remValue / divLong);
			quotient.intLen = (quotient.value[0] == 0) ? 0 : 1;
			quotient.offset = 0;

			value[0] = (int) (remValue - (quotient.value[0] * divLong));
			offset = 0;
			intLen = (value[0] == 0) ? 0 : 1;

			return;
		}

		if (quotient.value.length < intLen)
			quotient.value = new int[intLen];
		quotient.offset = 0;
		quotient.intLen = intLen;

		// Normalize the divisor
		int shift = 32 - BigInteger.bitLen(divisor);

		int rem = value[offset];
		long remLong = rem & LONG_MASK;
		if (remLong < divLong)
		{
			quotient.value[0] = 0;
		} else
		{
			quotient.value[0] = (int) (remLong / divLong);
			rem = (int) (remLong - (quotient.value[0] * divLong));
			remLong = rem & LONG_MASK;
		}

		int xlen = intLen;
		int[] qWord = new int[2];
		while (--xlen > 0)
		{
			long dividendEstimate = (remLong << 32) | (value[offset + intLen - xlen] & LONG_MASK);
			if (dividendEstimate >= 0)
			{
				qWord[0] = (int) (dividendEstimate / divLong);
				qWord[1] = (int) (dividendEstimate - (qWord[0] * divLong));
			} else
			{
				divWord(qWord, dividendEstimate, divisor);
			}
			quotient.value[intLen - xlen] = (int) qWord[0];
			rem = (int) qWord[1];
			remLong = rem & LONG_MASK;
		}

		// Unnormalize
		if (shift > 0)
			value[0] = rem %= divisor;
		else
			value[0] = rem;
		intLen = (value[0] == 0) ? 0 : 1;

		quotient.normalize();
	}

	/**
	 * Calculates the quotient and remainder of this div b and places them in
	 * the MutableBigInteger objects provided.
	 * 
	 * Uses Algorithm D in Knuth section 4.3.1. Many optimizations to that
	 * algorithm have been adapted from the Colin Plumb C library. It special
	 * cases one word divisors for speed. The contents of a and b are not
	 * changed.
	 * 
	 */
	protected void divide(MutableBigInteger b, MutableBigInteger quotient, MutableBigInteger rem)
	{
		if (b.intLen == 0)
			throw new ArithmeticException("BigInteger divide by zero");

		// Dividend is zero
		if (intLen == 0)
		{
			quotient.intLen = quotient.offset = rem.intLen = rem.offset = 0;
			return;
		}

		int cmp = compare(b);

		// Dividend less than divisor
		if (cmp < 0)
		{
			quotient.intLen = quotient.offset = 0;
			rem.copyValue(this);
			return;
		}
		// Dividend equal to divisor
		if (cmp == 0)
		{
			quotient.value[0] = quotient.intLen = 1;
			quotient.offset = rem.intLen = rem.offset = 0;
			return;
		}

		quotient.clear();

		// Special case one word divisor
		if (b.intLen == 1)
		{
			rem.copyValue(this);
			rem.divideOneWord(b.value[b.offset], quotient);
			return;
		}

		// Copy divisor value to protect divisor
		int[] d = new int[b.intLen];
		for (int i = 0; i < b.intLen; i++)
			d[i] = b.value[b.offset + i];
		int dlen = b.intLen;

		// Remainder starts as dividend with space for a leading zero
		if (rem.value.length < intLen + 1)
			rem.value = new int[intLen + 1];

		for (int i = 0; i < intLen; i++)
			rem.value[i + 1] = value[i + offset];
		rem.intLen = intLen;
		rem.offset = 1;

		int nlen = rem.intLen;

		// Set the quotient size
		int limit = nlen - dlen + 1;
		if (quotient.value.length < limit)
		{
			quotient.value = new int[limit];
			quotient.offset = 0;
		}
		quotient.intLen = limit;
		int[] q = quotient.value;

		// D1 normalize the divisor
		int shift = 32 - BigInteger.bitLen(d[0]);
		if (shift > 0)
		{
			// First shift will not grow array
			BigInteger.primitiveLeftShift(d, dlen, shift);
			// But this one might
			rem.leftShift(shift);
		}

		// Must insert leading 0 in rem if its length did not change
		if (rem.intLen == nlen)
		{
			rem.offset = 0;
			rem.value[0] = 0;
			rem.intLen++;
		}

		int dh = d[0];
		long dhLong = dh & LONG_MASK;
		int dl = d[1];
		int[] qWord = new int[2];

		// D2 Initialize j
		for (int j = 0; j < limit; j++)
		{
			// D3 Calculate qhat
			// estimate qhat
			int qhat = 0;
			int qrem = 0;
			boolean skipCorrection = false;
			int nh = rem.value[j + rem.offset];
			int nh2 = nh + 0x80000000;
			int nm = rem.value[j + 1 + rem.offset];

			if (nh == dh)
			{
				qhat = ~0;
				qrem = nh + nm;
				skipCorrection = qrem + 0x80000000 < nh2;
			} else
			{
				long nChunk = (((long) nh) << 32) | (nm & LONG_MASK);
				if (nChunk >= 0)
				{
					qhat = (int) (nChunk / dhLong);
					qrem = (int) (nChunk - (qhat * dhLong));
				} else
				{
					divWord(qWord, nChunk, dh);
					qhat = qWord[0];
					qrem = qWord[1];
				}
			}

			if (qhat == 0)
				continue;

			if (!skipCorrection)
			{ // Correct qhat
				long nl = rem.value[j + 2 + rem.offset] & LONG_MASK;
				long rs = ((qrem & LONG_MASK) << 32) | nl;
				long estProduct = (dl & LONG_MASK) * (qhat & LONG_MASK);

				if (unsignedLongCompare(estProduct, rs))
				{
					qhat--;
					qrem = (int) ((qrem & LONG_MASK) + dhLong);
					if ((qrem & LONG_MASK) >= dhLong)
					{
						estProduct = (dl & LONG_MASK) * (qhat & LONG_MASK);
						rs = ((qrem & LONG_MASK) << 32) | nl;
						if (unsignedLongCompare(estProduct, rs))
							qhat--;
					}
				}
			}

			// D4 Multiply and subtract
			rem.value[j + rem.offset] = 0;
			int borrow = mulsub(rem.value, d, qhat, dlen, j + rem.offset);

			// D5 Test remainder
			if (borrow + 0x80000000 > nh2)
			{
				// D6 Add back
				divadd(d, rem.value, j + 1 + rem.offset);
				qhat--;
			}

			// Store the quotient digit
			q[j] = qhat;
		} // D7 loop on j

		// D8 Unnormalize
		if (shift > 0)
			rem.rightShift(shift);

		rem.normalize();
		quotient.normalize();
	}

	/**
	 * Compare two longs as if they were unsigned. Returns true iff one is
	 * bigger than two.
	 */
	private boolean unsignedLongCompare(long one, long two)
	{
		return (one + Long.MIN_VALUE) > (two + Long.MIN_VALUE);
	}

	/**
	 * This method divides a long quantity by an int to estimate qhat for two
	 * multi precision numbers. It is used when the signed value of n is less
	 * than zero.
	 */
	private void divWord(int[] result, long n, int d)
	{
		long dLong = d & LONG_MASK;

		if (dLong == 1)
		{
			result[0] = (int) n;
			result[1] = 0;
			return;
		}

		// Approximate the quotient and remainder
		long q = (n >>> 1) / (dLong >>> 1);
		long r = n - q * dLong;

		// Correct the approximation
		while (r < 0)
		{
			r += dLong;
			q--;
		}
		while (r >= dLong)
		{
			r -= dLong;
			q++;
		}

		// n - q*dlong == r && 0 <= r <dLong, hence we're done.
		result[0] = (int) q;
		result[1] = (int) r;
	}

	/**
	 * Returns the modInverse of this mod p. This and p are not affected by the
	 * operation.
	 */
	protected MutableBigInteger mutableModInverse(MutableBigInteger p)
	{
		// Modulus is odd, use Schroeppel's algorithm
		if (p.isOdd())
			return modInverse(p);

		// Base and modulus are even, throw exception
		if (isEven())
			throw new ArithmeticException("BigInteger not invertible.");

		// Get even part of modulus expressed as a power of 2
		int powersOf2 = p.getLowestSetBit();

		// Construct odd part of modulus
		MutableBigInteger oddMod = new MutableBigInteger(p);
		oddMod.rightShift(powersOf2);

		if (oddMod.isOne())
			return modInverseMP2(powersOf2);

		// Calculate 1/a mod oddMod
		MutableBigInteger oddPart = modInverse(oddMod);

		// Calculate 1/a mod evenMod
		MutableBigInteger evenPart = modInverseMP2(powersOf2);

		// Combine the results using Chinese Remainder Theorem
		MutableBigInteger y1 = modInverseBP2(oddMod, powersOf2);
		MutableBigInteger y2 = oddMod.modInverseMP2(powersOf2);

		MutableBigInteger temp1 = new MutableBigInteger();
		MutableBigInteger temp2 = new MutableBigInteger();
		MutableBigInteger result = new MutableBigInteger();

		oddPart.leftShift(powersOf2);
		oddPart.multiply(y1, result);

		evenPart.multiply(oddMod, temp1);
		temp1.multiply(y2, temp2);

		result.add(temp2);
		result.divide(p, temp1, temp2);
		return temp2;
	}

	/*
	 * Calculate the multiplicative inverse of this mod 2^k.
	 */
	private MutableBigInteger modInverseMP2(int k)
	{
		if (isEven())
			throw new ArithmeticException("Non-invertible. (GCD != 1)");

		if (k > 64)
			return euclidModInverse(k);

		int t = inverseMod32(value[offset + intLen - 1]);

		if (k < 33)
		{
			t = (k == 32 ? t : t & ((1 << k) - 1));
			return new MutableBigInteger(t);
		}

		long pLong = (value[offset + intLen - 1] & LONG_MASK);
		if (intLen > 1)
			pLong |= ((long) value[offset + intLen - 2] << 32);
		long tLong = t & LONG_MASK;
		tLong = tLong * (2 - pLong * tLong); // 1 more Newton iter step
		tLong = (k == 64 ? tLong : tLong & ((1L << k) - 1));

		MutableBigInteger result = new MutableBigInteger(new int[2]);
		result.value[0] = (int) (tLong >>> 32);
		result.value[1] = (int) tLong;
		result.intLen = 2;
		result.normalize();
		return result;
	}

	/*
	 * Returns the multiplicative inverse of val mod 2^32. Assumes val is odd.
	 */
	protected static int inverseMod32(int val)
	{
		// Newton's iteration!
		int t = val;
		t *= 2 - val * t;
		t *= 2 - val * t;
		t *= 2 - val * t;
		t *= 2 - val * t;
		return t;
	}

	/*
	 * Calculate the multiplicative inverse of 2^k mod mod, where mod is odd.
	 */
	private static MutableBigInteger modInverseBP2(MutableBigInteger mod, int k)
	{
		// Copy the mod to protect original
		return fixup(new MutableBigInteger(1), new MutableBigInteger(mod), k);
	}

	/**
	 * Calculate the multiplicative inverse of this mod mod, where mod is odd.
	 * This and mod are not changed by the calculation.
	 * 
	 * This method implements an algorithm due to Richard Schroeppel, that uses
	 * the same intermediate representation as Montgomery Reduction ("Montgomery
	 * Form"). The algorithm is described in an unpublished manuscript entitled
	 * "Fast Modular Reciprocals."
	 */
	private MutableBigInteger modInverse(MutableBigInteger mod)
	{
		MutableBigInteger p = new MutableBigInteger(mod);
		MutableBigInteger f = new MutableBigInteger(this);
		MutableBigInteger g = new MutableBigInteger(p);
		SignedMutableBigInteger c = new SignedMutableBigInteger(1);
		SignedMutableBigInteger d = new SignedMutableBigInteger();
		MutableBigInteger temp = null;
		SignedMutableBigInteger sTemp = null;

		int k = 0;
		// Right shift f k times until odd, left shift d k times
		if (f.isEven())
		{
			int trailingZeros = f.getLowestSetBit();
			f.rightShift(trailingZeros);
			d.leftShift(trailingZeros);
			k = trailingZeros;
		}

		// The Almost Inverse Algorithm
		while (!f.isOne())
		{
			// If gcd(f, g) != 1, number is not invertible modulo mod
			if (f.isZero())
				throw new ArithmeticException("BigInteger not invertible.");

			// If f < g exchange f, g and c, d
			if (f.compare(g) < 0)
			{
				temp = f;
				f = g;
				g = temp;
				sTemp = d;
				d = c;
				c = sTemp;
			}

			// If f == g (mod 4)
			if (((f.value[f.offset + f.intLen - 1] ^ g.value[g.offset + g.intLen - 1]) & 3) == 0)
			{
				f.subtract(g);
				c.signedSubtract(d);
			} else
			{ // If f != g (mod 4)
				f.add(g);
				c.signedAdd(d);
			}

			// Right shift f k times until odd, left shift d k times
			int trailingZeros = f.getLowestSetBit();
			f.rightShift(trailingZeros);
			d.leftShift(trailingZeros);
			k += trailingZeros;
		}

		while (c.sign < 0)
			c.signedAdd(p);

		return fixup(c, p, k);
	}

	/*
	 * The Fixup Algorithm Calculates X such that X = C * 2^(-k) (mod P) Assumes
	 * C<P and P is odd.
	 */
	private static MutableBigInteger fixup(MutableBigInteger c, MutableBigInteger p, int k)
	{
		MutableBigInteger temp = new MutableBigInteger();
		// Set r to the multiplicative inverse of p mod 2^32
		int r = -inverseMod32(p.value[p.offset + p.intLen - 1]);

		for (int i = 0, numWords = k >> 5; i < numWords; i++)
		{
			// V = R * c (mod 2^j)
			int v = r * c.value[c.offset + c.intLen - 1];
			// c = c + (v * p)
			p.mul(v, temp);
			c.add(temp);
			// c = c / 2^j
			c.intLen--;
		}
		int numBits = k & 0x1f;
		if (numBits != 0)
		{
			// V = R * c (mod 2^j)
			int v = r * c.value[c.offset + c.intLen - 1];
			v &= ((1 << numBits) - 1);
			// c = c + (v * p)
			p.mul(v, temp);
			c.add(temp);
			// c = c / 2^j
			c.rightShift(numBits);
		}

		// In theory, c may be greater than p at this point (Very rare!)
		while (c.compare(p) >= 0)
			c.subtract(p);

		return c;
	}

	/**
	 * Uses the extended Euclidean algorithm to compute the modInverse of base
	 * mod a modulus that is a power of 2. The modulus is 2^k.
	 */
	private MutableBigInteger euclidModInverse(int k)
	{
		MutableBigInteger b = new MutableBigInteger(1);
		b.leftShift(k);
		MutableBigInteger mod = new MutableBigInteger(b);

		MutableBigInteger a = new MutableBigInteger(this);
		MutableBigInteger q = new MutableBigInteger();
		MutableBigInteger r = new MutableBigInteger();

		b.divide(a, q, r);
		MutableBigInteger swapper = b;
		b = r;
		r = swapper;

		MutableBigInteger t1 = new MutableBigInteger(q);
		MutableBigInteger t0 = new MutableBigInteger(1);
		MutableBigInteger temp = new MutableBigInteger();

		while (!b.isOne())
		{
			a.divide(b, q, r);

			if (r.intLen == 0)
				throw new ArithmeticException("BigInteger not invertible.");

			swapper = r;
			r = a;
			a = swapper;

			if (q.intLen == 1)
				t1.mul(q.value[q.offset], temp);
			else
				q.multiply(t1, temp);
			swapper = q;
			q = temp;
			temp = swapper;

			t0.add(q);

			if (a.isOne())
				return t0;

			b.divide(a, q, r);

			if (r.intLen == 0)
				throw new ArithmeticException("BigInteger not invertible.");

			swapper = b;
			b = r;
			r = swapper;

			if (q.intLen == 1)
				t0.mul(q.value[q.offset], temp);
			else
				q.multiply(t0, temp);
			swapper = q;
			q = temp;
			temp = swapper;

			t1.add(q);
		}
		mod.subtract(t1);
		return mod;
	}

}

class SignedMutableBigInteger extends MutableBigInteger
{

	/**
	 * The sign of this MutableBigInteger.
	 */
	int sign = 1;

	// Constructors

	/**
	 * The default constructor. An empty MutableBigInteger is created with a one
	 * word capacity.
	 */
	SignedMutableBigInteger()
	{
		super();
	}

	/**
	 * Construct a new MutableBigInteger with a magnitude specified by the int
	 * val.
	 */
	SignedMutableBigInteger(int val)
	{
		super(val);
	}

	/**
	 * Construct a new MutableBigInteger with a magnitude equal to the specified
	 * MutableBigInteger.
	 */
	SignedMutableBigInteger(MutableBigInteger val)
	{
		super(val);
	}

	/**
	 * Signed addition built upon unsigned add and subtract.
	 */
	void signedAdd(MutableBigInteger addend)
	{
		if (sign == 1)
			add(addend);
		else
			sign = sign * subtract(addend);

	}

	/**
	 * Signed subtraction built upon unsigned add and subtract.
	 */
	void signedSubtract(SignedMutableBigInteger addend)
	{
		if (sign == addend.sign)
			sign = sign * subtract(addend);
		else
			add(addend);

	}
}
