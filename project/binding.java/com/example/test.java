package com.example;
public class test
{
    static
    {
        System.loadLibrary("42");
    }
    public native static int get();
    public native static void set(int i);
    public static void main(String[] args)
    {
        test haha = new test();
        test.set(10);
        System.out.println(test.get());
    }
}
