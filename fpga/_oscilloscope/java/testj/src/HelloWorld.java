//javac HelloWorld.java
//javah -jni HelloWorld

//java -Djava.library.path=. HelloWorld
public class HelloWorld
{      
    static
    {
            System.loadLibrary("HelloWorld");
    }
    private native void sayHello();
    public static void main(String[] args)
    {
            new HelloWorld().sayHello();
    }
}