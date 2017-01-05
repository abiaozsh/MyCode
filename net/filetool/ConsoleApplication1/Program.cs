using System;
using System.Collections.Generic;
using System.Text;
using ClassLibrary1;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace ConsoleApplication1
{
	class Program
	{

		static void Main(string[] args)
		{
            Proc(new string[] { @"createfilelist", "cc.xml", "c:\\" });


            //Proc(@"createfilelist DISK1A2A_toshibainner.xml u:\".Split(' '));
            //Proc(@"createfilelist DISK1B3B_toshibaext.xml v:\".Split(' '));
            //Proc(@"createfilelist DISK2B3A_wd2G.xml j:\".Split(' '));


            //Proc(@"createfilelist DISK1A2Atoshibainner.xml u:\".Split(' '));
            //Proc(@"createfilelist DISK1B3Btoshibaext.xml v:\".Split(' '));
            //Proc(@"createfilelist i.xml i:\".Split(' '));
            //Proc(@"createfilelist d.xml d:\".Split(' '));
                //Proc("createfilelist t.xml t:\\".Split(' '));
            /*

                Proc("makemd5 test.xml".Split(' '));

                Proc("makedeletefilelist test.xml test.txt".Split(' '));

                Console.WriteLine("done");
                Console.ReadLine();
            */
		}

		static void diff()
		{
			TRoot tf = TRoot.load("169mm.xml");
			TRoot tf2 = TRoot.load("169mm2.xml");
			FileStream fs = new FileStream("del.txt", FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs, Encoding.UTF8);

			List<TFile> filelist = new List<TFile>();
			tf.MakeList(tf.root.TrimEnd('\\'), null, filelist);

			Dictionary<string, TFile> dict = new Dictionary<string, TFile>();
			List<TFile> filelist2 = new List<TFile>();
			tf2.MakeList(tf2.root.TrimEnd('\\'), null, filelist2);
			foreach (var file2 in filelist2)
			{
				if (!dict.ContainsKey(file2.size + file2.md5))
				{
					dict.Add(file2.size + file2.md5, file2);
				}
			}

			foreach (var file in filelist)
			{
				bool exist = false;
				if (dict.ContainsKey(file.size + file.md5))
				{
					exist = true;
				}
				if (exist)
				{
					sw.WriteLine("del \"" + file.ToString() + "\"");
				}
				sw.Flush();
				fs.Flush();
			}

			fs.Close();

		}











		static void Proc(string[] args)
		{

			if (args[0] == "createfilelist")
			{
				TRoot tf = GetFolder(args[2]);

				tf.save(args[1]);
			}
			else if (args[0] == "updatefilelist")
			{
				TRoot tf = TRoot.load(args[1]);

				if (args.Length >= 3 && !string.IsNullOrEmpty(args[2]))
				{
					tf.root = args[2];
				}

				UpdateFolder(tf);

				tf.save(args[1]);
			}
			else if (args[0] == "clearfilelist")
			{
				TRoot tf = TRoot.load(args[1]);

				if (args.Length >= 3 && !string.IsNullOrEmpty(args[2]))
				{
					tf.root = args[2];
				}

				tf.ClearFolder(tf.root);

				tf.save(args[1]);
			}
			else if (args[0] == "makemd5")
			{
				TRoot tf = TRoot.load(args[1]);
				StaticInfo.statictf = tf;
				StaticInfo.arg1 = args[1];
				Console.CancelKeyPress += new ConsoleCancelEventHandler(Console_CancelKeyPress);

				if (args.Length >= 3 && !string.IsNullOrEmpty(args[2]))
				{
					tf.root = args[2];
				}

				try
				{
					tf.MD5Folder(tf.root);
				}
				catch (Exception ex)
				{

				}
				finally
				{
					tf.save(args[1]);
				}
			}
			else if (args[0] == "verifymd5")
			{

			}
			else if (args[0] == "extractfilelist")
			{
			}
			else if (args[0] == "compare")
			{
				TRoot tf1 = TRoot.load(args[1]);
				TRoot tf2 = TRoot.load(args[2]);

				tf1.Compare(tf2, tf1.root);
			}
			else if (args[0] == "convCsv")
			{
				TRoot tf = TRoot.load(args[1]);

				List<TFile> filelist = new List<TFile>();
				{
					FileStream fs = new FileStream(args[2], FileMode.CreateNew, FileAccess.Write);

					StreamWriter sw = new StreamWriter(fs, Encoding.UTF8);

					tf.MakeList(tf.root, sw, filelist);

					sw.Flush();

					sw.Close();

					fs.Close();
				}

				{
					FileStream fs = new FileStream(args[3], FileMode.CreateNew, FileAccess.Write);

					StreamWriter sw = new StreamWriter(fs, Encoding.UTF8);

					filelist.Sort(delegate(TFile f1, TFile f2)
					{
						return -f1.size.CompareTo(f2.size);
					});

					TFile lastFile = filelist[0];
					for (int i = 1; i < filelist.Count; i++)
					{
						if (lastFile.size == filelist[i].size && lastFile.md5 == filelist[i].md5)
						{
							sw.WriteLine(lastFile.name);
							sw.WriteLine(filelist[i].name);
							sw.WriteLine();
						}
						lastFile = filelist[i];
					}

					sw.Flush();

					sw.Close();

					fs.Close();
				}


			}
			//makedeletefilelist aa.xml aa.txt
			else if (args[0] == "makedeletefilelist")
			{
				TRoot tf = TRoot.load(args[1]);
				Dictionary<string, TFile> dict = new Dictionary<string, TFile>();
				List<TFile> list = new List<TFile>();
				tf.MakeDelList(dict, list);

				FileStream fs = new FileStream(args[2], FileMode.Create, FileAccess.Write);

				StreamWriter sw = new StreamWriter(fs, Encoding.Default);

				foreach (var item in list)
				{
					sw.WriteLine("del \"" + item.getFullPath() + "\"");
				}

				sw.Flush();

				sw.Close();

				fs.Close();

			}

		}

		static void Console_CancelKeyPress(object sender, ConsoleCancelEventArgs e)
		{
			StaticInfo.statictf.save(StaticInfo.arg1);
		}

		public static TRoot GetFolder(string dir)
		{
			DirectoryInfo di = new DirectoryInfo(dir);

			TRoot tRoot = new TRoot();

			TFolder tFolder = GetFolder(di);

			tRoot.root = dir;

			tRoot.fileList = tFolder.fileList;
			tRoot.folderList = tFolder.folderList;
			tRoot.name = tFolder.name;
			tRoot.filecount = tFolder.filecount;
			tRoot.datetime = tFolder.datetime;
			tRoot.size = tFolder.size;

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
				tf.datetime = di.LastWriteTime;
				tf.name = di.Name;
				long size = 0;
				int filecount = 0;
				foreach (DirectoryInfo di2 in di.GetDirectories())
				{
					TFolder tempf = GetFolder(di2);
					tf.folderList.Add(tempf);
					size += tempf.size;
					filecount += tempf.filecount;
				}

				foreach (FileInfo fi2 in di.GetFiles())
				{
					TFile tfile = new TFile();
					tfile.name = fi2.Name;
					tfile.size = fi2.Length;
					tfile.datetime = fi2.LastWriteTime;
					tf.fileList.Add(tfile);
					size += fi2.Length;
					filecount++;
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

		public static void UpdateFolder(TRoot tRoot)
		{
			DirectoryInfo di = new DirectoryInfo(tRoot.root);

			UpdateFolder(di, tRoot);
		}

		public static void UpdateFolder(DirectoryInfo di, TFolder tf)
		{
			try
			{
				long size = 0;
				int filecount = 0;
				foreach (DirectoryInfo di2 in di.GetDirectories())
				{
					TFolder tfsub = tf.folderList.Find(temp => (temp.name == di2.Name));
					if (tfsub != null)
					{
						UpdateFolder(di2, tfsub);

						size += tfsub.size;
						filecount += tfsub.filecount;
					}
					else
					{
						tfsub = new TFolder();
						tfsub.fileList = new List<TFile>();
						tfsub.folderList = new List<TFolder>();
						tfsub.name = di2.Name;
						tfsub.datetime = di2.LastWriteTime;
						Console.WriteLine(tfsub);
						UpdateFolder(di2, tfsub);

						tf.folderList.Add(tfsub);

						size += tfsub.size;
						filecount += tfsub.filecount;
					}
				}

				foreach (FileInfo fi2 in di.GetFiles())
				{
					TFile tfile = tf.fileList.Find(temp => (temp.name == fi2.Name));
					if (tfile != null)
					{
						if (tfile.datetime == fi2.LastWriteTime)
						{
							size += fi2.Length;
							filecount++;
						}
						else
						{
							tfile.name = fi2.Name;
							Console.WriteLine(tfile);
							tfile.size = fi2.Length;
							tfile.datetime = fi2.LastWriteTime;
							size += fi2.Length;
							filecount++;
						}
					}
					else
					{
						tfile = new TFile();
						tfile.name = fi2.Name;
						tfile.size = fi2.Length;
						Console.WriteLine(tfile);
						tfile.datetime = fi2.LastWriteTime;
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
		}
	}
}
