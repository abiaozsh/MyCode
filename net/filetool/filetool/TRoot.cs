using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Security.Cryptography;
using FileTool;

namespace FileTool
{

	[Serializable]
	public class TFile
	{
		public string name;
		public long size;
		public string datetime;
		public TFolder parent;
		public TRoot root;

		public DateTime getDatetime()
		{
			DateTime ret;
			DateTime.TryParse(datetime, out ret);
			return ret;
		}
		public void setDatetime(DateTime d)
		{
			datetime = d.ToString("yyyy-MM-dd HH:mm:ss");
		}
		public bool CompareDatetime(DateTime d)
		{
			return datetime == d.ToString("yyyy-MM-dd HH:mm:ss");
		}

		public override string ToString()
		{
			return name;
		}

		public string getFullPath()
		{
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

		public void setParent(TRoot root)
		{
			foreach (TFile file in fileList)
			{
				file.parent = this;
				file.root = root;
			}
			foreach (TFolder folder in folderList)
			{
				folder.parent = this;
				folder.root = root;
				folder.setParent(root);
			}
		}

		public void clearParent()
		{
			foreach (TFile file in fileList)
			{
				file.parent = null;
				file.root = null;
			}
			foreach (TFolder folder in folderList)
			{
				folder.parent = null;
				folder.root = null;
				folder.clearParent();
			}
		}
	}

	[Serializable]
	public class TRoot : TFolder
	{
		public string path;

		public Dictionary<String, String> md5List;

		public void save(string file)
		{
			Sort();
			clearParent();
			FileStream myFileStream = new FileStream(file, FileMode.Create, FileAccess.Write);

			string jsonString = NewtonJson.Serialize(this);
			//string jsonString = SystemJson.Serialize(this);

			byte[] buff = Encoding.UTF8.GetBytes(jsonString);
			myFileStream.Write(buff, 0, buff.Length);

			myFileStream.Close();
			myFileStream.Dispose();
			setParent(this);
		}
		public static TRoot load(string file)
		{
			FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read);
			StreamReader sr = new StreamReader(fs);
			string s = sr.ReadToEnd();
			fs.Close();
			fs.Dispose();

			TRoot tRoot = NewtonJson.DeSerialize(s);
			//TRoot tRoot = SystemJson.DeSerialize(s);

			tRoot.setParent(tRoot);
			return tRoot;

		}

		public void addMd5List(TFolder folder)
		{
			foreach (var item in folder.fileList)
			{
				var path = item.getFullPath();
				if (!md5List.ContainsKey(path))
				{
					Console.WriteLine(path);
					md5List.Add(path, null);
				}
			}
			foreach (var item in folder.folderList)
			{
				addMd5List(item);
			}

		}
		public void clearMd5()
		{

		}

		public void makeAvatar(TFolder folder, string path)
		{
			{
				string temp = path + folder.getFullPath();
				if (!Directory.Exists(temp))
				{
					Directory.CreateDirectory(temp);
				}
			}
			foreach (var item in folder.fileList)
			{
				var fullpath = item.getFullPath();
				var temp = path + fullpath;
				string md5 = "";
				if (md5List.ContainsKey(fullpath))
				{
					md5 = md5List[fullpath];
				}
				FileStream fs = new FileStream(temp, FileMode.CreateNew, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				sw.WriteLine(md5);
				sw.Flush();
				sw.Close();
				fs.Close();
			}
			foreach (var item in folder.folderList)
			{
				makeAvatar(item, path);
			}
		}

		public void UpdateFolder()
		{
			DirectoryInfo di = new DirectoryInfo(path);

			TFolder tFolder = GetFolder(di);

			fileList = tFolder.fileList;
			folderList = tFolder.folderList;
			name = tFolder.name;
			filecount = tFolder.filecount;
			datetime = tFolder.datetime;
			size = tFolder.size;

			this.setParent(this);
		}



		public static TRoot GetFolder(string dir)
		{
			DirectoryInfo di = new DirectoryInfo(dir);

			TRoot tRoot = new TRoot();

			TFolder tFolder = GetFolder(di);

			if (dir.EndsWith("\\"))
			{
				if (dir.Length == tFolder.name.Length)
				{
					tRoot.path = "";
				}
				else
				{
					tRoot.path = dir.Substring(0, dir.Length - tFolder.name.Length - 1);
				}
			}
			else
			{
				tRoot.path = dir.Substring(0, dir.Length - tFolder.name.Length);
			}

			tRoot.fileList = tFolder.fileList;
			tRoot.folderList = tFolder.folderList;
			tRoot.name = tFolder.name;
			tRoot.filecount = tFolder.filecount;
			tRoot.datetime = tFolder.datetime;
			tRoot.size = tFolder.size;

			tRoot.md5List = new Dictionary<string, string>();

			tRoot.setParent(tRoot);
			return tRoot;
		}

		public static TFolder GetFolder(DirectoryInfo di)
		{
			TFolder tf = new TFolder();
			tf.folderList = new List<TFolder>();
			tf.fileList = new List<TFile>();
			Console.WriteLine(di.FullName);
			try
			{
				tf.setDatetime(di.LastWriteTime);
				tf.name = di.Name;
				long size = 0;
				int filecount = 0;
				foreach (DirectoryInfo di2 in di.GetDirectories())
				{
					if ((di2.Attributes & FileAttributes.ReparsePoint) != FileAttributes.ReparsePoint)
					{
						TFolder tempf = GetFolder(di2);
						tf.folderList.Add(tempf);
						size += tempf.size;
						filecount += tempf.filecount;
					}
				}

				foreach (FileInfo fi2 in di.GetFiles())
				{
					if ((fi2.Attributes & FileAttributes.ReparsePoint) != FileAttributes.ReparsePoint)
					{
						TFile tfile = new TFile();
						tfile.name = fi2.Name;
						tfile.size = fi2.Length;
						tfile.setDatetime(fi2.LastWriteTime);
						tf.fileList.Add(tfile);
						size += fi2.Length;
						filecount++;
					}
				}

				tf.size = size;
				tf.filecount = filecount;
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}
			return tf;
		}

	}

	public class Md5Maker
	{
		public bool stop = false;
		public void makeMd5(TRoot root)
		{
			List<String> list = new List<string>();
			foreach (var key in root.md5List.Keys)
			{
				if (root.md5List[key] == null)
				{
					list.Add(key);
				}
			}

			DateTime d = DateTime.Now;
			int cnt = 0;
			int tempfile = 0;
			foreach (var key in list)
			{
				string md5 = MD5Encoding(root.path + key);
				root.md5List[key] = md5;
				Console.WriteLine(key + " " + md5);
				Console.WriteLine((cnt++) + "/" + list.Count);

				if (DateTime.Now.Subtract(d).TotalSeconds > 300)
				{
					d = DateTime.Now;
					root.save("temp" + (tempfile++) + ".json");
					tempfile &= 3;
					Console.WriteLine("saving");
				}

				if (stop)
				{
					stop = false;
					break;
				}
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
	}

}