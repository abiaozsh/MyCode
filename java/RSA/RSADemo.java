import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.math.BigInteger;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.interfaces.RSAPrivateCrtKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.RSAPrivateKeySpec;
import java.security.spec.RSAPublicKeySpec;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.sun.org.apache.xerces.internal.impl.dv.util.Base64;

public class RSADemo {

	public static void main(String[] args) throws Exception {

		GenSaveKey();

		String data = "转自：RSA加密算法虽然不分平台，标准都是一样的，但是各个平台的实现方式都不尽相同，下面来我来说说java与.net平台之间该如何进行RSA加密验证，即java端加密-》.net端验证和.net端加密-》java端验证。我对RSA算法也只是停留在应用的层面，所以我就以应用的角度来说说这个过程。首先，我们来看一下java端的私钥和公钥的样子：私钥：注意，如果.net端只是验证，可以只导入NET_PUBLIC_KEY;如果.net端既加密又验证，那么只导入NET_PRIVATE_KEY即可，不用导入NET_PUBLIC_KEY。下面给出完整代码及使用说明：使用说明：1.使用java端的GenerateKeys方法，生成java端和.net端需要的公钥和私钥2.java端调用Sign方法进行加密，调用Verify方法进行验证3..net端调用signData方法进行加密，调用verifySignature方法进行验证.net端c#代码：Rsa 加密标准的制定已经过去了十多年了. 这两天在看rsa 加密的文章,基本上都是在说 .net 与 java 之间的 rsa加密是不能互通的.因为项目有用到,所以花了点时间对rsa加密做了一点点了解,发现,不管是java 还是 C# 都对 rsa 的标准加密进行了实现, 是 对于标准是实现,不能互通就讲不过去了. 今天特意写了一段java 代码试了一下,发现是完全可以的.密钥的描述: C#(.net) 中有三种方式导出密钥,一种是blob,一种是 xml 另一种是参数,其中xml 的方式是把 参数进行了 xml序列化.blob 的形式我没看懂是什么意思,只知道文档上说是给微软的什么api用的,下面给出其参数的形式.";

		String sign = Sign(data);

		System.out.println(sign);

		System.out.println(Verify(data, sign));

	}

	private static void SaveFile(String filename, String val) throws IOException {
		FileOutputStream fos = new FileOutputStream(filename);
		OutputStreamWriter osw = new OutputStreamWriter(fos, "utf-8");
		osw.write(val);
		osw.flush();
		osw.close();
		fos.close();
	}

	private static byte[] removeMSZero(byte[] data) {
		byte[] data1;
		int len = data.length;
		if (data[0] == 0) {
			data1 = new byte[data.length - 1];
			System.arraycopy(data, 1, data1, 0, len - 1);
		} else
			data1 = data;

		return data1;
	}

	private static String privateKeyToNetFormat(byte[] encodedPrivkey) {
		try {
			StringBuffer buff = new StringBuffer(1024);

			PKCS8EncodedKeySpec pvkKeySpec = new PKCS8EncodedKeySpec(encodedPrivkey);
			KeyFactory keyFactory = KeyFactory.getInstance("RSA");
			RSAPrivateCrtKey pvkKey = (RSAPrivateCrtKey) keyFactory.generatePrivate(pvkKeySpec);

			buff.append("<RSAKeyValue>");
			buff.append("<Modulus>" + Base64.encode(removeMSZero(pvkKey.getModulus().toByteArray())) + "</Modulus>");
			buff.append("<Exponent>" + Base64.encode(removeMSZero(pvkKey.getPublicExponent().toByteArray())) + "</Exponent>");
			buff.append("<P>" + Base64.encode(removeMSZero(pvkKey.getPrimeP().toByteArray())) + "</P>");
			buff.append("<Q>" + Base64.encode(removeMSZero(pvkKey.getPrimeQ().toByteArray())) + "</Q>");
			buff.append("<DP>" + Base64.encode(removeMSZero(pvkKey.getPrimeExponentP().toByteArray())) + "</DP>");
			buff.append("<DQ>" + Base64.encode(removeMSZero(pvkKey.getPrimeExponentQ().toByteArray())) + "</DQ>");
			buff.append("<InverseQ>" + Base64.encode(removeMSZero(pvkKey.getCrtCoefficient().toByteArray())) + "</InverseQ>");
			buff.append("<D>" + Base64.encode(removeMSZero(pvkKey.getPrivateExponent().toByteArray())) + "</D>");
			buff.append("</RSAKeyValue>");

			return buff.toString().replaceAll("[ \t\n\r]", "");
		} catch (Exception e) {
			System.err.println(e);
			return null;
		}
	}

	private static String publicKeyToNetFormat(byte[] encodedPrivkey) {
		try {
			StringBuffer buff = new StringBuffer(1024);

			PKCS8EncodedKeySpec pvkKeySpec = new PKCS8EncodedKeySpec(encodedPrivkey);
			KeyFactory keyFactory = KeyFactory.getInstance("RSA");
			RSAPrivateCrtKey pvkKey = (RSAPrivateCrtKey) keyFactory.generatePrivate(pvkKeySpec);
			buff.append("<RSAKeyValue>");
			buff.append("<Modulus>" + Base64.encode(removeMSZero(pvkKey.getModulus().toByteArray())) + "</Modulus>");
			buff.append("<Exponent>" + Base64.encode(removeMSZero(pvkKey.getPublicExponent().toByteArray())) + "</Exponent>");
			buff.append("</RSAKeyValue>");
			return buff.toString().replaceAll("[ \t\n\r]", "");
		} catch (Exception e) {
			System.err.println(e);
			return null;
		}
	}

	public static void GenSaveKey() {
		try {
			KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");
			keyGen.initialize(2048);
			KeyPair keypair = keyGen.genKeyPair();
			PrivateKey privateKey = keypair.getPrivate();

			byte[] privateKeyBytes = privateKey.getEncoded();

			String netPrivateKey = privateKeyToNetFormat(privateKeyBytes);
			SaveFile("private_public_key.xml", netPrivateKey);

			String netPublicKey = publicKeyToNetFormat(privateKeyBytes);
			SaveFile("public_key.xml", netPublicKey);
		} catch (Exception ex1) {
			System.out.println(ex1);
		}
	}

	private static String getNode(Node node, String name) {
		for (int i = 0; i < node.getChildNodes().getLength(); i++) {
			Node child = node.getChildNodes().item(i);
			if (name.equals(child.getNodeName())) {
				return child.getFirstChild().getNodeValue();
			}
		}
		return "";
	}

	public static String Sign(String val) throws Exception {

		File f = new File("private_public_key.xml");
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document doc = builder.parse(f);

		NodeList nl = doc.getChildNodes();
		Node root = nl.item(0);

		if ("RSAKeyValue".equals(root.getNodeName())) {

			BigInteger Modulus = new BigInteger(1, Base64.decode(getNode(root, "Modulus")));
			BigInteger D = new BigInteger(1, Base64.decode(getNode(root, "D")));

			RSAPrivateKeySpec prvKeySpec = new RSAPrivateKeySpec(Modulus, D);
			PrivateKey privateKey = KeyFactory.getInstance("RSA").generatePrivate(prvKeySpec);

			Signature rsa = Signature.getInstance("SHA1withRSA");
			rsa.initSign(privateKey);
			rsa.update(val.getBytes("utf-8"));
			return Base64.encode(rsa.sign());

		}
		return "";

	}

	public static boolean Verify(String val, String sign) throws Exception {

		File f = new File("public_key.xml");
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document doc = builder.parse(f);

		NodeList nl = doc.getChildNodes();
		Node root = nl.item(0);

		if ("RSAKeyValue".equals(root.getNodeName())) {

			BigInteger Modulus = new BigInteger(1, Base64.decode(getNode(root, "Modulus")));
			BigInteger Exponent = new BigInteger(1, Base64.decode(getNode(root, "Exponent")));

			RSAPublicKeySpec keySpec = new RSAPublicKeySpec(Modulus, Exponent);
			PublicKey publicKey = KeyFactory.getInstance("RSA").generatePublic(keySpec);

			Signature rsa = Signature.getInstance("SHA1withRSA");
			rsa.initVerify(publicKey);
			rsa.update(val.getBytes("utf-8"));
			return rsa.verify(Base64.decode(sign));

		}
		return false;
	}

}
