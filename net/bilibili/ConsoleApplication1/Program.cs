using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.Runtime.Serialization.Json;

namespace ConsoleApplication1
{
	[Serializable]
	class Obj
	{
		public string title;
	}

	class Program
	{
		/*
	{
		"is_completed": true,
		"total_bytes": 37382867,
		"downloaded_bytes": 37382867,
		"title": "【みうめ?メイリア（GARNiDELiA）?仮面ライアー217】極楽浄土【第4弾!!!】",
		"type_tag": "lua.hdmp4.bili2api.2",
		"cover": "http://i0.hdslb.com/bfs/archive/224b7dfe0e13ebe5cfd4590a87cfdf2f310d0343.jpg",
		"prefered_video_quality": 200,
		"guessed_total_bytes": 0,
		"total_time_milli": 225209,
		"danmaku_size": 0,
		"avid": 4468694,
		"spid": 0,
		"seasion_id": 0,
		"page_data": {
		 */
		static void Main(string[] args)
		{
			string[] files = Directory.GetDirectories(@"e:\bilibili\");
			foreach (string s in files)
			{
				string name = load(s + @"\1\entry.json");

                name = name.Replace("\\", "");
                name = name.Replace("/", "");
                name = name.Replace(":", "");
                name = name.Replace("-", "");
                name = name.Replace("?", "？");

				string tmp = s + "_" + name;

				Directory.Move(s, tmp);
			}
		}

		public static string load(string file)
		{
			FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read);
			StreamReader sr = new StreamReader(fs);
			string s = sr.ReadToEnd();
			fs.Close();
			fs.Dispose();

			MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(s));

			DataContractJsonSerializer mySerializer = new DataContractJsonSerializer(typeof(Obj));
			object myObject = mySerializer.ReadObject(ms);
			Obj tRoot = (Obj)myObject;

			string ss = tRoot.title;

			return ss;
		}
	}
}
