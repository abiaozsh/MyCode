package mySensor;

import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class MySensor implements SensorEventListener {
	protected SensorManager sensorManager;
	Sensor gyro;
	Sensor acce;

	MySensorListener msl;

	public MySensor(MySensorListener msl, Activity activity) {
		sensorManager = (SensorManager) activity.getSystemService(Context.SENSOR_SERVICE);

		List<Sensor> list = sensorManager.getSensorList(Sensor.TYPE_ALL);

		for (Sensor item : list) {

			if (item.getType() == Sensor.TYPE_GYROSCOPE && !item.getVendor().startsWith("Google")) {
				gyro = item;
			}

			if (item.getType() == Sensor.TYPE_ACCELEROMETER && !item.getVendor().startsWith("Google")) {
				acce = item;
			}
		}

		this.msl = msl;

		int delay = SensorManager.SENSOR_DELAY_FASTEST;
		sensorManager.registerListener(this, gyro, delay);
		sensorManager.registerListener(this, acce, delay);

	}

	public float adjX;
	public float adjY;
	public float adjZ;

	double CurrGryox = 0;
	double CurrGryoy = 0;
	double CurrGryoz = 0;
	double CurrAccex = 0;
	double CurrAccey = 0;
	double CurrAccez = 0;

	double Accex[] = new double[64];
	double Accey[] = new double[64];
	double Accez[] = new double[64];
	int dataIdx = 0;

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// accuracy = accuracy;
	}

	public void setAdj(float adjX, float adjY, float adjZ) {
		this.adjX = adjX;
		this.adjY = adjY;
		this.adjZ = adjZ;
	}

	public void onSensorChanged(SensorEvent e) {
		try {
			if (e.sensor == gyro) {

				CurrGryox = e.values[0];
				CurrGryoy = e.values[1];
				CurrGryoz = e.values[2];

			} else if (e.sensor == acce) {
				CurrAccex = e.values[0];
				CurrAccey = e.values[1];
				CurrAccez = e.values[2];

				Accex[dataIdx] = CurrAccex;
				Accey[dataIdx] = CurrAccey;
				Accez[dataIdx] = CurrAccez;

				double x = 0;
				double y = 0;
				double z = 0;
				double accu = 0;
				double fact = 1;
				int idx = dataIdx;
				for (int i = 0; i < 64; i++) {
					x += Accex[idx] * fact;
					y += Accey[idx] * fact;
					z += Accez[idx] * fact;
					accu += fact;
					fact = fact * 0.5;
					idx--;
					if (idx == -1) {
						idx = 63;
					}
				}

				dataIdx++;
				if (dataIdx == 64) {
					dataIdx = 0;
				}

				x /= accu;
				y /= accu;
				z /= accu;

				// 这边要反一下的
				x -= CurrGryoy;
				y += CurrGryox;
				// z += CurrGryoz / 5;

				x -= adjX;
				y -= adjY;
				z -= adjZ;
				msl.onSensorChanged(CurrGryox, CurrGryoy, CurrGryoz, CurrAccex, CurrAccey, CurrAccez, x, y, z);
			}
		} catch (Throwable t) {
			t.printStackTrace();
		}
	}
}
