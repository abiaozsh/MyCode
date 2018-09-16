import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;

import org.codehaus.jackson.annotate.JsonIgnoreProperties;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.type.TypeFactory;

public class Extract {

	@JsonIgnoreProperties(ignoreUnknown = true)
	public static class ObjF {
		public String avid;
		public String title;
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

	public static String root = "e:\\bilibili\\";

	public static void main(String[] args) throws IOException {

		File dir = new File(root);

		File[] files = dir.listFiles();

		for (int i = 0; i < files.length; i++) {
			File file = files[i];

			if (file.isDirectory()) {
				proc(file);
			}
		}

		files = dir.listFiles();

		for (int i = 0; i < files.length; i++) {
			File file = files[i];
			if (!file.isDirectory()) {
				if (file.getName().endsWith(".blv")) {
					String n = file.getPath();
					file.renameTo(new File(n.substring(0, n.length() - 4) + ".flv"));
				}
				if (file.getName().endsWith(".4m.sum")) {
					file.delete();
				}
			}
		}

	}

	public static void proc(File dir) throws IOException {

		File[] files = dir.listFiles();

		for (int i = 0; i < files.length; i++) {
			File file = files[i];
			if (file.isDirectory()) {
				proc2(file);
			}
		}

	}

	public static void proc2(File dir) throws IOException {

		File[] files = dir.listFiles();

		ObjF obj = null;
		File fold = null;
		for (int i = 0; i < files.length; i++) {
			File file = files[i];

			if ("entry.json".equals(file.getName())) {

				FileInputStream fis = new FileInputStream(file);
				InputStreamReader isr = new InputStreamReader(fis, "utf-8");
				char[] buff = new char[4096];
				int len = isr.read(buff);
				String s = new String(buff, 0, len);

				obj = deserialize(s, ObjF.class);
			}

			if (file.isDirectory()) {
				fold = file;
			}
		}

		proc3(fold, obj, dir);

	}

	public static void proc3(File dir, ObjF obj, File pos) throws IOException {

		File[] files = dir.listFiles();

		for (int i = 0; i < files.length; i++) {
			File file = files[i];
			if (!"index.json".equals(file.getName())) {

				Files.copy(file.toPath(), (new File(root + obj.avid + "_" + obj.title + "_" + pos.getName() + "_" + file.getName())).toPath());

			}

		}
	}

}
