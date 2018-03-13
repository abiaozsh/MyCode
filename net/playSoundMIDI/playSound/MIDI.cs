using System;
using System.Collections.Generic;
using System.Text;

namespace playSound
{
	class MIDI
	{
		public class Event
		{
			public int time;
			public int dely;
			public int channel;
			public int op;
			public int key;
			public string data;

		}

		public int ff;
		public int nn;
		public int dd;

		public List<byte[]> tracks = new List<byte[]>();


		public List<Event> events = new List<Event>();
		private static string X(int n)
		{
			return "0x" + n.ToString("X") + " ";
		}
		private static string X(byte n)
		{
			if (n < 0x10)
			{
				return "0x0" + n.ToString("X") + " ";
			}
			else
			{
				return "0x" + n.ToString("X") + " ";
			}
		}
		private static MIDI.Event getEvent(int time, int channel, string data)
		{
			MIDI.Event cevent = new MIDI.Event();
			cevent.time = time;
			cevent.channel = channel;
			cevent.data = data;
			return cevent;
		}
		private static MIDI.Event getEvent(int time, int channel, string data, int op, int key)
		{
			MIDI.Event cevent = new MIDI.Event();
			cevent.time = time;
			cevent.channel = channel;
			cevent.data = data;
			cevent.op = op;
			cevent.key = key;
			return cevent;
		}

		private static int sort(MIDI.Event e1, MIDI.Event e2)
		{
			if (e1.time - e2.time == 0)
			{
				return e1.channel - e2.channel;
			}
			return e1.time - e2.time;
		}

		public static MIDI processMIDI(string filename)
		{
			MIDI midi = new MIDI();

			System.IO.FileStream fs = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);

			byte[] buff = new byte[256 * 1024];

			fs.Read(buff, 0, (int)fs.Length);

			fs.Close();

			{
				midi.ff = (buff[8] << 8) + buff[9];
				midi.nn = (buff[10] << 8) + buff[11];
				midi.dd = (buff[12] << 8) + buff[13];
				int idx = 14;
				for (int i = 0; i < midi.nn; i++)
				{
					int size = (int)(buff[idx + 4] << 24) + (buff[idx + 5] << 16) + (buff[idx + 6] << 8) + (buff[idx + 7]);

					byte[] track = new byte[size];
					System.Array.Copy(buff, idx + 8, track, 0, size);
					midi.tracks.Add(track);
					idx += size + 8;
				}
			}

			try
			{
				int channel = 0;
				foreach (byte[] track in midi.tracks)
				{
					channel++;
					int idx = 0;

					int totaltime = 0;

					try
					{
						while (true)
						{
							if (idx == track.Length)
							{
								break;
							}

							int deltaTime = track[idx++];

							if (deltaTime > 127)
							{
								int tempdeltaTime = track[idx++];

								if (tempdeltaTime > 127)
								{
									int tempdeltaTime1 = track[idx++];
									if (tempdeltaTime1 > 127)
									{
										throw new Exception();
									}
									else
									{
										deltaTime = ((deltaTime & 0x7F) << 14) + ((tempdeltaTime & 0x7F) << 7) + tempdeltaTime1;
									}
								}
								else
								{
									deltaTime = ((deltaTime & 0x7F) << 7) + tempdeltaTime;
								}
							}

							totaltime += deltaTime;

							byte byteStatus = track[idx++];

							if (byteStatus == 0xFF)
							{
								#region FF 其他格式
								byte byteType = track[idx++];

								if (byteType == 0x03)//歌曲标题 音轨名称
								{
									byte byteLen = track[idx++];

									if (byteLen > 127)
									{
										throw new Exception();
									}
									string str = Encoding.GetEncoding("GB2312").GetString(track, idx, byteLen);
									idx += byteLen;

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + str));
								}
								else if (byteType == 0x04)//乐器名称
								{
									byte byteLen = track[idx++];

									if (byteLen > 127)
									{
										throw new Exception();
									}
									string str = Encoding.GetEncoding("GB2312").GetString(track, idx, byteLen);
									idx += byteLen;

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + str));
								}
								else if (byteType == 0x51)//速度 
								{
									byte byteLen = track[idx++];

									if (byteLen != 0x03)
									{
										throw new Exception();
									}

									byte tt1 = track[idx++];
									byte tt2 = track[idx++];
									byte tt3 = track[idx++];

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(tt1) + X(tt2) + X(tt3)));
									//TODO 3字节整数 1个四分音符的微秒数
								}
								else if (byteType == 0x58)//节拍
								{
									byte byteLen = track[idx++];

									if (byteLen != 0x04)
									{
										throw new Exception();
									}

									byte nn = track[idx++];
									byte dd = track[idx++];
									byte cc = track[idx++];
									byte bb = track[idx++];

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(nn) + X(dd) + X(cc) + X(bb)));
									//TODO 分子 分母 节拍器时钟 一个四分音符包含的三十二分音符的个数
								}
								else if (byteType == 0x59)//调号
								{
									byte byteLen = track[idx++];

									if (byteLen != 0x02)
									{
										throw new Exception();
									}

									byte sf = track[idx++];//升降号数 -7~0~7
									byte mf = track[idx++];//大调小调

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(sf) + X(mf)));
									//TODO 
								}
								else if (byteType == 0x2F)//音轨结束标志
								{
									byte byteLen = track[idx++];

									if (byteLen != 0x00)
									{
										throw new Exception();
									}

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType)));
									//TODO
								}
								else if (byteType == 0x20)//MIDI Channel 前缀 关联紧跟的 meta-events  和 sysex-events 的 MIDI channel。直到下一个<midi-event>（必须包含MIDI channel 信息）cc MIDI channel 1-16 范围： 00-0F
								{
									byte temp1 = track[idx++];
									byte temp2 = track[idx++];
									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(temp1) + "\t" + X(temp2)));
								}
								else if (byteType == 0x21)//?
								{
									byte byteLen = track[idx++];
									if (byteLen != 0x01)
									{
										throw new Exception();
									}

									byte sf = track[idx++];
									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(byteType) + "\t" + X(sf)));
								}
								else
								{
									throw new Exception();
								}
								#endregion
							}
							else
							{
								if ((byteStatus & 0xF0) == 0x80)
								{
									//ノートオフ
									byte cn = track[idx++];
									byte sp = track[idx++];

									midi.events.Add(getEvent(totaltime, channel, "", -1, cn));
								}
								else if ((byteStatus & 0xF0) == 0x90)
								{
									//ノートオン
									byte cn = track[idx++];
									byte sp = track[idx++];

									midi.events.Add(getEvent(totaltime, channel, "", 1, cn));
								}
								else if ((byteStatus & 0xF0) == 0xA0)
								{
									throw new Exception();
								}
								else if ((byteStatus & 0xF0) == 0xB0)
								{
									//コントロールチェンジ
									byte cn = track[idx++];
									byte nn = track[idx++];

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(cn) + X(nn)));
								}
								else if ((byteStatus & 0xF0) == 0xC0)
								{
									//プログラムチェンジ
									byte cn = track[idx++];

									midi.events.Add(getEvent(totaltime, channel, X(byteStatus) + "\t" + X(cn)));
								}
								else if ((byteStatus & 0xF0) == 0xD0)
								{
									throw new Exception();
								}
								else if ((byteStatus & 0xF0) == 0xE0)
								{
									throw new Exception();
								}
								else if (byteStatus == 0xF0)
								{
									byte data;
									while ((data = track[idx++]) != 0xF7) { }
									//系统码
								}
								else
								{
									throw new Exception();
								}
							}
						}
					}
					catch (Exception ex)
					{
						//throw ex;
					}
				}
			}
			catch (Exception ex)
			{
				//MessageBox.Show("error");
			}


			//foreach (MIDI.Event cevent in midi.events)
			//{
			//    if (cevent.op == -1)
			//    {
			//        cevent.time += 100;
			//    }
			//}
			midi.events.Sort(sort);
			int oldtime = 0;
			foreach (MIDI.Event cevent in midi.events)
			{
				cevent.dely = cevent.time - oldtime;
				oldtime = cevent.time;
			}

			return midi;
		}
	}
}
