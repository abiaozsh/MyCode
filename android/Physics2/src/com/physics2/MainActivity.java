package com.physics2;

import java.util.ArrayList;

import mySensor.MySensor;
import mySensor.MySensorListener;
import Core.PDot;
import Core.PLine;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.ImageView;

public class MainActivity extends Activity implements OnTouchListener,
		MySensorListener {
	MySensor ms;
	private ImageView image;
	private Paint paint;
	private Canvas canvas;
	private Bitmap bitmap;

	int status = 0;// 1=design 2=play

	ArrayList<PDot> dList = new ArrayList<PDot>();
	ArrayList<PLine> lineList = new ArrayList<PLine>();

	double gravityx = 0;
	double gravityy = -0.1;
	double airDrag = 0.99;

	int height = 1050;
	int width = 650;
	public Thread t;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		image = (ImageView) findViewById(R.id.imageView1);
		bitmap = Bitmap.createBitmap(width, height, Config.ARGB_8888);
		image.setImageBitmap(bitmap);
		image.setOnTouchListener(this);
		paint = new Paint();
		paint.setColor(0xFFFF0000);
		paint.setStrokeWidth(2);
		canvas = new Canvas(bitmap);
		init();
		t = new Thread(new ThreadShow());
		t.start();
		ms = new MySensor(this, this);
		status = 1;
	}

	int tmpx;
	int tmpy;
	int running = 0;

	public boolean onTouch(View v, MotionEvent event) {
		int action = event.getAction();
		switch (action) {
		case MotionEvent.ACTION_DOWN:
			running = 0;
			tmpx = (int) event.getX();
			tmpy = (int) (height - event.getY());
			break;
		case MotionEvent.ACTION_MOVE:

			break;
		case MotionEvent.ACTION_UP:

			int x = (int) event.getX();
			int y = (int) (height - event.getY());
			int headingx = (x - tmpx);
			int headingy = (y - tmpy);

			double dist = Math.sqrt(headingx * headingx + headingy * headingy);
			if (dist < 0.01)
				dist = 0.01;
			double arc = Math.asin((double) headingx / dist);
			if (headingx > 0 && headingy < 0) {
				arc = 3.14159265 - arc;
			} else if (headingx < 0 && headingy > 0) {
				arc = 3.14159265 * 2 + arc;
			} else if (headingx < 0 && headingy < 0) {
				arc = 3.14159265 - arc;
			}

			dList = new ArrayList<PDot>();
			PDot d;

			for (double i = -0.5; i < 0.5; i += 0.1) {
				d = new PDot();
				d.posx = tmpx;
				d.posy = tmpy;
				d.headingx = Math.sin(arc + i) * dist * 0.2;
				d.headingy = Math.cos(arc + i) * dist * 0.2;
				dList.add(d);
			}

			running = 1;

			break;

		default:
			break;
		}
		return true;
	}

	public void onSensorChanged() {
		// d3dDevice.world.getCamera().setPosition(0,50, 0);
		// TextView tv = (TextView) this.findViewById(R.id.textView1);
		// tv.setText(sensorValueX + "," + sensorValueY + "," + sensorValueZ);
		gravityx = -ms.g_x / 10;
		gravityy = -ms.g_y / 10;
	}

	// handler���������
	Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			if (msg.what == 1) {
				image.invalidate();// image.setImageBitmap(bitmap);//canvas.save();
			}
		};
	};

	// �߳���
	class ThreadShow implements Runnable {

		@Override
		public void run() {
			while (true) {
				try {
					Thread.sleep(30);
					if (running == 0) {
						continue;
					}
					canvas.drawARGB(255, 255, 255, 255);

					for (PLine l : lineList) {
						canvas.drawLine((float) l.x1, (float) (height - l.y1),
								(float) l.x2, (float) (height - l.y2), paint);
					}

					for (PDot d : dList) {
						if (!d.CheckTouch(lineList)) {
							d.headingx *= airDrag;
							d.headingy *= airDrag;
							d.headingx = d.headingx + gravityx;
							d.headingy = d.headingy + gravityy;
							d.posx += d.headingx;
							d.posy += d.headingy;
						}
						float x1 = (float) d.posx;
						float y1 = (float) (height - d.posy);
						float x2 = (float) (d.posx + d.headingx);
						float y2 = (float) (height - d.posy - d.headingy);

						canvas.drawRect(x2, y2, x2 + 2, y2 + 2, paint);
						canvas.drawLine(x1, y1, x2, y2, paint);
					}

					Message msg = new Message();
					msg.what = 1;
					handler.sendMessage(msg);

				} catch (Exception e) {
					e.printStackTrace();
					System.out.println("thread error...");
				}
			}
		}
	}

	private void init() {

		lineList = new ArrayList<PLine>();
		PLine l;

		l = new PLine();
		l.x1 = 1;
		l.y1 = 1;
		l.x2 = width - 2;
		l.y2 = 1;
		lineList.add(l);
		l = new PLine();
		l.x1 = 1;
		l.y1 = 1;
		l.x2 = 1;
		l.y2 = height - 2;
		lineList.add(l);
		l = new PLine();
		l.x1 = 1;
		l.y1 = height - 2;
		l.x2 = width - 2;
		l.y2 = height - 2;
		lineList.add(l);
		l = new PLine();
		l.x1 = width - 2;
		l.y1 = 1;
		l.x2 = width - 2;
		l.y2 = height - 2;
		lineList.add(l);

		l = new PLine();
		l.x1 = 37;
		l.y1 = 68;
		l.x2 = 71;
		l.y2 = 29;
		lineList.add(l);

		l = new PLine();
		l.x1 = 236;
		l.y1 = 27;
		l.x2 = 276;
		l.y2 = 58;
		lineList.add(l);

		l = new PLine();
		l.x1 = 111;
		l.y1 = 159;
		l.x2 = 216;
		l.y2 = 160;
		lineList.add(l);

		l = new PLine();
		l.x1 = 56;
		l.y1 = 127;
		l.x2 = 92;
		l.y2 = 101;
		lineList.add(l);

		l = new PLine();
		l.x1 = 207;
		l.y1 = 86;
		l.x2 = 244;
		l.y2 = 130;
		lineList.add(l);

		l = new PLine();
		l.x1 = 152;
		l.y1 = 70;
		l.x2 = 150;
		l.y2 = 32;
		lineList.add(l);

		l = new PLine();
		l.x1 = 78;
		l.y1 = 79;
		l.x2 = 144;
		l.y2 = 109;
		lineList.add(l);

		l = new PLine();
		l.x1 = 175;
		l.y1 = 91;
		l.x2 = 211;
		l.y2 = 62;
		lineList.add(l);

		l = new PLine();
		l.x1 = 104;
		l.y1 = 41;
		l.x2 = 80;
		l.y2 = 17;
		lineList.add(l);

		l = new PLine();
		l.x1 = 206;
		l.y1 = 39;
		l.x2 = 227;
		l.y2 = 14;
		lineList.add(l);

	}

}