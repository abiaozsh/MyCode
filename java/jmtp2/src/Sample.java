import java.util.ArrayList;

import jmtp.PortableDevice;
import jmtp.PortableDeviceFolderObject;
import jmtp.PortableDeviceManager;
import jmtp.PortableDeviceObject;
import jmtp.PortableDeviceStorageObject;

public class Sample {
	static {
		System.loadLibrary("jmtp"); // 注意：加载动态链接库，不写扩展名，名字要与dll的文件名一样
	}

	// static Folder.TRoot tRoot;

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Throwable {

		// tRoot = new Folder.TRoot();

		Folder.TRoot root = GetFolder(0);

		root.save("E:\\memcard2.json");

	}

	public static Folder.TRoot GetFolder(int dev) {
		Folder.TRoot tRoot = new Folder.TRoot();
		PortableDeviceStorageObject root = getRoot(dev);
		Folder.TFolder tFolder = GetFolder(root);

		tRoot.path = root.getName();

		tRoot.fileList = tFolder.fileList;
		tRoot.folderList = tFolder.folderList;
		tRoot.name = tFolder.name;
		tRoot.filecount = tFolder.filecount;
		tRoot.datetime = tFolder.datetime;
		tRoot.size = tFolder.size;

		return tRoot;
	}

	public static Folder.TFolder GetFolder(PortableDeviceObject folder) {
		try {
			Folder.TFolder tf = new Folder.TFolder();
			tf.folderList = new ArrayList<Folder.TFolder>();
			tf.fileList = new ArrayList<Folder.TFile>();

			PortableDeviceObject[] objs = null;
			if (PortableDeviceStorageObject.class.isAssignableFrom(folder.getClass())) {
				PortableDeviceStorageObject f = (PortableDeviceStorageObject) folder;
				objs = f.getChildObjects();
				tf._SetDatetime(f.getDateModified());
				tf.name = f.getName();

			} else if (PortableDeviceFolderObject.class.isAssignableFrom(folder.getClass())) {
				PortableDeviceFolderObject f = (PortableDeviceFolderObject) folder;
				objs = f.getChildObjects();
				tf._SetDatetime(f.getDateModified());
				tf.name = f.getOriginalFileName();

			} else {
				return null;
			}

			System.out.println(tf.name);
			long size = 0;
			int filecount = 0;
			for (int i = 0; i < objs.length; i++) {
				PortableDeviceObject obj = objs[i];

				if (PortableDeviceFolderObject.class.isAssignableFrom(obj.getClass())) {
					Folder.TFolder tempf = GetFolder((PortableDeviceFolderObject) obj);
					tf.folderList.add(tempf);
					size += tempf.size;
					filecount += tempf.filecount;
				} else {
					Folder.TFile tfile = new Folder.TFile();
					tfile.name = obj.getOriginalFileName();
					System.out.println(tfile._GetFullPath());
					tfile.size = obj.getSize().longValue();
					tfile._SetDatetime(obj.getDateModified());
					tf.fileList.add(tfile);
					size += obj.getSize().longValue();
					filecount++;
				}
			}
			tf.size = size;
			tf.filecount = filecount;
			return tf;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	static PortableDeviceStorageObject getRoot(int idx) {

		PortableDeviceManager manager = new PortableDeviceManager();
		for (PortableDevice device : manager) {
			device.open();
			PortableDeviceObject[] roots = device.getRootObjects();
			for (int i = 0; i < roots.length; i++) {
				if (i == idx) {
					PortableDeviceStorageObject root = (PortableDeviceStorageObject) roots[i];
					return root;
				}
			}
		}
		return null;
	}

}
