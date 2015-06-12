package com.example.proj3d;

import java.io.IOException;
import java.util.Timer;

import javax.microedition.khronos.opengles.GL10;

import mySensor.MySensor;
import mySensor.MySensorListener;
import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;

import com.threed.jpct.Camera;
import com.threed.jpct.Object3D;
import com.threed.jpct.Primitives;
import com.threed.jpct.Texture;
import com.threed.jpct.TextureInfo;
import com.threed.jpct.TextureManager;
import com.threed.jpct.World;

import d3d.D3DDevice;
import d3d.OnRefreshListener;

public class MainActivity extends Activity implements OnRefreshListener, MySensorListener {
	protected D3DDevice d3dDevice;
	MySensor ms;

	Timer t;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		d3dDevice = new D3DDevice(this, this);
		t = new Timer();
	}

	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.main, menu);
		return true;
	}

	public boolean onOptionsItemSelected(MenuItem item) {
		int item_id = item.getItemId();
		if (ms == null) {
			return false;
		}
		switch (item_id) {
		case R.id.Reset:
			ms.resetM();
			break;
		case R.id.Cali:
			ms.CalibrateGyro();
			break;
		case R.id.CaliXStart:
			ms.CalibrateGyroxStart();
			break;
		case R.id.CaliYStart:
			ms.CalibrateGyroyStart();
			break;
		case R.id.CaliZStart:
			ms.CalibrateGyrozStart();
			break;
		case R.id.CaliXEnd:
			ms.CalibrateGyroxEnd();
			break;
		case R.id.CaliYEnd:
			ms.CalibrateGyroyEnd();
			break;
		case R.id.CaliZEnd:
			ms.CalibrateGyrozEnd();
			break;
		default:
			break;
		}
		return true;
	}

	private Object3D[] cubes = new Object3D[6];

	public void onSurfaceChanged(World world) {
		try {
			TextureManager tm = TextureManager.getInstance();
			AssetManager am = getResources().getAssets();
			Texture t;
			t = new Texture(am.open("1.bmp"), false);
			tm.addTexture("t1", t);
			t = new Texture(am.open("2.bmp"), false);
			tm.addTexture("t2", t);
			t = new Texture(am.open("3.bmp"), false);
			tm.addTexture("t3", t);
			t = new Texture(am.open("4.bmp"), false);
			tm.addTexture("t4", t);
			t = new Texture(am.open("5.bmp"), false);
			tm.addTexture("t5", t);
			t = new Texture(am.open("6.bmp"), false);
			tm.addTexture("t6", t);

			for (int i = 0; i < 6; i++) {
				cubes[i] = Primitives.getCube(1);
				cubes[i].calcTextureWrapSpherical();
			}

			float v = 10;
			TextureInfo ti;

			cubes[0].translate(0, v, 0);
			ti = new TextureInfo(tm.getTextureID("t1"));
			cubes[0].setTexture(ti);

			cubes[1].translate(0, -v, 0);
			ti = new TextureInfo(tm.getTextureID("t2"));
			cubes[1].setTexture(ti);

			cubes[2].translate(v, 0, 0);
			ti = new TextureInfo(tm.getTextureID("t3"));
			cubes[2].setTexture(ti);

			cubes[3].translate(-v, 0, 0);
			ti = new TextureInfo(tm.getTextureID("t4"));
			cubes[3].setTexture(ti);

			cubes[4].translate(0, 0, v);
			ti = new TextureInfo(tm.getTextureID("t5"));
			cubes[4].setTexture(ti);

			cubes[5].translate(0, 0, -v);
			ti = new TextureInfo(tm.getTextureID("t6"));
			cubes[5].setTexture(ti);

			for (int i = 0; i < 6; i++) {
				cubes[i].strip();
				cubes[i].build();

				world.addObject(cubes[i]);
			}

			// Object3D[] moons = Loader.load3DS(am.open("moon.3DS"), 1f);
			// for (Object3D moon : moons) {
			// world.addObject(moon);
			// }

			ms = new MySensor(this, this);
			d3dDevice.world.setCameraTo(ms.c);
			// d3dDevice.world.getCamera().setBack(ms.c.getBack());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void BeforeRender(GL10 gl) {

	}

	public void AfterRender(GL10 gl) {
		// d3dDevice.ang *= 0.99;
	}

	public void onSensorChanged() {
		// d3dDevice.world.getCamera().setPosition(0,50, 0);
		// TextView tv = (TextView) this.findViewById(R.id.textView1);
		// tv.setText(sensorValueX + "," + sensorValueY + "," + sensorValueZ);

	}

	float orix = 0;
	float oriy = 0;
	int mode = 0;
	float oldDist;

	@Override
	public boolean onTouchEvent(MotionEvent e) {
		float x = e.getRawX();
		float y = e.getRawY();
		int action = e.getAction() & MotionEvent.ACTION_MASK;

		if (action == MotionEvent.ACTION_DOWN) {
			orix = x;
			oriy = y;
			mode = 1;
		} else if (action == MotionEvent.ACTION_UP) {
			mode = 0;
		} else if (action == MotionEvent.ACTION_POINTER_UP) {
			mode -= 1;
		} else if (action == MotionEvent.ACTION_POINTER_DOWN) {
			oldDist = spacing(e);
			mode += 1;
		} else if (action == MotionEvent.ACTION_MOVE) {
			if (mode >= 2) {
				float newDist = spacing(e);
				zoom(newDist / oldDist);
				oldDist = newDist;
			} else {
				if (ms != null) {
					if (Math.abs(orix - x) < 50) {
						ms.c.moveCamera(Camera.CAMERA_MOVELEFT, (orix - x) / 100f);
					}

					if (Math.abs(oriy - y) < 50) {
						ms.c.moveCamera(Camera.CAMERA_MOVEUP, (oriy - y) / 100f);
					}

				}
			}
			orix = x;
			oriy = y;
		}

		return false;
	}

	private void zoom(float f) {
		if (ms != null) {
			ms.c.moveCamera(Camera.CAMERA_MOVEIN, (1 - f) / 1f);
		}
	}

	private float spacing(MotionEvent event) {
		float x = event.getX(0) - event.getX(1);
		float y = event.getY(0) - event.getY(1);
		return (float) Math.sqrt(x * x + y * y);
	}

}
