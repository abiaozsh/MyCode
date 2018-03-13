using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX.DirectSound;
using System.Drawing;

namespace playSound
{
	public class Music
	{
		public static Dictionary<string,Music> table = new Dictionary<string,Music>();
		public static string[] data = {
"C1	","32.703",
"C1♯	","34.648",
"D1	","36.708",
"D1♯	","38.891",
"E1	","41.203",
"F1	","43.654",
"F1♯	","46.249",
"G1	","48.999",
"G1♯	","51.913",
"A1	","55.000",
"A1♯	","58.270",
"B1	","61.735",
"C2	","65.406",
"C2♯	","69.296",
"D2	","73.416",
"D2♯	","77.782",
"E2	","82.407",
"F2	","87.307",
"F2♯	","92.499",
"G2	","97.999",
"G2♯	","103.83",
"A2	","110.00",
"A2♯	","116.54",
"B2	","123.47",
"C3	","130.81",
"C3♯	","138.59",
"D3	","146.83",
"D3♯	","155.56",
"E3	","164.81",
"F3	","174.61",
"F3♯	","185.00",
"G3	","196.00",
"G3♯	","207.65",
"A3	","220.00",
"A3♯	","233.08",
"B3	","246.94",
"C4	","261.63",
"C4♯	","277.18",
"D4	","293.66",
"D4♯	","311.13",
"E4	","329.63",
"F4	","349.23",
"F4♯	","369.99",
"G4	","392.00",
"G4♯	","415.30",
"A4	","440.00",
"A4♯	","466.16",
"B4	","493.88",
"C5	","523.25",
"C5♯	","554.37",
"D5	","587.33",
"D5♯	","622.25",
"E5	","659.26",
"F5	","698.46",
"F5♯	","739.99",
"G5	","783.99",
"G5♯	","830.61",
"A5	","880.00",
"A5♯	","932.33",
"B5	","987.77",
"C6	","1046.5",
"C6♯	","1108.7",
"D6	","1174.7",
"D6♯	","1244.5",
"E6	","1318.5",
"F6	","1396.9",
"F6♯	","1480.0",
"G6	","1568.0",
"G6♯	","1661.2",
"A6	","1760.0",
"A6♯	","1864.7",
"B6	","1975.5",
"C7	","2093.0",
"C7♯	","2217.5",
"D7	","2349.3",
"D7♯	","2489.0",
"E7	","2637.0",
"F7	","2793.8",
"F7♯	","2960.0",
"G7	","3136.0",
"G7♯	","3322.4",
"A7	","3520.0",
"A7♯	","3729.3",
"B7	","3951.1"};




		public Button button;
		SecondaryBuffer buffer;

		static int sps = 44100;
		//format.SamplesPerSecond = 22050;
		//format.SamplesPerSecond = 11025;
		static int buffsize = sps * 10 * 2;
		public string name;

		public void init(Device device, double frequency, string name)
		{
			this.name = name;
			WaveFormat format = new WaveFormat();
			format.SamplesPerSecond = sps;
			//format.BitsPerSample = 8;
			format.BitsPerSample = 16;
			format.Channels = 1;
			format.BlockAlign = (short)(format.Channels * (format.BitsPerSample / 8));
			format.AverageBytesPerSecond = format.BlockAlign * format.SamplesPerSecond;
			format.FormatTag = WaveFormatTag.Pcm;

			BufferDescription desc = new BufferDescription();
			desc.BufferBytes = buffsize;
			desc.CanGetCurrentPosition = true;
			desc.ControlPositionNotify = false;
			desc.ControlVolume = false;
			desc.Format = format;
			desc.GlobalFocus = true;
			buffer = new SecondaryBuffer(desc, device);

			initdata(frequency);
		}

		public void Play()
		{
			button.BackColor = Color.Red;
			buffer.Play(0, BufferPlayFlags.Default);
		}

		public void Stop()
		{
			button.BackColor = Color.White;
			buffer.Stop();
			buffer.SetCurrentPosition(0);
		}

		void initdata(double frequency)
		{

			byte[] data = new byte[buffsize];

			double v = frequency * Math.PI * 2 / sps;
			double n = 0;
			double fade = 1;
			for (int i = 0; i < buffsize>>1; i++)
			{
				n = i * v;
				double val = Math.Sin(n) * fade;

				int ival = (int)(30000 * val);

				data[(i << 1) ] = (byte)(ival & 0xFF);
				data[(i << 1) + 1] = (byte)((ival >> 8) & 0xFF);
				fade *= 0.9999;
			}

			buffer.Write(0, data, LockFlag.None);
		}

	}
}
