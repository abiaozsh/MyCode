package com.example.contentprovider;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.Contacts;
import android.provider.ContactsContract;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.telephony.gsm.SmsManager;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ProgressBar;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.TextView;

public class MainActivity extends Activity implements OnItemClickListener{
	private ListView listView;
	private ProgressBar progressBar;
	private TextView textView;
	private ArrayList<MyContacts> list;
	private MyAdapter myAdapter;
	private ArrayList<MyMessageList> myMessageLists;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		listView = (ListView)findViewById(R.id.list_view);
		progressBar = (ProgressBar)findViewById(R.id.progressbar);
		textView = (TextView)findViewById(R.id.load);
//		list = new ArrayList<MyContacts>();
		myMessageLists = new ArrayList<MyMessageList>();
		listView.setOnItemClickListener(this);
//		MyTask myTask = new MyTask();
//		myTask.execute("");
		MyTask2 myTask2 = new MyTask2();
		myTask2.execute("");
		
	}
	
	class MyTask extends AsyncTask<String, Integer, ArrayList<MyContacts>>{

		@Override
		protected void onProgressUpdate(Integer... values) {
			// TODO Auto-generated method stub
			super.onProgressUpdate(values);
			progressBar.setProgress(values[0]);
			
		}

		@Override
		protected ArrayList<MyContacts> doInBackground(String... params) {
			// TODO Auto-generated method stub
			Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);	
			int total = cursor.getColumnCount();
			int i = 0;
			while (cursor.moveToNext()) {
//				ContactsContract.Contacts.
				//调用publishProgress公布进度,最后onProgressUpdate方法将被执行    
                publishProgress((int) ((i/(float)total) * 100));  
                
				MyContacts contacts = new MyContacts();
				contacts.setId(cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID)));
				contacts.setName(cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME)));
				Cursor cursor2 = getContentResolver().query(
						ContactsContract.CommonDataKinds.Phone.CONTENT_URI, 
						null, 
						ContactsContract.CommonDataKinds.Phone.CONTACT_ID+"="+cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID)), 
						null, 
						null);
				ArrayList<String> phones = new ArrayList<String>();
				while (cursor2.moveToNext()) {
					phones.add(cursor2.getString(cursor2.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER)));
				}
				contacts.setPhonenumbers(phones);
				list.add(contacts);
				cursor2.close();
				i++;
			}		
			cursor.close();
			return list;
		}

		@Override
		protected void onPostExecute(ArrayList<MyContacts> result) {
			// TODO Auto-generated method stub			
			super.onPostExecute(result);
			progressBar.setVisibility(View.GONE);
			textView.setVisibility(View.GONE);
			myAdapter = new MyAdapter(MainActivity.this, result);		
			listView.setAdapter(myAdapter);
		}		
	}


	class MyTask2 extends AsyncTask<String, Integer, ArrayList<MyMessageList>>{

		@Override
		protected void onPostExecute(ArrayList<MyMessageList> result) {
			// TODO Auto-generated method stub
			super.onPostExecute(result);
			progressBar.setVisibility(View.GONE);
			textView.setVisibility(View.GONE);
			MyAdapter2 adapter2 = new MyAdapter2(MainActivity.this, myMessageLists);
			listView.setAdapter(adapter2);
		}

		@Override
		protected ArrayList<MyMessageList> doInBackground(String... params) {
			// TODO Auto-generated method stub
			ContentResolver resolver = getContentResolver();
			Cursor cursor = resolver.query(Uri.parse("content://mms-sms/conversations"), 
					new String[]{ "* from threads--" }, 
					null, 
					null, 
					null);
			System.out.println(cursor.getCount());
			if(cursor.getCount()>0){
				while (cursor.moveToNext()) {
					MyMessageList messageList = new MyMessageList();
					messageList.setThreadid((cursor.getString(cursor.getColumnIndex("_id"))));
					SimpleDateFormat sfd = new SimpleDateFormat("yyyy-MM-dd hh:mm");
					Date date = new Date(Long.parseLong(cursor.getString(cursor.getColumnIndex("date"))));
					String time = sfd.format(date);
					messageList.setDate(time);
					messageList.setMessagecount(cursor.getString(cursor.getColumnIndex("message_count")));
					messageList.setSnippet(cursor.getString(cursor.getColumnIndex("snippet")));				
					
					ContentResolver resolver2 = getContentResolver();
					Cursor cursor2 = resolver2.query(Uri.parse("content://sms/"), 
							new String[]{"address"}, 
							"thread_id=?", 
							new String[]{cursor.getString((cursor.getColumnIndex("_id")))},
							null);
						if(cursor2.moveToNext()){
							System.out.println(cursor2.getString(cursor2.getColumnIndex("address")));
							messageList.setPhone(cursor2.getString(cursor2.getColumnIndex("address")));					
						}					
					cursor2.close();
					myMessageLists.add(messageList);
				}
				System.out.println(myMessageLists.size());
			}
			
			cursor.close();		
			
			
			
			return myMessageLists;
//			return null;
		}
		
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}


	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position,
			long id) {
		// TODO Auto-generated method stub
		if(Integer.parseInt(myMessageLists.get(position).getMessagecount())>0){
			Intent intent = new Intent();
			intent.putExtra("threadid", myMessageLists.get(position).getThreadid());
			intent.putExtra("phone", myMessageLists.get(position).getPhone());
			intent.setClass(this, MessageDetailActivity.class);
			startActivity(intent);
//			intent.setAction("android.intent.action.CALL");
//			System.out.println(list.get(position).getPhonenumbers().get(0));
//			intent.setData(Uri.parse("tel:"+list.get(position).getPhonenumbers().get(0)));
//			startActivity(intent);
			
			//发短信
//			intent.setAction(Intent.ACTION_SENDTO);
//			intent.setData(Uri.parse("smsto:"+list.get(position).getPhonenumbers().get(0)));
//			intent.putExtra("sms_body","aaaaaaaaaaaaa");
//			startActivity(intent);
			//
//			SmsManager manager = SmsManager.getDefault();
//			manager.sendTextMessage("13011868125", 
//					null, 
//					"aaaaaaaaa", 
//					null, 
//					null);
		}		
	}

}
