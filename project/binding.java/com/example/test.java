package com.example;
public class test
{
	static
	{
		System.loadLibrary("finalanswer");
	}
	public native static void birth();
	public native static void death();
	public native static void print();
	public static void main(String[] args)
	{
		birth();
		print();
		death();
	}
}
