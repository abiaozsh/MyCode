package com.example.contentprovider;

import java.util.ArrayList;

public class MyContacts {
	private String id;
	private String name;
	private ArrayList<String> phonenumbers;
	public MyContacts(){
		this.id = "";
		this.name = "";
		this.phonenumbers = null;
	}
	public ArrayList<String> getPhonenumbers() {
		return phonenumbers;
	}
	public void setPhonenumbers(ArrayList<String> phonenumbers) {
		this.phonenumbers = phonenumbers;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
}
