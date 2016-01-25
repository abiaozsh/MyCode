package Camera;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;
import android.util.Log;

public class MyCamera {
	public static String ComputerIP = "192.168.43.46";
	// public static String ComputerIP = "192.168.0.10";

	public Camera c;

	public SendData sendData = new SendData();

	public class SendData {
		public double currentPower = 0;

		public double adjGryoxConst = 0;
		public double adjGryoyConst = 0;
		public double adjGryozConst = 0;
		public double adjGryoxTemp = 0;
		public double adjGryoyTemp = 0;
		public double adjGryozTemp = 0;

		public int poweron = 0;

		public double pwm1;// up +
		public double pwm2;// down +
		public double pwm3;// left -
		public double pwm4;// right -
		public double minPower = 10;

		public double CurrGryoxAccum = 0;
		public double CurrGryoyAccum = 0;
		public double CurrGryozAccum = 0;
		public double CurrGryox;
		public double CurrGryoy;
		public double CurrGryoz;
		public byte data1;
		public byte data2;
		public byte data3;
		public byte data4;

		public String Message;
	}

	public void Init() {
		int id = 0;
		for (int i = 0; i < Camera.getNumberOfCameras(); i++) {
			Camera.CameraInfo info = new Camera.CameraInfo();
			Camera.getCameraInfo(i, info);
			if (info.facing == CameraInfo.CAMERA_FACING_FRONT) {
				// if (info.facing == CameraInfo.CAMERA_FACING_BACK) {
				id = i;
			}
		}
		c = Camera.open(id);

		try {
			c.setPreviewCallback(new StreamIt(this)); // 设置回调的类
			c.startPreview(); // 开始预览
			c.autoFocus(null); // 自动对焦
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

class StreamIt implements Camera.PreviewCallback {
	static int i = 0;
	MyCamera mc;

	MyThread[] threadPool = new MyThread[5];

	public StreamIt(MyCamera mc) {
		this.mc = mc;
	}

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		i++;
		if (i == 1) {
			i = 0;
			Parameters param = camera.getParameters();
			Size size = param.getPreviewSize();
			int format = param.getPreviewFormat();// ImageFormat.NV21
			try {
				YuvImage image = new YuvImage(data, format, size.width, size.height, null);
				StringBuffer sb = new StringBuffer();

				sb.append("currentPower:" + Double.toString(mc.sendData.currentPower) + "\r\n");

				sb.append("adjGryoxConst:" + Double.toString(mc.sendData.adjGryoxConst) + "\t");
				sb.append("adjGryoyConst:" + Double.toString(mc.sendData.adjGryoyConst) + "\t");
				sb.append("adjGryozConst:" + Double.toString(mc.sendData.adjGryozConst) + "\r\n");
				sb.append("adjGryoxTemp:" + Double.toString(mc.sendData.adjGryoxTemp) + "\t");
				sb.append("adjGryoyTemp:" + Double.toString(mc.sendData.adjGryoyTemp) + "\t");
				sb.append("adjGryozTemp:" + Double.toString(mc.sendData.adjGryozTemp) + "\r\n");

				sb.append("poweron:" + Integer.toString(mc.sendData.poweron) + "\r\n");

				sb.append("pwm1:" + Double.toString(mc.sendData.pwm1) + "\t");
				sb.append("pwm2:" + Double.toString(mc.sendData.pwm2) + "\t");
				sb.append("pwm3:" + Double.toString(mc.sendData.pwm3) + "\t");
				sb.append("pwm4:" + Double.toString(mc.sendData.pwm4) + "\r\n");
				sb.append("minPower:" + Double.toString(mc.sendData.minPower) + "\r\n");

				sb.append("CurrGryoxAccum:" + Double.toString(mc.sendData.CurrGryoxAccum) + "\t");
				sb.append("CurrGryoyAccum:" + Double.toString(mc.sendData.CurrGryoyAccum) + "\t");
				sb.append("CurrGryozAccum:" + Double.toString(mc.sendData.CurrGryozAccum) + "\r\n");
				sb.append("CurrGryox:" + Double.toString(mc.sendData.CurrGryox) + "\t");
				sb.append("CurrGryoy:" + Double.toString(mc.sendData.CurrGryoy) + "\t");
				sb.append("CurrGryoz:" + Double.toString(mc.sendData.CurrGryoz) + "\r\n");

				sb.append("data1:" + Byte.toString(mc.sendData.data1) + "\t");
				sb.append("data2:" + Byte.toString(mc.sendData.data2) + "\t");
				sb.append("data3:" + Byte.toString(mc.sendData.data3) + "\t");
				sb.append("data4:" + Byte.toString(mc.sendData.data4) + "\r\n");

				sb.append("msg:" + mc.sendData.Message + "\r\n");

				try {
					aa: for (int i = 0; i < 5; i++) {
						if (threadPool[i] == null || threadPool[i].free) {
							threadPool[i] = new MyThread();
							threadPool[i].SetData(image, size, sb.toString().getBytes());
							threadPool[i].start();
							break aa;
						}
					}
				} catch (Throwable e) {
					e.toString();
				}
			} catch (Exception ex) {
				Log.e("Sys", "Error:" + ex.getMessage());
			}
		}
	}
}

class MyThread extends Thread {
	public YuvImage image;
	public byte[] dataSend;
	public Size size;
	public boolean free = false;

	public MyThread() {
	}

	public void SetData(YuvImage image, Size size, byte[] dataSend) {
		this.image = image;
		this.dataSend = dataSend;
		this.size = size;
	}

	public void run() {
		try {
			// 将图像数据通过Socket发送出去
			Socket tempSocket = new Socket(MyCamera.ComputerIP, 6000);
			OutputStream outstream = tempSocket.getOutputStream();
			byte[] len = new byte[2];
			len[0] = (byte) (dataSend.length & 0xFF);
			len[1] = (byte) ((dataSend.length >> 8) & 0xFF);
			outstream.write(len);
			outstream.write(dataSend);
			image.compressToJpeg(new Rect(0, 0, size.width, size.height), 80, outstream);

			outstream.flush();
			outstream.close();
			tempSocket.close();
		} catch (IOException e) {
		} catch (Throwable e) {
		} finally {
			free = true;
		}
	}

}