import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.List;

import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.type.TypeFactory;

public class Folder {
	final static SimpleDateFormat dateformatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");

	public static class StaticInfo {
		public static TRoot statictf;
		public static String arg1;
	}

	public static class TFile {
		public String name;
		public long size;
		public String datetime;
		public TFolder parent;
		public TRoot root;

		public Date _GetDatetime() {
			try {
				return dateformatter.parse(datetime);
			} catch (Throwable t) {
				return null;
			}
		}

		public void _SetDatetime(Date d) {
			if(d==null){
				datetime = "1999-01-01 11:11:11.111";
				return;
			}
			Calendar c = Calendar.getInstance();
			c.setTime(d);
			c.add(Calendar.YEAR, -1900);
			datetime = dateformatter.format(c.getTime());
		}

		public String _ToString() {
			return name;
		}

		public String _GetFullPath() {
			String path = "";
			TFolder f = this.parent;
			while (f != null) {
				path = f.name + "\\" + path;
				f = f.parent;
			}
			return path + this.name;
		}
	}

	public static class TFolder extends TFile {
		public int filecount;
		public List<TFile> fileList;
		public List<TFolder> folderList;

		public String _ToString() {
			return "<" + name + ">";
		}

		public void setParent(TRoot root) {
			for (TFile file : fileList) {
				file.parent = this;
				file.root = root;
			}
			for (TFolder folder : folderList) {
				folder.parent = this;
				folder.root = root;
				folder.setParent(root);
			}
		}

		public void clearParent() {
			for (TFile file : fileList) {
				file.parent = null;
				file.root = null;
			}
			for (TFolder folder : folderList) {
				folder.parent = null;
				folder.root = null;
				folder.clearParent();
			}
		}

	}

	public static class TRoot extends TFolder {
		public String path;
		
		public HashMap<String, String> md5List;

		public void save(String file) throws Throwable {

			clearParent();

			FileOutputStream fs = new FileOutputStream(file);
			OutputStreamWriter sw = new OutputStreamWriter(fs, "utf-8");
			String jsonString = serialize(this);

			//jsonString = formatJson(jsonString);

			sw.write(jsonString);
			sw.flush();
			fs.close();

		}

	}

	// public static TRoot load(String file) {
	// DataContractJsonSerializer mySerializer = new
	// DataContractJsonSerializer(typeof(TRoot));
	// FileStream myFileStream = new FileStream(file, FileMode.Open,
	// FileAccess.Read, FileShare.Read);
	// object myObject = mySerializer.ReadObject(myFileStream);
	// myFileStream.Close();
	// myFileStream.Dispose();
	// TRoot tRoot = (TRoot) myObject;
	// tRoot.setParent();
	// return tRoot;
	//
	// }

	public static String serialize(Object object) {
		ObjectMapper objectMapper = new ObjectMapper();
		Writer write = new StringWriter();
		try {
			// objectMapper.configure(SerializationConfig.Feature.FAIL_ON_EMPTY_BEANS,
			// false);
			objectMapper.writeValue(write, object);
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return write.toString();
	}

	public static <T> T deserialize(String json, Class<T> clazz) {
		ObjectMapper objectMapper = new ObjectMapper();
		Object object = null;
		try {
			object = objectMapper.readValue(json, TypeFactory.rawClass(clazz));
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return (T) object;
	}

	// public static <T> T deserialize(String json, TypeReference<T> typeRef) {
	// try {
	// return (T) objectMapper.readValue(json, typeRef);
	// } catch (Throwable e) {
	// e.printStackTrace();
	// }
	// return null;
	// }

	public static String formatJson(String jsonStr) {
		if (null == jsonStr || "".equals(jsonStr))
			return "";
		StringBuilder sb = new StringBuilder();
		char last = '\0';
		char current = '\0';
		int indent = 0;
		for (int i = 0; i < jsonStr.length(); i++) {
			last = current;
			current = jsonStr.charAt(i);
			switch (current) {
			case '{':
			case '[':
				sb.append(current);
				sb.append("\r\n");
				indent++;
				addIndentBlank(sb, indent);
				break;
			case '}':
			case ']':
				sb.append("\r\n");
				indent--;
				addIndentBlank(sb, indent);
				sb.append(current);
				break;
			case ',':
				sb.append(current);
				if (last != '\\') {
					sb.append("\r\n");
					addIndentBlank(sb, indent);
				}
				break;
			default:
				sb.append(current);
			}
		}

		return sb.toString();
	}

	private static void addIndentBlank(StringBuilder sb, int indent) {
		for (int i = 0; i < indent; i++) {
			sb.append("\t");
		}
	}

}
