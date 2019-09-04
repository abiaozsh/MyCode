package com.example.contentprovider;

import java.util.ArrayList;
import java.util.List;



import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView.FindListener;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class MyAdapter extends BaseAdapter {
	private ArrayList<MyContacts> arrayList;
	private Context context;
	
	public MyAdapter(Context context,ArrayList<MyContacts> arrayList){
		this.context = context;
		this.arrayList = arrayList;	
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return arrayList.size();
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return arrayList.get(position);
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
		View view = inflater.inflate(R.layout.vlist, null);
		TextView textView = (TextView)view.findViewById(R.id.name);
		textView.setText(arrayList.get(position).getName());
		TextView textView2 = (TextView)view.findViewById(R.id.phone);
		String phoneString = " ";
		ArrayList<String> phone= arrayList.get(position).getPhonenumbers();
		for (int i = 0; i < phone.size(); i++) {
			phoneString = phoneString + phone.get(i);
		}
		textView2.setText(phoneString);
		return view;
	}

}
