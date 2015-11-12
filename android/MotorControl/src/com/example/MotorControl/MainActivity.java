package com.example.MotorControl;

import java.util.Timer;

import Uart.Com;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MainActivity extends Activity {
	Com com;
	Timer t;

	final byte CMD_SENDDATA1Xa = 1; /* 0~255 1x */
	final byte CMD_SENDDATA1Xb = 2; /* 256~511 1x */
	final byte CMD_SENDDATA2X = 3; /* 512~1023 2x */
	final byte CMD_SENDDATA4X = 4; /* 1024~2047 4x */
	final byte CMD_SENDDATA8X = 5; /* 2048~4095 8x */
	final byte CMD_SENDDATA16X = 6; /* 4096~8191 16x */
	final byte CMD_START = 7; /* on/off */
	final byte CMD_SETMAXPWR = 8;
	final byte CMD_LINEUP = 9;
	final byte CMD_PITCH = 10; /* PITCH */
	final byte CMD_REVERSE = 11; /* REVERSE */
	final byte CMD_SETCPU = 12;

	TextView tv1;
	TextView tv2;

	SeekBar s1;
	SeekBar s2;

	CheckBox checkBox1;
	CheckBox checkBox2;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		try {

			com = new Com(this);
			t = new Timer();

			tv1 = (TextView) findViewById(R.id.txt1);
			tv2 = (TextView) findViewById(R.id.txt2);

			Button b1 = (Button) findViewById(R.id.button1);
			b1.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					com.Send(CMD_START);
					com.Send(250);
					tv1.setText("start");
				}
			});

			Button b2 = (Button) findViewById(R.id.button2);
			b2.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					com.Send(CMD_SETMAXPWR);
					com.Send(255);
					s2.setProgress(0);
					tv2.setText("0");
				}
			});

			Button b3 = (Button) findViewById(R.id.button3);
			b3.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					tv1.setText("test start");
					com.Send(0x55);
					com.Send(0x00);
					tv1.setText("test end");
				}
			});

			Button b4 = (Button) findViewById(R.id.button4);
			b4.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					tv1.setText("Init start");
					com.Init();
					tv1.setText("Init end");
				}
			});

			s1 = (SeekBar) findViewById(R.id.seekBar1);
			s1.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

				@Override
				public void onProgressChanged(SeekBar arg0, int arg1,
						boolean arg2) {

					int v = s1.getProgress();
					int cmd = 0;
					int val = 0;
					if (v < 256) {
						cmd = CMD_SENDDATA1Xa;
						val = (v);
					} else if (v < 512) {
						cmd = CMD_SENDDATA1Xb;
						val = ((v - 256));
					} else if (v < 768) {
						cmd = CMD_SENDDATA2X;
						val = ((v - 512));
					} else if (v < 1024) {
						cmd = (CMD_SENDDATA4X);
						val = (((v - 768)));
					} else if (v < 1280) {
						cmd = (CMD_SENDDATA8X);
						val = (((v - 1024)));
					} else if (v < 1536) {
						cmd = (CMD_SENDDATA16X);
						val = (((v - 1280)));
					} else {
					}
					com.Send(cmd);
					com.Send(val);
					int vv = val;
					int targetSpeed = 0;
					switch ((byte) cmd) {
					case CMD_SENDDATA1Xa:// 10 /*0~255 1x*/
						targetSpeed = vv;
						break;
					case CMD_SENDDATA1Xb:// 11 /*256~511 1x*/
						targetSpeed = vv + 256;
						break;
					case CMD_SENDDATA2X:// 12 /*512~1023 2x*/
						targetSpeed = (vv << 1) + 512;
						break;
					case CMD_SENDDATA4X:// 13 /*1024~2047 4x*/
						targetSpeed = (vv << 2) + 1024;
						break;
					case CMD_SENDDATA8X:// 14 /*2048~4095 8x*/
						targetSpeed = (vv << 3) + 2048;
						break;
					case CMD_SENDDATA16X:// 15 /*4096~8191 16x*/
						targetSpeed = (vv << 4) + 4096;
						break;
					}

					tv1.setText(Integer.toString(val) + ","
							+ Integer.toString(targetSpeed));

				}

				@Override
				public void onStartTrackingTouch(SeekBar arg0) {
				}

				@Override
				public void onStopTrackingTouch(SeekBar arg0) {
				}
			});

			s2 = (SeekBar) findViewById(R.id.seekBar2);
			s2.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

				@Override
				public void onProgressChanged(SeekBar arg0, int arg1,
						boolean arg2) {
					byte data = 0;
					int val = 0;
					if (s2.getProgress() == 256) {
						val = 256;
						data = 0;
					} else if (s2.getProgress() >= 224) {
						val = 224;
						data = 1;
					} else if (s2.getProgress() >= 192) {
						val = 192;
						data = 2;
					} else if (s2.getProgress() >= 160) {
						val = 160;
						data = 3;
					} else if (s2.getProgress() >= 128) {
						val = 128;
						data = 4;
					} else if (s2.getProgress() >= 96) {
						val = 96;
						data = 5;
					} else if (s2.getProgress() >= 64) {
						val = 64;
						data = 6;
					} else if (s2.getProgress() >= 32) {
						val = 32;
						data = 7;
					} else if (s2.getProgress() >= 16) {
						val = 16;
						data = 8;
					} else if (s2.getProgress() >= 8) {
						val = 8;
						data = 9;
					} else if (s2.getProgress() >= 4) {
						val = 4;
						data = 10;
					} else if (s2.getProgress() >= 2) {
						val = 2;
						data = 11;
					} else {
						val = 1;
						data = 12;
					}
					
					s2.setProgress(val);

					com.Send(CMD_SETMAXPWR);
					com.Send(data);

					tv2.setText(Integer.toString(val));

				}

				@Override
				public void onStartTrackingTouch(SeekBar arg0) {
				}

				@Override
				public void onStopTrackingTouch(SeekBar arg0) {
				}
			});

			checkBox1 = (CheckBox) findViewById(R.id.checkBox1);
			checkBox1.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					if (checkBox1.isChecked()) {
						com.Send(CMD_PITCH);
						com.Send(1);
						tv1.setText("Pitch on");
					} else {
						com.Send(CMD_PITCH);
						com.Send(0);
						tv1.setText("Pitch off");
					}
				}
			});

			checkBox2 = (CheckBox) findViewById(R.id.checkBox2);
			checkBox2.setOnClickListener(new OnClickListener() {
				public void onClick(View v) {
					if (checkBox2.isChecked()) {
						com.Send(CMD_REVERSE);
						com.Send(1);
						tv1.setText("Reverse on");
					} else {
						com.Send(CMD_REVERSE);
						com.Send(0);
						tv1.setText("Reverse off");
					}
				}
			});

		} catch (Throwable t) {
			t.toString();
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

}
