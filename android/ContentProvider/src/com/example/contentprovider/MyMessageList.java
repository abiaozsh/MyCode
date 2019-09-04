package com.example.contentprovider;

import android.R.integer;

public class MyMessageList {
	private String phone;
	private String threadid;
	private String date;
	private String messagecount;
	private String snippet;
	
	public MyMessageList(){
		threadid = "";
		date = "0000-00-00";
		messagecount = 0+"";
		snippet = "";
		phone = "";
	}
	
	public String getThreadid() {
		return threadid;
	}
	public void setThreadid(String threadid) {
		this.threadid = threadid;
	}
	public String getDate() {
		return date;
	}
	public void setDate(String date) {
		this.date = date;
	}
	public String getMessagecount() {
		return messagecount;
	}
	public void setMessagecount(String messagecount) {
		this.messagecount = messagecount;
	}
	public String getSnippet() {
		return snippet;
	}
	public void setSnippet(String snippet) {
		this.snippet = snippet;
	}
	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

}
