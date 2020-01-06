import java.awt.*;
import java.awt.event.*;
import java.util.*;

//javac HelloWorld.java
//javah -jni HelloWorld

//java -Djava.library.path=. HelloWorld
public class HelloWorld {
    static {
        try {
            System.loadLibrary("HelloWorld");
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }

    private static native int sayHello(short[] array);

    
    private static native long init();
    
    //return position
    private static native int dumpData(long gpio,short[] array);
    
    private static native int fetchData(long gpio);
    
    private static native int startRecord(long gpio);
    
    private static native int stopRecord(long gpio);
    
    private static native int test(long gpio);
    
    private static native int test0(int val);
    private static native int test1();
    
    public static void main(String[] args) {
        try {
            short[] array = new short[100];
            array[0] = 111;
            int val = sayHello(array);
            System.out.println(array[2]);
            System.out.println(array[3]);
            System.out.println(val);
            
            long gpio = init();
            int testv = test(gpio);
            System.out.println(testv);
            
            
            test0(123);
            int vvv = test1();
            System.out.println(vvv);

        } catch (Throwable t) {
            t.printStackTrace();
        }

        DrawFrame form = new DrawFrame();
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                form.refresh();
            }
        };
        Timer timer = new Timer();
        // 定义开始等待时间  --- 等待 5 秒
        // 1000ms = 1s
        long delay = 100;
        // 定义每次执行的间隔时间
        long intevalPeriod = 100;
        // schedules the task to be run in an interval
        // 安排任务在一段时间内运行
        timer.scheduleAtFixedRate(task, delay, intevalPeriod);
    }

    static int i = 0;

    public static class DrawFrame extends Frame {
        private static final long serialVersionUID = 1L;

        public void refresh(){
            this.repaint();
        }
        public DrawFrame() {
            DrawFrame self = this;
            this.setSize(basex + 1024 + 20, basey + 512 + 20);
            this.addWindowListener(new WindowAdapter() {
                public void windowClosing(WindowEvent e) {
                    System.exit(0);
                }
            });

            this.addKeyListener(new KeyAdapter() {
                public void keyPressed(KeyEvent e) {
                    int code = e.getKeyCode();

                    System.out.println(code);

                }
            });

            this.addMouseListener(new MouseAdapter() {
                public void mouseEntered(MouseEvent e) {
                    //self.validate();
                    //self.repaint();
                }

                public void mouseClicked(MouseEvent e) {
                    //self.repaint();
                }

                public void mouseWheelMoved(MouseWheelEvent e) {
                    //self.validate();
                    //self.repaint();

                }
            });
            self.setVisible(true);
        }

        static final int basex = 15;
        static final int basey = 140;

        public void paint(Graphics g) {
            super.paint(g);
            Graphics2D g2 = (Graphics2D) g;
            g2.setColor(Color.LIGHT_GRAY);
            g2.fillRect(basex, basey, 1024, 512);
            g2.setColor(Color.GRAY);
            g2.drawRect(basex, basey, 1024, 256);
            g2.drawRect(basex, basey + 256, 1024, 256);

            g2.setColor(Color.BLACK);
            g2.drawString("fdsa", 60, 50);
            g2.drawLine(50, 50, 55, 55 + i);
            i++;

        }

    }

}