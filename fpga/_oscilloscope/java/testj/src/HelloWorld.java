//javac HelloWorld.java
//javah -jni HelloWorld

//java -Djava.library.path=. HelloWorld
public class HelloWorld
{      
    static
    {
            System.loadLibrary("HelloWorld");
    }
    private static native int sayHello(short[] array);
    public static void main(String[] args)
    {
      short[] array = new short[100];
      array[0] = 111;
            int val = sayHello(array);//new HelloWorld().
            System.out.println(array[2]);
            System.out.println(array[3]);
            System.out.println(val);
    }
}