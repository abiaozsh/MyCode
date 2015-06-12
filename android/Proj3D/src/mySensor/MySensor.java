package mySensor;

import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import com.threed.jpct.Camera;
import com.threed.jpct.SimpleVector;

public class MySensor implements SensorEventListener {
	protected SensorManager sensorManager;
	Sensor gyro;
	Sensor acce;
	Sensor gyro2;

	// float g = 9.8f;

	MySensorListener msl;

	public Camera c;
	// public Camera g;
	public SimpleVector mov = new SimpleVector();

	public MySensor(MySensorListener msl, Activity activity) {
		sensorManager = (SensorManager) activity
				.getSystemService(Context.SENSOR_SERVICE);

		List<Sensor> list = sensorManager.getSensorList(Sensor.TYPE_ALL);

		for (Sensor item : list) {
			if (item.getType() == Sensor.TYPE_GYROSCOPE
					&& !item.getVendor().startsWith("Google")) {
				gyro = item;
			}
			if (item.getType() == Sensor.TYPE_GYROSCOPE
					&& item.getVendor().startsWith("Google")) {
				gyro2 = item;
			}

			if (item.getType() == Sensor.TYPE_LINEAR_ACCELERATION
					&& item.getVendor().startsWith("Google")) {
				acce = item;
			}

			// if (item.getType() == Sensor.TYPE_ACCELEROMETER
			// && !item.getVendor().startsWith("Google")) {
			// acce = item;
			// }
		}

		c = new Camera();
		// g = new Camera();

		this.msl = msl;

		int delay = SensorManager.SENSOR_DELAY_FASTEST;
		sensorManager.registerListener(this, gyro, delay);
		//sensorManager.registerListener(this, acce, delay);
		// sensorManager.registerListener(this, gyro2, delay);

		resetM();

	}

	public float chgX;
	public float chgY;
	public float chgZ;
	
	int Cali_cnt_Gyro = 0;
	int Cali_cnt_Acce = 0;
	float zero_x = 0;
	float zero_y = 0;
	float zero_z = 0;
	int Cali_cnt_Gyrox = 0;
	int Cali_cnt_Gyroy = 0;
	int Cali_cnt_Gyroz = 0;
	float cali_x = 100;
	float cali_y = 100;
	float cali_z = 100;
	float g_x = 0;
	float g_y = 0;
	float g_z = 0;

	public void CalibrateGyro() {
		Cali_cnt_Gyro = 100;
		Cali_cnt_Acce = 100;
	}

	private void CalibrateGyro(SensorEvent e) {
		float x = e.values[0];
		float y = e.values[1];
		float z = e.values[2];
		if (Cali_cnt_Gyro > 0) {
			Cali_cnt_Gyro--;

			if (Cali_cnt_Gyro == 99) {
				zero_x = 0;
				zero_y = 0;
				zero_z = 0;
			}
			if (Cali_cnt_Gyro < 90) {
				zero_x += x;
				zero_y += y;
				zero_z += z;
			}
			if (Cali_cnt_Gyro == 0) {
				zero_x /= 90;
				zero_y /= 90;
				zero_z /= 90;
			}
		}
	}

	private void CalibrateAcce(SensorEvent e) {
		float x = e.values[0];
		float y = e.values[1];
		float z = e.values[2];
		if (Cali_cnt_Acce > 0) {
			Cali_cnt_Acce--;

			if (Cali_cnt_Acce == 99) {
				g_x = 0;
				g_y = 0;
				g_z = 0;
			}
			if (Cali_cnt_Acce < 90) {
				g_x += x;
				g_y += y;
				g_z += z;
			}
			if (Cali_cnt_Acce == 0) {
				g_x /= 90;
				g_y /= 90;
				g_z /= 90;
				mov.x = 0;
				mov.y = 0;
				mov.z = 0;
			}
		}
	}

	public void resetM() {
		c.setPosition(0, 0, 0);
		c.setOrientation(SimpleVector.create(0, 1, 0),
				SimpleVector.create(0, 0, 1));
	}

	public void CalibrateGyroxStart() {
		Cali_cnt_Gyrox = 1;
		cali_x = 0;
	}

	public void CalibrateGyroxEnd() {
		Cali_cnt_Gyrox = 0;
		cali_x = Math.abs(cali_x) / (3.1415926f * 2f);
	}

	public void CalibrateGyroyStart() {
		Cali_cnt_Gyroy = 1;
		cali_y = 0;
	}

	public void CalibrateGyroyEnd() {
		Cali_cnt_Gyroy = 0;
		cali_y = Math.abs(cali_y) / (3.1415926f * 2f);
	}

	public void CalibrateGyrozStart() {
		Cali_cnt_Gyroz = 1;
		cali_z = 0;
	}

	public void CalibrateGyrozEnd() {
		Cali_cnt_Gyroz = 0;
		cali_z = Math.abs(cali_z) / (3.1415926f * 2f);
	}

	public void CalibrateAcce() {

	}

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// accuracy = accuracy;
	}

	public void onSensorChanged(SensorEvent e) {
		if (e.sensor == gyro) {
			if (Cali_cnt_Gyro > 0) {
				CalibrateGyro(e);
				return;
			}
			float x = e.values[0];
			float y = e.values[1];
			float z = e.values[2];
			
			chgX = x;
			chgY = y;
			chgZ = z;

			x -= zero_x;
			y -= zero_y;
			z -= zero_z;

			if (Cali_cnt_Gyrox > 0) {
				cali_x += x;
				return;
			}
			if (Cali_cnt_Gyroy > 0) {
				cali_y += y;
				return;
			}
			if (Cali_cnt_Gyroz > 0) {
				cali_z += z;
				return;
			}

			float ax = x / cali_x;
			float ay = y / cali_y;
			float az = z / cali_z;

			float r = (float) Math.sqrt(ax * ax + ay * ay + az * az);
			SimpleVector v = SimpleVector.create(-ax / r, ay / r, az / r);
			c.rotateCameraAxis(v, r);

			msl.onSensorChanged();
		} else if (e.sensor == acce) {
			if (Cali_cnt_Acce > 0) {
				CalibrateAcce(e);
				return;
			}

			float x = e.values[0];
			float y = e.values[1];
			float z = e.values[2];

			x -= g_x;
			y -= g_y;
			z -= g_z;

			x/=100f;
			y/=100f;
			z/=100f;
			
			 mov.x += x;
			 mov.y += y;
			 mov.z += z;

			c.moveCamera(Camera.CAMERA_MOVELEFT, mov.x);
			c.moveCamera(Camera.CAMERA_MOVEIN, mov.y);
			c.moveCamera(Camera.CAMERA_MOVEUP, mov.z);

			msl.onSensorChanged();
		}
	}
}
