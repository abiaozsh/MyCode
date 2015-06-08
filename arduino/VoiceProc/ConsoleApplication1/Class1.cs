using System.Text;
using System;
using System.IO;

public class WaveAccess
{

	private byte[] riff;
	private byte[] riffSize;
	private byte[] waveID;
	private byte[] fmtID;
	private byte[] notDefinition;
	private byte[] waveType;
	private byte[] channel;
	private byte[] sample;
	private byte[] send;
	private byte[] blockAjust;
	private byte[] bitNum;
	private byte[] unknown;
	private byte[] dataID;
	private byte[] dataLength;
	short[] data;
	private string longFileName;
	public string LongFileName
	{
		get
		{
			return longFileName;
		}
	}
	public string ShortFileName
	{
		get
		{
			int pos = LongFileName.LastIndexOf("\\");
			return LongFileName.Substring(pos + 1);
		}
	}
	public short[] Data
	{
		get
		{
			return data;
		}
		set
		{
			data = value;
		}
	}
	public string Riff
	{
		get
		{
			return Encoding.Default.GetString(riff);
		}
		set
		{
			riff = Encoding.Default.GetBytes(value);
		}
	}
	public uint RiffSize
	{
		get
		{
			return BitConverter.ToUInt32(riffSize, 0);
		}
		set
		{
			riffSize = BitConverter.GetBytes(value);
		}
	}
	public string WaveID
	{
		get
		{
			return Encoding.Default.GetString(waveID);
		}
		set
		{
			waveID = Encoding.Default.GetBytes(value);
		}
	}
	public	string	 FmtID	{
		get
		{
			return			 Encoding.Default.GetString(fmtID);
		}
		set
		{ 
		fmtID = Encoding.Default.GetBytes(value); }




	}
	public
	int
	 NotDefinition
	{






		get
		{

			return

			 BitConverter.ToInt32(notDefinition,

			0

			);
		}






		set

		{ notDefinition = BitConverter.GetBytes(value); }




	}
	public
	short
	 WaveType
	{






		get
		{

			return

			 BitConverter.ToInt16(waveType,

			0

			);
		}






		set

		{ waveType = BitConverter.GetBytes(value); }




	}
	public
	ushort
	 Channel
	{





		get
		{

			return

			 BitConverter.ToUInt16(channel,

			0

			);
		}






		set

		{ channel = BitConverter.GetBytes(value); }



	}
	public
	uint
	 Sample
	{






		get
		{

			return

			 BitConverter.ToUInt32(sample,

			0

			);
		}






		set

		{ sample = BitConverter.GetBytes(value); }



	}
	public
	uint
	 Send
	{




		get
		{

			return

			 BitConverter.ToUInt32(send,

			0

			);
		}




		set

		{ send = BitConverter.GetBytes(value); }


	}
	public
	ushort
	 BlockAjust
	{




		get
		{

			return

			 BitConverter.ToUInt16(blockAjust,

			0

			);
		}




		set

		{ blockAjust = BitConverter.GetBytes(value); }


	}
	public
	ushort
	 BitNum
	{




		get
		{

			return

			 BitConverter.ToUInt16(bitNum,

			0

			);
		}




		set

		{ bitNum = BitConverter.GetBytes(value); }


	}
	public
	ushort
	 Unknown
	{



		get
		{




			if

			 (unknown ==

			null

			)
			{




				return



				1

				;


			}




			else






				return

				 BitConverter.ToUInt16(unknown,

				0

				);

		}




		set

		{ unknown = BitConverter.GetBytes(value); }

	}
	public
	string
	 DataID
	{




		get
		{

			return

			 Encoding.Default.GetString(dataID);
		}




		set

		{ dataID = Encoding.Default.GetBytes(value); }


	}
	public
	uint
	 DataLength
	{



		get
		{

			return

			 BitConverter.ToUInt32(dataLength,

			0

			);
		}




		set

		{ dataLength = BitConverter.GetBytes(value); }


	}
	public WaveAccess() { }
	public WaveAccess(string filepath)
	{
		try
		{
			riff = new byte[4];
			riffSize = new byte[4];
			waveID = new byte[4];
			fmtID = new byte[4];
			notDefinition = new byte[4];
			waveType = new byte[2];
			channel = new byte[2];
			sample = new byte[4];
			send = new byte[4];
			blockAjust = new byte[2];
			bitNum = new byte[2];
			unknown = new byte[2];
			dataID = new byte[4];
			//52
			dataLength = new byte[4];
			//56 个字节
			longFileName = filepath;
			FileStream fs = new FileStream(filepath, FileMode.Open);
			BinaryReader bread = new BinaryReader(fs);
			riff = bread.ReadBytes(4);
			riffSize = bread.ReadBytes(4);
			waveID = bread.ReadBytes(4);
			fmtID = bread.ReadBytes(4);
			notDefinition = bread.ReadBytes(4);
			waveType = bread.ReadBytes(2);
			channel = bread.ReadBytes(2);
			sample = bread.ReadBytes(4);
			send = bread.ReadBytes(4);
			blockAjust = bread.ReadBytes(2);
			bitNum = bread.ReadBytes(2);
			if (BitConverter.ToUInt32(notDefinition, 0) == 18)
			{
				unknown = bread.ReadBytes(2);
			}
			dataID = bread.ReadBytes(4);
			dataLength = bread.ReadBytes(4);
			uint length = DataLength / 2;
			data = new short[length];
			for (int i = 0; i < length; i++)
			{
				data[i] = bread.ReadInt16();
				//读入2字节有符号整数
			}
			fs.Close();
			bread.Close();
		}
		catch
		 (System.Exception ex)
		{
			Console.Write(ex.Message);
		}
	}

	public short[] GetData(uint begin, uint end)
	{
		if
		 ((end - begin) >= Data.Length)




			return

			 Data;




		else
		{




			uint

			 temp = end - begin +

			1

			;




			short

			[] dataTemp =

			new



			short

			[temp];




			uint

			 j = begin;




			for
			 (
			int
			 i =

			0

			; i < temp; i++)
			{
				dataTemp[i] = Data[j];


				j++;


			}




			return

			 dataTemp;


		}


	}

	public bool bulidWave(string fileName)
	{
		try
		{
			FileInfo fi = new FileInfo(fileName);
			if (fi.Exists) fi.Delete();
			FileStream fs = new FileStream(fileName, FileMode.CreateNew);
			BinaryWriter bwriter = new BinaryWriter(fs);

			//二进制写入243
			bwriter.Seek(0, SeekOrigin.Begin);
			bwriter.Write(Encoding.Default.GetBytes(this.Riff));

			//不可以直接写入string类型的字符串，字符串会有串结束符，比原来的bytes多一个字节
			bwriter.Write(this.RiffSize);
			bwriter.Write(Encoding.Default.GetBytes(this.WaveID));
			bwriter.Write(Encoding.Default.GetBytes(this.FmtID));
			bwriter.Write(this.NotDefinition);
			bwriter.Write(this.WaveType);
			bwriter.Write(this.Channel);
			bwriter.Write(this.Sample);
			bwriter.Write(this.Send);
			bwriter.Write(this.BlockAjust);
			bwriter.Write(this.BitNum);
			//if (this.Unknown != 0)
				bwriter.Write(this.Unknown);
			bwriter.Write(Encoding.Default.GetBytes(this.DataID));
			bwriter.Write(this.DataLength);
			for (int i = 0; i < this.Data.Length; i++)
			{
				bwriter.Write(this.Data[i]);
			}
			bwriter.Flush();
			fs.Close();
			bwriter.Close();
			fi = null;
			return true;
		}
		catch
		 (System.Exception ex)
		{
			Console.Write(ex.Message);
			return false;
		}
	}
}



