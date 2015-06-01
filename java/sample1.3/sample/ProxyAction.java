//package sample;
//
//import java.io.ByteArrayOutputStream;
//import java.io.FileInputStream;
//import java.io.IOException;
//import java.io.InputStream;
//import java.io.OutputStream;
//import java.net.InetSocketAddress;
//import java.net.ServerSocket;
//import java.net.Socket;
//import java.net.SocketException;
//import java.security.KeyStore;
//import java.util.ArrayList;
//import java.util.HashMap;
//
//import javax.net.ssl.KeyManagerFactory;
//import javax.net.ssl.SSLContext;
//import javax.net.ssl.SSLServerSocketFactory;
//
//import sun.misc.BASE64Decoder;
//import webServer.MultipartData;
//import webServer.Request;
//import webServer.RequestDispatcher;
//import webServer.Response;
//import webServer.Server;
//import webServer.Servlet;
//import webServer.Session;
//
//import com.sun.corba.se.impl.javax.rmi.CORBA.Util;
//
//public class ProxyAction extends Servlet {
//	private static ProxyThread proxy;
//
//	@Override
//	public void doReq(Request req, Response res, Server server) throws Exception {
//		String cmd = req.getGETParam("cmd");
//		String port = req.getGETParam("port");
//
//	}
//
//}
//
//final class ProxyThread extends Thread {
//	ServerSocket sv;
//
//	ArrayList<Processor> ps = new ArrayList<Processor>();
//
//	public ProxyThread(boolean SSL, int port) {
//		if (SSL) {
//			sv = getSSLServerSocket(port);
//		} else {
//			sv = new ServerSocket(port);
//		}
//	}
//
//	@Override
//	public void run() {
//		try {
//			while (true) {
//				if (sv == null) {
//					break;
//				}
//				try {
//					Socket s;
//					s = sv.accept();
//					Processor p = new Processor(s, ps);
//					String ip = ((InetSocketAddress) s.getRemoteSocketAddress()).getAddress().getHostAddress();
//					String port = Integer.toString(((InetSocketAddress) s.getRemoteSocketAddress()).getPort());
//					p.setName("Request Prosessor(" + ip + ":" + port + ")");
//					ps.add(p);
//					p.start();
//				} catch (SocketException e) {
//					if ("socket closed".equals(e.getMessage())) {
//						break;
//					}
//				} catch (Exception e) {
//					server.log(e);
//				}
//			}
//		} catch (Exception e) {
//			server.log(e);
//		} finally {
//			try {
//				sv.close();
//			} catch (Exception e) {
//				//
//			}
//		}
//	}
//
//	public void shutdown() {
//		try {
//			sv.close();
//			sv = null;
//		} catch (IOException e) {
//			//
//		}
//		for (Processor p : ps) {
//			p.shutdown();
//		}
//	}
//
//	private static ServerSocket getSSLServerSocket(int port) throws Exception {
//		ServerSocket s = null;
//
//		char keyStorePass[] = "123456".toCharArray();
//		char keyPassword[] = "123456".toCharArray();
//
//		// KeyStore ks = KeyStore.getInstance("JKS");
//		KeyStore ks = KeyStore.getInstance(KeyStore.getDefaultType());
//
//		ks.load(new FileInputStream("clientKeys.key"), keyStorePass);
//
//		// KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509");
//		KeyManagerFactory kmf = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
//
//		kmf.init(ks, keyPassword);
//
//		SSLContext sslContext = SSLContext.getInstance("SSLv3");
//		// SSLContext sslContext = SSLContext.getInstance("TLS");
//		// SSLContext sslContext = SSLContext.getInstance("SSL");
//		// SSLContext sslContext = SSLContext.getInstance("TLSv1");
//
//		sslContext.init(kmf.getKeyManagers(), null, null);
//
//		SSLServerSocketFactory factory = sslContext.getServerSocketFactory();
//
//		s = factory.createServerSocket(port);
//
//		return s;
//	}
//}
//
//final class Processor extends Thread {
//	private final static int[] CHARS = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//
//	boolean cutOff = false;
//
//	private Socket socket;
//
//	private Server server;
//
//	private ArrayList ps;
//
//	private InputStream is;
//
//	private OutputStream os;
//
//	Processor(Socket socket, Server server, ArrayList ps) throws IOException {
//		this.socket = socket;
//		this.server = server;
//		this.ps = ps;
//		is = socket.getInputStream();
//		os = socket.getOutputStream();
//	}
//
//	void shutdown() {
//		this.cutOff = true;
//	}
//
//	@Override
//	public void run() {
//		try {
//			while (true) {
//				String head = getHead(is);
//				if (head == null) {
//					break;
//				}
//
//				setRequest(head, req);
//
//				if (req.contentLength > 0) {
//					req = getContent(req, is);
//				}
//
//				req.server = server;
//
//				server.log("requested:" + req.getPath() + "[" + req.getRemoteAddr() + ":" + req.getRemotePort() + "]");
//
//				RequestDispatcher rd = server.getRequestDispatcher(req.getPath());
//
//				Response res = new Response(req, server);
//
//				res.req = req;
//				res.GZIP = req.GZIP;
//				res.closeConnection = req.closeConnection;
//
//				rd.forward(req, res);
//
//				writeResponse(res, req, os);
//
//				if (cutOff || res.closeConnection) {
//					break;
//				}
//			}
//		} catch (SocketException se) {
//			// SocketException
//			se.printStackTrace();
//		} catch (Exception e) {
//			server.log(e);
//		} finally {
//			try {
//				ps.remove(this);
//				os.close();
//				is.close();
//				socket.close();
//			} catch (Exception e) {
//				//
//			}
//		}
//	}
//
//	private String getHead(InputStream is) throws IOException {
//		try {
//			ByteArrayOutputStream bos = new ByteArrayOutputStream();
//			byte[] b = new byte[4];
//			for (int i = 0; i < 4; i++) {
//				int v = is.read();
//				if (v >= 0) {
//					b[i] = (byte) v;
//				} else {
//					return null;
//				}
//			}
//			while (true) {
//				if (b[0] == '\r' && b[1] == '\n' && b[2] == '\r' && b[3] == '\n') {
//					break;
//				}
//				bos.write(b[0]);
//				b[0] = b[1];
//				b[1] = b[2];
//				b[2] = b[3];
//				int v = is.read();
//				if (v >= 0) {
//					b[3] = (byte) v;
//				} else {
//					return null;
//				}
//			}
//			return new String(bos.toByteArray(), "ISO-8859-1");
//		} catch (SocketException e) {
//			// SocketException
//			return null;
//		}
//	}
//
//	private String getUnicode(String s) throws IOException {
//		char[] bsrc = s.toCharArray();
//		byte[] b = new byte[bsrc.length];
//		int idx = 0;
//		int i = 0;
//		while (i < bsrc.length) {
//			if (bsrc[i] != '%') {
//				b[idx] = (byte) bsrc[i];
//				idx++;
//				i++;
//			} else {
//				i++;
//				int va = CHARS[bsrc[i]];
//				i++;
//				int vb = CHARS[bsrc[i]];
//				i++;
//				int v = (va << 4) + vb;
//				b[idx] = (byte) v;
//				idx++;
//			}
//		}
//		return new String(b, 0, idx, "utf-8");
//	}
//
//	private void setRequest(String sReq, Request req) throws Exception {
//		try {
//			req.setRemoteAddress(socket.getRemoteSocketAddress());
//			String[] level1 = sReq.split("\r\n");
//			Session session = null;
//			for (int i = 0; i < level1.length; i++) {
//				if (i == 0) {
//					String[] level2 = level1[i].split(" ");
//					if ("GET".equalsIgnoreCase(level2[0])) {
//						req.setMethod(Request.METHOD_GET);
//					}
//					if ("POST".equalsIgnoreCase(level2[0])) {
//						req.setMethod(Request.METHOD_POST);
//					}
//					String[] level3 = level2[1].split("\\?");// ?
//					req.setPath(getUnicode(level3[0]));
//					if (level3.length > 1) {
//						String[] level4 = level3[1].split("&");
//						for (int j = 0; j < level4.length; j++) {
//							String[] level5 = getUnicode(level4[j]).split("=");
//							if (level5.length == 2) {
//								req.setGETParam(level5[0], level5[1]);
//							}
//						}
//					}
//					String[] level31 = level2[2].split("/");
//					if (level31.length == 2) {
//						if ("HTTP".equals(level31[0])) {
//							req.setProtocol(Request.PROTOCOL_HTTP);
//						} else {
//							req.setProtocol(Request.PROTOCOL_OTHER);
//						}
//						req.setHTTPVersion(level31[1]);
//					}
//				} else {
//					String[] level2 = level1[i].split(": ", 2);
//					if (level2.length == 2) {
//						String name = level2[0];
//						String value = level2[1];
//						if ("Cookie".equals(name)) {
//							String[] cookies = value.split("; ");
//							for (int j = 0; j < cookies.length; j++) {
//								String[] level3 = cookies[j].split("=", 2);
//								if (level3.length == 2) {
//									String cookie_name = level3[0];
//									String cookie_value = level3[1];
//									if (Response.JSESSIONID.equals(cookie_name)) {
//										session = server.getSession(cookie_value);
//									} else {
//										req.setCookie(cookie_name, cookie_value);
//									}
//								}
//							}
//						} else if ("Accept-Encoding".equals(name)) {
//							if (value.indexOf("gzip") >= 0) {
//								req.GZIP = true;
//							}
//						} else if ("Content-Length".equals(name)) {
//							req.contentLength = Util.strToInt(value);
//						} else if ("Authorization".equals(name)) {
//							if (value.startsWith("Basic")) {
//								BASE64Decoder decoder = new BASE64Decoder();
//								String authorization = value.substring(6);
//								authorization = new String(decoder.decodeBuffer(authorization));
//								req.authorizationUsername = authorization.substring(0, authorization.indexOf(':'));
//								req.authorizationPassword = authorization.substring(authorization.indexOf(':') + 1);
//							}
//						} else if ("Content-Type".equals(name)) {
//							req.setContentType(value);
//						} else if ("Accept-Charset".equals(name)) {
//							req.setCharset(getCharset(value));
//						} else if ("Accept-Language".equals(name)) {
//							req.setLocale(getLocale(value));
//						} else if ("Connection".equals(name)) {
//							if (!"close".equalsIgnoreCase(value)) {
//								req.closeConnection = true;
//							}
//						} else {
//							req.setHTTPParam(name, value);
//						}
//					}
//				}
//			}
//			if (session != null) {
//				req.setSession(session);
//				session.lastUsedTime = System.currentTimeMillis();
//				req.sessionIsNull = false;
//			} else {
//				req.sessionIsNull = true;
//			}
//		} catch (Exception e) {
//			throw new Exception("error on request");
//		}
//	}
//
//	private Request getContent(Request r, InputStream is) throws IOException {
//		byte[] b = getByteArray(is, r.contentLength);
//		if (r.getContentType() != null && r.getContentType().startsWith("multipart/form-data")) {
//			String boundary = r.getContentType().split("boundary=")[1];
//			if (boundary.length() > 0) {
//				byte[] bBoundary = boundary.getBytes("ISO-8859-1");
//				int lastIndex = 2 + bBoundary.length + 2;
//				for (int i = 2 + bBoundary.length + 2; i < (b.length - bBoundary.length); i++) {
//					boolean flg = true;
//					for (int j = 0; j < bBoundary.length; j++) {
//						if (b[i + j] != bBoundary[j]) {
//							flg = false;
//							break;
//						}
//					}
//					if (flg) {
//						if (i > lastIndex) {
//							int len = i - lastIndex - 4;
//							byte[] head = new byte[len];
//							int headLength = len;
//							for (int k = 0; k < len; k++) {
//								if (k < len - 4 && b[lastIndex + k] == '\r' && b[lastIndex + k + 1] == '\n' && b[lastIndex + k + 2] == '\r' && b[lastIndex + k + 3] == '\n') {
//									headLength = k;
//									k += 4;
//									break;
//								}
//								head[k] = b[lastIndex + k];
//							}
//							String sHead = new String(head, 0, headLength, server.currentConfig.defaultEncoding);
//							byte[] data = null;
//							if (len - headLength - 4 > 0) {
//								data = new byte[len - headLength - 4];
//								System.arraycopy(b, lastIndex + headLength + 4, data, 0, len - headLength - 4);
//							}
//							MultipartData d = new MultipartData();
//							String[] level1 = sHead.split("\r\n");
//							for (int l = 0; l < level1.length; l++) {
//								String[] level2 = level1[l].split(": ", 2);
//								String[] level3 = level2[1].split("; ");
//								for (int m = 0; m < level3.length; m++) {
//									String[] level4 = level3[m].split("=");
//									if ("name".equals(level4[0])) {
//										d.name = level4[1].substring(1, level4[1].length() - 1);
//									}
//									if ("filename".equals(level4[0])) {
//										d.filename = level4[1].substring(1, level4[1].length() - 1);
//									}
//								}
//							}
//							d.bArray = data;
//							r.setMultipartData(d.name, d);
//							i += bBoundary.length + 2;
//							lastIndex = i;
//						}
//					}
//				}
//			}
//		} else {
//			String s = new String(b, "ISO-8859-1");
//			String[] level2 = s.split("&");
//			for (int j = 0; j < level2.length; j++) {
//				String[] level3 = getUnicode(level2[j].replace('+', ' ')).split("=", 2);
//				if (level3.length == 2) {
//					r.setPOSTParam(level3[0], level3[1]);
//				}
//			}
//		}
//		return r;
//	}
//
//
//	private byte[] getByteArray(InputStream si, int size) throws IOException {
//		ByteArrayOutputStream o = new ByteArrayOutputStream();
//		int count = size;
//		while (count > 0) {
//			byte[] buf = new byte[1024];
//			int l = si.read(buf, 0, 1024);
//			count -= l;
//			o.write(buf, 0, l);
//			if (count <= 0)
//				break;
//		}
//		return o.toByteArray();
//	}
//
//}
