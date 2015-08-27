using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.DirectX;
using Microsoft.DirectX.DirectSound;
using System.IO;
using System.Threading;
using System.Configuration;

namespace dsound2
{
	public class SoundRecord
	{
		public const int cNotifyNum = 16;       //缓冲队列的数目
		private int mNextCuptureOffset = 0;     //该次录音缓冲去的起始点
		private int mSampleCount = 0;           //录制的样本数目
		private int mNotifySize = 0;            //每次通知大小
		private int mBufferSize = 0;            //缓冲队列大小
		private string mFileName = @"c:/";          //文件名
		private FileStream mWaveFile = null;   //文件流
		private BinaryWriter mWriter = null;   //写文件
		private Capture mCapDev = null;         //音频捕捉设备
		private CaptureBuffer mRecBuffer = null;//缓冲区对象
		private Notify mNotify = null;          //消息通知对象
		private WaveFormat mWavFormat;        //录音的格式
		private Thread mNotifyThread = null;   //处理缓冲区消息的线程
		private AutoResetEvent mNotificationEvent = null;//通知事件

		public SoundRecord()
		{
			//初始化音频捕捉设备
			InitCaptureDevice();
			//设定录音格式
			mWavFormat = CreateWaveFormat();
		}

		//设定录音结束后保存的文件，包括路径
		public void setFileName(string filename)
		{
			mFileName = filename;
		}

		//开始录音
		public void RecStart()
		{
			//创建录音文件
			CreateSoundFile();
			//创建录音缓冲区，并开始录音
			CreateCaptureBuffer();
			//建立通知消息，当缓冲区满的时候处理方法
			InitNotifiCaions();
			mRecBuffer.Start(true);
		}

		//停止录音
		public void RecStop()
		{
			//关闭通知消息
			try
			{
				if (mNotificationEvent != null)
				{
					mNotificationEvent.Set();
				}
			}
			catch (Exception)
			{
				throw;
			}
			//停止录音
			mRecBuffer.Stop();
			//写入缓冲区最后的数据
			//RecordCapturedData();
			//回写长度信息
			mWriter.Seek(4, SeekOrigin.Begin);
			mWriter.Write((int)(mSampleCount + 36));//写文件长度
			mWriter.Seek(40, SeekOrigin.Begin);
			mWriter.Write(mSampleCount);           //写数据长度
			mWriter.Close();
			mWaveFile.Close();
			mWriter = null;
			mWaveFile = null;
		}

		//初始化录音设备，出处
		private bool InitCaptureDevice()
		{
			CaptureDevicesCollection devices = new CaptureDevicesCollection();   //枚举音频捕捉设备
			Guid deviceGuid = Guid.Empty;                                         //音频捕捉设备的ID

			if (devices.Count > 0)
			{
				deviceGuid = devices[0].DriverGuid;
			}
			else
			{
				Console.WriteLine("系统中没有音频捕捉设备");
				return false;
			}

			//用指定的捕捉设备创建Capture对象
			try
			{
				mCapDev = new Capture(deviceGuid);
			}
			catch (DirectXException e)
			{
				Console.WriteLine(e.Message);
				return false;
			}
			return true;
		}

		//创建录音格式，此处使用16bit，16KHz，Mono的录音格式
		private WaveFormat CreateWaveFormat()
		{
			WaveFormat format = new WaveFormat();
			format.FormatTag = WaveFormatTag.Pcm;     //PCM
			format.SamplesPerSecond = 44100;           //16KHz
			format.BitsPerSample = 16;                  //16bit
			format.Channels = 1;                      //Mono
			format.BlockAlign = (short)(format.Channels * (format.BitsPerSample / 8));
			format.AverageBytesPerSecond = format.BlockAlign * format.SamplesPerSecond;

			return format;
		}

		//创建录音使用的缓冲区
		private void CreateCaptureBuffer()
		{
			//缓冲区的描述对象
			CaptureBufferDescription bufferdescription = new CaptureBufferDescription();
			if (null != mNotify)
			{
				mNotify.Dispose();
				mNotify = null;
			}
			if (null != mRecBuffer)
			{
				mRecBuffer.Dispose();
				mRecBuffer = null;
			}

			//设置通知的大小，默认为1秒钟
			mNotifySize = (1024 > mWavFormat.AverageBytesPerSecond / 8 ? 1024 : (mWavFormat.AverageBytesPerSecond / 8));
			mNotifySize -= mNotifySize % mWavFormat.BlockAlign;

			//设定缓冲区大小
			mBufferSize = mNotifySize * cNotifyNum;
			//创建缓冲区描述
			bufferdescription.BufferBytes = mBufferSize;
			bufferdescription.Format = mWavFormat;      //录音格式

			//创建缓冲区
			mRecBuffer = new CaptureBuffer(bufferdescription, mCapDev);
			mNextCuptureOffset = 0;

		}

		//初始化通知事件，将原缓冲区分成16个缓冲队列，在每个缓冲队列的结束点设定通知点
		private bool InitNotifiCaions()
		{
			if (null == mRecBuffer)
			{
				Console.WriteLine("为创建录音缓冲区");
				return false;
			}

			//创建一个通知事件，当缓冲队列满了就激发该事件
			mNotificationEvent = new AutoResetEvent(false);
			//创建一个线程管理缓冲区事件
			if (null == mNotifyThread)
			{
				mNotifyThread = new Thread(new ThreadStart(WaitThread));
				mNotifyThread.Start();
			}
			//设定通知的位置
			BufferPositionNotify[] PositionNotify = new BufferPositionNotify[cNotifyNum + 1];
			for (int i = 0; i < cNotifyNum; i++)
			{
				PositionNotify[i].Offset = (mNotifySize * i) + mNotifySize - 1;
				PositionNotify[i].EventNotifyHandle = mNotificationEvent.Handle;
			}
			mNotify = new Notify(mRecBuffer);
			mNotify.SetNotificationPositions(PositionNotify, cNotifyNum);
			return true;
		}
		//将录制的数据写入wav文件
		private void RecordCapturedData()
		{
			byte[] CaptureData = null;
			int ReadPos;
			int CapturePos;
			int LockSize;

			mRecBuffer.GetCurrentPosition(out  CapturePos, out ReadPos);
			LockSize = ReadPos - mNextCuptureOffset;
			if (LockSize < 0)
				LockSize += mBufferSize;

			LockSize -= (LockSize % mNotifySize);

			if (0 == LockSize)
			{
				return;
			}
			//读取缓冲区内的数据
			CaptureData = (byte[])mRecBuffer.Read(mNextCuptureOffset, typeof(byte), LockFlag.None, LockSize);

			//写入Wav文件
			mWriter.Write(CaptureData, 0, CaptureData.Length);
			//更新已经录制的数据长度
			mSampleCount += CaptureData.Length;
			mNextCuptureOffset += CaptureData.Length;
			mNextCuptureOffset %= mBufferSize;

		}

		//接收缓冲区满消息的处理线程
		private void WaitThread()
		{
			while (true)
			{
				//等待缓冲区的通知消息
				mNotificationEvent.WaitOne(Timeout.Infinite, true);
				//录制数据
				RecordCapturedData();
			}
		}
		//创建保存的波形文件
		private void CreateSoundFile()
		{
			string filename = @"c:/a.wav";
			mWaveFile = new FileStream(filename, FileMode.Create);

			mWriter = new BinaryWriter(mWaveFile);

			char[] ChunkRiff = { 'R', 'I', 'F', 'F' };
			char[] ChunkType = { 'W', 'A', 'V', 'E' };
			char[] ChunkFmt = { 'f', 'm', 't', ' ' };
			char[] ChunkData = { 'd', 'a', 't', 'a' };

			short shPad = 1;
			int nFormatChunkLength = 16;
			int nLength = 0;

			short shBytesPerSample = 0;
			if (8 == mWavFormat.BitsPerSample && 1 == mWavFormat.Channels)
				shBytesPerSample = 1;
			else if ((8 == mWavFormat.BitsPerSample && 2 == mWavFormat.Channels) || (16 == mWavFormat.BitsPerSample && 1 == mWavFormat.Channels))
				shBytesPerSample = 2;
			else if (16 == mWavFormat.BitsPerSample && 2 == mWavFormat.Channels)
				shBytesPerSample = 4;


			//RIFF块
			mWriter.Write(ChunkRiff);
			mWriter.Write(nLength);
			mWriter.Write(ChunkType);

			//WAVE块
			mWriter.Write(ChunkFmt);
			mWriter.Write(nFormatChunkLength);
			mWriter.Write(shPad);
			mWriter.Write(mWavFormat.Channels);
			mWriter.Write(mWavFormat.SamplesPerSecond);
			mWriter.Write(mWavFormat.AverageBytesPerSecond);
			mWriter.Write(shBytesPerSample);
			mWriter.Write(mWavFormat.BitsPerSample);

			//数据块
			mWriter.Write(ChunkData);
			mWriter.Write((int)0);

		}

	}
}
