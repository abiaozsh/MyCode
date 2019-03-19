using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;

namespace FileTool
{
	public class SystemJson
	{
		public static string Serialize(TRoot root)
		{
			DataContractJsonSerializer mySerializer = new DataContractJsonSerializer(typeof(TRoot));
			MemoryStream ms = new MemoryStream();
			mySerializer.WriteObject(ms, root);
			string jsonString = Encoding.UTF8.GetString(ms.ToArray());

			jsonString = formatJson(jsonString);
			return jsonString;
		}

		public static TRoot DeSerialize(string s) {
			DataContractJsonSerializer mySerializer = new DataContractJsonSerializer(typeof(TRoot));

			s = s.Replace("\r\n", "");
			s = s.Replace("\t", "");
			s = s.Replace("\0", "");

			MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(s));

			object myObject = mySerializer.ReadObject(ms);
			TRoot tRoot = (TRoot)myObject;
			return tRoot;
		}

		private static void addIndentBlank(StringBuilder sb, int indent)
		{
			for (int i = 0; i < indent; i++)
			{
				sb.Append("\t");
			}
		}

		public static String formatJson(String jsonStr)
		{
			if (null == jsonStr || "".Equals(jsonStr))
				return "";
			StringBuilder sb = new StringBuilder();
			char last = '\0';
			char current = '\0';
			int indent = 0;
			for (int i = 0; i < jsonStr.Length; i++)
			{
				last = current;
				current = jsonStr[i];
				switch (current)
				{
					case '{':
					case '[':
						sb.Append(current);
						sb.Append("\r\n");
						indent++;
						addIndentBlank(sb, indent);
						break;
					case '}':
					case ']':
						sb.Append("\r\n");
						indent--;
						addIndentBlank(sb, indent);
						sb.Append(current);
						break;
					case ',':
						sb.Append(current);
						if (last != '\\')
						{
							sb.Append("\r\n");
							addIndentBlank(sb, indent);
						}
						break;
					default:
						sb.Append(current);
						break;
				}
			}

			return sb.ToString();
		}

	}
}
