package rsa;

import java.awt.Button;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Label;
import java.awt.TextArea;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

public class Main
{
	public static void main(String[] args) throws Exception
	{
		new Client();
		new Server();
	}
}

class Server extends Frame implements WindowListener
{
	private static final long serialVersionUID = 1L;
	private PublicKey publicKey;
	private PrivateKey privateKey;
	public void windowClosing(WindowEvent e)
	{
		System.exit(0);
	}
	private Label l1 = new Label("public_key");
	private TextArea public_key = new TextArea("",3,100,TextArea.SCROLLBARS_NONE);
	private Button gpk = new Button("generate public_key");
	private Label l3 = new Label("coded");
	private TextArea coded = new TextArea("",3,100,TextArea.SCROLLBARS_NONE);
	private Button decode = new Button("decode");
	private Label l2 = new Label("text");
	private TextArea text = new TextArea("",3,100,TextArea.SCROLLBARS_NONE);
	public Server()
	{
		setTitle("RSA Server");
		addWindowListener(this);
		setLayout(new FlowLayout());
		setSize(740,400);
		this.setLocation(200,200);
		gpk.addActionListener(new Generate_public_key());
		
		add(l1);
		add(public_key);
		add(gpk);
		add(l3);
		add(coded);
		decode.addActionListener(new Decode());
		add(decode);
		add(l2);
		add(text);
		
		setVisible(true);
	}

	class Generate_public_key implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			KeyPair kp = RSA.getKeyPair(512);
			publicKey = kp.publicKey;
			privateKey = kp.privateKey;
			public_key.setText(Util.toString(publicKey.n));
		}
	}
	class Decode implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			String s = coded.getText();
			byte[] b = Util.getByte(s);
			try
			{
				ByteArrayInputStream bos = new ByteArrayInputStream(b);
				RSAInputStream ros = new RSAInputStream(bos,privateKey);
				byte[] buffer = new byte[1024000];
				int l = ros.read(buffer);
				text.setText(new String(buffer,0,l,"utf-8"));
				
			}
			catch(UnsupportedEncodingException ee)
			{
			}
			catch(IOException ee)
			{
			}
		}
	}
	public void windowActivated(WindowEvent e){};
	public void windowClosed(WindowEvent e){};
	public void windowDeactivated(WindowEvent e){};
	public void windowDeiconified(WindowEvent e){};
	public void windowIconified(WindowEvent e){};
	public void windowOpened(WindowEvent e){};
}

class Client extends Frame implements WindowListener
{
	private static final long serialVersionUID = 1L;
	private PublicKey publicKey;
	public void windowClosing(WindowEvent e)
	{
		System.exit(0);
	}
	private Label l1 = new Label("public_key");
	private TextArea public_key = new TextArea("",3,100,TextArea.SCROLLBARS_NONE);
	private Button ipk = new Button("input public_key");
	private Label l2 = new Label("text");
	private TextArea text = new TextArea("",3,100,TextArea.SCROLLBARS_NONE);
	private Button code = new Button("code");
	private Label l3 = new Label("coded");
	private TextArea coded = new TextArea("",3,100,TextArea.SCROLLBARS_NONE);

	public Client()
	{
		setTitle("RSA Client");
		addWindowListener(this);
		setLayout(new FlowLayout());
		setSize(740,400);
		this.setLocation(0,0);
		ipk.addActionListener(new GetPublicKey());
		
		add(l1);
		add(public_key);
		add(ipk);
		add(l2);
		add(text);
		code.addActionListener(new Code());
		add(code);
		add(l3);
		add(coded);
		setVisible(true);
	}

	class GetPublicKey implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			publicKey = new PublicKey();
			publicKey.length = 512;
			String spk = public_key.getText();
			publicKey.n = Util.getByte(spk);
			l2.setText("ok coded");
		}
	}
	class Code implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			String s = text.getText();
			try
			{
				byte[] b = s.getBytes("utf-8");
				ByteArrayOutputStream bos = new ByteArrayOutputStream();
				RSAOutputStream ros = new RSAOutputStream(bos,publicKey);
				ros.write(b);
				ros.close();
				bos.flush();
				byte[] bout = bos.toByteArray();
				coded.setText(Util.toString(bout));
				
			}
			catch(UnsupportedEncodingException ee)
			{
			}
			catch(IOException ee)
			{
			}
		}
	}

	public void windowActivated(WindowEvent e){};
	public void windowClosed(WindowEvent e){};
	public void windowDeactivated(WindowEvent e){};
	public void windowDeiconified(WindowEvent e){};
	public void windowIconified(WindowEvent e){};
	public void windowOpened(WindowEvent e){};
}

class Util
{
	private final static int[] CHARS = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
		0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	public static byte[] getByte(String s)
	{
		char[] bsrc = s.toCharArray();
		byte[] b = new byte[bsrc.length/2];
		int idx = 0;
		int i = 0;
		while (i < bsrc.length)
		{
			int va = CHARS[bsrc[i]];
			i++;
			int vb = CHARS[bsrc[i]];
			i++;
			int v = (va << 4) + vb;
			b[idx] = (byte) v;
			idx++;
		}
		return b;
	}
	public static String toString(byte[] b)
	{
		StringBuffer sb = new StringBuffer();
		for(int i=0;i<b.length;i++)
		{
			int v = b[i] & 0x0FF;
			if(v<16)
			{
				sb.append("0");
			}
			sb.append(Integer.toHexString(v).toUpperCase());
		}
		return sb.toString();
	}
}