import jmtp.PortableDevice;
import jmtp.PortableDeviceManager;
import jmtp.PortableDeviceObject;
import jmtp.PortableDeviceStorageObject;


public class Delete {
	static {
		System.loadLibrary("jmtp"); // ע�⣺���ض�̬���ӿ⣬��д��չ��������Ҫ��dll���ļ���һ��
	}


	public static void main(String[] args) {
		PortableDeviceStorageObject root = getRoot(0);
		PortableDeviceObject[]  list = root.getChildObjects();
		for(PortableDeviceObject item:list){
			if("HaierRRSLJ".equals(item.getOriginalFileName())){
				try{
					System.out.print(item.canDelete());
				item.delete();
				}catch(Throwable t){
					t.printStackTrace();
				}
			}
			
		}
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
