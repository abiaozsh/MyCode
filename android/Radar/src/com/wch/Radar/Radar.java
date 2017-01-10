package com.wch.Radar;

import java.io.FileInputStream;
import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class Radar extends Activity {
	public ImgContainer imgContainer = new ImgContainer();

	int POS = 1;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		Button testButton = (Button) findViewById(R.id.testButton);
		testButton.setOnClickListener(new TestListener());

		Button LD1H = (Button) findViewById(R.id.LD1H);
		LD1H.setOnClickListener(new LD1H());

		Button LD2H = (Button) findViewById(R.id.LD2H);
		LD2H.setOnClickListener(new LD2H());

		Button LD1D = (Button) findViewById(R.id.LD1D);
		LD1D.setOnClickListener(new LD1D());

		Button LD2D = (Button) findViewById(R.id.LD2D);
		LD2D.setOnClickListener(new LD2D());

		Button QP = (Button) findViewById(R.id.QP);
		QP.setOnClickListener(new QP());

		Button NH = (Button) findViewById(R.id.NH);
		NH.setOnClickListener(new NH());

		Button HD = (Button) findViewById(R.id.HD);
		HD.setOnClickListener(new HD());

		Button ClearAll = (Button) findViewById(R.id.ClearAll);
		ClearAll.setOnClickListener(new ClearAll());

		String[] fl = fileList();
		long totalsize = 0;
		for (int i = 0; i < fl.length; i++) {
			FileInputStream fis = null;
			try {
				fis = openFileInput(fl[i]);
				totalsize += fis.available();
				fis.close();
			} catch (IOException e) {
			}
		}

		double size = totalsize;

		size = size / 1024;

		size = size / 1024;

		TextView textView1 = (TextView) findViewById(R.id.textView1);
		String m = Double.toString(size);
		textView1.setText(m);

		handler = new MyHandler(this);

		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);

	}

	public class TestListener implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			NetThread netThread = new NetThread("test", handler, imgContainer, Radar.this, POS);
			netThread.start();
		}
	}

	public class QP implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			POS = 1;
			TextView textView1 = (TextView) findViewById(R.id.textView1);
			String m = "QP";
			textView1.setText(m);
		}
	}

	public class NH implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			POS = 2;
			TextView textView1 = (TextView) findViewById(R.id.textView1);
			String m = "NH";
			textView1.setText(m);
		}
	}

	public class HD implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			POS = 3;
			TextView textView1 = (TextView) findViewById(R.id.textView1);
			String m = "HD";
			textView1.setText(m);
		}
	}

	public class LD1H implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			NetThread netThread = new NetThread("ld1h", handler, imgContainer, Radar.this, POS);
			netThread.start();
		}
	}

	public class LD2H implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			NetThread netThread = new NetThread("ld2h", handler, imgContainer, Radar.this, POS);
			netThread.start();
		}
	}

	public class LD1D implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			NetThread netThread = new NetThread("ld1d", handler, imgContainer, Radar.this, POS);
			netThread.start();
		}
	}

	public class LD2D implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			NetThread netThread = new NetThread("ld2d", handler, imgContainer, Radar.this, POS);
			netThread.start();
		}
	}

	public class ClearAll implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			NetThread netThread = new NetThread("clearall", handler, imgContainer, Radar.this, POS);
			netThread.start();
		}
	}

	Handler handler;

	static class MyHandler extends Handler {
		Radar a;

		public MyHandler(Radar a) {
			this.a = a;
		}

		@Override
		public void handleMessage(Message msg) {

			if (msg.getData().getString("msg") != null) {
				TextView textView1 = (TextView) a.findViewById(R.id.textView1);
				String m = msg.getData().getString("msg");
				textView1.setText(m);
			}

			String cmd = msg.getData().getString("cmd");
			if ("updateimg".equals(cmd)) {
				ImageView img = (ImageView) a.findViewById(R.id.ImageView);
				img.setImageBitmap(a.imgContainer.bmp);
			}
		}
	};

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.uart_loop_back, menu);
		return true;
	}

}
