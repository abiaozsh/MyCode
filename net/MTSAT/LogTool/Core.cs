using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Net;
using System.Diagnostics;
using System.Web;
using System.Runtime.Serialization.Json;
using System.Data.Common;
using System.Collections;
using System.IO.Compression;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;
using System.Threading;

namespace LogTool
{
    public class Core
    {
        CookieContainer cookieContainer = new CookieContainer();
        string token1;
        string saveDir = @"E:\\MTSAT\\";
        public int errorCount = 0;

        //public List<Job> jobs;
        public static void log(string log)
        {
            FileStream fs = new FileStream("log.txt", FileMode.Append, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(DateTime.Now.ToString()+"\t" + log);
            sw.Flush();
            fs.Flush();
            sw.Close();
            fs.Close();
        }

        public static void error(string log)
        {
            FileStream fs = new FileStream("error.txt", FileMode.Append, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(DateTime.Now.ToString() + "\t" + log);
            sw.Flush();
            fs.Flush();
            sw.Close();
            fs.Close();
        }

        public class Job
        {
            public Job(Core c)
            {
                this.c = c;
            }
            public Core c;
            public DateTime target;
            public long length;
            public long index;
            public int status = 0;
            public int fail = 0;

            public override string ToString()
            {
                int prog = ((int)((index * 1.0 / (length * 1.0 + 1)) * 100.0));
                return target.ToString() + "\t" + length.ToString("###,###") + "\t" + index.ToString("###,###") + "\t" + prog + "%\t" + status + "\terr:" + fail;
            }

            public void start()
            {
                Thread t = new Thread(startDownload);

                t.Start(this);
            }
        }

        public void init()
        {
            string ss = requests("https://seg-web.nict.go.jp/wsdb_osndisk/shareDirDownload/bDw2maKV",
                "seg-web.nict.go.jp",
                "",
                null);

            token1 = ss.Substring(ss.IndexOf("FIXED_TOKEN = ") + 15, 40);

        }

        public static void startDownload(Object o)
        {
            Job job = (Job)o;
            job.c.Download(job);
        }

        public void Download(Job job)
        {
            try
            {
                job.status = 1;

                DateTime date = job.target;

                string y = string.Format("{0:D4}", date.Year);//"2018";
                string M = string.Format("{0:D2}", date.Month);//"01";
                string d = string.Format("{0:D2}", date.Day); //"03";
                string h = string.Format("{0:D2}", date.Hour); //"23";
                int minute = date.Minute;
                minute = minute / 10 * 10;
                string m = string.Format("{0:D2}", minute); //"10";

                string fn = "hima8" + y + M + d + h + m + "00fd.png";
                string year_date_hour = y + "/" + M + "-" + d + "/" + h;

                string token = "fc41451e41f2746b65ba30492802960984755f61";
                token = token1;

                //https://seg-web.nict.go.jp/wsdb_osndisk/shareDirDownload/bDw2maKV
                //http://agora.ex.nii.ac.jp/digital-typhoon/globe/hisd/Hsfd/full/2018/01/02/20180102001000.jpg
                //http://sc-web.nict.go.jp/

                string post = "_method=POST&data[FileSearch][is_compress]=false&data[FileSearch][fixedToken]=" + token + "&data[FileSearch][hashUrl]=bDw2maKV&action=dir_download_dl&filelist[0]=/osn-disk/webuser/wsdb/share_directory/bDw2maKV/png/Pifd/" + year_date_hour + "/" + fn + "&dl_path=/osn-disk/webuser/wsdb/share_directory/bDw2maKV/png/Pifd/" + year_date_hour + "/" + fn + "";

                request("https://seg-web.nict.go.jp/wsdb_osndisk/fileSearch/download",
                    "seg-web.nict.go.jp",
                    "https://seg-web.nict.go.jp/wsdb_osndisk/shareDirDownload/bDw2maKV",
                    post,
                    fn, job);
                job.status = 9;
            }
            catch (Exception ex)
            {
                job.status = 2;
                job.fail++;
                if (job.fail >= 20)
                {
                    job.status = 9;
                    error(job.ToString());
                    errorCount++;
                }
            }

        }

        public string requests(string url, string host, string referer, string postdata)
        {
            CookieCollection cookies;
            System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(url);
            if (postdata != null)
            {
                Myrq.Method = "POST";
            }
            else
            {
                Myrq.Method = "GET";
            }
            Myrq.Headers["Accept-Language"] = "zh-CN,zh;q=0.";
            Myrq.Host = host;
            Myrq.Referer = referer;
            Myrq.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
            Myrq.UserAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:39.0) Gecko/20100101 Firefox/39.0";
            Myrq.ContentType = "application/x-www-form-urlencoded";
            Myrq.CookieContainer = cookieContainer;
            Myrq.Headers["Accept-Encoding"] = "gzip, deflate";//Accept-Encoding	gzip, deflate

            if (postdata != null)
            {
                Encoding encoding = Encoding.UTF8;//根据网站的编码自定义  
                byte[] postData = encoding.GetBytes(postdata);//postDataStr即为发送的数据，格式还是和上次说的一样  
                Myrq.ContentLength = postData.Length;
                Stream requestStream = Myrq.GetRequestStream();
                requestStream.Write(postData, 0, postData.Length);
                requestStream.Flush();
                requestStream.Close();
                Myrq.Timeout = 100000;
            }

            System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
            cookies = myrp.Cookies;
            System.IO.Stream st = myrp.GetResponseStream();

            if (myrp.ContentEncoding.ToLower().Contains("gzip"))
            {
                //gzip格式
                st = new GZipStream(st, CompressionMode.Decompress);
            }

            StreamReader sr = new StreamReader(st);
            string s = sr.ReadToEnd();
            st.Close();
            myrp.Close();
            Myrq.Abort();

            return s;
        }

        public void request(string url, string host, string referer, string postdata, string fn, Job job)//, bool gz
        {
            CookieCollection cookies;
            System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(url);
            if (postdata != null)
            {
                Myrq.Method = "POST";
            }
            else
            {
                Myrq.Method = "GET";
            }
            Myrq.Headers["Accept-Language"] = "zh-CN,zh;q=0.";
            Myrq.Host = host;
            Myrq.Referer = referer;
            Myrq.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
            Myrq.UserAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:39.0) Gecko/20100101 Firefox/39.0";
            Myrq.ContentType = "application/x-www-form-urlencoded";
            Myrq.CookieContainer = cookieContainer;
            Myrq.Headers["Accept-Encoding"] = "gzip, deflate";//Accept-Encoding	gzip, deflate

            if (postdata != null)
            {
                Encoding encoding = Encoding.UTF8;//根据网站的编码自定义  
                byte[] postData = encoding.GetBytes(postdata);//postDataStr即为发送的数据，格式还是和上次说的一样  
                Myrq.ContentLength = postData.Length;
                Stream requestStream = Myrq.GetRequestStream();
                requestStream.Write(postData, 0, postData.Length);
                requestStream.Flush();
                requestStream.Close();
                Myrq.Timeout = 100000;
            }

            System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
            long size = myrp.ContentLength;
            job.length = size;
            cookies = myrp.Cookies;
            System.IO.Stream st = myrp.GetResponseStream();

            if (myrp.ContentEncoding.ToLower().Contains("gzip"))
            {
                //gzip格式
                st = new GZipStream(st, CompressionMode.Decompress);
            }
            FileStream fs;
            if (false)
            {
                string dir1 = fn.Substring(0, 13) + "\\";
                string dir2 = fn.Substring(0, 15) + "\\";

                string dir = saveDir + dir1 + dir2;

                if (!Directory.Exists(dir))
                {
                    Directory.CreateDirectory(dir);
                }

                fs = new FileStream(saveDir + dir1 + dir2 + fn, FileMode.Create, FileAccess.Write);
            }
            else
            {
                fs = new FileStream(saveDir + fn, FileMode.Create, FileAccess.Write);
            }

            try
            {
                long idx = 0;
                //var a = fs.CanSeek;
                //for (long i = 0; i < job.length; i++)
                //{
                //    fs.WriteByte(0);
                //}
                //fs.Flush();
                //fs.Seek(0, SeekOrigin.Begin);
                while (true)
                {
                    int data = st.ReadByte();
                    if (data == -1) break;
                    fs.WriteByte((byte)data);
                    //fs.Flush();
                    idx++;
                    job.index = idx;
                }
                if (idx != job.length)
                {
                    throw new Exception();
                }
            }
            finally
            {
                fs.Close();
            }
        }
    }
}
