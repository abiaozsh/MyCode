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
	//public static String ComputerIP = "192.168.43.46";
	// public static String ComputerIP = "192.168.0.10";

	public Camera c;

	public SendData sendData = new SendData();

	public class SendData {
		public double currentPower = 0;

		public int poweron = 0;

		public float adjxConst = 0;
		public float adjyConst = 0;
		public String senseData;

		public double pwm1;// up +
		public double pwm2;// down +
		public double pwm3;// left -
		public double pwm4;// right -
		public double minPower = 10;

		public double gravityx = 0;
		public double gravityy = 0;
		public double gravityxAccum = 0;
		public double gravityyAccum = 0;

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
				
				String fmt = "%0+11.6f";
				String fmt2 = "%0+5.1f";
								
				sb.append("currentPower:" + String.format(fmt,mc.sendData.currentPower) + "\r\n");

				sb.append("adjxConst:" + String.format(fmt,mc.sendData.adjxConst) + "\t");
				sb.append("adjyConst:" + String.format(fmt,mc.sendData.adjyConst) + "\r\n");
				sb.append(mc.sendData.senseData + "\r\n");

				sb.append("poweron:" + Integer.toString(mc.sendData.poweron) + "\r\n");

				// up 2
				// left 4 right 3
				// down 1
				sb.append("\r\n");
				sb.append("     pwm2:" + String.format(fmt2,mc.sendData.pwm2) + "\r\n");
				sb.append("pwm4:" + String.format(fmt2,mc.sendData.pwm4) + "\t");
				sb.append("pwm3:" + String.format(fmt2,mc.sendData.pwm3) + "\r\n");
				sb.append("     pwm1:" + String.format(fmt2,mc.sendData.pwm1) + "\r\n");
				sb.append("\r\n");
				sb.append("minPower:" + String.format(fmt,mc.sendData.minPower) + "\r\n");

				sb.append("gravityx:" + String.format(fmt,mc.sendData.gravityx) + "\t");
				sb.append("gravityy:" + String.format(fmt,mc.sendData.gravityy) + "\r\n");
				sb.append("gravityxAccum:" +String.format(fmt,mc.sendData.gravityxAccum) + "\t");
				sb.append("gravityyAccum:" + String.format(fmt,mc.sendData.gravityyAccum) + "\r\n");

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