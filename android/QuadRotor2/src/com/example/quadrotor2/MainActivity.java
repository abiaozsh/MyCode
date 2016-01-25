package com.example.quadrotor2;

import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
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

public class MainActivity extends Activity implements MySensorListener {
	MySensor ms;
	MyCamera cam;
	Com com;
	Timer t;

	public StringBuilder Message = new StringBuilder();
	public byte Z = 0;
	public byte CMD_START = 1;/* START */
	public byte CMD_NOSTART = 2;/* START off */
	public byte CMD_SETPWRSIMP = 3;/* set power simple */
	public byte CMD_PITCH = 4;/* PITCH */

	public double currentPower = 0;

	public double adjGryoxConst = 0;
	public double adjGryoyConst = 0;
	public double adjGryozConst = 0;
	public double adjGryoxTemp = 0;
	public double adjGryoyTemp = 0;
	public double adjGryozTemp = 0;

	public int poweron = 2;

	public double pwm1;// up +
	public double pwm2;// down +
	public double pwm3;// left -
	public double pwm4;// right -
	public double minPower = 10;

	public double CurrGryoxAccum = 0;
	public double CurrGryoyAccum = 0;
	public double CurrGryozAccum = 0;
	public double CurrGryox;
	public double CurrGryoy;
	public double CurrGryoz;

	public byte data1;
	public byte data2;
	public byte data3;
	public byte data4;

	// MyHandler myHandler;
	MyHandlerCmd myHandlerCmd;

	class Task extends TimerTask {
		MainActivity act;

		public Task(MainActivity act) {
			this.act = act;
		}

		// 20ms运行一次
		@Override
		public void run() {
			try {

				pwm1 = currentPower;
				pwm2 = currentPower;
				pwm3 = currentPower;
				pwm4 = currentPower;

				// PID
				{
					CurrGryoxAccum = 0;// += CurrGryox;
					CurrGryoyAccum = 0;// += CurrGryoy;
					CurrGryozAccum = 0;// += CurrGryoz;

					pwm1 += CurrGryox + (CurrGryoxAccum / 100);
					pwm2 -= CurrGryox + (CurrGryoxAccum / 100);

					pwm3 += CurrGryoy + (CurrGryoyAccum / 100);
					pwm4 -= CurrGryoy + (CurrGryoyAccum / 100);

					pwm1 += CurrGryoz + (CurrGryozAccum / 100);
					pwm2 += CurrGryoz + (CurrGryozAccum / 100);
					pwm3 -= CurrGryoz + (CurrGryozAccum / 100);
					pwm4 -= CurrGryoz + (CurrGryozAccum / 100);
				}

				if (pwm1 > 255)
					pwm1 = 255;
				if (pwm1 < minPower)
					pwm1 = minPower;

				if (pwm2 > 255)
					pwm2 = 255;
				if (pwm2 < minPower)
					pwm2 = minPower;

				if (pwm3 > 255)
					pwm3 = 255;
				if (pwm3 < minPower)
					pwm3 = minPower;

				if (pwm4 > 255)
					pwm4 = 255;
				if (pwm4 < minPower)
					pwm4 = minPower;

				pwm1 = Math.round(pwm1);
				pwm2 = Math.round(pwm2);
				pwm3 = Math.round(pwm3);
				pwm4 = Math.round(pwm4);

				cam.sendData.currentPower = currentPower;
				cam.sendData.adjGryoxConst = adjGryoxConst;
				cam.sendData.adjGryoyConst = adjGryoyConst;
				cam.sendData.adjGryozConst = adjGryozConst;
				cam.sendData.adjGryoxTemp = adjGryoxTemp;
				cam.sendData.adjGryoyTemp = adjGryoyTemp;
				cam.sendData.adjGryozTemp = adjGryozTemp;
				cam.sendData.poweron = poweron;
				cam.sendData.pwm1 = pwm1;
				cam.sendData.pwm2 = pwm2;
				cam.sendData.pwm3 = pwm3;
				cam.sendData.pwm4 = pwm4;
				cam.sendData.minPower = minPower;
				cam.sendData.CurrGryoxAccum = CurrGryoxAccum;
				cam.sendData.CurrGryoyAccum = CurrGryoyAccum;
				cam.sendData.CurrGryozAccum = CurrGryozAccum;
				cam.sendData.CurrGryox = CurrGryox;
				cam.sendData.CurrGryoy = CurrGryoy;
				cam.sendData.CurrGryoz = CurrGryoz;
				cam.sendData.data1 = data1;
				cam.sendData.data2 = data2;
				cam.sendData.data3 = data3;
				cam.sendData.data4 = data4;

				cam.sendData.Message = Message.toString();
				if (poweron == 1) {
					act.com.Send(CMD_SETPWRSIMP, (byte) pwm1, CMD_SETPWRSIMP, (byte) pwm2, CMD_SETPWRSIMP, (byte) pwm3, CMD_SETPWRSIMP, (byte) pwm4);
				} else if (poweron == 2) {
				} else if (poweron == 0) {
					act.com.Send(CMD_SETPWRSIMP, Z, CMD_SETPWRSIMP, Z, CMD_SETPWRSIMP, Z, CMD_SETPWRSIMP, Z);
				}

			} catch (Throwable t) {

			}
		}
	}

	class ThreadControl extends Thread {
		@SuppressWarnings("resource")
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

		final int ADJXC = 1;
		final int ADJYC = 2;
		final int ADJZC = 3;
		final int ADJXT = 4;
		final int ADJYT = 5;
		final int ADJZT = 6;
		final int ADJPWR = 7;
		final int CALI = 8;
		final int RST = 9;
		final int PWRON = 10;
		final int PWROFF = 11;
		final int SETPWR = 12;
		final int SETSTARTPWR1 = 13;
		final int SETSTARTPWR2 = 14;
		final int SETSTARTPWR3 = 15;
		final int SETSTARTPWR4 = 16;
		final int SETMINPWR = 17;
		final int RST2 = 18;
		final int LFW = 19;
		final int LBK = 20;
		final int LOF = 21;
		final int RFW = 22;
		final int RBK = 23;
		final int ROF = 24;
		final int PUSHDATA = 25;

		@Override
		public void handleMessage(Message msg) {
			Bundle b = msg.getData();
			byte[] data = b.getByteArray("data");
			int cmd = data[0];
			double value = getDouble(data, 1);

			switch (cmd) {
			case ADJXC:
				adjGryoxConst = value;
				break;
			case ADJYC:
				adjGryoyConst = value;
				break;
			case ADJZC:
				adjGryozConst = value;
				break;
			case ADJXT:
				adjGryoxTemp = value;
				break;
			case ADJYT:
				adjGryoyTemp = value;
				break;
			case ADJZT:
				adjGryozTemp = value;
				break;
			case ADJPWR:
				currentPower += value;
				break;
			case SETPWR:
				currentPower = value;
				break;
			case CALI:
				ms.CalibrateGyro();
				break;
			case RST:
				com.Init();
				currentPower = 0;
				adjGryoxConst = 0;
				adjGryoxConst = 0;
				adjGryoxConst = 0;
				break;
			case RST2:
				com.Init();
				break;
			case LFW:
				data1 = (byte) 1;
				data2 = (byte) 240;
				break;
			case LBK:
				data1 = (byte) 2;
				data2 = (byte) 240;
				break;
			case LOF:
				data1 = (byte) 0;
				data2 = (byte) 0;
				break;
			case RFW:
				data3 = (byte) 3;
				data4 = (byte) 240;
				break;
			case RBK:
				data3 = (byte) 4;
				data4 = (byte) 240;
				break;
			case ROF:
				data3 = (byte) 0;
				data4 = (byte) 0;
				break;
			case PUSHDATA:
				com.Send(data1, data2, data3, data4);
				break;
			case PWRON:
				poweron = 1;
				Message.delete(0, Message.length());
				break;
			case PWROFF:
				poweron = 0;
				break;
			case SETSTARTPWR1:
				poweron = 2;
				if (value >= 10000) {
					// com.Send(CMD_SETPWRSIMP, (byte) (value - 10000), Z, Z, Z, Z, Z, Z);
					com.Send(CMD_START, Z, Z, Z, Z, Z, Z, Z);
				} else if (value >= 0) {
					com.Send(CMD_SETPWRSIMP, (byte) (value), Z, Z, Z, Z, Z, Z);
				} else {
					com.Send(CMD_NOSTART, Z, Z, Z, Z, Z, Z, Z);
				}
				break;
			case SETSTARTPWR2:
				poweron = 2;
				if (value >= 10000) {
					// com.Send(Z, Z, CMD_SETPWRSIMP, (byte) (value - 10000), Z, Z, Z, Z);
					com.Send(Z, Z, CMD_START, Z, Z, Z, Z, Z);
				} else if (value >= 0) {
					com.Send(Z, Z, CMD_SETPWRSIMP, (byte) (value), Z, Z, Z, Z);
				} else {
					com.Send(Z, Z, CMD_NOSTART, Z, Z, Z, Z, Z);
				}
				break;
			case SETSTARTPWR3:
				poweron = 2;
				if (value >= 10000) {
					// com.Send(Z, Z, Z, Z, CMD_SETPWRSIMP, (byte) (value - 10000), Z, Z);
					com.Send(Z, Z, Z, Z, CMD_START, Z, Z, Z);
				} else if (value >= 0) {
					com.Send(Z, Z, Z, Z, CMD_SETPWRSIMP, (byte) (value), Z, Z);
				} else {
					com.Send(Z, Z, Z, Z, CMD_NOSTART, Z, Z, Z);
				}
				break;
			case SETSTARTPWR4:
				poweron = 2;
				if (value >= 10000) {
					// com.Send(Z, Z, Z, Z, Z, Z, CMD_SETPWRSIMP, (byte) (value - 10000));
					com.Send(Z, Z, Z, Z, Z, Z, CMD_START, Z);
				} else if (value >= 0) {
					com.Send(Z, Z, Z, Z, Z, Z, CMD_SETPWRSIMP, (byte) (value));
				} else {
					com.Send(Z, Z, Z, Z, Z, Z, CMD_NOSTART, Z);
				}
				break;
			case SETMINPWR:
				minPower = value;
				break;
			}

		}
	}

	/*
	 * class MyHandler extends Handler { public MyHandler() { }
	 * 
	 * public MyHandler(Looper L) { super(L); }
	 * 
	 * @Override public void handleMessage(Message msg) { Bundle b = msg.getData(); String Gryo = b.getString("Gryo"); String p1 = b.getString("pwm1"); String p2 = b.getString("pwm2"); String p3 = b.getString("pwm3"); String p4 = b.getString("pwm4");
	 * 
	 * } }
	 */

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		try {
			// myHandler = new MyHandler(MainActivity.this.getMainLooper());
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

	}

	public static double getDouble(byte[] b, int index) {
		long l;
		l = b[0 + index];
		l &= 0xff;
		l |= ((long) b[1 + index] << 8);
		l &= 0xffff;
		l |= ((long) b[2 + index] << 16);
		l &= 0xffffff;
		l |= ((long) b[3 + index] << 24);
		l &= 0xffffffffl;
		l |= ((long) b[4 + index] << 32);
		l &= 0xffffffffffl;
		l |= ((long) b[5 + index] << 40);
		l &= 0xffffffffffffl;
		l |= ((long) b[6 + index] << 48);
		l &= 0xffffffffffffffl;
		l |= ((long) b[7 + index] << 56);
		return Double.longBitsToDouble(l);
	}

}
