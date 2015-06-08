using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.Text.RegularExpressions;
using System.Speech.Synthesis;
using System.Speech.AudioFormat;

namespace ConsoleApplication1
{
	struct file
	{
		public string name;
		public string cont;
	}
	class Program
	{
		static FileStream fs9;
		static StreamWriter sw9;

		static int countall = 0;

		//string head = "[ASCII 44100Hz, Channels: 1, Samples: 11637504, Flags: 1]                     ";
		static void Main(string[] args)
		{
			fs9 = new FileStream(@"e:\data.txt", FileMode.Create, FileAccess.Write);
			sw9 = new StreamWriter(fs9);


			Proc("零", "D0");
			Proc("伊", "D1");
			Proc("二", "D2");
			Proc("三", "D3");
			Proc("四", "D4");
			Proc("五", "D5");
			Proc("六", "D6");
			Proc("七", "D7");
			Proc("八", "D8");
			Proc("九", "D9");
			Proc("腰", "W1yao");
			Proc("十", "W2shi");
			Proc("百", "W3bai");
			Proc("千", "W4qian");
			Proc("万", "W5wan");
			Proc("点", "W6dian");
			Proc("负", "W7fu");
			Proc("分", "W8fen");
			Proc("秒", "W9miao");

			sw9.WriteLine("//" + countall);

			sw9.Flush();
			fs9.Close();
			//Proc(@"e:\sample.txt", @"e:\sample out.txt", @"e:\sample outCompact.txt", @"e:\sample outMid.txt");
		}

		public static void Proc(string word, string name)
		{
			SpeechSynthesizer synth = new SpeechSynthesizer();
			SpeechAudioFormatInfo fmt = new SpeechAudioFormatInfo(44100, AudioBitsPerSample.Sixteen, AudioChannel.Mono);
			synth.SelectVoiceByHints(VoiceGender.Male, VoiceAge.Adult);
			synth.SetOutputToWaveFile(@"d:\temp.wav", fmt);
			synth.Speak(word);
			synth.Dispose();

			WaveAccess w = new WaveAccess(@"d:\temp.wav");

			int samples = w.Data.Length;

			short[] data = w.Data;

			//低通滤波
			int filter = 5;
			short[] data3 = new short[samples];
			for (int i = filter; i < samples; i++)
			{
				int sum = 0;
				for (int j = 0; j < filter; j++)
				{
					sum += data[i - j];
				}
				data3[i] = (short)(sum / filter);
			}
			data = data3;

			//微分
			short[] data2 = new short[samples];
			for (int i = 1; i < samples; i++)
			{
				data2[i] = (short)(data[i] - data[i - 1]);
			}
			data = data2;

			for (int i = 0; i < samples; i++)
			{
				if (data[i] > 0)
				{
					data[i] = 1000;
				}
				else
				{
					data[i] = 0;
				}
			}
			//22us/sample;
			int[] datax = new int[samples];

			int dataidx = 0;
			int olddata = 0;
			int count = 0;
			for (int i = 0; i < samples; i++)
			{
				count++;
				if (data[i] != olddata)
				{
					datax[dataidx] = count;
					dataidx++;
					olddata = data[i];
					count = 0;
				}
			}

			sw9.Write("int len_" + name + " = " + dataidx + ";");
			sw9.Write("PROGMEM prog_uint8_t " + name + "[]={");
			int maxdata = 0;
			for (int i = 0; i < dataidx; i++)
			{
				//sw1.Write(datax[i] + ",");
				sw9.Write(datax[i] + ",");
				if (datax[i] > maxdata)
				{
					maxdata = datax[i];
				}
			}
			//sw1.WriteLine("};");
			sw9.WriteLine("};");
			//sw1.Flush();
			//fs3.Close();
			countall += dataidx;

			short flg = 0;
			int datayidx = 0;
			short[] datay = new short[samples];
			for (int i = 0; i < dataidx; i++)
			{
				for (int j = 0; j < datax[i]; j++)
				{
					datay[datayidx++] = flg;
				}
				if (flg == 0)
				{
					flg = 10000;
				}
				else
				{
					flg = 0;
				}
			}

			w.Data = datay;
			w.bulidWave("d:\\out_" + name + ".wav");

		}
	}
}
