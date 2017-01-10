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

public class MyCamera {
	public static String ComputerIP = null;
	public static int Quality = 50;

	public Camera c = null;
	public int facing = CameraInfo.CAMERA_FACING_BACK;
	public int F = CameraInfo.CAMERA_FACING_FRONT;
	public int B = CameraInfo.CAMERA_FACING_BACK;

	MyThread[] threadPool = new MyThread[5];

	public void ClearPool() {
		for (int i = 0; i < 5; i++) {
			try {
				if (threadPool[i] != null && !threadPool[i].free) {
					threadPool[i].stop();
					threadPool[i] = null;
				}
			} catch (Throwable e) {
			}
		}
	}

	public void Init() {
		if (c != null) {
			c.stopPreview();
			c.setPreviewCallback(null);
			c.release();
			c = null;
		}
		int id = 0;
		for (int i = 0; i < Camera.getNumberOfCameras(); i++) {
			Camera.CameraInfo info = new Camera.CameraInfo();
			Camera.getCameraInfo(i, info);
			if (info.facing == facing) {
				id = i;
			}
		}

		try {
			c = Camera.open(id);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void prevOn() {
		c.setPreviewCallback(new StreamIt(this));
		c.startPreview();
	}

	public void prevOff() {
		c.setPreviewCallback(null);
		c.stopPreview();
	}

	public void Takepic() {
		c.setOneShotPreviewCallback(new StreamIt(this));
		c.startPreview();
	}

	public void setLed(int val) {
		Parameters param = c.getParameters();
		if(val==1){
			param.setFlashMode(Parameters.FLASH_MODE_TORCH);
		}else{
			param.setFlashMode(Parameters.FLASH_MODE_OFF);
		}
		c.setParameters(param);
	}
	
	
}

class StreamIt implements Camera.PreviewCallback {

	MyCamera mc;

	public StreamIt(MyCamera mc) {
		this.mc = mc;
	}

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		if (MyCamera.ComputerIP == null) {
			return;
		}
		try {
			
			try {
				aa: for (int i = 0; i < 5; i++) {
					if (mc.threadPool[i] == null || (mc.threadPool[i].free)) {
						mc.threadPool[i] = new MyThread();
						Parameters param = camera.getParameters();
						Size size = param.getPreviewSize();
						int format = param.getPreviewFormat();// ImageFormat.NV21
						YuvImage image = new YuvImage(data, format, size.width, size.height, null);
						mc.threadPool[i].SetData(image, size);
						mc.threadPool[i].start();
						break aa;
					}
				}
			} catch (Throwable e) {
			}
		} catch (Exception ex) {
		}
	}

}

class MyThread extends Thread {
	public YuvImage image;
	public Size size;
	public boolean free = false;

	public MyThread() {
	}

	public void SetData(YuvImage image, Size size) {
		this.image = image;
		this.size = size;
	}

	public void run() {
		try {
			// 将图像数据通过Socket发送出去
			Socket tempSocket = new Socket(MyCamera.ComputerIP, 6000);
			tempSocket.setSoTimeout(1000);
			
			OutputStream outstream = tempSocket.getOutputStream();
			
			outstream.write(2);// type==2
			image.compressToJpeg(new Rect(0, 0, size.width, size.height), MyCamera.Quality, outstream);

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