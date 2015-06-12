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
	//public static String ComputerIP = "192.168.0.10";

	public Camera c;
	public byte[] dataSend = new byte[64];

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

	public StreamIt(MyCamera mc) {
		this.mc = mc;
	}

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		i++;
		if (i == 3) {
			i = 0;
			Parameters param = camera.getParameters();
			Size size = param.getPreviewSize();
			int format = param.getPreviewFormat();// ImageFormat.NV21
			try {
				YuvImage image = new YuvImage(data, format, size.width, size.height, null);
				Thread th = new MyThread(image, size, mc.dataSend);
				th.start();
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

	public MyThread(YuvImage image, Size size, byte[] dataSend) {
		this.image = image;
		this.dataSend = dataSend;
		this.size = size;
	}

	public void run() {
		try {
			// 将图像数据通过Socket发送出去
			Socket tempSocket = new Socket(MyCamera.ComputerIP, 6000);
			OutputStream outstream = tempSocket.getOutputStream();
			outstream.write(dataSend);
			image.compressToJpeg(new Rect(0, 0, size.width, size.height), 80, outstream);

			outstream.flush();
			outstream.close();
			tempSocket.close();
		} catch (IOException e) {
		}
	}

}