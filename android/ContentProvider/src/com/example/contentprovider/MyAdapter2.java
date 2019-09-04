package com.example.contentprovider;

import java.util.ArrayList;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class MyAdapter2 extends BaseAdapter {
	private ArrayList<MyMessageList> myMessageLists;
	private Context context;
	
	public MyAdapter2(Context context,ArrayList<MyMessageList> myMessageLists){
		this.context = context;
		this.myMessageLists = myMessageLists;
	}
	

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return myMessageLists.size();
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return myMessageLists.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		LayoutInflater inflater = LayoutInflater.from(context);
		View view = inflater.inflate(R.layout.b, null);
		TextView textView = (TextView)view.findViewById(R.id.threadid);
		TextView textView2 = (TextView)view.findViewById(R.id.date);
		TextView textView3 = (TextView)view.findViewById(R.id.messagecount);
		TextView textView4 = (TextView)view.findViewById(R.id.snippet);
		
		System.out.println("**********"+myMessageLists.get(position).getThreadid());
		System.out.println(myMessageLists.get(position).getDate());
		textView.setText(""+myMessageLists.get(position).getPhone());
		textView2.setText(""+myMessageLists.get(position).getDate());
		textView3.setText("count:"+myMessageLists.get(position).getMessagecount());	
		textView4.setText(""+myMessageLists.get(position).getSnippet());
			
		return view;
	}

}
