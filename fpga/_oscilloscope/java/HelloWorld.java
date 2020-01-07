import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.util.*;

/*
javac HelloWorld.java
javah -jni HelloWorld

sudo chmod 777 run.sh
java -Djava.library.path=. HelloWorld
*/

public class HelloWorld {
    static {
        try {
            System.loadLibrary("HelloWorld");
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }

    private static native int sayHello(short[] array);

    private static native int init();

    // return position
    private static native int dumpData(short[] array);

    private static native int fetchData();

    private static native int startRecord();

    private static native int stopRecord();

    private static native int test();

    public static void main(String[] args) {
        try {
            // short[] array = new short[100];
            // array[0] = 111;
            // int val = sayHello(array);
            // System.out.println(array[2]);
            // System.out.println(array[3]);
            // System.out.println(val);
            //

            // init();
            // int testv = test();
            // System.out.println(testv);

        } catch (Throwable t) {
            t.printStackTrace();
        }

        for (int i = 0; i < 16 * 1024 * 1024; i++) {
            double v = i;
            double v1 = (Math.sin(v / 50) * 0.2 + Math.sin(v / 100000)*0.7) * 127 + 127;
            double v2 = (Math.sin(v / 50 + 2) * 0.2 + Math.sin(v / 10000)*0.7) * 127 + 127;
            int iv1 = (int) v1;
            int iv2 = (int) v2;
            buff[i] = (short) ((iv1 << 8) | iv2);
        }

        new DrawFrame();
    }

    static int start = 7 * 1024 * 1024;
    static int end = 8 * 1024 * 1024;
    static short[] buff = new short[16 * 1024 * 1024];
    static int mousex = 0;
    static int mousedown = 0;

    private static class DrawFrame extends Frame {
        private static final long serialVersionUID = 1L;

        static final int basex = 15;
        static final int basey = 100;

        public DrawFrame() {
            DrawFrame self = this;

            Canvas c = new Canvas();
            c.setVisible(true);
            self.add(c);
            BufferedImage img = new BufferedImage(1024, 512, BufferedImage.TYPE_INT_ARGB);
            Graphics gg = img.getGraphics();

            KeyAdapter keyAdapter = new KeyAdapter() {
                public void keyPressed(KeyEvent e) {
                    int code = e.getKeyCode();
                    int diff = end - start;
                    if (code == KeyEvent.VK_UP) {
                        start = (int) (start + diff * 0.1f);
                        end = (int) (end - diff * 0.1f);
                    }
                    if (code == KeyEvent.VK_DOWN) {
                        start = (int) (start - diff * 0.1f);
                        end = (int) (end + diff * 0.1f);
                    }
                    if(code== KeyEvent.VK_A){
                      draw(c, gg, img);
                    }
                }
            };
            self.addKeyListener(keyAdapter);
            c.addKeyListener(keyAdapter);

            MouseAdapter mouseAdapter = new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    mousex = e.getX();
                    mousedown = 1;
                }

                public void mouseReleased(MouseEvent e) {
                    int diff = end - start;

                    int dist = mousex - e.getX();

                    start += (int) ((long) dist * diff / 1024);
                    end += (int) ((long) dist * diff / 1024);

                    mousedown = 0;
                }

                public void mouseMoved(MouseEvent e) {
                    if (mousedown == 1) {

                        int diff = end - start;

                        int dist = mousex - e.getX();

                        start += (int) ((long) dist * diff / 1024);
                        end += (int) ((long) dist * diff / 1024);

                    }
                }

                public void mouseWheelMoved(MouseWheelEvent e) {
                    int diff = end - start;
                    if (e.getWheelRotation() < 0) {
                        start = (int) (start + diff * 0.1f);
                        end = (int) (end - diff * 0.1f);
                    } else {
                        start = (int) (start - diff * 0.1f);
                        end = (int) (end + diff * 0.1f);

                    }

                }
            };
            self.addMouseListener(mouseAdapter);
            c.addMouseListener(mouseAdapter);
            self.addMouseWheelListener(mouseAdapter);
            c.addMouseWheelListener(mouseAdapter);
            self.addMouseMotionListener(mouseAdapter);
            c.addMouseMotionListener(mouseAdapter);

            TimerTask task = new TimerTask() {
                @Override
                public void run() {

                    draw(c, gg, img);

                }
            };
            Timer timer = new Timer();
            //timer.scheduleAtFixedRate(task, 100, 500);

            self.addWindowListener(new WindowAdapter() {
                public void windowClosing(WindowEvent e) {
                    System.exit(0);
                }
            });
            self.setSize(basex + 1024 + 20, basey + 512 + 50);
            self.setVisible(true);
        }

        //Color cback = new Color(240, 240, 240);
        //Color cbox = new Color(230, 230, 230);
        //Color cline = new Color(255, 240, 240);
        Color cback = new Color(100, 100, 100);
        Color cbox = new Color(0, 0, 0);
        Color cline = new Color(255, 50, 50);

        private void draw(Canvas c, Graphics gg, BufferedImage img) {

            Graphics g = c.getGraphics();
            if (g != null) {

                gg.setColor(cback);
                gg.fillRect(0, 0, 1024, 512);
                gg.setColor(cbox);
                gg.drawRect(0, 0, 1024, 256);
                gg.drawRect(0, 0 + 256, 1024, 256);

                gg.setColor(cline);

                int cnt = 0;
                long accu1 = 0;
                long accu2 = 0;
                int oldx = -1;
                int oldval1 = 0;
                int oldval2 = 0;
                int top1 = 0;
                int bottom1 = 255;
                int top2 = 0;
                int bottom2 = 255;
                for (int i = start; i < end; i++)// 数组位置
                {
                    long temp = (i - start) * 1024L;

                    int x = (int) (temp / (end - start));

                    if (i > 0 && i < 16 * 1024 * 1024) {
                        int v1 = buff[i] & 0xFF;
                        int v2 = (buff[i] >> 8) & 0xFF;
                        if (v1 > top1) {
                            top1 = v1;
                        }
                        if (v1 < bottom1) {
                            bottom1 = v1;
                        }
                        if (v2 > top2) {
                            top2 = v2;
                        }
                        if (v2 < bottom2) {
                            bottom2 = v2;
                        }
                        accu1 += v1;
                        accu2 += v2;
                    }

                    cnt++;
                    if (x != oldx) {
                        int val1 = (int) (accu1 / cnt);
                        int val2 = (int) (accu2 / cnt);
                        gg.drawLine(x, 255 - val1, oldx, 255 - oldval1);
                        gg.drawLine(x, 511 - val2, oldx, 511 - oldval2);
                        if (cnt > 10) {
                            gg.drawLine(x, 255 - top1, x, 255 - bottom1);
                            gg.drawLine(x, 511 - top2, x, 511 - bottom2);
                        }
                        cnt = 0;
                        accu1 = 0;
                        accu2 = 0;
                        oldx = x;
                        oldval1 = val1;
                        oldval2 = val2;
                        top1 = 0;
                        bottom1 = 255;
                        top2 = 0;
                        bottom2 = 255;
                    }
                }
                // System.out.println("done");

                g.drawImage(img, basex, basey, null);

                g.clearRect(0, 0, 600, 100);

                g.setColor(Color.RED);
                g.drawString(start + "," + end, 60, 50);

            }
        }
    }

}