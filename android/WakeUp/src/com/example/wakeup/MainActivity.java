package com.example.wakeup;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.MenuItem;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public boolean onOptionsItemSelected(MenuItem item) {
		int item_id = item.getItemId();
		switch (item_id) {
		case R.id.Wake10: {
			TestOpenOS w = new TestOpenOS();
			w.mac = "00-50-04-1D-A2-B4";
			w.ipAddress = "192.168.0.10";
			w.sendOpenOS();
		}
			break;
		case R.id.Wake30: {
			TestOpenOS w = new TestOpenOS();
			w.mac = "00-50-04-1D-A2-B4";
			w.ipAddress = "192.168.0.30";
			w.sendOpenOS();
		}
			break;
		default:
			break;
		}
		return true;
	}

}

class TestOpenOS {
	public int port = 10000;
	public String mac = "00-50-04-1D-A2-B4";
	public String ipAddress = "255.255.255.255";

	public void sendOpenOS() {
		InetAddress destHost = null;
		DatagramSocket ds = null;
		try {
			destHost = InetAddress.getByName(ipAddress);
			byte[] destMac = getMacBytes(mac);
			byte[] magicBytes = new byte[102];
			// �����ݰ���ǰ��λ����0xFF����FF���Ķ�����
			// ��һ��BroadCast�����������ݰ����������ݾͿ����ˡ�
			// FF FF FF FF FF FF��6��FF�����ݵĿ�ʼ��
			// ������16��MAC��ַ�Ϳ����ˡ�
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < destMac.length; j++) {
					magicBytes[6 + destMac.length * i + j] = destMac[j];
					magicBytes[j] = (byte) 0xFF;
				}
			}

			// --------------------�鿴���ݱ�����������---------------------

			DatagramPacket dp = null;
			try {
				dp = new DatagramPacket(magicBytes, magicBytes.length, destHost, port);
				ds = new DatagramSocket();
				ds.send(dp);
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		} finally {
			if (null != ds) {
				// �ر����ݱ���
				ds.close();
			}
		}
	}

	private static byte[] getMacBytes(String macStr) throws IllegalArgumentException {
		byte[] bytes = new byte[6];
		String[] hex = macStr.split("(\\:|\\-)");
		if (hex.length != 6) {
			throw new IllegalArgumentException("Invalid MAC address.");
		}
		try {
			for (int i = 0; i < 6; i++) {
				bytes[i] = (byte) Integer.parseInt(hex[i], 16);
			}
		} catch (NumberFormatException e) {
			throw new IllegalArgumentException("Invalid hex digit in MAC address.");
		}
		return bytes;
	}

}