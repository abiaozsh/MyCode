package com.example.quadrotor2;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
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
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

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

	public float adjxConst = 0;
	public float adjyConst = 0;
	public float adjZConst = 0;
	public String senseData;
	public int poweron = 2;

	public double pwm1;// up +
	public double pwm2;// down +
	public double pwm3;// left -
	public double pwm4;// right -
	public double minPower = 10;

	public double gravityx = 0;
	public double gravityy = 0;
	public double gravityxAccum = 0;
	public double gravityyAccum = 0;

	class Task extends TimerTask {
		MainActivity act;
		NetThread[] threadPool = new NetThread[5];

		public Task(MainActivity act) {
			this.act = act;
			for (int i = 0; i < 5; i++) {
				threadPool[i] = new NetThread();
				threadPool[i].start();
			}
		}

		class NetThread extends Thread {
			public boolean free = false;

			public void run() {
				while (true) {
					free = false;
					StringBuffer sb = new StringBuffer();

					String fmt = "%0+11.6f";
					String fmt2 = "%0+5.1f";

					sb.append("currentPower:" + String.format(fmt, currentPower) + "\r\n");

					sb.append("adjxConst:" + String.format(fmt, adjxConst) + "\t");
					sb.append("adjyConst:" + String.format(fmt, adjyConst) + "\r\n");
					sb.append(senseData + "\r\n");

					sb.append("poweron:" + Integer.toString(poweron) + "\r\n");

					// up 2
					// left 4 right 3
					// down 1
					sb.append("\r\n");
					sb.append("     pwm2:" + String.format(fmt2, pwm2) + "\r\n");
					sb.append("pwm4:" + String.format(fmt2, pwm4) + "\t");
					sb.append("pwm3:" + String.format(fmt2, pwm3) + "\r\n");
					sb.append("     pwm1:" + String.format(fmt2, pwm1) + "\r\n");
					sb.append("\r\n");
					sb.append("minPower:" + String.format(fmt, minPower) + "\r\n");

					sb.append("gravityx:" + String.format(fmt, gravityx) + "\t");
					sb.append("gravityy:" + String.format(fmt, gravityy) + "\r\n");
					sb.append("gravityxAccum:" + String.format(fmt, gravityxAccum) + "\t");
					sb.append("gravityyAccum:" + String.format(fmt, gravityyAccum) + "\r\n");

					sb.append("msg:" + Message.toString() + "\r\n");
					byte[] dataSend = sb.toString().getBytes();

					try {
						// 将图像数据通过Socket发送出去
						Socket tempSocket = new Socket(MyCamera.ComputerIP, 6000);
						tempSocket.setSoTimeout(1000);
						OutputStream outstream = tempSocket.getOutputStream();

						{
							byte[] len = new byte[2];
							len[0] = (byte) (dataSend.length & 0xFF);
							len[1] = (byte) ((dataSend.length >> 8) & 0xFF);
							outstream.write(1);
							outstream.write(len);
							outstream.write(dataSend);
						}

						outstream.flush();
						outstream.close();
						tempSocket.close();

					} catch (IOException e) {
					} catch (Throwable e) {
					} finally {
						free = true;
					}
					try {
						Thread.sleep(1000000);
					} catch (Throwable ex) {

					}
				}
			}

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

					// side add
					// adjZConst

					pwm4 += gravityx * 20;// + (gravityyAccum / 100)
					pwm3 -= gravityx * 20;// + (gravityyAccum / 100)

					pwm1 += gravityy * 20;// + (gravityxAccum / 100)
					pwm2 -= gravityy * 20;// + (gravityxAccum / 100)

					pwm4 += adjZConst;// +- ~30
					pwm3 += adjZConst;
					pwm1 -= adjZConst;
					pwm2 -= adjZConst;

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

				if (poweron == 1) {
					act.com.Send(CMD_SETPWRSIMP, (byte) pwm1, CMD_SETPWRSIMP, (byte) pwm2, CMD_SETPWRSIMP, (byte) pwm3, CMD_SETPWRSIMP, (byte) pwm4);
				} else if (poweron == 2) {
				} else if (poweron == 0) {
					act.com.Send(CMD_SETPWRSIMP, Z, CMD_SETPWRSIMP, Z, CMD_SETPWRSIMP, Z, CMD_SETPWRSIMP, Z);
				}

				// 发送数据
				if (MyCamera.ComputerIP != null) {
					try {

						try {
							aa: for (int i = 0; i < 5; i++) {
								if ((threadPool[i].free)) {
									threadPool[i].interrupt();
									break aa;
								}
							}
						} catch (Throwable e) {
						}
					} catch (Exception ex) {
					}
				}

			} catch (Throwable t) {

			}
		}
	}

	// MyHandler myHandler;
	MyHandlerCmd myHandlerCmd;

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
		final int SETCAMB = 6;
		final int ADJPWR = 7;
		final int SETCAMF = 8;
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
		final int CON = 25;
		final int COF = 26;
		final int PUSHDATA = 27;
		final int SETIP = 28;
		final int CAMON = 29;
		final int CAMOFF = 30;
		final int TAKEPIC = 31;
		final int SETQUALITY = 32;
		final int SETLED = 33;
		
		int AL = 0x04;
		int AH = 0x08;
		int BL = 0x10;
		int BH = 0x20;
		int CL = 0x80;
		int CH = 0x40;
		int DL = 0x01;
		int DH = 0x02;

		int A = 0;
		int B = 0;
		int C = 0;

		class AotoOffTimer extends Thread {

			public AotoOffTimer() {

			}

			public void run() {
				while (true) {
					try {
						Thread.sleep(1000);
						com.Send((byte) 0);
					} catch (Exception ex) {
					}
				}
			}
		}

		AotoOffTimer timer = new AotoOffTimer();

		@Override
		public void handleMessage(Message msg) {
			Bundle b = msg.getData();
			byte[] data = b.getByteArray("data");
			int cmd = data[0];
			double value = getDouble(data, 1);

			switch (cmd) {
			case SETIP:
				int v0 = data[1] & 0x00FF;
				int v1 = data[2] & 0x00FF;
				int v2 = data[3] & 0x00FF;
				int v3 = data[4] & 0x00FF;

				String ip = v0 + "." + v1 + "." + v2 + "." + v3;
				et.setText(ip);
				cam.ClearPool();
				MyCamera.ComputerIP = ip;
				break;
			case CAMON:
				cam.prevOn();
				break;
			case CAMOFF:
				cam.prevOff();
				break;
			case TAKEPIC:
				cam.Takepic();
				break;
			case SETQUALITY:
				MyCamera.Quality = (int) value;
				break;
			case SETLED:
				cam.setLed((int) value);
				break;

			case ADJXC:
				adjxConst = (float) value;
				break;
			case ADJYC:
				adjyConst = (float) value;
				break;
			case ADJZC:
				adjZConst = (float) value;
				break;
			case ADJPWR:
				currentPower += value;
				break;
			case SETPWR:
				currentPower = value;
				break;
			case SETCAMB:
				cam.facing = cam.B;
				cam.Init();
				break;
			case SETCAMF:
				cam.facing = cam.F;
				cam.Init();
				break;
			case RST:
				com.Init();
				currentPower = 0;
				break;
			case RST2:
				com.Init8();
				timer.start();
				break;
			case LFW:
				A = 1;
				timer.interrupt();
				break;
			case LBK:
				A = -1;
				timer.interrupt();
				break;
			case LOF:
				A = 0;
				break;
			case RFW:
				B = 1;
				timer.interrupt();
				break;
			case RBK:
				B = -1;
				timer.interrupt();
				break;
			case ROF:
				B = 0;
				break;
			case CON:
				C = 1;
				break;
			case COF:
				C = -1;
				break;
			case PUSHDATA:
				int val = 0;
				if (A == 1) {
					val |= AH | BL;
				}
				if (A == -1) {
					val |= AL | BH;
				}
				if (B == 1) {
					val |= CL | DH;
				}
				if (B == -1) {
					val |= CH | DL;
				}
				if (C == 1) {
					val |= 0x04 + 0x08 + 0x10 + 0x20;
					C = 0;
				}
				if (C == -1) {
					val |= 0x80 + 0x40 + 0x01 + 0x02;
					C = 0;
				}
				com.Send((byte) val);
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
	EditText et;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		try {
			// myHandler = new MyHandler(MainActivity.this.getMainLooper());
			myHandlerCmd = new MyHandlerCmd(MainActivity.this.getMainLooper());
			ms = new MySensor(this, this);

			com = new Com(this);
			com.Message = Message;
			t = new Timer();
			Task tsk = new Task(this);
			t.scheduleAtFixedRate(tsk, 0, 20);

			ThreadControl control = new ThreadControl();
			control.start();

			cam = new MyCamera();
			cam.Init();
			et = (EditText) findViewById(R.id.editText1);
			et.setText(MyCamera.ComputerIP);
			{
				Button b = (Button) findViewById(R.id.buttonok);
				b.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						MyCamera.ComputerIP = (String) et.getText().toString();
					}
				});
			}
			{
				Button b = (Button) findViewById(R.id.button_192_168_0_10);
				b.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						et.setText("192.168.0.10");
						MyCamera.ComputerIP = (String) et.getText().toString();
					}
				});
			}
			{
				Button b = (Button) findViewById(R.id.button192_168_43_151);
				b.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						et.setText("192.168.43.151");
						MyCamera.ComputerIP = (String) et.getText().toString();
					}
				});

			}

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

	public void onSensorChanged(double CurrGryox, double CurrGryoy, double CurrGryoz, double CurrAccex, double CurrAccey, double CurrAccez, double x, double y, double z) {

		ms.setAdj(adjxConst, adjyConst, 0);

		this.gravityx = x;
		this.gravityy = y;
		String fmt = "%0+11.6f";
		String temp = "";
		temp += "CurrGryox:" + String.format(fmt, CurrGryox) + "\t";
		temp += "CurrGryoy:" + String.format(fmt, CurrGryoy) + "\t";
		temp += "CurrGryoz:" + String.format(fmt, CurrGryoz) + "\r\n";

		temp += "CurrAccex:" + String.format(fmt, CurrAccex) + "\t";
		temp += "CurrAccey:" + String.format(fmt, CurrAccey) + "\t";
		temp += "CurrAccez:" + String.format(fmt, CurrAccez) + "\r\n";
		this.senseData = temp;
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
