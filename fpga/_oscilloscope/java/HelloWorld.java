import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.FileInputStream;
import java.util.*;

/*
javac HelloWorld.java
javah -jni HelloWorld
gcc -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/ -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/linux -fPIC -shared HelloWorld.c -o libHelloWorld.so

java -Djava.library.path=. HelloWorld

sudo mount tmpfs /mnt/tmpfs -t tmpfs -o size=50m
cd /mnt/tmpfs

sudo chmod 777 run.sh

*/

public class HelloWorld {
    static {
        try {
            System.loadLibrary("HelloWorld");
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }

    static int fetchDataLastvalA;
    static int fetchDataLastvalB;
    
    private static native int init(); 
    private static native int dumpData(byte[] arrayA, byte[] arrayB); 
    private static native int startRecord(); 
    private static native int stopRecord(); 
    private static native int fetchData(); 
    private static native int setFrameRate(int val); 
    private static native int fetchFrame(byte[] frameA, byte[] frameB); 
    private static native int test(); 
    // dummy start
/* 
    private static int init() {
        return 0;
    }

    private static int dumpData(byte[] arrayA, byte[] arrayB) {
        try {
            // FileInputStream fis = new FileInputStream("/mnt/tmpfs/c.dat");
            FileInputStream fis = new FileInputStream("d:\\c.dat");
            byte[] buff2 = new byte[32 * 1024 * 1024];
            fis.read(buff2);
            fis.close();

            for (int i = 0; i < 16 * 1024 * 1024; i++) {
                arrayA[i] = buff2[i * 2];
                arrayB[i] = buff2[i * 2 + 1];
            }
            return 123000;
        } catch (Throwable t) {
            return 0;
        }
    }

    private static int startRecord() {
        return 0;
    }

    private static int stopRecord() {
        return 0;
    }

    static int cnt;

    private static int fetchData() {
        cnt++;
        double v = cnt;
        double v1 = (Math.sin(v / 10) * 0.2 + Math.sin(v / 1000) * 0.7) * 127 + 127;
        double v2 = (Math.sin(v / 10 + 2) * 0.2 + Math.sin(v / 100) * 0.7) * 127 + 127;
        int iv1 = (int) v1;
        int iv2 = (int) v2;
        return ((iv1 << 8) | iv2);

    }

    private static int setFrameRate(int val) {
        return 0;
    }

    private static int fetchFrame(byte[] frameA, byte[] frameB) {
        for (int i = 0; i < 1024; i++) {
            cnt++;
            double v = cnt;
            double v1 = (Math.sin(v / 10) * 0.2 + Math.sin(v / 1000) * 0.7) * 127 + 127;
            double v2 = (Math.sin(v / 10 + 2) * 0.2 + Math.sin(v / 100) * 0.7) * 127 + 127;
            frameA[i] = (byte) v1;
            frameB[i] = (byte) v2;
        }
        return 0;
    }
*/
    // dummy end

    public static void main(String[] args) {
        init();
        new DrawFrame();
    }

    static void calcArray(int pos) {
        byte[] tempA = new byte[16 * 1024 * 1024];
        byte[] tempB = new byte[16 * 1024 * 1024];

        int idx = 0;
        for (int i = pos; i < 16 * 1024 * 1024; i++) {
            tempA[idx] = buffA[i];
            tempB[idx] = buffB[i];
            idx++;
        }
        for (int i = 0; i < pos; i++) {
            tempA[idx] = buffA[i];
            tempB[idx] = buffB[i];
            idx++;
        }
        buffA = tempA;
        buffB = tempB;

        buffA_T = new ArrayList<byte[]>();
        buffB_T = new ArrayList<byte[]>();
        buffA_B = new ArrayList<byte[]>();
        buffB_B = new ArrayList<byte[]>();
        int size = 16 * 1024 * 1024;
        // 0:8 * 1024 * 1024
        // 1:4 * 1024 * 1024
        // 2:2 * 1024 * 1024
        // 3:1 * 1024 * 1024
        // 4:512 * 1024
        // 5:256 * 1024
        // 6:128 * 1024
        // 7:64 * 1024
        // 8:32 * 1024
        // 9:16 * 1024
        // 10:8 * 1024
        // 11:4 * 1024
        // 12:2 * 1024
        // 13:1 * 1024
        for (int i = 0; i < 14; i++) {
            size = size / 2;
            buffA_T.add(new byte[size]);
            buffB_T.add(new byte[size]);
            buffA_B.add(new byte[size]);
            buffB_B.add(new byte[size]);
            if (i == 0) {
                byte[] bat = buffA_T.get(i);
                byte[] bab = buffA_B.get(i);
                byte[] bbt = buffB_T.get(i);
                byte[] bbb = buffB_B.get(i);
                for (int j = 0; j < size; j++) {
                    int va1 = buffA[j * 2] & 0xFF;
                    int va2 = buffA[j * 2 + 1] & 0xFF;
                    int vb1 = buffB[j * 2] & 0xFF;
                    int vb2 = buffB[j * 2 + 1] & 0xFF;
                    bat[j] = (byte) Math.max(va1, va2);
                    bab[j] = (byte) Math.min(va1, va2);
                    bbt[j] = (byte) Math.max(vb1, vb2);
                    bbb[j] = (byte) Math.min(vb1, vb2);
                }
            } else {
                byte[] bat = buffA_T.get(i);
                byte[] bab = buffA_B.get(i);
                byte[] bbt = buffB_T.get(i);
                byte[] bbb = buffB_B.get(i);

                byte[] batl = buffA_T.get(i - 1);
                byte[] babl = buffA_B.get(i - 1);
                byte[] bbtl = buffB_T.get(i - 1);
                byte[] bbbl = buffB_B.get(i - 1);

                for (int j = 0; j < size; j++) {
                    int vat1 = batl[j * 2] & 0xFF;
                    int vat2 = batl[j * 2 + 1] & 0xFF;
                    int vab1 = babl[j * 2] & 0xFF;
                    int vab2 = babl[j * 2 + 1] & 0xFF;
                    int vbt1 = bbtl[j * 2] & 0xFF;
                    int vbt2 = bbtl[j * 2 + 1] & 0xFF;
                    int vbb1 = bbbl[j * 2] & 0xFF;
                    int vbb2 = bbbl[j * 2 + 1] & 0xFF;
                    bat[j] = (byte) Math.max(vat1, vat2);
                    bab[j] = (byte) Math.min(vab1, vab2);
                    bbt[j] = (byte) Math.max(vbt1, vbt2);
                    bbb[j] = (byte) Math.min(vbb1, vbb2);
                }

            }
        }
    }

    static int pos = 8 * 1024 * 1024;
    static int scale = 14;// 0:buffA, 1.2.3 buffA_T 0 1 2
    static int framerate = 1;
    
    static byte[] frameA = new byte[1024];
    static byte[] frameB = new byte[1024];

    static byte[] buffA = new byte[16 * 1024 * 1024];
    static byte[] buffB = new byte[16 * 1024 * 1024];
    static ArrayList<byte[]> buffA_T;
    static ArrayList<byte[]> buffA_B;
    static ArrayList<byte[]> buffB_T;
    static ArrayList<byte[]> buffB_B;

    static int mousex = 0;
    static int mouseOldpos = 0;

    static int currPos = 0;
    static boolean recording = false;

    static Graphics g;

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
                    if (code == KeyEvent.VK_SPACE) {
                        recording = !recording;
                        if (recording) {
                            self.setTitle("Recording");
                            startRecord();

                        } else {
                            self.setTitle("Stop");
                            stopRecord();
                            {
                                int pos = dumpData(buffA, buffB);

                                calcArray(pos);
                            }
                            draw(c, gg, img);
                        }
                    }
                    if (code == KeyEvent.VK_UP) {
                        framerate++;
                        setFrameRate(framerate);
                        self.setTitle("framerate:"+framerate);
                        //scale++;
                        //if (scale > 14)
                        //    scale = 14;
                    }
                    if (code == KeyEvent.VK_DOWN) {
                        framerate--;
                        setFrameRate(framerate);
                        self.setTitle("framerate:"+framerate);
                        //scale--;
                        //if (scale < 0)
                        //    scale = 0;
                    }
                }
            };
            self.addKeyListener(keyAdapter);
            c.addKeyListener(keyAdapter);

            MouseAdapter mouseAdapter = new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    mousex = e.getX();
                    mouseOldpos = pos;
                    draw(c, gg, img);
                }

                public void mouseReleased(MouseEvent e) {
                    // int dist = mousex - e.getX();
                    // pos += dist << scale;
                }

                public void mouseDragged(MouseEvent e) {
                    int dist = mousex - e.getX();
                    pos = mouseOldpos + (dist << scale);
                    draw(c, gg, img);
                }

                public void mouseWheelMoved(MouseWheelEvent e) {

                    if (e.getWheelRotation() < 0) {
                        scale--;
                        if (scale < 0)
                            scale = 0;
                    } else {
                        scale++;
                        if (scale > 14)
                            scale = 14;
                    }
                    draw(c, gg, img);
                }
            };
            c.addMouseListener(mouseAdapter);
            c.addMouseWheelListener(mouseAdapter);
            c.addMouseMotionListener(mouseAdapter);

            TimerTask task = new TimerTask() {
                @Override
                public void run() {
                    try {
                        if (recording) {
                            if (false) {
                                currPos++;
                                if (currPos == 1024) {
                                    currPos = 0;
                                }
                                int data = fetchData();
                                if (g == null) {
                                    g = c.getGraphics();
                                }
                                g.setColor(cback);
                                g.drawLine(currPos + basex + 1, basey + 256, currPos + basex + 1, 511 + basey);
                                g.drawLine(currPos + basex + 1, basey + 0, currPos + basex + 1, 255 + basey);
                                g.setColor(cline);

                                int valA = data & 0xFF;
                                int valB = (data >> 8) & 0xFF;
                                if (currPos > 0) {
                                    g.drawLine(currPos + basex, 255 - valA + basey, currPos - 1 + basex,
                                            255 - fetchDataLastvalA + basey);
                                    g.drawLine(currPos + basex, 511 - valB + basey, currPos - 1 + basex,
                                            511 - fetchDataLastvalB + basey);
                                }
                                fetchDataLastvalA = valA;
                                fetchDataLastvalB = valB;
                            } else {
                                fetchFrame(frameA, frameB);
                                drawframe(c, gg, img);
                            }

                        }
                    } catch (Throwable t) {
                        t.printStackTrace();
                    }
                }
            };
            Timer timer = new Timer();
            timer.scheduleAtFixedRate(task, 100, 150);

            self.addWindowListener(new WindowAdapter() {
                public void windowClosing(WindowEvent e) {
                    System.exit(0);
                }
            });
            self.setSize(basex + 1024 + 20, basey + 512 + 50);
            self.setVisible(true);
        }

        // Color cback = new Color(240, 240, 240);
        // Color cbox = new Color(230, 230, 230);
        // Color cline = new Color(255, 220, 220);
        Color cback = new Color(50, 50, 50);
        Color cbox = new Color(0, 0, 0);
        Color cline = new Color(255, 0, 0);

        private void drawframe(Canvas c, Graphics gg, BufferedImage img) {
            if (g == null) {
                g = c.getGraphics();
            }
            gg.setColor(cback);
            gg.fillRect(0, 0, 1024, 512);
            gg.setColor(cbox);
            gg.drawRect(0, 0, 1024, 256);
            gg.drawRect(0, 0 + 256, 1024, 256);
            gg.setColor(cline);

            int last_vbat = 0;
            int last_vbbt = 0;
            for (int i = 0; i < 1024; i++) {

                int vbat = frameA[i] & 0xFF;
                int vbbt = frameB[i] & 0xFF;

                if (i > 0) {
                    gg.drawLine(i, 255 - vbat, i - 1, 255 - last_vbat);
                    gg.drawLine(i, 511 - vbbt, i - 1, 511 - last_vbbt);
                }

                last_vbat = vbat;
                last_vbbt = vbbt;
            }

            g.drawImage(img, basex, basey, null);

            g.clearRect(0, 0, 600, 100);

            g.setColor(Color.RED);
            g.drawString(pos + "," + scale, 60, 50);

        }

        private void draw(Canvas c, Graphics gg, BufferedImage img) {
            if (recording)
                return;
            if (buffA_T == null)
                return;
            if (g == null) {
                g = c.getGraphics();
            }
            gg.setColor(cback);
            gg.fillRect(0, 0, 1024, 512);
            gg.setColor(cbox);
            gg.drawRect(0, 0, 1024, 256);
            gg.drawRect(0, 0 + 256, 1024, 256);
            gg.setColor(cline);

            int loc_pos = pos >> scale;
            byte[] bat;
            byte[] bab;
            byte[] bbt;
            byte[] bbb;
            if (scale == 0) {
                bat = buffA;
                bab = buffA;
                bbt = buffB;
                bbb = buffB;
            } else {
                bat = buffA_T.get(scale - 1);
                bab = buffA_B.get(scale - 1);
                bbt = buffB_T.get(scale - 1);
                bbb = buffB_B.get(scale - 1);
            }

            int last_vbat = 0;
            int last_vbab = 0;
            int last_vbbt = 0;
            int last_vbbb = 0;
            for (int i = 0; i < 1024; i++) {
                int newpos = loc_pos + i - 512;
                if (newpos < 0 || newpos >= bat.length || newpos < 0 || newpos >= bbt.length) {
                    continue;
                }
                int vbat = bat[newpos] & 0xFF;
                int vbab = bab[newpos] & 0xFF;
                int vbbt = bbt[newpos] & 0xFF;
                int vbbb = bbb[newpos] & 0xFF;

                gg.drawLine(i, 255 - vbat, i, 255 - vbab);
                gg.drawLine(i, 511 - vbbt, i, 511 - vbbb);

                if (i > 0) {
                    gg.drawLine(i, 255 - vbat, i - 1, 255 - last_vbat);
                    gg.drawLine(i, 255 - vbab, i - 1, 255 - last_vbab);
                    gg.drawLine(i, 511 - vbbt, i - 1, 511 - last_vbbt);
                    gg.drawLine(i, 511 - vbbb, i - 1, 511 - last_vbbb);
                }

                last_vbat = vbat;
                last_vbab = vbab;
                last_vbbt = vbbt;
                last_vbbb = vbbb;

            }

            g.drawImage(img, basex, basey, null);

            g.clearRect(0, 0, 600, 100);

            g.setColor(Color.RED);
            g.drawString(pos + "," + scale, 60, 50);

        }
    }

}