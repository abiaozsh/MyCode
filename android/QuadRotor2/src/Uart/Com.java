package Uart;

import android.app.Activity;
import android.content.Context;
import android.hardware.usb.UsbManager;

import com.wch.wchusbdriver.CH34xAndroidDriver;

public class Com {
	public CH34xAndroidDriver com;

	private static final String ACTION_USB_PERMISSION = "com.wch.wchusbdriver.USB_PERMISSION";

	public Com(Activity activity) {
		com = new CH34xAndroidDriver((UsbManager) activity.getSystemService(Context.USB_SERVICE), activity, ACTION_USB_PERMISSION);
		if (!com.UsbFeatureSupported()) {
			com = null;
		}
	}

	public void Init() {
		int baudRate = 9600;
		byte dataBit = 8;
		byte stopBit = 1;
		byte parity = 0;
		byte flowControl = 0;
		if (com.isConnected()) {
			com.UartInit();
			com.SetConfig(baudRate, dataBit, stopBit, parity, flowControl);
		} else {
		}
	}

	//0~1000
	public void Write(int channel, int data) {
        byte[] buff = new byte[3];
        buff[0] = (byte)(0x0D4 | channel);
        buff[1] = (byte)(data & 0x0FF);
        buff[2] = (byte)((data >> 8) & 0x0FF);

		try {
			com.WriteData(buff, 3);
		} catch (Exception e) {
			// Toast.makeText(this.activity, "send Error",
			// Toast.LENGTH_SHORT).show();

		}
	}

	public void ResumeUsbList() {
		try {
			if (2 == com.ResumeUsbList()) {
				com.CloseDevice();
				// Toast.makeText(this.activity, "err",
				// Toast.LENGTH_SHORT).show();
			}
		} catch (Exception e) {
			// Toast.makeText(this.activity, "send Error",
			// Toast.LENGTH_SHORT).show();

		}
	}

}
