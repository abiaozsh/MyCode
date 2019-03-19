using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FileTool
{
	class NewtonJson
	{
		public static string Serialize(TRoot t)
		{
			return Newtonsoft.Json.JsonConvert.SerializeObject(t);
		}

		public static TRoot DeSerialize(string jsonString)
		{
			return Newtonsoft.Json.JsonConvert.DeserializeObject<TRoot>(jsonString);
		}

	}
}
