using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml.Serialization;
using System.Security.Cryptography;

namespace ClassLibrary1
{
	public class StaticInfo
	{
		public static TRoot statictf;
		public static string arg1;

	}

	[Serializable]
	public class TFile
	{
		public string name;
		public long size;
		public DateTime datetime;
		public string md5;
		public TFolder parent;

		public override string ToString()
		{
			return name;
		}

		public string getFullPath(){
			string path = "";
			TFolder f = this.parent;
			while (f != null)
			{
				path = f.name + "\\" + path;
				f = f.parent;
			}
			return path + this.name;
		}
	}

	[Serializable]
	public class TFolder : TFile
	{
		public int filecount;
		public List<TFile> fileList;
		public List<TFolder> folderList;
		public override string ToString()
		{
			return "<" + name + ">";
		}

		public void Sort()
		{
			fileList.Sort(delegate(TFile f1, TFile f2)
			{
				return f1.name.CompareTo(f2.name);
			});

			folderList.Sort(delegate(TFolder f1, TFolder f2)
			{
				return f1.name.CompareTo(f2.name);
			});

			foreach (TFolder folder in folderList)
			{
				folder.Sort();
			}
		}

		public long md5size()
		{
			long size = 0;
			foreach (TFile file in fileList)
			{
				size += file.md5 == null ? 0 : file.size;
			}
			foreach (TFolder folder in folderList)
			{
				size += folder.md5size();
			}
			return size;
		}

		public void setParent()
		{
			foreach (TFile file in fileList)
			{
				file.parent = this;
			}
			foreach (TFolder folder in folderList)
			{
				folder.parent = this;
				folder.setParent();
			}
		}

		public void clearParent()
		{
			foreach (TFile file in fileList)
			{
				file.parent = null;
			}
			foreach (TFolder folder in folderList)
			{
				folder.parent = null;
				folder.clearParent();
			}
		}

		public void MD5Folder(string dir)
		{
			foreach (TFile file in fileList)
			{
				if (file.md5 == null)
				{
					Console.Write(dir + "\\" + file.name + " ");
					long md5size = StaticInfo.statictf.md5size();
					Console.Title = (md5size / 1024 / 1024).ToString() + "/" + (StaticInfo.statictf.size / 1024 / 1024) + " " + ((double)md5size / (double)StaticInfo.statictf.size).ToString();
					string md5 = MD5Encoding(dir + "\\" + file.name);
					file.md5 = md5;
					Console.WriteLine(file.md5);
				}
			}
			foreach (TFolder folder in folderList)
			{
				folder.MD5Folder(dir + "\\" + folder.name);
			}
		}

		public static string MD5Encoding(string filename)
		{
			try
			{
				MD5 md5 = MD5.Create();

				FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);

				byte[] hs = md5.ComputeHash(fs);

				fs.Close();

				StringBuilder sb = new StringBuilder();

				foreach (byte b in hs)
				{
					sb.Append(b.ToString("x2"));
				}
				return sb.ToString();
			}
			catch (Exception e)
			{
				Console.WriteLine(e);
				//Console.WriteLine("continue?y/n");
				//string s = Console.ReadLine();
				//if (s == "n")
				//{
				//    throw e;
				//}
				return null;
			}

		}

		public void Compare(TFolder folder2, string dir)
		{
			foreach (TFile file in fileList)
			{
				TFile file2 = folder2.fileList.Find(temp => (temp.name == file.name));
				if (file2 != null)
				{
					if (file.md5 != file2.md5)
					{
						Console.WriteLine("\"" + (dir + "\\" + file.name).Replace("\"", "\"\"") + "\"," +
							"\"" + file.size + "\"," +
							"\"" + file.datetime.ToString("yyyy-MM-dd HH:mm:ss") + "\"," +
							"\"" + file.md5 + "\"");
					}
				}
			}
			foreach (TFolder folder in folderList)
			{
				TFolder f2 = folder2.folderList.Find(temp => (temp.name == folder.name));
				if (f2 != null)
				{
					folder.Compare(f2, dir + "\\" + folder.name);
				}
			}
		}

		public void ClearFolder(string dir)
		{
			List<TFile> fileList_temp = new List<TFile>();

			List<TFolder> folderList_temp = new List<TFolder>();

			foreach (TFile file in fileList)
			{
				if (!File.Exists(dir + "\\" + file.name))
				{
					Console.WriteLine(dir + "\\" + file.name);
				}
				else
				{
					fileList_temp.Add(file);
				}
			}
			foreach (TFolder folder in folderList)
			{
				if (!Directory.Exists(dir + "\\" + folder.name))
				{
					Console.WriteLine(dir + "\\" + folder.name);
				}
				else
				{
					folder.ClearFolder(dir + "\\" + folder.name);
					folderList_temp.Add(folder);
				}
			}

			fileList = fileList_temp;
			folderList = folderList_temp;
		}

		public void MakeList(string dir, StreamWriter sw, List<TFile> filelist)
		{
			foreach (TFile file in fileList)
			{
				if (sw != null)
				{
					sw.WriteLine("\"" + (dir + "\\" + file.name).Replace("\"", "\"\"") + "\"," +
						"\"" + file.size + "\"," +
						"\"" + file.datetime.ToString("yyyy-MM-dd HH:mm:ss") + "\"," +
						"\"" + file.md5 + "\"");
				}
				file.name = dir + "\\" + file.name;
				filelist.Add(file);
				//Console.WriteLine(dir + "\\" + file.name);
			}
			foreach (TFolder folder in folderList)
			{
				if (sw != null)
				{
					sw.WriteLine("\"" + (dir + "\\" + folder.name).Replace("\"", "\"\"") + "\"," +
						"\" \"," +
						"\" \"," +
						"\" \"");
				}
				//Console.WriteLine(dir + "\\" + folder.name + "\\");
				folder.MakeList(dir + "\\" + folder.name, sw, filelist);
			}
		}

		public void MakeDelList(Dictionary<string, TFile> dict, List<TFile> list)
		{
			foreach (TFile file in fileList)
			{
				if (dict.ContainsKey(file.md5))
				{
					list.Add(file);
				}
				else
				{
					dict.Add(file.md5, file);
				}
			}
			foreach (TFolder folder in folderList)
			{
				folder.MakeDelList(dict, list);
			}
		}

	}

	[Serializable]
	public class TRoot : TFolder
	{
		public string root;


		/*
		public static void save(TRoot tf, string file)
		{
			IFormatter formatter = new BinaryFormatter();
			Stream stream = new FileStream(file, FileMode.Create, FileAccess.Write, FileShare.None);
			formatter.Serialize(stream, tf);
			stream.Close();
		}

		public static TRoot load(string file)
		{
			IFormatter formatter = new BinaryFormatter();
			Stream stream = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read);
			TRoot tf = (TRoot)formatter.Deserialize(stream);
			stream.Close();
			return tf;
		}
		*/

		public void save(string file)
		{
			Sort();
			clearParent();
			XmlSerializer mySerializer = new XmlSerializer(typeof(TRoot));
			FileStream myFileStream = new FileStream(file, FileMode.Create, FileAccess.Write, FileShare.None);
			StreamWriter myWriter = new StreamWriter(myFileStream, Encoding.UTF8);
			mySerializer.Serialize(myWriter, this);
			myWriter.Close();
			myWriter.Dispose();

		}

		public static TRoot load(string file)
		{
			XmlSerializer mySerializer = new XmlSerializer(typeof(TRoot));
			FileStream myFileStream = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read);
			StreamReader sr = new StreamReader(myFileStream, Encoding.UTF8);
			object myObject = mySerializer.Deserialize(sr);
			myFileStream.Close();
			myFileStream.Dispose();
			TRoot tRoot = (TRoot)myObject;
			tRoot.setParent();
			return tRoot;

		}

	}

}