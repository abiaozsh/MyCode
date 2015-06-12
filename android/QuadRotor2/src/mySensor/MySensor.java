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
	Sensor gyro2;

	// float g = 9.8f;

	MySensorListener msl;

	public MySensor(MySensorListener msl, Activity activity) {
		sensorManager = (SensorManager) activity.getSystemService(Context.SENSOR_SERVICE);

		List<Sensor> list = sensorManager.getSensorList(Sensor.TYPE_ALL);

		for (Sensor item : list) {
			if (item.getType() == Sensor.TYPE_GYROSCOPE && !item.getVendor().startsWith("Google")) {
				gyro = item;
			}
			if (item.getType() == Sensor.TYPE_GYROSCOPE && item.getVendor().startsWith("Google")) {
				gyro2 = item;
			}

			if (item.getType() == Sensor.TYPE_LINEAR_ACCELERATION && item.getVendor().startsWith("Google")) {
				acce = item;
			}

			// if (item.getType() == Sensor.TYPE_ACCELEROMETER
			// && !item.getVendor().startsWith("Google")) {
			// acce = item;
			// }
		}

		// c = new Camera();
		// g = new Camera();

		this.msl = msl;

		int delay = SensorManager.SENSOR_DELAY_FASTEST;
		sensorManager.registerListener(this, gyro, delay);
		sensorManager.registerListener(this, acce, delay);
		// sensorManager.registerListener(this, gyro2, delay);

	}

	public float chgX;
	public float chgY;
	public float chgZ;

	int Cali_cnt_Gyro = 0;
	int Cali_cnt_Acce = 0;
	float GyroCalix = 0;
	float GyroCaliy = 0;
	float GyroCaliz = 0;
	float AcceCalix = 0;
	float AcceCaliy = 0;
	float AcceCaliz = 0;

	double CurrGryox = 0;
	double CurrGryoy = 0;
	double CurrGryoz = 0;
	double CurrAccex = 0;
	double CurrAccey = 0;
	double CurrAccez = 0;

	public void ModifyGryo(double x, double y, double z) {
		CurrGryox += x;
		CurrGryoy += y;
		CurrGryoz += z;
	}

	public void CalibrateGyro() {
		Cali_cnt_Gyro = 1000;
		Cali_cnt_Acce = 100;
		CurrGryox = 0;
		CurrGryoy = 0;
		CurrGryoz = 0;
		CurrAccex = 0;
		CurrAccey = 0;
		CurrAccez = 0;
	}

	private void CalibrateGyro(SensorEvent e) {
		float x = e.values[0];
		float y = e.values[1];
		float z = e.values[2];
		if (Cali_cnt_Gyro > 0) {
			Cali_cnt_Gyro--;

			if (Cali_cnt_Gyro == 999) {
				GyroCalix = 0;
				GyroCaliy = 0;
				GyroCaliz = 0;
			}
			if (Cali_cnt_Gyro < 998) {
				GyroCalix += x;
				GyroCaliy += y;
				GyroCaliz += z;
			}
			if (Cali_cnt_Gyro == 0) {
				GyroCalix /= 998;
				GyroCaliy /= 998;
				GyroCaliz /= 998;
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
				AcceCalix = 0;
				AcceCaliy = 0;
				AcceCaliz = 0;
			}
			if (Cali_cnt_Acce < 90) {
				AcceCalix += x;
				AcceCaliy += y;
				AcceCaliz += z;
			}
			if (Cali_cnt_Acce == 0) {
				AcceCalix /= 90;
				AcceCaliy /= 90;
				AcceCaliz /= 90;
			}
		}
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

			x -= GyroCalix;
			y -= GyroCaliy;
			z -= GyroCaliz;

			CurrGryox += x;
			CurrGryoy += y;
			CurrGryoz += z;

			msl.onSensorChanged(CurrGryox, CurrGryoy, CurrGryoz, CurrAccex, CurrAccey, CurrAccez);
		} else if (e.sensor == acce) {
			if (Cali_cnt_Acce > 0) {
				CalibrateAcce(e);
				return;
			}

			float x = e.values[0];
			float y = e.values[1];
			float z = e.values[2];

			x -= AcceCalix;
			y -= AcceCaliy;
			z -= AcceCaliz;

			CurrAccex += x;
			CurrAccey += y;
			CurrAccez += z;

			msl.onSensorChanged(CurrGryox, CurrGryoy, CurrGryoz, CurrAccex, CurrAccey, CurrAccez);
		}
	}
}
