package com.example.contentprovider;

import java.util.ArrayList;

import android.content.Context;
import android.text.Layout;
import android.text.style.BulletSpan;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView;

public class MyAdapter3 extends BaseAdapter {
	private ArrayList<DetailMessage> arrayList;
	private Context context;

	public MyAdapter3(Context context,ArrayList<DetailMessage> arrayList){
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
		View view = inflater.inflate(R.layout.message, null);
		TextView messageTextView = (TextView)view.findViewById(R.id.message_text);
		TextView dateTextView = (TextView)view.findViewById(R.id.message_date);
		if(arrayList.get(position).getType() == 1){
			messageTextView.setBackgroundResource(R.drawable.left);
			messageTextView.setText(arrayList.get(position).getMessageString());			
			dateTextView.setText(arrayList.get(position).getDateString());
		}else if(arrayList.get(position).getType() == 2){
			messageTextView.setBackgroundResource(R.drawable.right);
			messageTextView.setText(arrayList.get(position).getMessageString());
			LayoutParams mp = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);  //item的宽高
	        LayoutParams lp = new LayoutParams(mp);	        
	        lp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
	        messageTextView.setLayoutParams(lp);
			dateTextView.setText(arrayList.get(position).getDateString());
			LayoutParams mp2 = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);  //item的宽高
	        LayoutParams lp2 = new LayoutParams(mp);	        
	        lp2.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			lp2.addRule(RelativeLayout.BELOW, R.id.message_text);
			dateTextView.setLayoutParams(lp2);
		}else{
			
		}		
		return view;
	}

}
