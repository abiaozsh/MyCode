package com.wch.wchusbdriver;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.PictureCallback;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;

import com.wch.AutoCamera.R;

public class UartLoopBackActivity extends Activity {
	/* thread to read the data */
	public ReadThread handlerThread;
	private CameraSurfacePreview mCameraSurPreview = null;

	EditText Text1;
	EditText Text2;

	Button configButton;
	Button testButton;

	class ThreadControl extends Thread {
		@SuppressWarnings("resource")
		public void run() {

			// android.hardware.camera2
			// android.h

			ServerSocket ss = null;
			while (true) {
				if (ss == null) {
					try {
						ss = new ServerSocket(5000);
					} catch (Exception e) {
					}
				}
				try {
					Socket s = ss.accept();
					InputStream ins = s.getInputStream();
					OutputStream ous = s.getOutputStream();

					byte[] data = new byte[2];// CMD + DATA

					ins.read(data);

					if (data[0] == 10) {// get Current Data

					} else if (data[0] == 12) {// getFileList
						String[] fl = fileList();

						String dirs = "";
						for (int i = 0; i < fl.length; i++) {
							dirs += fl[i] + "\r\n";
						}
						SendData(dirs, ous);
					} else if (data[0] == 11) {// delete File
						String[] fl = fileList();
						int idx = data[1];
						UartLoopBackActivity.this.deleteFile(fl[idx]);
						SendData(fl[idx], ous);
					} else if (data[0] == 13) {// getFile
						String[] fl = fileList();
						int idx = data[1];
						SendData(fl[idx], ous);
						InputStream fis = UartLoopBackActivity.this.openFileInput(fl[idx]);
						byte[] filebuff = new byte[20480000];
						int len = fis.read(filebuff);
						SendData(filebuff, len, ous);
						fis.close();
					}

					s.close();
					// Message msg = myHandlerCmd.obtainMessage();
					// Bundle b = new Bundle();
					// b.putByteArray("data", data);
					// msg.setData(b);
					// myHandlerCmd.sendMessage(msg);
				} catch (Exception ex) {

				}
			}

		}

		private void SendData(String sdata, OutputStream ous) {
			try {
				byte[] sizebuff = new byte[1];
				byte[] sendBuff = null;

				sendBuff = sdata.getBytes();

				sizebuff[0] = (byte) (sendBuff.length & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((sendBuff.length >> 8) & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((sendBuff.length >> 16) & 0x0FF);
				ous.write(sizebuff);

				ous.write(sendBuff);
			} catch (Exception e) {
			}
		}

		private void SendData(byte[] sendBuff, int length, OutputStream ous) {
			try {
				if (length <= 0) {
					ous.write(0);
					ous.write(0);
					ous.write(0);
				}
				byte[] sizebuff = new byte[1];

				sizebuff[0] = (byte) (length & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((length >> 8) & 0x0FF);
				ous.write(sizebuff);
				sizebuff[0] = (byte) ((length >> 16) & 0x0FF);
				ous.write(sizebuff);

				ous.write(sendBuff, 0, length);
			} catch (Exception e) {
			}

		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		/* create editable text objects */
		Text1 = (EditText) findViewById(R.id.Text1);
		Text2 = (EditText) findViewById(R.id.Text2);

		configButton = (Button) findViewById(R.id.configButton);
		configButton.setOnClickListener(new OpenDeviceListener());

		testButton = (Button) findViewById(R.id.testButton);
		testButton.setOnClickListener(new TestListener());

		FrameLayout preview = (FrameLayout) findViewById(R.id.camera_preview);
		mCameraSurPreview = new CameraSurfacePreview(this);// , prevCallback
		preview.addView(mCameraSurPreview);

		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);

		ThreadControl control = new ThreadControl();
		control.start();

	}

	public class OpenDeviceListener implements View.OnClickListener {

		@Override
		public void onClick(View v) {
			try {

				Text1.setText("on");

				handlerThread = new ReadThread(handler);
				handlerThread.start();

			} catch (Exception ex) {
				Text1.setText(ex.toString());

			}
		}

	}

	// boolean takepic = false;

	public class TestListener implements View.OnClickListener {

		@Override
		public void onClick(View v) {

			// {
			// Camera.Parameters parameters = mCameraSurPreview.mCamera.getParameters();
			// parameters.setFlashMode(Camera.Parameters.FLASH_MODE_TORCH);
			// mCameraSurPreview.mCamera.setParameters(parameters);
			// }
			// try {
			// Thread.sleep(3000);
			// } catch (InterruptedException e) {
			// }

			mCameraSurPreview.takePicture(callback);
			// takepic = true;
			// try {
			// Thread.sleep(1000);
			// } catch (InterruptedException e) {
			// }
			// {
			// Camera.Parameters parameters = mCameraSurPreview.mCamera.getParameters();
			// parameters.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
			// mCameraSurPreview.mCamera.setParameters(parameters);
			// }

		}

	}

	final Camera.PictureCallback callback = new Camera.PictureCallback() {

		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			Text1.setText("start");
			try {
				Bitmap bm = BitmapFactory.decodeByteArray(data, 0, data.length);

				long date = System.currentTimeMillis();

				DateFormat sdf = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss");
				String filename = sdf.format(date) + ".jpg";

				FileOutputStream fos = openFileOutput(filename, Activity.MODE_APPEND);

				// image.compressToJpeg(new Rect(0, 0, size.width, size.height), 80, fos);
				bm.compress(Bitmap.CompressFormat.JPEG, 100, fos);

				fos.flush();
				fos.close();
				camera.startPreview();
				Text1.setText("done");
			} catch (Exception e) {
				Text1.setText(e.toString());
			}

		}

	};
	//
	// final Camera.PreviewCallback prevCallback = new Camera.PreviewCallback() {
	//
	// @Override
	// public void onPreviewFrame(byte[] data, Camera camera) {
	// if (takepic) {
	// Text1.setText("start");
	// try {
	// Parameters param = camera.getParameters();
	// Size size = param.getPreviewSize();
	// int format = param.getPreviewFormat();
	// YuvImage image = new YuvImage(data, format, size.width, size.height, null);
	//
	// long date = System.currentTimeMillis();
	//
	// DateFormat sdf = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss");
	// String filename = sdf.format(date) + ".jpg";
	//
	// FileOutputStream fos = openFileOutput(filename, Activity.MODE_APPEND);
	//
	// image.compressToJpeg(new Rect(0, 0, size.width, size.height), 100, fos);
	//
	// fos.flush();
	// fos.close();
	// camera.startPreview();
	// Text1.setText("done");
	// } catch (Exception e) {
	// Text1.setText(e.toString());
	// }
	//
	// /*
	// * Parameters param = camera.getParameters(); Size size = param.getPreviewSize(); int format = param.getPreviewFormat();// ImageFormat.NV21 try { YuvImage image = new YuvImage(data, format, size.width, size.height, null);
	// *
	// * } catch (Exception ex) { }
	// */
	// takepic = false;
	// }
	// }
	// };

	final Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			mCameraSurPreview.takePicture(callback);
		}
	};

	/* usb input data handler */
	private class ReadThread extends Thread {
		/* constructor */
		Handler mhandler;

		ReadThread(Handler h) {
			mhandler = h;
			this.setPriority(Thread.MIN_PRIORITY);
		}

		public void run() {
			while (true) {

				try {
					Thread.sleep(60000);
				} catch (InterruptedException e) {
				}

				Message msg = mhandler.obtainMessage();
				mhandler.sendMessage(msg);

			}
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.uart_loop_back, menu);
		return true;
	}

}

class CameraSurfacePreview extends SurfaceView implements SurfaceHolder.Callback {
	private SurfaceHolder mHolder;
	public Camera mCamera;
	public Camera c = null;
	public int F = CameraInfo.CAMERA_FACING_FRONT;
	public int B = CameraInfo.CAMERA_FACING_BACK;
	public int facing = B;

	// Camera.PreviewCallback prevCallback;

	public CameraSurfacePreview(Context context) {// , Camera.PreviewCallback prevCallback
		super(context);
		// this.prevCallback = prevCallback;
		// Install a SurfaceHolder.Callback so we get notified when the
		// underlying surface is created and destroyed.
		mHolder = getHolder();
		mHolder.addCallback(this);
		// deprecated setting, but required on Android versions prior to 3.0
		mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
	}

	public void surfaceCreated(SurfaceHolder holder) {

		// Open the Camera in preview mode
		int id = 0;
		for (int i = 0; i < Camera.getNumberOfCameras(); i++) {
			Camera.CameraInfo info = new Camera.CameraInfo();
			Camera.getCameraInfo(i, info);
			if (info.facing == facing) {
				id = i;
			}
		}
		mCamera = Camera.open(id);
		Camera.Parameters parameters = mCamera.getParameters();
		// parameters.setFlashMode(Camera.Parameters.FLASH_MODE_ON);
		parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
		// parameters.setPreviewSize(2560, 1920);
		mCamera.setParameters(parameters);
//		try {
//			mCamera.setPreviewDisplay(holder);
//		} catch (IOException e) {
//		}
		// mCamera.setPreviewCallback(prevCallback);
		mCamera.setPreviewCallback(null);
		mCamera.startPreview();

	}

	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

		try {
			mCamera.startPreview();

		} catch (Exception e) {

		}
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		if (mCamera != null) {
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
		}

	}

	public void takePicture(PictureCallback imageCallback) {
		mCamera.takePicture(null, null, imageCallback);
	}
}