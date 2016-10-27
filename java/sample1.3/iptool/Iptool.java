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

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		String oldip = "";
		while (true) {
			try {
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

				Thread.sleep(60000);
			} catch (Throwable t) {
				t.printStackTrace();
				break;
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
