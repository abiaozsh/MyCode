using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.DirectX;
using Microsoft.DirectX.DirectSound;
using System.IO;
using System.Threading;
using System.Configuration;

namespace dsound
{
	public class SoundRecord
	{
		public int[,] spect = new int[512, 512];
		public int curLine = 0;



		public const int cNotifyNum = 16;       //缓冲队列的数目
		private int mNextCuptureOffset = 0;     //该次录音缓冲去的起始点
		private int mNotifySize = 0;            //每次通知大小
		private int mBufferSize = 0;            //缓冲队列大小
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

		//开始录音
		public void RecStart()
		{
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
		}

		//初始化录音设备，出处
		private void InitCaptureDevice()
		{
			CaptureDevicesCollection devices = new CaptureDevicesCollection();   //枚举音频捕捉设备
			Guid deviceGuid = Guid.Empty;                                         //音频捕捉设备的ID

			deviceGuid = devices[0].DriverGuid;

			//用指定的捕捉设备创建Capture对象
			mCapDev = new Capture(deviceGuid);
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

			//设置通知的大小
			mNotifySize = (4096);

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
		short[] buff = new short[65536 * 2];
		int buffidx = 0;
		int procidx = 0;
		int numbers = 1024;
		int totalcount = 0;

		//将录制的数据写入wav文件
		private void RecordCapturedData()
		{
			byte[] CaptureData = null;
			int ReadPos;
			int CapturePos;
			int LockSize;

			mRecBuffer.GetCurrentPosition(out CapturePos, out ReadPos);
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
			//Console.WriteLine(CaptureData.Length);
			//TODO mWriter.Write(CaptureData, 0, CaptureData.Length);
			if ((CaptureData.Length & 2) != 0)
			{
				Console.WriteLine("error");
			}

			for (int i = 0; i < CaptureData.Length / 2; i++)
			{
				short data = CaptureData[i * 2];
				data |= (short)(CaptureData[i * 2 + 1] << 8);
				buff[buffidx++] = data;
			}
			if (buffidx - procidx >= numbers)
			{
				proc(buff, procidx, numbers);
				Console.WriteLine("proc:" + totalcount++);
				procidx += numbers;
			}
			procidx &= 0x0FFFF;
			buffidx &= 0x0FFFF;

			mNextCuptureOffset += CaptureData.Length;
			//更新已经录制的数据长度
			mNextCuptureOffset %= mBufferSize;

		}


		public void proc(short[] array, int start, int n)
		{
			int[] a = new int[n];

			for (int i = 0; i < n; i++)
			{
				a[i] = array[start + i];//最接近整数
			}

			Complex[] A = new Complex[n];
			Complex w = fft.ww(n, 1);
			fft.FFT(n, a, w, A);
			float[] result = new float[n];

			//float max = 0.1f;
			for (int i = 1; i < n / 2; i++)
			{
				float v = Math.Abs(A[i].real());
				result[i] = v;//最接近整数
				//if (v > max)
				//{
				//	max = v;
				//}
			}
			//max = 1110.1f;
			{
				for (int i = 1; i < 256; i++)
				{
					if (i >= array.Length)
					{
						break;
					}
					float j = result[i] / n / 8;// /256

					spect[curLine, i] = (int)(j * 255);
				}
			}

			curLine++;
			if (curLine == 512)
			{
				curLine = 0;
			}
		}

		public void proc1(short[] array, int n)
		{
			long[] real = new long[n];

			for (int i = 0; i < n; i++)
			{
				real[i] = array[i];
			}

			FastFFT.fft(real, n);

			int[] result = new int[n];

			for (int i = 1; i < n; i++)
			{
				int v = (int)real[i];
				if (v < 0)
				{
					v = -v;
				}
				result[i] = v;
			}
			{
				for (int i = 2; i < 256; i++)
				{
					if (i >= array.Length)
					{
						break;
					}
					int j = (result[i] / n);

					spect[curLine, i] = (int)(j);
				}
			}

			curLine++;
			if (curLine == 512)
			{
				curLine = 0;
			}
		}

	}
}


/*
 */