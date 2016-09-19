package iptool;

import java.util.List;
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
		// while (true) {
		// try {
		// Thread.sleep(60000);
		// } catch (Throwable t) {
		// }
		// }

		SimpleMailSender mail = new SimpleMailSender("abiaozsh@sina.cn", "920662zsh");

		try {
			mail.send("abiaozsh@sina.cn", "new", "newfewfdsfdsfsd");
		} catch (Throwable t) {
			t.printStackTrace();
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
