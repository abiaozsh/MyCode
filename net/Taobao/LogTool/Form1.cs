using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using System.Threading;
using System.Net;
using System.Diagnostics;
using System.Runtime.Serialization.Json;

namespace LogTool
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                slots[0] = new Slot();
                slots[1] = new Slot();
                slots[2] = new Slot();
                slots[3] = new Slot();
                listBox1.Items.Add(slots[0]);
                listBox1.Items.Add(slots[1]);
                listBox1.Items.Add(slots[2]);
                listBox1.Items.Add(slots[3]);
            }
            catch (Exception ex)
            {
                ex.ToString();
            }
        }

        public void ShowPage(List<Taobao.Mods.ItemList.Data.Auction> data)
        {
            if (data.Count <= 0) return;
            string temp = System.IO.Path.GetTempFileName();
            string newfile = temp.Substring(0, temp.Length - 3) + "html";
            FileStream fs = new FileStream(newfile, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);

            sw.WriteLine("<html>");
            sw.WriteLine("<head>");
            sw.WriteLine("<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />");
            sw.WriteLine("</head>");
            sw.WriteLine("<body>");

            sw.WriteLine("<script>");
            sw.WriteLine("function show(n)");
            sw.WriteLine("{");
            sw.WriteLine("  document.getElementById(n).src=document.getElementById(n).getAttribute(\"data\");");
            sw.WriteLine("  document.getElementById(n).height=100;");
            sw.WriteLine("  document.getElementById(n).width=100;");
            sw.WriteLine("}");
            sw.WriteLine("</script>");

            sw.WriteLine("<table border='1'>");
            string bg = "";

            foreach (var item in data)
            {
                sw.WriteLine("<tr" + bg + ">");

                sw.WriteLine("<td>");
                sw.WriteLine("<a href='http:" + item.detail_url + "' target='_blank'>" + item.raw_title + "</a>");
                sw.WriteLine("</td>");

                sw.WriteLine("<td>");
                //sw.WriteLine("<img src='http:" + item.pic_url + "'>");
                sw.WriteLine("<img id='n" + item.nid + "' data='http:" + item.pic_url + "'>");
                sw.WriteLine("</td>");

                sw.WriteLine("<td>");
                //sw.WriteLine(item.view_price);
                sw.WriteLine("<a href='javascript:show(\"n" + item.nid + "\")'>" + item.view_price + "</a>");
                sw.WriteLine("</td>");
                sw.WriteLine("<td>");
                sw.WriteLine(item.reserve_price);
                sw.WriteLine("</td>");
                sw.WriteLine("<td>");
                sw.WriteLine(item.view_fee);
                sw.WriteLine("</td>");

                sw.WriteLine("<td>");
                sw.WriteLine(item.item_loc);
                sw.WriteLine("</td>");

                sw.WriteLine("<td>");
                sw.WriteLine(item.comment_count);
                sw.WriteLine("</td>");

                sw.WriteLine("<td>");
                sw.WriteLine((item.shopcard.isTmall ? "TM " : "") + "<a href='http:" + item.shopLink + "' target='_blank'>" + item.nick + "</a>");
                sw.WriteLine("</td>");

                sw.WriteLine("</tr>");

                if (bg == "")
                {
                    bg = " style='background:#DDD;'";
                }
                else
                {
                    bg = "";
                }

            }
            sw.WriteLine("</table>");
            sw.WriteLine("</body>");
            sw.WriteLine("</html>");

            sw.Flush();
            fs.Close();
            Process p = new Process();
            p.StartInfo = new ProcessStartInfo(newfile);
            p.Start();
        }

        class Slot
        {
            public string Title = "";
            public List<Taobao.Mods.ItemList.Data.Auction> List = new List<Taobao.Mods.ItemList.Data.Auction>();
            public override string ToString()
            {
                return Title;
            }
        }

        Slot[] slots = new Slot[4];

        private void button1_Click_1(object sender, EventArgs e)
        {
            List<Taobao.Mods.ItemList.Data.Auction> l1 = new List<Taobao.Mods.ItemList.Data.Auction>();
            List<Taobao.Mods.ItemList.Data.Auction> l2 = new List<Taobao.Mods.ItemList.Data.Auction>();
            List<Taobao.Mods.ItemList.Data.Auction> l3 = new List<Taobao.Mods.ItemList.Data.Auction>();
            List<Taobao.Mods.ItemList.Data.Auction> l4 = new List<Taobao.Mods.ItemList.Data.Auction>();

            if (slots[0] != null) l1.AddRange(slots[0].List);
            if (slots[1] != null) l2.AddRange(slots[1].List);
            if (slots[2] != null) l3.AddRange(slots[2].List);
            if (slots[3] != null) l4.AddRange(slots[3].List);

            l1 = filt(l1, l2, l3, l4);
            l2 = filt(l2, l1, l3, l4);
            l3 = filt(l3, l2, l1, l4);
            l4 = filt(l4, l2, l3, l1);

            ShowPage(l1);
            ShowPage(l2);
            ShowPage(l3);
            ShowPage(l4);
        }

        private void proc(string n, string t, string h, string lo, List<Taobao.Mods.ItemList.Data.Auction> l)
        {
            if (!string.IsNullOrEmpty(n))
            {
                for (int i = 0; i < int.Parse(n); i++)
                {
                    var tb = Request(t, i, h, lo);
                    if (tb.mods.itemlist.data != null)
                    {
                        l.AddRange(tb.mods.itemlist.data.auctions);
                    }
                }
            }
        }

        private List<Taobao.Mods.ItemList.Data.Auction> filt(List<Taobao.Mods.ItemList.Data.Auction> l, List<Taobao.Mods.ItemList.Data.Auction> l1, List<Taobao.Mods.ItemList.Data.Auction> l2, List<Taobao.Mods.ItemList.Data.Auction> l3)
        {
            List<Taobao.Mods.ItemList.Data.Auction> ret = new List<Taobao.Mods.ItemList.Data.Auction>();
            foreach (var item in l)
            {
                bool exist = false;
                if (l1.Count > 0)
                {
                    foreach (var item1 in l1)
                    {
                        if (item1.nick == item.nick)
                        {
                            exist = true;
                        }
                    }
                }
                else
                {
                    exist = true;
                }

                if (exist)
                {
                    exist = false;
                    if (l2.Count > 0)
                    {
                        foreach (var item2 in l2)
                        {
                            if (item2.nick == item.nick)
                            {
                                exist = true;
                            }
                        }
                    }
                    else
                    {
                        exist = true;
                    }

                }
                if (exist)
                {
                    exist = false;
                    if (l3.Count > 0)
                    {
                        foreach (var item3 in l3)
                        {
                            if (item3.nick == item.nick)
                            {
                                exist = true;
                            }
                        }
                    }
                    else
                    {
                        exist = true;
                    }

                }

                if (exist)
                {
                    ret.Add(item);
                }
            }

            return ret;
        }

        [Serializable]
        public class Taobao
        {
            public Mods mods;
            public class Mods
            {
                public ItemList itemlist;

                public class ItemList
                {
                    public Data data;

                    public class Data
                    {
                        public List<Auction> auctions;

                        public class Auction
                        {
                            public string nid;
                            public string raw_title;
                            public string nick;
                            public string detail_url;
                            public string pic_url;
                            public string view_price;
                            public string reserve_price;
                            public string view_fee;
                            public string item_loc;
                            public string comment_count;
                            public string shopLink;
                            public Shopcard shopcard;

                            public class Shopcard
                            {
                                public bool isTmall;
                            }
                        }
                    }
                }
            }
        }

        public Taobao Request(string search, int no, string h, string l)
        {
            this.Text = search + " " + no;
            string _url;
            string filter = "";
            if (!string.IsNullOrEmpty(h) || !string.IsNullOrEmpty(l))
            {
                filter = "&filter=reserve_price[" + l + "," + h + "]";
            }
            string sort = "&sort=sale-desc";

            string loc = "&loc=%E6%B1%9F%E8%8B%8F%2C%E6%B5%99%E6%B1%9F%2C%E4%B8%8A%E6%B5%B7";

            if (false)//江浙沪
            {
                loc = "";
            }

            if (no == 0)
            {
                _url = "https://s.taobao.com/search?q=" + search + "&imgfile=&js=1&stats_click=search_radio_all%3A1&initiative_id=staobaoz_20151105&ie=utf8" + filter + sort + loc;
            }
            else if (no == 1)
            {
                _url = "https://s.taobao.com/search?q=" + search + "&imgfile=&js=1&stats_click=search_radio_all%3A1&initiative_id=staobaoz_20151105&ie=utf8&bcoffset=0&p4plefttype=3%2C1&p4pleftnum=1%2C3&s=44" + filter + sort + loc;
            }
            else
            {
                _url = "https://s.taobao.com/search?q=" + search + "&imgfile=&js=1&stats_click=search_radio_all%3A1&initiative_id=staobaoz_20151105&ie=utf8&bcoffset=-3&p4plefttype=3%2C1&p4pleftnum=1%2C3&ntoffset=-3&s=" + (no * 44) + filter + sort + loc;
            }
            System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(_url);
            System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
            System.IO.Stream st = myrp.GetResponseStream();
            StreamReader sr = new StreamReader(st);
            while (true)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                if (line.Trim().StartsWith("g_page_config"))
                {
                    sr.Close();
                    st.Close();
                    myrp.Close();
                    Myrq.Abort();
                    return DeSerial(line.Trim().Substring("g_page_config = ".Length).TrimEnd(';'));
                }
            }

            sr.Close();
            st.Close();
            myrp.Close();
            Myrq.Abort();
            return null;
        }
        private Taobao DeSerial(string jsonString)
        {
            DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Taobao));
            MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(jsonString));
            Taobao obj = (Taobao)ser.ReadObject(ms);
            return obj;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var item = (Slot)listBox1.SelectedItem;
            if (item != null)
            {
                proc(pages.Text, search.Text, priceHi.Text, priceLo.Text, item.List);
                item.Title = pages.Text + "," + search.Text + "," + priceHi.Text + "," + priceLo.Text;
                listBox1.Items.Clear();
                listBox1.Items.Add(slots[0]);
                listBox1.Items.Add(slots[1]);
                listBox1.Items.Add(slots[2]);
                listBox1.Items.Add(slots[3]);
            }
        }

    }

}
