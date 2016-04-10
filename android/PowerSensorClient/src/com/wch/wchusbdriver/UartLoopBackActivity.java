package com.wch.wchusbdriver;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.StrictMode;
import android.view.Menu;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

public class UartLoopBackActivity extends Activity {

	EditText Text1;
	EditText Text2;

	Button configButton;
	Button testButton;

	// byte timeout; // time out
	public Context global_context;
	public boolean isConfiged = false;
	public boolean READ_ENABLE = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder().detectDiskReads() // 捕捉读取磁盘
				.detectDiskWrites() // 捕捉写入磁盘
				.detectNetwork() // 捕捉网络访问 或使用detectAll() 火力全开
				.penaltyLog() // 捕捉LogCat日志
				.build());

		StrictMode.setVmPolicy(new StrictMode.VmPolicy.Builder().detectLeakedSqlLiteObjects().detectLeakedClosableObjects().penaltyLog().penaltyDeath().build());

		/* create editable text objects */
		Text1 = (EditText) findViewById(R.id.Text1);
		Text2 = (EditText) findViewById(R.id.Text2);

		global_context = this;

		configButton = (Button) findViewById(R.id.configButton);
		configButton.setOnClickListener(new OpenDeviceListener());

		testButton = (Button) findViewById(R.id.testButton);
		testButton.setOnClickListener(new TestListener());

		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);

	}

	public class OpenDeviceListener implements View.OnClickListener {

		@Override
		public void onClick(View v) {
			try {
				String ip = Text1.getText().toString();

				// 将图像数据通过Socket发送出去
				Socket tempSocket = new Socket(ip, 5000);
				OutputStream ous = tempSocket.getOutputStream();
				InputStream ins = tempSocket.getInputStream();
				byte[] snd = new byte[2];

				snd[0] = 10;
				snd[1] = 0;

				ous.write(snd);
				ous.flush();

				String s = receive(ins);

				ous.close();
				tempSocket.close();

				Text2.setText(s);
			} catch (IOException e) {
				Text2.setText(e.toString());
			} catch (Throwable e) {
				Text2.setText(e.toString());
			} finally {
			}

		}

		private String receive(InputStream ins) throws IOException {
			byte[] data = new byte[1];

			byte[] receive = new byte[3];
			int len3 = 0;
			while (len3 < 3) {
				len3 += ins.read(receive, len3, 3 - len3);
			}
			int len = (((int) receive[0]) & 0x0FF) + (((int) receive[1] << 8) & 0x0FF) + (((int) receive[2] << 16) & 0x0FF);

			byte[] receiveBuff = new byte[len];
			int len2 = 0;
			while (len2 < len) {
				len2 += ins.read(receiveBuff, len2, len - len2);
			}
			String s = new String(receiveBuff);
			return s;
		}
	}

	public class TestListener implements View.OnClickListener {

		@Override
		public void onClick(View v) {
		}

	}

	String Strbuff = "";
	String watts = "";
	int intval = 0;

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.uart_loop_back, menu);
		return true;
	}

}
