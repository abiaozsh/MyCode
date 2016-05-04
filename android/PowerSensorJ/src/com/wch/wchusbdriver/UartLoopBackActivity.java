package com.wch.wchusbdriver;

import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import android.app.Activity;
import android.content.Context;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.Menu;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

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
					OutputStream ous = s.getOutputStream();

					byte[] data = new byte[2];// CMD + DATA

					ins.read(data);

					if (data[0] == 10) {// get Current Data
						String w = "";
						for (int i = 7; i >= 1; i--) {
							double watt = 0;
							if (datas[i] - datas[i - 1] > 0) {
								watt = 2250000.0 / (datas[i] - datas[i - 1]);// (w)
							}
							w += Double.toString(watt) + "\r\n";
						}
						SendData(w, ous);
					} else if (data[0] == 12) {// getFileList
						String[] fl = fileList();

						String dirs = "";
						for (int i = 0; i < fl.length; i++) {
							dirs += fl[i] + "\r\n";
						}
						SendData(dirs, ous);
					} else if (data[0] == 11) {// delete File
						String[] fl = fileList();
						int idx = data[1];
						UartLoopBackActivity.this.deleteFile(fl[idx]);
						SendData(fl[idx], ous);
					} else if (data[0] == 13) {// getFile
						String[] fl = fileList();
						int idx = data[1];
						SendData(fl[idx], ous);
						InputStream fis = UartLoopBackActivity.this.openFileInput(fl[idx]);
						int len = fis.available();
						SendDatax(len, ous, fis);
						fis.close();
					}

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

		private void SendData(String sdata, OutputStream ous) {
			try {
				byte[] sizebuff = new byte[1];
				byte[] sendBuff = null;

				sendBuff = sdata.getBytes();

				sizebuff[0] = (byte) (sendBuff.length & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((sendBuff.length >> 8) & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((sendBuff.length >> 16) & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((sendBuff.length >> 24) & 0x0FF);
				ous.write(sizebuff);

				ous.write(sendBuff);
			} catch (Exception e) {
			}
		}

		private void SendDatax(int length, OutputStream ous, InputStream fis) {
			try {

				byte[] sizebuff = new byte[1];

				sizebuff[0] = (byte) (length & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((length >> 8) & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((length >> 16) & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((length >> 24) & 0x0FF);
				ous.write(sizebuff);

				byte[] filebuff = new byte[1024];
				while (true) {

					int len = fis.read(filebuff);
					if (len <= 0)
						break;
					ous.write(filebuff);
				}
			} catch (Exception e) {
			}

		}
	}

	class MyHandlerCmd extends Handler {
		public MyHandlerCmd() {
		}

		public MyHandlerCmd(Looper L) {
			super(L);
		}

		@Override
		public void handleMessage(Message msg) {
			// Bundle b = msg.getData();
			// byte[] data = b.getByteArray("data");
			// int cmd = data[0];
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		INIT();
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

		myHandlerCmd = new MyHandlerCmd(UartLoopBackActivity.this.getMainLooper());
		ThreadControl control = new ThreadControl();
		control.start();

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

	/*
	 * static int T1 = 100; static int T2 = 100; private static int pos = 0; private static int count8 = 0; private static int[] buff = new int[256]; private static int sum8 = 0; private static int PEAKSense = 0; private static int LOWSense = 0; private static int CoolDown = 0;
	 * 
	 * private static boolean PROC(int val) { boolean ret = false; sum8 += val; count8++; if (count8 == 8) { count8 = 0; buff[pos++] = sum8 / 8; pos &= 0x0FF; sum8 = 0; int total = 0; { int idx = pos - 4; idx &= 0x0FF; for (int i = 0; i < 256 - 4; i++) { idx--; idx &= 0x0FF; total += buff[idx]; } total /= 256 - 4; }
	 * 
	 * { int idx = pos; int active = 0; for (int i = 0; i < 4; i++) { idx--; idx &= 0x0FF; if (total - buff[idx] > T1) active = 1; } if (active != 0) { LOWSense = 8; } }
	 * 
	 * if ((buff[(pos - 3) & 0x0FF] - buff[(pos - 1) & 0x0FF]) > T2) { PEAKSense = 8; }
	 * 
	 * if (LOWSense > 0 && PEAKSense > 0) { if (CoolDown == 0) { CoolDown = 16; ret = true; } }
	 * 
	 * if (PEAKSense > 0) PEAKSense--; if (LOWSense > 0) LOWSense--; if (CoolDown > 0) CoolDown--; } return ret; }
	 */

	static int T1 = 70;
	static int CD = 25;
	private static int pos = 0;
	private static int count8 = 0;
	private static int[] buff = new int[256];
	private static int sum8 = 0;
	private static int LOWSense = 0;
	private static int CoolDown = 0;

	private static void INIT() {
		for (int i = 0; i < 256; i++) {
			buff[i] = 512;
		}
	}

	private static int PROC(int val) {
		int ret = 0;
		sum8 += val;
		count8++;
		if (count8 == 8) {
			count8 = 0;
			buff[pos++] = sum8 / 8;
			pos &= 0x0FF;
			sum8 = 0;
			int total = 0;
			{
				int idx = pos - 4;
				idx &= 0x0FF;
				for (int i = 0; i < 256 - 4; i++) {
					idx--;
					idx &= 0x0FF;
					total += buff[idx];
				}
				total /= 256 - 4;
			}

			{
				int idx = pos;
				int active = 0;
				for (int i = 0; i < 4; i++) {
					idx--;
					idx &= 0x0FF;
					if (buff[idx] - total > T1)
						active++;
				}
				if (active >= 2) {
					LOWSense = 1;
				}
			}

			if (LOWSense > 0)// && PEAKSense > 0
			{
				if (CoolDown == 0) {
					CoolDown = CD;
					ret = 1;
				}
			}

			if (LOWSense > 0)
				LOWSense--;
			if (CoolDown > 0)
				CoolDown--;
		}
		return ret;
	}

	String Strbuff = "";
	String watts = "";
	int intval = 0;
	char[] readBuffer = new char[65536];
	int actualNumBytes;
	int error = 0;
	String exce = "";

	final Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {

			if (error == 0) {
				Text1.setText(Integer.toString(intval));
				Text2.setText(watts);
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
										int idx = Strbuff.indexOf('\n');
										if (idx == 0) {
											Strbuff = Strbuff.substring(1);
										} else if (idx > 0) {
											String stemp = Strbuff.substring(0, idx - 1);
											Strbuff = Strbuff.substring(idx + 1);
											try {
												intval = Integer.parseInt(stemp);
											} catch (Exception ee) {
												intval = -1;
											}
											if (intval >= 1024) {
												intval = -1;
											}
											if (intval != -1) {
												int ret = PROC(intval);
												if (ret != 0) {
													datas[0] = datas[1];
													datas[1] = datas[2];
													datas[2] = datas[3];
													datas[3] = datas[4];
													datas[4] = datas[5];
													datas[5] = datas[6];
													datas[6] = datas[7];
													datas[7] = System.currentTimeMillis();
													try {
														DateFormat sdf = new SimpleDateFormat("yyyy_MM_dd");
														String filename = sdf.format(datas[7]) + ".txt";

														FileOutputStream fos = openFileOutput(filename, Activity.MODE_APPEND);
														OutputStreamWriter sw = new OutputStreamWriter(fos);
														sw.write(Long.toString(datas[7]) + "\r\n");
														sw.flush();
														fos.close();
													} catch (Exception e) {
													}

													double watt = 0;
													for (int i = 7; i >= 1; i--) {
														if (datas[i] - datas[i - 1] > 0) {
															watt += 2250000.0 / (datas[i] - datas[i - 1]);// (w)
														}
													}
													watt /= 7;
													
													if (watt > 300) {
														T1 = 70;
														CD = 25;
													} else {
														T1 = 50;
														CD = 100;
													}
												}
											}
										} else {
											break;
										}
									}

									watts = "";
									for (int i = 7; i >= 1; i--) {
										double watt = 0;
										if (datas[i] - datas[i - 1] > 0) {
											watt = 2250000.0 / (datas[i] - datas[i - 1]);// (w)
										}

										watts += Double.toString(watt) + "\r\n";

									}

								} else {
									watts = Strbuff.substring(0, 20);
									try {
										DateFormat sdf = new SimpleDateFormat("yyyy_MM_dd");
										String filename = sdf.format(datas[7]) + ".txt";

										FileOutputStream fos = openFileOutput(filename, Activity.MODE_APPEND);
										OutputStreamWriter sw = new OutputStreamWriter(fos);
										sw.write(Strbuff);
										Strbuff = "";
										sw.flush();
										fos.close();
									} catch (Exception e) {
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
