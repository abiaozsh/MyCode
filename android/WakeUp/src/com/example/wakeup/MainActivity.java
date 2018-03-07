package com.example.wakeup;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		String mac = "";
		String ip = "";
		try {
			FileInputStream fis = this.openFileInput("config.txt");
			InputStreamReader isr = new InputStreamReader(fis);
			BufferedReader br = new BufferedReader(isr);
			mac = br.readLine();
			ip = br.readLine();
			br.close();
			isr.close();
			fis.close();
		} catch (Throwable t) {
		}

		if ("".equals(mac)) {
			mac = "00-50-04-1D-A2-B4";

		}
		if ("".equals(ip)) {
			ip = "192.168.1.40";
		}

		{
			EditText et = (EditText) findViewById(R.id.editText1);
			et.setText(mac);
		}
		{
			EditText et = (EditText) findViewById(R.id.editText2);
			et.setText(ip);
		}

		{
			Button b = (Button) findViewById(R.id.buttonok);
			b.setOnClickListener(this);
		}

	}

	public void onClick(View v) {
		String mac = "";
		String ip = "";
		{
			EditText et = (EditText) findViewById(R.id.editText1);
			mac = et.getText().toString();
		}
		{
			EditText et = (EditText) findViewById(R.id.editText2);
			ip = et.getText().toString();
		}

		TestOpenOS w = new TestOpenOS();
		w.mac = mac;
		w.ipAddress = ip;
		w.sendOpenOS();

		try {
			FileOutputStream fos = this.openFileOutput("config.txt", MODE_PRIVATE);
			OutputStreamWriter osw = new OutputStreamWriter(fos);
			osw.write(mac + "\r\n");
			osw.write(ip + "\r\n");
			osw.close();
			fos.close();
			Toast.makeText(MainActivity.this, "ok!" + mac + " " + ip, Toast.LENGTH_LONG).show();
		} catch (Throwable t) {
		}

	}

	// public boolean onOptionsItemSelected(MenuItem item) {
	// int item_id = item.getItemId();
	// switch (item_id) {
	// case R.id.Wake10: {
	// TestOpenOS w = new TestOpenOS();
	// w.mac = "00-50-04-1D-A2-B4";
	// w.ipAddress = "192.168.0.10";
	// w.sendOpenOS();
	// }
	// break;
	// case R.id.Wake30: {
	// TestOpenOS w = new TestOpenOS();
	// w.mac = "00-50-04-1D-A2-B4";
	// w.ipAddress = "192.168.0.30";
	// w.sendOpenOS();
	// }
	// break;
	// default:
	// break;
	// }
	// return true;
	// }

}

class TestOpenOS {
	public int port = 10000;
	public String mac = "";
	public String ipAddress = "";

	public void sendOpenOS() {
		InetAddress destHost = null;
		DatagramSocket ds = null;
		try {
			destHost = InetAddress.getByName(ipAddress);
			byte[] destMac = getMacBytes(mac);
			byte[] magicBytes = new byte[102];
			// 将数据包的前六位放入0xFF即“FF”的二进制
			// 发一个BroadCast包，包的内容包括以下数据就可以了。
			// FF FF FF FF FF FF，6个FF是数据的开始，
			// 紧跟着16次MAC地址就可以了。
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < destMac.length; j++) {
					magicBytes[6 + destMac.length * i + j] = destMac[j];
					magicBytes[j] = (byte) 0xFF;
				}
			}

			// --------------------查看数据报发出的数据---------------------

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
				// 关闭数据报流
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