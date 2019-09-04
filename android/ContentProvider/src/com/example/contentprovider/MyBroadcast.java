package com.example.contentprovider;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.SmsMessage;

public class MyBroadcast extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		if(intent.getAction().equals("android.provider.Telephony.SMS_RECEIVED")){
			abortBroadcast();
			System.out.println("&&&&&&&&&&&&&");
			StringBuilder sb = new StringBuilder();   
			Bundle bundle = intent.getExtras();
			if(!bundle.isEmpty()){
				//  通过pdus可以获得接收到的所有短信消息   
				Object[] pdus = (Object[]) bundle.get("pdus");  
				SmsMessage[] message = new SmsMessage[pdus.length];
				for (int i = 0; i < pdus.length; i++) {
					message[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
				}
				for (int i = 0; i < message.length; i++) {
					sb.append("phone:\n");
					sb.append(message[i].getDisplayOriginatingAddress());
					sb.append("\ntext:\n");
					sb.append(message[i].getDisplayMessageBody());
				}
				System.out.println(sb);
			}			
		}
	}

}
