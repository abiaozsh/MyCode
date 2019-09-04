package com.example.contentprovider;

public class DetailMessage {
	private String dateString;
	private String messageString;
	private int type;
	
	
	public DetailMessage (){
		dateString = "";
		messageString = "";
		type = -1;
	}
	public String getDateString() {
		return dateString;
	}
	public void setDateString(String dateString) {
		this.dateString = dateString;
	}
	public String getMessageString() {
		return messageString;
	}
	public void setMessageString(String messageString) {
		this.messageString = messageString;
	}
	public int getType() {
		return type;
	}
	public void setType(int type) {
		this.type = type;
	}

}
