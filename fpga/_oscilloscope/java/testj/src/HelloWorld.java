import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.WindowConstants;

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
            
            
            
            
                    // TODO Auto-generated method stub
        DrawFrame df=new DrawFrame();
        df.setVisible(true);
            
            
            
            
            
    }
    
    
    public static class DrawFrame extends JFrame{

    public DrawFrame(){
        super();
        initialize();//调用初始化方法
    }
    //初始化方法
    public void initialize(){
        this.setSize(300, 200);//设置窗体大小
        //设置窗体的关闭方式
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        add(new CanvasPanel());//设置窗体面板为绘图面板对象
        this.setTitle("绘图实例2");//设置窗体标题
    }
    
    //创建内部类
    class CanvasPanel extends JPanel{
        public void paint(Graphics g){
            super.paint(g);
            Graphics2D g2=(Graphics2D)g;//强制类型转换位Graphics2D类型
            //Graphics2D是推荐使用的绘图类，但是程序设计中提供的绘图对象大多是
            //Graphics类的实例，这时应该使用强制类型转换将其转换为Graphics
            Shape[] shapes=new Shape[4];//声明图形数组
            shapes[0]=new Ellipse2D.Double(5, 5, 100, 100);//创建圆形对象
            //创建矩形对象
            shapes[1]=new Rectangle2D.Double(110,5,100,100);//
            shapes[2]=new Rectangle2D.Double(15, 15, 80, 80);//创建矩形对象
            shapes[3]=new Ellipse2D.Double(120, 15, 80, 80);//创建圆形对象
            for(Shape shape:shapes){//遍历图型数组
                Rectangle2D bounds=shape.getBounds2D();
                if(bounds.getWidth()==80){
                    g2.fill(shape);//填充图形
                }else{
                    g2.draw(shape);//绘制图形
                }
            }
            
        }
    }

}
}