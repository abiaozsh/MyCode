package com.wch.pressuresensor;

import android.app.Activity;
import android.content.Context;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

import com.wch.wchusbdriver.CH34xAndroidDriver;

public class UartLoopBackActivity extends Activity {
	private static final String ACTION_USB_PERMISSION = "com.wch.wchusbdriver.USB_PERMISSION";
	/* thread to read the data */
	public readThread handlerThread;
	protected final Object ThreadLock = new Object();

	/* declare UART interface variable */
	public CH34xAndroidDriver uartInterface;

	EditText Text1;
	EditText Text2;

	Button configButton;
	Button testButton;

	// 1600imp/kwh
	// 1imp/ 0.625 wh
	long datas[] = new long[8];

	// byte timeout; // time out
	public Context global_context;
	public boolean isConfiged = false;
	public boolean READ_ENABLE = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		/* create editable text objects */
		Text1 = (EditText) findViewById(R.id.Text1);
		Text2 = (EditText) findViewById(R.id.Text2);

		global_context = this;

		configButton = (Button) findViewById(R.id.configButton);
		configButton.setOnClickListener(new OpenDeviceListener());

		testButton = (Button) findViewById(R.id.testButton);
		testButton.setOnClickListener(new TestListener());
		uartInterface = new CH34xAndroidDriver((UsbManager) getSystemService(Context.USB_SERVICE), UartLoopBackActivity.this, ACTION_USB_PERMISSION);

		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);

		if (READ_ENABLE == false) {
			READ_ENABLE = true;
			handlerThread = new readThread(handler);
			handlerThread.start();
		}

	}

	public class OpenDeviceListener implements View.OnClickListener {

		@Override
		public void onClick(View v) {
			boolean flags;
			int baudRate; /* baud rate */

			byte stopBit; /* 1:1stop bits, 2:2 stop bits */
			byte dataBit; /* 8:8bit, 7: 7bit 6: 6bit 5: 5bit */
			byte parity; /* 0: none, 1: odd, 2: even, 3: mark, 4: space */
			byte flowControl; /* 0:none, 1: flow control(CTS,RTS) */

			baudRate = 115200;
			/* stop bits */
			stopBit = 1;

			/* data bits */
			dataBit = 8;

			/* parity */
			parity = 0;

			/* flow control */
			flowControl = 0;

			uartInterface.ResumeUsbList();
			if (!uartInterface.UsbFeatureSupported()) {
				Text1.setText("not support");
			} else {
				if (uartInterface.isConnected()) {
					flags = uartInterface.UartInit();
					if (!flags) {
						Text1.setText("Init error");
					} else {
						if (uartInterface.SetConfig(baudRate, dataBit, stopBit, parity, flowControl)) {
							Text1.setText("set ok");
						} else {
							Text1.setText("set error");
						}
					}
				} else {
					Text1.setText("not Connected");
				}
			}
		}

	}

	public class TestListener implements View.OnClickListener {

		@Override
		public void onClick(View v) {

			uartInterface.CloseDevice();

			Text1.setText("close done");
		}

	}

	// Calibration values
	static int ac1;
	static int ac2;
	static int ac3;
	static int ac4;
	static int ac5;
	static int ac6;
	static int b1;
	static int b2;
	static int mb;
	static int mc;
	static int md;
	static long b5;
	static int t;
	static long p;

	static int OSS = 3;

	// Calculate temperature in deg C
	static int bmp085GetTemperature(int ut) {
		long x1, x2;

		x1 = (((long) ut - (long) ac6) * (long) ac5) >> 15;
		x2 = ((long) mc << 11) / (x1 + md);
		b5 = x1 + x2;

		long temp = ((b5 + 8) >> 4);

		return (int) temp;
	}

	// Calculate pressure given up
	// calibration values must be known
	// b5 is also required so bmp085GetTemperature(...) must be called first.
	// Value returned will be pressure in units of Pa.
	static long bmp085GetPressure(long up) {
		long x1, x2, x3, b3, b6, p;
		long b4, b7;

		b6 = b5 - 4000;
		// Calculate B3
		x1 = (b2 * (b6 * b6) >> 12) >> 11;
		x2 = (ac2 * b6) >> 11;
		x3 = x1 + x2;
		b3 = (((((long) ac1) * 4 + x3) << OSS) + 2) >> 2;

		// Calculate B4
		x1 = (ac3 * b6) >> 13;
		x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
		x3 = ((x1 + x2) + 2) >> 2;
		b4 = (ac4 * (long) (x3 + 32768)) >> 15;

		b7 = ((long) (up - b3) * (50000 >> OSS));
		if (b7 < 0x80000000)
			p = (b7 << 1) / b4;
		else
			p = (b7 / b4) << 1;

		x1 = (p >> 8) * (p >> 8);
		x1 = (x1 * 3038) >> 16;
		x2 = (-7357 * p) >> 16;
		p += (x1 + x2 + 3791) >> 4;

		long temp = p;
		return temp;
	}

	static double calcAltitude(double pressure) {
		double A = pressure / 101325;
		double B = 1 / 5.25588;
		double C = Math.pow(A, B);
		C = 1 - C;
		C = C / 0.0000225577;
		return C;
	}

	String Strbuff = "";
	char[] readBuffer = new char[65536];
	int actualNumBytes;
	int error = 0;
	String exce = "";

	final Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {

			if (error == 0) {
				String txt = "";

				try {
					txt += "temp:" + bmp085GetTemperature(t) + "\r\n";
				} catch (Exception ex) {
					txt += ex.toString();
				}

				try {
					txt += "press:" + bmp085GetPressure(p) + "\r\n";
				} catch (Exception ex) {
					txt += ex.toString();
				}
				
				
				try {
					txt += "alt:" + calcAltitude(p) + "\r\n";
				} catch (Exception ex) {
					txt += ex.toString();
				}
			
				
				
				/*
				 * txt += "ac1:" + Integer.toString(ac1); txt += "ac2:" + Integer.toString(ac2); txt += "ac3:" + Integer.toString(ac3); txt += "ac4:" + Integer.toString(ac4) + "\r\n"; txt += "ac5:" + Integer.toString(ac5) + "\r\n"; txt += "ac6:" + Integer.toString(ac6) + "\r\n"; txt += "b1:" + Integer.toString(b1) + "\r\n"; txt += "b2:" + Integer.toString(b2) + "\r\n"; txt += "mb:" + Integer.toString(mb) + "\r\n"; txt += "mc:" + Integer.toString(mc) + "\r\n"; txt += "md:" + Integer.toString(md) + "\r\n"; txt += "b5:" + Long.toString(b5) + "\r\n"; txt += "t:" + Integer.toString(t); txt += "p:" + Integer.toString(p) + "\r\n";
				 */

				Text2.setText(txt);
			} else {
				Text2.setText(exce);
			}
		}
	};

	/* usb input data handler */
	private class readThread extends Thread {
		/* constructor */
		Handler mhandler;

		readThread(Handler h) {
			mhandler = h;
			this.setPriority(Thread.MIN_PRIORITY);
		}

		public void run() {
			while (READ_ENABLE) {
				Message msg = mhandler.obtainMessage();
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
				}
				synchronized (ThreadLock) {
					if (uartInterface != null) {
						try {
							int actualNumBytes = uartInterface.ReadData(readBuffer, 65536);

							if (actualNumBytes > 0) {
								Strbuff += String.copyValueOf(readBuffer, 0, actualNumBytes);
								actualNumBytes = 0;

								if (true) {
									while (true) {
										int idx = Strbuff.indexOf('|');
										if (idx == 0) {
											Strbuff = Strbuff.substring(1);
										} else if (idx > 0) {
											String line = Strbuff.substring(0, idx);
											Strbuff = Strbuff.substring(idx + 1);

											char key = line.charAt(0);

											long val = 0;
											try {
												val = Long.parseLong(line.split(":")[1]);
												switch (key) {
												case 'a':
													ac1 = (int) val;
													break;
												case 'b':
													ac2 = (int) val;
													break;
												case 'c':
													ac3 = (int) val;
													break;
												case 'd':
													ac4 = (int) val;
													break;
												case 'e':
													ac5 = (int) val;
													break;
												case 'f':
													ac6 = (int) val;
													break;
												case 'g':
													b1 = (int) val;
													break;
												case 'h':
													b2 = (int) val;
													break;
												case 'i':
													mb = (int) val;
													break;
												case 'j':
													mc = (int) val;
													break;
												case 'k':
													md = (int) val;
													break;
												case 't':
													t = (int) val;
													break;
												case 'p':
													p = val;
													break;
												}
											} catch (Exception ex) {
											}

										} else {
											break;
										}
									}

								}
								mhandler.sendMessage(msg);
							}
						} catch (Exception ex) {
							error = 1;
							exce = ex.toString();
							mhandler.sendMessage(msg);
							while (true) {
								try {
									Thread.sleep(10000);
								} catch (InterruptedException e) {
								}
							}
						}
					}
				}
			}
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.uart_loop_back, menu);
		return true;
	}

}
