package sample;

import java.io.IOException;
import java.io.StringWriter;
import java.io.Writer;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.JsonParseException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.type.TypeFactory;

import com.sun.xml.internal.bind.api.TypeReference;

/** 
 * Json序列化工具 
 *  
 * @author http://blog.csdn.net/xxd851116 
 * @date 2014年3月26日 下午1:21:59 
 */
public class JsonUtils {

	private static ObjectMapper objectMapper = new ObjectMapper();

	public static String serialize(Object object) throws JsonGenerationException, JsonMappingException, IOException {
		Writer write = new StringWriter();

		//objectMapper.configure(SerializationConfig.Feature.FAIL_ON_EMPTY_BEANS, false);
		objectMapper.writeValue(write, object);

		return write.toString();
	}

	public static <T> T deserialize(String json, Class<T> clazz) throws JsonParseException, JsonMappingException, IOException {
		Object object = null;

		object = objectMapper.readValue(json, TypeFactory.rawClass(clazz));

		return (T) object;
	}

}