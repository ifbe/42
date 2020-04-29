public class test {
	static{
		//System.load("/Users/ifbe/Desktop/ifbe/glue/fromjava/intoc/add.so");
		System.loadLibrary("add");
	}
	public native static int add(int a,int b);
	public static void main(String[] args) {
		System.out.println(add(1,2));
	}
}
