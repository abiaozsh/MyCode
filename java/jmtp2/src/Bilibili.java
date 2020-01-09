import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;

import org.codehaus.jackson.annotate.JsonIgnoreProperties;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.type.TypeFactory;

import be.derycke.pieter.com.COMException;
import jmtp.PortableDevice;
import jmtp.PortableDeviceFolderObject;
import jmtp.PortableDeviceManager;
import jmtp.PortableDeviceObject;
import jmtp.PortableDeviceStorageObject;
import jmtp.PortableDeviceToHostImpl32;

//https://github.com/ultrah/jMTPe
public class Bilibili {

	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {

		//getList(1, "a.txt");

		copy(1, "b.txt", "I:\\bilibili6");
	}

	public static void getList(int dev, String fn) throws Exception {

		FileOutputStream fos = new FileOutputStream(fn);
		OutputStreamWriter osw = new OutputStreamWriter(fos);

		PortableDeviceStorageObject root = getRoot(dev);
		PortableDeviceFolderObject bilibili = getBilibili(root);
		PortableDeviceObject[] objs = bilibili.getChildObjects();

		ArrayList<ObjF> list = new ArrayList<ObjF>();

		Comparator<ObjF> comparatorName = new Comparator<ObjF>() {
			@Override
			public int compare(ObjF a, ObjF b) {
				return a.title.compareTo(b.title);
			}
		};

		Comparator<ObjF> comparatorSize = new Comparator<ObjF>() {
			@Override
			public int compare(ObjF a, ObjF b) {
				return (a.size > b.size) ? -1 : 1;
			}
		};

		for (PortableDeviceObject item : objs) {
			System.out.println(item.getOriginalFileName());

			PortableDeviceObject[] all = getList(item);
			if (all == null)
				continue;
			boolean first = true;
			ObjF objf = new ObjF();
			objf.size = 0;
			for (PortableDeviceObject item2 : all) {
				PortableDeviceObject entry = (PortableDeviceObject) findFile(item2, "entry.json");

				deleteFile("entry.json");
				try {
					host.copyFromPortableDeviceToHost(entry.getID(), ".", device);
				} catch (COMException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				FileInputStream fis = new FileInputStream("entry.json");
				InputStreamReader isr = new InputStreamReader(fis, "utf-8");
				char[] buff = new char[4096];
				int len = isr.read(buff);
				String s = new String(buff, 0, len);
				Obj obj = deserialize(s, Obj.class);

				isr.close();
				fis.close();

				if (first) {
					first = false;
					objf.id = item.getOriginalFileName();
					objf.title = obj.title;
				}
				objf.size += obj.downloaded_bytes;
			}

			list.add(objf);
		}
		Collections.sort(list, comparatorSize);
		for (ObjF item : list) {

			osw.write(item.id + "," + (item.size / 1024 / 1024) + "," + item.title + "\r\n");
			osw.flush();
		}

		fos.close();
	}

	static PortableDevice device;

	static PortableDeviceStorageObject getRoot(int idx) {

		PortableDeviceManager manager = new PortableDeviceManager();
		for (PortableDevice device : manager) {
			device.open();
			Bilibili.device = device;
			PortableDeviceObject[] roots = device.getRootObjects();
			if (roots.length > idx) {
				for (int i = 0; i < roots.length; i++) {
					if (i == idx) {
						PortableDeviceStorageObject root = (PortableDeviceStorageObject) roots[i];
						return root;
					}
				}
			}
		}
		return null;
	}

	static PortableDeviceObject findFile(PortableDeviceObject folder, String name) {
		if (PortableDeviceStorageObject.class.isAssignableFrom(folder.getClass())) {
			PortableDeviceStorageObject f = (PortableDeviceStorageObject) folder;
			PortableDeviceObject[] objs = f.getChildObjects();
			for (PortableDeviceObject item : objs) {
				if (name.equals(item.getOriginalFileName())) {
					return item;
				}
			}
		} else if (PortableDeviceFolderObject.class.isAssignableFrom(folder.getClass())) {
			PortableDeviceFolderObject f = (PortableDeviceFolderObject) folder;
			PortableDeviceObject[] objs = f.getChildObjects();
			for (PortableDeviceObject item : objs) {
				if (name.equals(item.getOriginalFileName())) {
					return item;
				}
			}
		} else {
			return null;
		}
		return null;
	}

	static PortableDeviceObject findFile(PortableDeviceObject folder) {
		if (PortableDeviceStorageObject.class.isAssignableFrom(folder.getClass())) {
			PortableDeviceStorageObject f = (PortableDeviceStorageObject) folder;
			PortableDeviceObject[] objs = f.getChildObjects();
			for (PortableDeviceObject item : objs) {
				return item;
			}
		} else if (PortableDeviceFolderObject.class.isAssignableFrom(folder.getClass())) {
			PortableDeviceFolderObject f = (PortableDeviceFolderObject) folder;
			PortableDeviceObject[] objs = f.getChildObjects();
			for (PortableDeviceObject item : objs) {
				return item;
			}
		} else {
			return null;
		}
		return null;
	}

	static PortableDeviceObject[] getList(PortableDeviceObject folder) {
		if (PortableDeviceStorageObject.class.isAssignableFrom(folder.getClass())) {
			PortableDeviceStorageObject f = (PortableDeviceStorageObject) folder;
			PortableDeviceObject[] objs = f.getChildObjects();
			return objs;
		} else if (PortableDeviceFolderObject.class.isAssignableFrom(folder.getClass())) {
			PortableDeviceFolderObject f = (PortableDeviceFolderObject) folder;
			PortableDeviceObject[] objs = f.getChildObjects();
			return objs;
		} else {
			return null;
		}
	}

	static PortableDeviceFolderObject getBilibili(PortableDeviceStorageObject root) {
		PortableDeviceFolderObject Android = (PortableDeviceFolderObject) findFile(root, "Android");
		PortableDeviceFolderObject data = (PortableDeviceFolderObject) findFile(Android, "data");
		PortableDeviceFolderObject bili = (PortableDeviceFolderObject) findFile(data, "tv.danmaku.bili");
		PortableDeviceFolderObject download = (PortableDeviceFolderObject) findFile(bili, "download");
		return download;
	}

	static PortableDeviceToHostImpl32 host = new PortableDeviceToHostImpl32();

	public static boolean createDir(String destDirName) {
		File dir = new File(destDirName);
		if (dir.exists()) {// 判断目录是否存在
			return false;
		}
		if (!destDirName.endsWith(File.separator)) {// 结尾是否以"/"结束
			destDirName = destDirName + File.separator;
		}
		if (dir.mkdirs()) {// 创建目标目录
			return true;
		} else {
			return false;
		}
	}

	public static void copyFolder(PortableDeviceFolderObject folder, String target) {
		PortableDeviceObject[] objs = folder.getChildObjects();
		createDir(target);
		for (PortableDeviceObject item : objs) {

			if (PortableDeviceFolderObject.class.isAssignableFrom(item.getClass())) {
				copyFolder((PortableDeviceFolderObject) item, target + "\\" + item.getOriginalFileName());
			} else {
				try {
					System.out.println("\t" + item.getOriginalFileName());
					host.copyFromPortableDeviceToHost(item.getID(), target, device);
				} catch (COMException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		}

	}

	@JsonIgnoreProperties(ignoreUnknown = true)
	public static class Obj {
		public String title;
		public long total_bytes;
		public long downloaded_bytes;
		// "total_bytes":30569301,"downloaded_bytes":30569301,
	}

	public static <T> T deserialize(String json, Class<T> clazz) {
		ObjectMapper objectMapper = new ObjectMapper();
		T object = null;
		try {
			object = (T) objectMapper.readValue(json, TypeFactory.rawClass(clazz));
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return object;
	}

	public static boolean deleteFile(String fileName) {
		File file = new File(fileName);
		// 如果文件路径所对应的文件存在，并且是一个文件，则直接删除
		if (file.exists() && file.isFile()) {
			if (file.delete()) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	public static class ObjF {
		public String id;
		public String title;
		public long size;
	}

	public static void copy(int dev, String fn, String path) throws Exception {

		FileInputStream fis1 = new FileInputStream(fn);
		InputStreamReader isr1 = new InputStreamReader(fis1);
		char[] buff = new char[1024 * 512];
		int len = isr1.read(buff);
		String s = new String(buff, 0, len);
		isr1.close();
		fis1.close();

		HashSet<String> hs = new HashSet<String>();
		String[] ss = s.split("\r\n");
		for (String s1 : ss) {
			String id = s1.split(",")[0];
			hs.add(id);
		}

		PortableDeviceStorageObject root = getRoot(dev);
		PortableDeviceFolderObject bilibili = getBilibili(root);
		PortableDeviceObject[] objs = bilibili.getChildObjects();
		for (PortableDeviceObject item : objs) {
			if (!hs.contains(item.getOriginalFileName()))
				continue;
			System.out.println(item.getOriginalFileName());
			PortableDeviceFolderObject one = (PortableDeviceFolderObject) findFile(item);
			if (one == null)
				continue;

			PortableDeviceObject entry = (PortableDeviceObject) findFile(one, "entry.json");

			deleteFile("entry.json");
			try {
				host.copyFromPortableDeviceToHost(entry.getID(), ".", device);
			} catch (COMException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			FileInputStream fis = new FileInputStream("entry.json");
			InputStreamReader isr = new InputStreamReader(fis, "utf-8");
			len = isr.read(buff);
			isr.close();
			fis.close();
			s = new String(buff, 0, len);
			Obj obj = deserialize(s, Obj.class);

			String name = obj.title;
			name = name.replace("\\", "");
			name = name.replace("/", "");
			name = name.replace(":", "");
			name = name.replace("-", "");
			name = name.replace("?", "？");
			name = name.replace("|", "｜");

			String tmp = item.getOriginalFileName() + "_" + name;

			copyFolder((PortableDeviceFolderObject) item, path + "\\" + tmp);

		}

	}

}
