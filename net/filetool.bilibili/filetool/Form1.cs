using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using FileTool;
using System.Threading;

namespace filetool
{
    public class Form1 : Form
    {

        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowDrop = true;
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(21, 41);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(401, 131);
            this.dataGridView1.TabIndex = 2;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(21, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 3;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(434, 184);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.dataGridView1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.DataGridView dataGridView1;

        private Button button1;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            dataGridView1.Columns.Add("avid", "avid");
            dataGridView1.Columns.Add("title", "title");
            dataGridView1.Columns.Add("upperid", "upperid");
            dataGridView1.Columns.Add("upper", "upper");
            dataGridView1.Columns.Add("path", "path");
            dataGridView1.Columns.Add("sTotalSize", "sTotalSize");
            dataGridView1.Columns.Add("realSize", "realSize");
            dataGridView1.Columns.Add("sRealSize", "sRealSize");
            //public long avid;
            //public string title;
            //public long upperid;
            //public string upper;
            //public string path;
            //public long totalSize;
            //public long realSize;
            //public string sTotalSize;
            //public string sRealSize;

        }

        void Form1_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.All;
        }


        private static string tostring(long size)
        {
            if (size < 1024)
            {
                return size.ToString() + "B";
            }
            else if (size < 1024 * 1024)
            {
                return Math.Round(((double)size) / 1024, 3) + "KB";
            }
            else if (size < 1024 * 1024 * 1024)
            {
                return Math.Round(((double)size) / 1024 / 1024, 3) + "MB";
            }
            else
            {
                return Math.Round(((double)size) / 1024 / 1024 / 1024, 3) + "GB";
            }
        }
        /// <summary>
        /// 使DataGridView的列自适应宽度
        /// </summary>
        /// <param name="dgViewFiles"></param>
        public static void AutoSizeColumn(DataGridView dgViewFiles)
        {
            //使列自使用宽度
            //对于DataGridView的每一个列都调整
            for (int i = 0; i < dgViewFiles.Columns.Count; i++)
            {
                //将每一列都调整为自动适应模式
                dgViewFiles.AutoResizeColumn(i, DataGridViewAutoSizeColumnMode.AllCells);
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        public class Entry
        {
            public long avid;
            public long total_bytes;
            public string title;
        }
        public class Info
        {
            public string message;
            public Data data;
            public class Data
            {
                public long aid;
                public Owner owner;
                public class Owner
                {
                    public long mid;
                    public string name;
                }
            }
        }
        public class Video
        {
            public long avid;
            public string title;
            public long upperid;
            public string upper;
            public string path;
            public long totalSize;
            public long realSize;
            public string sTotalSize;
            public string sRealSize;
        }

        public static Entry DeSerialize(string jsonString)
        {
            return Newtonsoft.Json.JsonConvert.DeserializeObject<Entry>(jsonString);
        }
        public static Info DeSerializeInfo(string jsonString)
        {
            return Newtonsoft.Json.JsonConvert.DeserializeObject<Info>(jsonString);
        }

        public static String httpreq(String _url)
        {
            string str = "";
            try
            {
                System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(_url);
                System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
                System.IO.Stream st = myrp.GetResponseStream();
                StreamReader sr = new StreamReader(st);

                str = sr.ReadToEnd();

                sr.Close();
                st.Close();
                myrp.Close();
                Myrq.Abort();
            }
            catch (Exception ex)
            {
                ex.ToString();
            }
            return str;
        }

        public static void scan(string dir, List<Video> list)
        {
            DirectoryInfo di = new DirectoryInfo(dir);
            scan(di, list);
        }
        public static void scan(DirectoryInfo di, List<Video> list)
        {
            try
            {
                int filecount = 0;
                String sinfo = null;
                Entry entry = null;
                foreach (FileInfo fi2 in di.GetFiles())
                {
                    if ((fi2.Attributes & FileAttributes.ReparsePoint) != FileAttributes.ReparsePoint)
                    {
                        if (fi2.Name == "entry.json")
                        {
                            FileStream fs = new FileStream(fi2.FullName, FileMode.Open, FileAccess.Read);
                            StreamReader sr = new StreamReader(fs);
                            String s = sr.ReadToEnd();
                            sr.Close();
                            fs.Close();
                            entry = DeSerialize(s);
                        }
                        if (fi2.Name == "info.json")
                        {
                            FileStream fs = new FileStream(fi2.FullName, FileMode.Open, FileAccess.Read);
                            StreamReader sr = new StreamReader(fs);
                            sinfo = sr.ReadToEnd();
                            sr.Close();
                            fs.Close();
                        }
                    }
                }

                if (entry != null)
                {
                    if(false){
                        Info info = null;
                        info = DeSerializeInfo(sinfo);
                        if (info == null)
                        {
                            sinfo = null;
                        }
                        else if (info.data == null)
                        {
                            if (info.message != "啥都木有" && info.message != "稿件不可见")
                            {
                                sinfo = null;
                            }
                        }
                    }
                    {
                        Console.WriteLine(di.FullName);
                        Video video = new Video();
                        video.avid = entry.avid;
                        video.title = entry.title;
                        video.totalSize = entry.total_bytes;
                        video.realSize = getSize(di);
                        video.path = di.FullName;
                        if (sinfo == null)
                        {
                            sinfo = httpreq("http://api.bilibili.com/x/web-interface/view?aid=" + entry.avid);
                            FileStream fs = new FileStream(di.FullName + "\\info.json", FileMode.Create, FileAccess.Write);
                            StreamWriter sw = new StreamWriter(fs);
                            sw.Write(sinfo);
                            sw.Close();
                            fs.Close();
                        }
                        Info info = null;
                        info = DeSerializeInfo(sinfo);
                        if (info != null && info.data != null)
                        {
                            video.upper = info.data.owner.name;
                            video.upperid = info.data.owner.mid;
                        }
                        video.sRealSize = tostring(video.realSize);
                        video.sTotalSize = tostring(video.totalSize);
                        list.Add(video);
                    }
                }
                else
                {
                    foreach (DirectoryInfo di2 in di.GetDirectories())
                    {
                        if ((di2.Attributes & FileAttributes.ReparsePoint) != FileAttributes.ReparsePoint)
                        {
                            scan(di2, list);
                        }
                    }
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
        public static long getSize(DirectoryInfo di)
        {
            try
            {
                long size = 0;
                foreach (FileInfo fi2 in di.GetFiles())
                {
                    if ((fi2.Attributes & FileAttributes.ReparsePoint) != FileAttributes.ReparsePoint)
                    {
                        size += fi2.Length;
                    }
                }

                foreach (DirectoryInfo di2 in di.GetDirectories())
                {
                    if ((di2.Attributes & FileAttributes.ReparsePoint) != FileAttributes.ReparsePoint)
                    {
                        size += getSize(di2);
                    }
                }

                return size;

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
            return 0;
        }

        private void sort(List<TFolder> folderList)
        {
            folderList.Sort(delegate(TFolder tf1, TFolder tf2)
            {
                return 0;
            });
        }

        private void button1_Click(object sender, EventArgs e)
        {
            List<Video> list = new List<Video>();
            scan("D:\\_bilibili\\", list);
            scan("D:\\bilibili.temp\\", list);
            scan("I:\\bilibili.temp\\", list);

            String s = Newtonsoft.Json.JsonConvert.SerializeObject(list);
            {
                FileStream fs = new FileStream("d:\\bilibiliList.json", FileMode.Create, FileAccess.Write);
                StreamWriter sw = new StreamWriter(fs);
                sw.Write(s);
                sw.Close();
                fs.Close();
            }
            foreach (Video video in list)
            {
                //dataGridView1.Columns.Add("avid", "avid");
                //dataGridView1.Columns.Add("title", "title");
                //dataGridView1.Columns.Add("upperid", "upperid");
                //dataGridView1.Columns.Add("upper", "upper");
                //dataGridView1.Columns.Add("path", "path");
                //dataGridView1.Columns.Add("sTotalSize", "sTotalSize");
                //dataGridView1.Columns.Add("realSize", "realSize");
                //dataGridView1.Columns.Add("sRealSize", "sRealSize");

                DataGridViewRow row = new DataGridViewRow();
                row.CreateCells(dataGridView1, video.avid, video.title, video.upperid, video.upper, video.path, video.sTotalSize, video.realSize, video.sRealSize);
                dataGridView1.Rows.Add(row);
            }
            //D:\bilibili\\fdsa
            //D:\bilibili\\新建文件夹
            //https://github.com/SocialSisterYi/bilibili-API-collect/blob/master/video/info.md
            //http://api.bilibili.com/x/web-interface/view?aid=202812135

        }
    }


}