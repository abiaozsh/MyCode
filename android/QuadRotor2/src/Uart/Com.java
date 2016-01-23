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
		int baudRate = 115200;
		byte dataBit = 5;
		byte stopBit = 1;
		byte parity = 0;
		byte flowControl = 0;
		if (com.isConnected()) {
			com.UartInit();
			com.SetConfig(baudRate, dataBit, stopBit, parity, flowControl);
		} else {
		}
	}

	public void Send(byte dataA1, byte dataA2, byte dataB1, byte dataB2, byte dataC1, byte dataC2, byte dataD1, byte dataD2) {
		//phone
		///////////////////////////////////////////////////////////////////////////////////////
		//txtup.setText(p2);
		//txtrightleft.setText(p4 + "     " + p3);
		//txtdown.setText(p1);
		///////////////////////////////////////////////////////////////////////////////////////
		
		//board
		//up 2
		//left 4 right 3
		//down 1
		
		byte[] d = new byte[20];

		// 0x00;
		d[0] = (byte) ((((0x0001 & 0x01) >> 0) << 4) | (((0x0001 & 0x01) >> 0) << 3) | (((0x0001 & 0x01) >> 0) << 2) | (((0x0001 & 0x01) >> 0) << 1) | 0x01);// start bit
		d[1] = (byte) ((((dataD1 & 0x80) >> 7) << 4) | (((dataC1 & 0x80) >> 7) << 3) | (((dataB1 & 0x80) >> 7) << 2) | (((dataA1 & 0x80) >> 7) << 1) | 0x00);
		d[2] = (byte) ((((dataD1 & 0x40) >> 6) << 4) | (((dataC1 & 0x40) >> 6) << 3) | (((dataB1 & 0x40) >> 6) << 2) | (((dataA1 & 0x40) >> 6) << 1) | 0x01);
		d[3] = (byte) ((((dataD1 & 0x20) >> 5) << 4) | (((dataC1 & 0x20) >> 5) << 3) | (((dataB1 & 0x20) >> 5) << 2) | (((dataA1 & 0x20) >> 5) << 1) | 0x00);
		d[4] = (byte) ((((dataD1 & 0x10) >> 4) << 4) | (((dataC1 & 0x10) >> 4) << 3) | (((dataB1 & 0x10) >> 4) << 2) | (((dataA1 & 0x10) >> 4) << 1) | 0x01);
		d[5] = (byte) ((((dataD1 & 0x08) >> 3) << 4) | (((dataC1 & 0x08) >> 3) << 3) | (((dataB1 & 0x08) >> 3) << 2) | (((dataA1 & 0x08) >> 3) << 1) | 0x00);
		d[6] = (byte) ((((dataD1 & 0x04) >> 2) << 4) | (((dataC1 & 0x04) >> 2) << 3) | (((dataB1 & 0x04) >> 2) << 2) | (((dataA1 & 0x04) >> 2) << 1) | 0x01);
		d[7] = (byte) ((((dataD1 & 0x02) >> 1) << 4) | (((dataC1 & 0x02) >> 1) << 3) | (((dataB1 & 0x02) >> 1) << 2) | (((dataA1 & 0x02) >> 1) << 1) | 0x00);
		d[8] = (byte) ((((dataD1 & 0x01) >> 0) << 4) | (((dataC1 & 0x01) >> 0) << 3) | (((dataB1 & 0x01) >> 0) << 2) | (((dataA1 & 0x01) >> 0) << 1) | 0x01);
		d[9] = (byte) ((((0x0000 & 0x01) >> 0) << 4) | (((0x0000 & 0x01) >> 0) << 3) | (((0x0000 & 0x01) >> 0) << 2) | (((0x0000 & 0x01) >> 0) << 1) | 0x00);// stop bit

		d[10] = (byte) ((((0x0001 & 0x01) >> 0) << 4) | (((0x0001 & 0x01) >> 0) << 3) | (((0x0001 & 0x01) >> 0) << 2) | (((0x0001 & 0x01) >> 0) << 1) | 0x01);// start bit
		d[11] = (byte) ((((dataD2 & 0x80) >> 7) << 4) | (((dataC2 & 0x80) >> 7) << 3) | (((dataB2 & 0x80) >> 7) << 2) | (((dataA2 & 0x80) >> 7) << 1) | 0x00);
		d[12] = (byte) ((((dataD2 & 0x40) >> 6) << 4) | (((dataC2 & 0x40) >> 6) << 3) | (((dataB2 & 0x40) >> 6) << 2) | (((dataA2 & 0x40) >> 6) << 1) | 0x01);
		d[13] = (byte) ((((dataD2 & 0x20) >> 5) << 4) | (((dataC2 & 0x20) >> 5) << 3) | (((dataB2 & 0x20) >> 5) << 2) | (((dataA2 & 0x20) >> 5) << 1) | 0x00);
		d[14] = (byte) ((((dataD2 & 0x10) >> 4) << 4) | (((dataC2 & 0x10) >> 4) << 3) | (((dataB2 & 0x10) >> 4) << 2) | (((dataA2 & 0x10) >> 4) << 1) | 0x01);
		d[15] = (byte) ((((dataD2 & 0x08) >> 3) << 4) | (((dataC2 & 0x08) >> 3) << 3) | (((dataB2 & 0x08) >> 3) << 2) | (((dataA2 & 0x08) >> 3) << 1) | 0x00);
		d[16] = (byte) ((((dataD2 & 0x04) >> 2) << 4) | (((dataC2 & 0x04) >> 2) << 3) | (((dataB2 & 0x04) >> 2) << 2) | (((dataA2 & 0x04) >> 2) << 1) | 0x01);
		d[17] = (byte) ((((dataD2 & 0x02) >> 1) << 4) | (((dataC2 & 0x02) >> 1) << 3) | (((dataB2 & 0x02) >> 1) << 2) | (((dataA2 & 0x02) >> 1) << 1) | 0x00);
		d[18] = (byte) ((((dataD2 & 0x01) >> 0) << 4) | (((dataC2 & 0x01) >> 0) << 3) | (((dataB2 & 0x01) >> 0) << 2) | (((dataA2 & 0x01) >> 0) << 1) | 0x01);
		d[19] = (byte) ((((0x0000 & 0x01) >> 0) << 4) | (((0x0000 & 0x01) >> 0) << 3) | (((0x0000 & 0x01) >> 0) << 2) | (((0x0000 & 0x01) >> 0) << 1) | 0x00);// stop bit

		try {
			com.WriteData(d, 20);
		} catch (Exception e) {
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
