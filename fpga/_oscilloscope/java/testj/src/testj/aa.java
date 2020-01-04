package testj;

import java.awt.Button;
import java.awt.Canvas;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class aa {
	public static void main(String[] args) {
		Frame f = new Frame("fdsa");
		f.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
		
		Button b = new Button("bbb");

		Canvas c = new Canvas();
		
		
		f.setSize(400, 300);
		f.add(c);
		
		f.add(b);
//		final JButton btn = new JButton("FullScreen");
//		btn.addActionListener(new ActionListener() {
//			public void actionPerformed(ActionEvent e) {
//				if (btn.getText().equals("FullScreen")) {
//					f.dispose();
//					f.setUndecorated(true);
//					f.getGraphicsConfiguration().getDevice().setFullScreenWindow(f);
//					f.setVisible(true);
//					btn.setText("NormalMode");
//				} else {
//					f.dispose();
//					f.setUndecorated(false);
//					f.getGraphicsConfiguration().getDevice().setFullScreenWindow(null);
//					f.setVisible(true);
//					btn.setText("FullScreen");
//				}
//			}
//		});

		//f.getContentPane().setLayout(new FlowLayout());
		//f.getContentPane().add(btn);
		//f.pack();
		//f.setLocationRelativeTo(null);
		//f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setVisible(true);
	}
}