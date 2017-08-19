package iptool;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Properties;

import javax.mail.Authenticator;
import javax.mail.Message.RecipientType;
import javax.mail.MessagingException;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

public class Iptool {
	static String oldip = "";

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		while (true) {
			try {
				V2();

				Thread.sleep(60000);
			} catch (Throwable t) {
				t.printStackTrace();
			}
		}

	}

	static void V1() throws Throwable {
		// http://192.168.0.1/RST_status.htm
		URL url = new URL("http://192.168.0.1/RST_status.htm");// "http://localhost:81/test"
		HttpURLConnection urlcon = (HttpURLConnection) url.openConnection();

		String loginPassword = "admin:920662";// "aa" + ":" + "bb";
		String encoded = new sun.misc.BASE64Encoder().encode(loginPassword.getBytes());

		urlcon.setRequestProperty("Authorization", "Basic " + encoded);

		InputStream is = urlcon.getInputStream();
		InputStreamReader isr = new InputStreamReader(is, "utf-8");

		StringBuilder sb = new StringBuilder();
		while (true) {
			int c = isr.read();
			if (c == -1)
				break;
			sb.append((char) c);
		}
		String s1 = sb.toString();

		String target = "<tr><td width=\"60%\"><b>IP地址</b></td><td>";
		String t2 = "</td></tr>";

		int index1 = s1.indexOf(target);
		String s2 = s1.substring(index1 + target.length());
		int index2 = s2.indexOf(t2);
		String ip = s2.substring(0, index2);

		MailIp(ip);

	}

	static void V2() throws Throwable {
		// http://192.168.0.1/RST_status.htm
		URL url = new URL("http://www.ip.cn/");// "http://localhost:81/test"
		HttpURLConnection urlcon = (HttpURLConnection) url.openConnection();

		// urlcon.setRequestProperty("Authorization", "Basic " + encoded);
		urlcon.setRequestProperty("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
		urlcon.setRequestProperty("Accept-Encoding", "deflate");
		urlcon.setRequestProperty("Accept-Language", "zh-CN,zh;q=0.8");
		urlcon.setRequestProperty("Cache-Control", "max-age=0");
		urlcon.setRequestProperty("Connection", "keep-alive");
		urlcon.setRequestProperty("Cookie", "UM_distinctid=15df927390e1b1-0345d371bc3e2e-3f63440c-1fa400-15df927390f39d; CNZZDATA123770=cnzz_eid%3D654128537-1503123642-%26ntime%3D1503123642");
		urlcon.setRequestProperty("Host", "www.ip.cn");
		urlcon.setRequestProperty("Upgrade-Insecure-Requests", "1");
		urlcon.setRequestProperty("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.101 Safari/537.36");

		InputStream is = urlcon.getInputStream();
		InputStreamReader isr = new InputStreamReader(is, "utf-8");

		StringBuilder sb = new StringBuilder();
		while (true) {
			int c = isr.read();
			if (c == -1)
				break;
			sb.append((char) c);
		}
		String s1 = sb.toString();

		String target = "<p>您现在的 IP：<code>";
		String t2 = "</code></p><p>";

		int index1 = s1.indexOf(target);
		String s2 = s1.substring(index1 + target.length());
		int index2 = s2.indexOf(t2);
		String ip = s2.substring(0, index2);

		MailIp(ip);
	}

	static void MailIp(String ip) {
		System.out.println("curr IP：" + ip);
		if (!oldip.equals(ip)) {
			oldip = ip;
			SimpleMailSender mail = new SimpleMailSender("abiaozsh@sina.cn", "920662zsh");

			try {
				mail.send("abiaozsh@sina.cn", "ip", ip);
			} catch (Throwable t) {
				t.printStackTrace();
			}
		}

	}

}

class MailAuthenticator extends Authenticator {
	private String username;
	private String password;

	public MailAuthenticator(String username, String password) {
		this.username = username;
		this.password = password;
	}

	String getPassword() {
		return password;
	}

	@Override
	protected PasswordAuthentication getPasswordAuthentication() {
		return new PasswordAuthentication(username, password);
	}

	String getUsername() {
		return username;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public void setUsername(String username) {
		this.username = username;
	}
}

class SimpleMailSender {
	MailAuthenticator authenticator;
	private final transient Properties props = System.getProperties();
	private transient Session session;

	public SimpleMailSender(final String smtpHostName, final String username, final String password) {
		init(username, password, smtpHostName);
	}

	public SimpleMailSender(final String username, final String password) {
		// 通过邮箱地址解析出smtp服务器，对大多数邮箱都管用
		final String smtpHostName = "smtp." + username.split("@")[1];
		init(username, password, smtpHostName);

	}

	private void init(String username, String password, String smtpHostName) {
		// 初始化props
		props.put("mail.smtp.auth", "true");
		props.put("mail.smtp.host", smtpHostName);
		// 验证
		authenticator = new MailAuthenticator(username, password);
		// 创建session
		session = Session.getInstance(props, authenticator);
	}

	public void send(String recipient, String subject, String content) throws AddressException, MessagingException {
		// 创建mime类型邮件
		final MimeMessage message = new MimeMessage(session);
		// 设置发信人
		message.setFrom(new InternetAddress(authenticator.getUsername()));
		// 设置收件人
		message.setRecipient(RecipientType.TO, new InternetAddress(recipient));
		// 设置主题
		message.setSubject(subject);
		// 设置邮件内容
		message.setContent(content, "text/html;charset=utf-8");
		// 发送
		Transport.send(message);
	}

}
