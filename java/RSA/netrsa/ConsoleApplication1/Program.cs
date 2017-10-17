using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Security.Cryptography;

namespace ConsoleApplication1
{
	class Program
	{


		private static void SaveFile(string filename, string val)
		{
			FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs, Encoding.UTF8);
			sw.Write(val);
			sw.Flush();
			sw.Close();
			fs.Close();
		}
		private static string ReadFile(string filename)
		{
			string val;
			FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs, Encoding.UTF8);
			val = sr.ReadToEnd();
			sr.Close();
			fs.Close();
			return val;
		}

		//生成公钥私钥对，并保存成文件
		public static void GenSaveKey()
		{
			//生成
			RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(2048);

			//导出参数到xml，true包括私钥
			string xml = rsa.ToXmlString(true);

			//保存到文件 这个文件自己保管好，不要遗失和泄露
			SaveFile("private_public_key.xml", xml);
		}

		//读取保存的公钥私钥对，并导出公钥
		public static void GetPublicKey()
		{
			//读取
			string private_xml = ReadFile("private_public_key.xml");
			RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(2048);
			rsa.FromXmlString(private_xml);

			//导出参数到xml，false不包括私钥
			string xml = rsa.ToXmlString(false);

			//保存到文件 这个文件发送给对方
			SaveFile("public_key.xml", xml);
		}

		//私钥签名
		public static string Sign(string input)
		{
			string private_xml = ReadFile("private_public_key.xml");
			RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(2048);
			rsa.FromXmlString(private_xml);

			byte[] data = Encoding.UTF8.GetBytes(input);
			byte[] output = rsa.SignData(data, "SHA1");
			return Convert.ToBase64String(output);
		}

		//公钥验证
		public static bool Verify(string input, string signedData)
		{
			string public_xml = ReadFile("public_key.xml");
			RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(2048);
			rsa.FromXmlString(public_xml);

			byte[] data = Encoding.UTF8.GetBytes(input);
			byte[] hash = Convert.FromBase64String(signedData);
			return rsa.VerifyData(data, "SHA1", hash);
		}

		static void Main(string[] args)
		{
			GenSaveKey();
			GetPublicKey();

			string data = @"转自：RSA加密算法虽然不分平台，标准都是一样的，但是各个平台的实现方式都不尽相同，下面来我来说说java与.net平台之间该如何进行RSA加密验证，即java端加密-》.net端验证和.net端加密-》java端验证。我对RSA算法也只是停留在应用的层面，所以我就以应用的角度来说说这个过程。首先，我们来看一下java端的私钥和公钥的样子：私钥：注意，如果.net端只是验证，可以只导入NET_PUBLIC_KEY;如果.net端既加密又验证，那么只导入NET_PRIVATE_KEY即可，不用导入NET_PUBLIC_KEY。下面给出完整代码及使用说明：使用说明：1.使用java端的GenerateKeys方法，生成java端和.net端需要的公钥和私钥2.java端调用Sign方法进行加密，调用Verify方法进行验证3..net端调用signData方法进行加密，调用verifySignature方法进行验证.net端c#代码：Rsa 加密标准的制定已经过去了十多年了. 这两天在看rsa 加密的文章,基本上都是在说 .net 与 java 之间的 rsa加密是不能互通的.因为项目有用到,所以花了点时间对rsa加密做了一点点了解,发现,不管是java 还是 C# 都对 rsa 的标准加密进行了实现, 是 对于标准是实现,不能互通就讲不过去了. 今天特意写了一段java 代码试了一下,发现是完全可以的.密钥的描述: C#(.net) 中有三种方式导出密钥,一种是blob,一种是 xml 另一种是参数,其中xml 的方式是把 参数进行了 xml序列化.blob 的形式我没看懂是什么意思,只知道文档上说是给微软的什么api用的,下面给出其参数的形式.";			
			string signedData = Sign(data);
			Console.WriteLine(data);
			Console.WriteLine(signedData);
			Console.WriteLine(Verify(data, signedData));

			Console.ReadLine();
		}
	}
}
