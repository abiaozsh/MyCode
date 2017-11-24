using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization.Json;

namespace LogTool
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }
        private Baidu DeSerial(string jsonString)
        {
            DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Baidu));
            MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(jsonString));
            Baidu obj = (Baidu)ser.ReadObject(ms);
            return obj;
        }

        [Serializable]
        public class Baidu
        {
            public List<Img> imgs;
            public class Img
            {
                public string thumbURL;
                public string middleURL;
                public string objURL;
                public string fromPageTitle;

            }
        }

        public Baidu Request(string search, int pn)
        {

            string _url = "https://image.baidu.com/search/avatarjson?tn=resultjsonavatarnew&ie=utf-8&word=" + Uri.EscapeDataString(search) + "&pn=" + pn * 30 + "&rn=30";


            System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(_url);
            System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
            System.IO.Stream st = myrp.GetResponseStream();
            StreamReader sr = new StreamReader(st);
            StringBuilder sb = new StringBuilder();
            while (true)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                sb.Append(line);
            }

            sr.Close();
            st.Close();
            myrp.Close();
            Myrq.Abort();
            return DeSerial(sb.ToString()); ;//null;
        }

        public void getImage(string url, string filename)
        {

            System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(url);
            System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
            System.IO.Stream st = myrp.GetResponseStream();

            FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);

            StringBuilder sb = new StringBuilder();
            while (true)
            {
                int data = st.ReadByte();
                if (data < 0) break;
                fs.WriteByte((byte)data);
            }
            fs.Flush();
            fs.Close();

            st.Close();
            myrp.Close();
            Myrq.Abort();

        }

        private void Form2_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string[] nameList = {
"小猫",//1
"小狗",//2
"小鸟",//3
"小轿车",//4
"公共汽车",//5
"货车",//6
"飞机",//7
"船",//8
"自行车",//9
"树木",//10
"路人",//11
"手机",//12
"马"};//13
            int id = 0;
            foreach (var name in nameList)
            {
                id++;
                for (int i = 0; i < 2; i++)//400
                {
                    Baidu ret = Request(name, i);

                    int idx = 0;
                    foreach (var item in ret.imgs)
                    {
                        string dir = id + "\\" + i + "\\";
                        Directory.CreateDirectory(dir);
                        getImage(item.thumbURL, dir + idx + ".jpg");
                        textBox1.Text = dir + idx + ".jpg";
                        Application.DoEvents();
                        idx++;
                    }
                }
            }

        }
    }
}
