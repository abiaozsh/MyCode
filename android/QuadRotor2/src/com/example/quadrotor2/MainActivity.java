package com.example.quadrotor2;

import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.DecimalFormat;
import java.util.Timer;
import java.util.TimerTask;

import mySensor.MySensor;
import mySensor.MySensorListener;
import Camera.MyCamera;
import Uart.Com;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements MySensorListener {
	MySensor ms;
	MyCamera cam;
	Com com;
	Timer t;

	public double currentPower = 0; // 0~1000 0:idle 1000:full power
	public double adjGryoxConst = 0;
	public double adjGryoyConst = 0;
	public double adjGryozConst = 0;
	public double adjGryoxTemp = 0;
	public double adjGryoyTemp = 0;
	public double adjGryozTemp = 0;

	public int poweron = 0;
	public int test;

	double pwm1;// up +
	double pwm2;// down +
	double pwm3;// left -
	double pwm4;// right -

	double CurrGryox;
	double CurrGryoy;
	double CurrGryoz;
	TextView tv;
	TextView txtup;
	TextView txtrightleft;
	TextView txtdown;

	MyHandler myHandler;
	MyHandlerCmd myHandlerCmd;

	class Task extends TimerTask {
		MainActivity act;

		public Task(MainActivity act) {
			this.act = act;
		}

		@Override
		public void run() {
			try {
				Message msg = myHandler.obtainMessage();
				Bundle b = new Bundle();
				DecimalFormat df1 = new DecimalFormat("0.000");

				pwm1 = currentPower;
				pwm2 = currentPower;
				pwm3 = currentPower;
				pwm4 = currentPower;

				pwm1 += CurrGryox;
				pwm2 -= CurrGryox;
				pwm3 += CurrGryoy;
				pwm4 -= CurrGryoy;
				pwm1 += CurrGryoz;
				pwm2 += CurrGryoz;
				pwm3 -= CurrGryoz;
				pwm4 -= CurrGryoz;

				if (pwm1 > 1000)
					pwm1 = 1000;
				if (pwm1 < 0)
					pwm1 = 0;

				if (pwm2 > 1000)
					pwm2 = 1000;
				if (pwm2 < 0)
					pwm2 = 0;

				if (pwm3 > 1000)
					pwm3 = 1000;
				if (pwm3 < 0)
					pwm3 = 0;

				if (pwm4 > 1000)
					pwm4 = 1000;
				if (pwm4 < 0)
					pwm4 = 0;

				pwm1 = Math.round(pwm1);
				pwm2 = Math.round(pwm2);
				pwm3 = Math.round(pwm3);
				pwm4 = Math.round(pwm4);

				b.putString("Gryo", df1.format(CurrGryox) + ",  " + df1.format(CurrGryoy) + ",  " + df1.format(CurrGryoz));
				msg.setData(b);
				myHandler.sendMessage(msg);
				if (poweron == 1) {
					b.putString("pwm1", Integer.toString((int) (17000 + pwm1 * (24000 - 17000) / 1000)));
					b.putString("pwm2", Integer.toString((int) (17000 + pwm2 * (24000 - 17000) / 1000)));
					b.putString("pwm3", Integer.toString((int) (17000 + pwm3 * (24000 - 17000) / 1000)));
					b.putString("pwm4", Integer.toString((int) (17000 + pwm4 * (24000 - 17000) / 1000)));
					act.com.Write(0, (int) (17000 + pwm1 * (24000 - 17000) / 1000));
					act.com.Write(1, (int) (17000 + pwm2 * (24000 - 17000) / 1000));
					act.com.Write(2, (int) (17000 + pwm3 * (24000 - 17000) / 1000));
					act.com.Write(3, (int) (17000 + pwm4 * (24000 - 17000) / 1000));
				} else if (poweron == 2) {
					if (test == 1) {
						b.putString("pwm1", "test");
						act.com.Write(1, 16000);
						act.com.Write(2, 16000);
						act.com.Write(3, 16000);
						act.com.Write(0, 17000);
					}
					if (test == 2) {
						b.putString("pwm2", "test");
						act.com.Write(0, 16000);
						act.com.Write(2, 16000);
						act.com.Write(3, 16000);
						act.com.Write(1, 17000);
					}
					if (test == 3) {
						b.putString("pwm3", "test");
						act.com.Write(0, 16000);
						act.com.Write(1, 16000);
						act.com.Write(3, 16000);
						act.com.Write(2, 17000);
					}
					if (test == 4) {
						b.putString("pwm4", "test");
						act.com.Write(0, 16000);
						act.com.Write(1, 16000);
						act.com.Write(2, 16000);
						act.com.Write(3, 17000);
					}
				} else if (poweron == 0) {
					b.putString("pwm1", Double.toString(0));
					b.putString("pwm2", Double.toString(0));
					b.putString("pwm3", Double.toString(0));
					b.putString("pwm4", Double.toString(0));
					act.com.Write(0, 16000);
					act.com.Write(1, 16000);
					act.com.Write(2, 15000);
					act.com.Write(3, 16000);
				}

			} catch (Throwable t) {

			}
		}
	}

	class ThreadControl extends Thread {
		public void run() {
			ServerSocket ss = null;
			while (true) {
				if (ss == null) {
					try {
						ss = new ServerSocket(5000);
					} catch (Exception e) {

					}
				}
				try {
					Socket s = ss.accept();
					InputStream ins = s.getInputStream();
					byte[] data = new byte[9];
					ins.read(data);
					s.close();
					Message msg = myHandlerCmd.obtainMessage();
					Bundle b = new Bundle();
					b.putByteArray("data", data);
					msg.setData(b);
					myHandlerCmd.sendMessage(msg);
				} catch (Exception ex) {

				}
			}

		}
	}

	class MyHandlerCmd extends Handler {
		public MyHandlerCmd() {
		}

		public MyHandlerCmd(Looper L) {
			super(L);
		}

		int ADJXC = 1;
		int ADJYC = 2;
		int ADJZC = 3;
		int ADJPWR = 4;
		int CALI = 5;
		int RST = 6;
		int START = 7;
		int STOP = 8;
		int TEST = 9;
		int SETPWR = 10;
		int ADJXT = 11;
		int ADJYT = 12;
		int ADJZT = 13;

		public void IntToByte(int val, byte[] ret, int offset) {
			ret[offset + 3] = (byte) (val >> 24 & 0xff);
			ret[offset + 2] = (byte) (val >> 16 & 0xff);
			ret[offset + 1] = (byte) (val >> 8 & 0xff);
			ret[offset + 0] = (byte) (val & 0xff);
		}

		public int ByteToInt(byte[] src, int offset) {
			int value;
			value = (int) ((src[offset] & 0xFF) | ((src[offset + 1] & 0xFF) << 8) | ((src[offset + 2] & 0xFF) << 16) | ((src[offset + 3] & 0xFF) << 24));
			return value;
		}

		@Override
		public void handleMessage(Message msg) {
			Bundle b = msg.getData();
			byte[] data = b.getByteArray("data");
			int cmd = data[0];
			double value = getDouble(data, 1);

			if (cmd == ADJXC) {
				adjGryoxConst = value;
			}
			if (cmd == ADJYC) {
				adjGryoyConst = value;
			}
			if (cmd == ADJZC) {
				adjGryozConst = value;
			}
			if (cmd == ADJXT) {
				adjGryoxTemp = value;
			}
			if (cmd == ADJYT) {
				adjGryoyTemp = value;
			}
			if (cmd == ADJZT) {
				adjGryozTemp = value;
			}

			if (cmd == ADJPWR) {
				currentPower += value;
			}

			if (cmd == SETPWR) {
				currentPower =  value;
			}

			if (cmd == CALI) {
				ms.CalibrateGyro();
			}

			if (cmd == RST) {
				com.Init();
				currentPower = 0; // 0~1000 0:idle 1000:full power
				adjGryoxConst = 0;
				adjGryoxConst = 0;
				adjGryoxConst = 0;
			}

			if (cmd == START) {
				poweron = 1;
			}
			if (cmd == STOP) {
				poweron = 0;
			}
			if (cmd == TEST) {
				poweron = 2;
				test = (int)value;
			}

		}
	}

	class MyHandler extends Handler {
		public MyHandler() {
		}

		public MyHandler(Looper L) {
			super(L);
		}

		@Override
		public void handleMessage(Message msg) {
			Bundle b = msg.getData();
			String Gryo = b.getString("Gryo");
			tv.setText(Gryo);
			String p1 = b.getString("pwm1");
			String p2 = b.getString("pwm2");
			String p3 = b.getString("pwm3");
			String p4 = b.getString("pwm4");
			txtup.setText(p2);
			txtrightleft.setText(p4 + "     " + p3);
			txtdown.setText(p1);

		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		try {
			myHandler = new MyHandler(MainActivity.this.getMainLooper());
			myHandlerCmd = new MyHandlerCmd(MainActivity.this.getMainLooper());
			ms = new MySensor(this, this);

			com = new Com(this);
			t = new Timer();
			Task tsk = new Task(this);
			t.scheduleAtFixedRate(tsk, 0, 20);
			ThreadControl control = new ThreadControl();
			control.start();

			cam = new MyCamera();
			cam.Init();

			tv = (TextView) findViewById(R.id.textView1);
			txtup = (TextView) findViewById(R.id.txtup);
			txtrightleft = (TextView) findViewById(R.id.txtrightleft);
			txtdown = (TextView) findViewById(R.id.txtdown);
			Button b = (Button) findViewById(R.id.button1);
			b.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					ms.CalibrateGyro();
				}
			});
			Button b2 = (Button) findViewById(R.id.button2);
			b2.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					com.Init();
					currentPower = 0; // 0~1000 0:idle 1000:full power
				}
			});
			Button test1 = (Button) findViewById(R.id.buttonTest1);
			test1.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					poweron = 2;
					test = 1;
				}
			});
			Button test2 = (Button) findViewById(R.id.buttonTest2);
			test2.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					poweron = 2;
					test = 2;
				}
			});
			Button test3 = (Button) findViewById(R.id.buttonTest3);
			test3.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					poweron = 2;
					test = 3;
				}
			});
			Button test4 = (Button) findViewById(R.id.buttonTest4);
			test4.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					poweron = 2;
					test = 4;
				}
			});

		} catch (Throwable t) {

		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	protected void onResume() {
		super.onResume();
		if (com != null) {
			com.ResumeUsbList();
		}
	}

	public void onSensorChanged(double CurrGryox, double CurrGryoy, double CurrGryoz, double CurrAccex, double CurrAccey, double CurrAccez) {

		ms.ModifyGryo(adjGryoxConst + adjGryoxTemp, adjGryoyConst + adjGryoyTemp, adjGryozConst + adjGryozTemp);

		this.CurrGryox = CurrGryox + adjGryoxConst + adjGryoxTemp;
		this.CurrGryoy = CurrGryoy + adjGryoyConst + adjGryoyTemp;
		this.CurrGryoz = CurrGryoz + adjGryozConst + adjGryozTemp;

		adjGryoxTemp = 0;
		adjGryoyTemp = 0;
		adjGryozTemp = 0;

		putDouble(cam.dataSend, this.CurrGryox, 0);
		putDouble(cam.dataSend, this.CurrGryoy, 8);
		putDouble(cam.dataSend, this.CurrGryoz, 16);

	}

	public static void putDouble(byte[] bb, double x, int index) {
		// byte[] b = new byte[8];
		long l = Double.doubleToLongBits(x);
		for (int i = 0; i < 8; i++) {
			bb[index + i] = (byte) l;
			l = l >> 8;
		}
	}
	public static double getDouble(byte[] b, int index)
	{
		long l;
		l = b[0 + index];
		l &= 0xff;
		l |= ((long)b[1 + index] << 8);
		l &= 0xffff;
		l |= ((long)b[2 + index] << 16);
		l &= 0xffffff;
		l |= ((long)b[3 + index] << 24);
		l &= 0xffffffffl;
		l |= ((long)b[4 + index] << 32);
		l &= 0xffffffffffl;
		l |= ((long)b[5 + index] << 40);
		l &= 0xffffffffffffl;
		l |= ((long)b[6 + index] << 48);
		l &= 0xffffffffffffffl;
		l |= ((long)b[7 + index] << 56);
		return Double.longBitsToDouble(l);
	}

}
