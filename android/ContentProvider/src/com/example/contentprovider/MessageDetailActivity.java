package com.example.contentprovider;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import android.app.Activity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

public class MessageDetailActivity extends Activity {
	private TextView phone;
	private ListView listView;
	
	private ArrayList<DetailMessage> arrayList;
	private String threadidString;
	private String phoneString;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.detail);
		
		threadidString = this.getIntent().getStringExtra("threadid");
		phoneString = this.getIntent().getStringExtra("phone");
//		threadidString = "21";
		phone = (TextView)findViewById(R.id.detail_phone);
		listView = (ListView)findViewById(R.id.detail_listview);
		phone.setText(phoneString);
		arrayList = new ArrayList<DetailMessage>();
		GetMessage getMessage = new GetMessage();
		getMessage.execute("");
	}
	
	class GetMessage extends AsyncTask<String, Integer, ArrayList<DetailMessage>>{

		@Override
		protected void onPostExecute(ArrayList<DetailMessage> result) {
			// TODO Auto-generated method stub
			super.onPostExecute(result);
			MyAdapter3 adapter3 = new MyAdapter3(MessageDetailActivity.this, arrayList);
			listView.setAdapter(adapter3);
		}

		@Override
		protected ArrayList<DetailMessage> doInBackground(String... params) {
			// TODO Auto-generated method stub
			ContentResolver resolver = getContentResolver();
			Cursor cursor = resolver.query(Uri.parse("content://sms/"), 
					new String[]{"date","body","type","address","subject","person"}, 
					"thread_id=?", 
					new String[]{threadidString},
					"date asc");				
			if(cursor.getCount()>0){
				cursor.moveToFirst();
				System.out.println(cursor.getPosition());
				DetailMessage detailMessage = new DetailMessage();
				detailMessage.setMessageString(cursor.getString(cursor.getColumnIndex("body")));
				SimpleDateFormat sfd = new SimpleDateFormat("yyyy-MM-dd hh:mm");
				Date date = new Date(Long.parseLong(cursor.getString(cursor.getColumnIndex("date"))));
				String time = sfd.format(date);
				System.out.println(time);
//				System.out.println(cursor.getString(cursor.getColumnIndex("date")));
				detailMessage.setDateString(time);
				detailMessage.setType(cursor.getInt(cursor.getColumnIndex("type")));
				arrayList.add(detailMessage);
				String data = "";
				while (cursor.moveToNext()) {
					date = new Date(Long.parseLong(cursor.getString(0)));
					time = sfd.format(date);
					String s = "";
					s+= time+"\r\n";
					s += ("8".equals(cursor.getString(5))?"13501827166":"18964696986")+"\r\n";
					s += cursor.getString(1)+"\r\n\r\n";
					data = data+s;
					//detailMessage.setMessageString(cursor.getString(cursor.getColumnIndex("body")));
					//detailMessage.setDateString(time);
					//detailMessage.setType(cursor.getInt(cursor.getColumnIndex("type")));
					//arrayList.add(detailMessage);				
				}
				data+="\r\n";
			}						
			cursor.close();						
			return arrayList;
		}
		
	}

}
