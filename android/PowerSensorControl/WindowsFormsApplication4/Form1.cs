﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication4
{
	public partial class Form1 : Form
	{
		public static string DATA;

		public Form1()
		{
			InitializeComponent();
		}

		public string PhoneIP = "192.168.0.5";

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		void Form1_FormClosed(object sender, System.Windows.Forms.FormClosedEventArgs e)
		{
			Application.Exit();
		}


		public string Send(byte cmd, byte data, bool twopart, out string part2)
		{
			string ret = "";
			part2 = "";
			try
			{
				int port = 5000;
				string host = PhoneIP;//服务器端ip地址

				IPAddress ip = IPAddress.Parse(host);
				IPEndPoint ipe = new IPEndPoint(ip, port);

				Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				clientSocket.Connect(ipe);

				byte[] snd = new byte[2];
				snd[0] = cmd;
				snd[1] = data;
				clientSocket.Send(snd);

				ret = receive(clientSocket);

				if (twopart)
				{
					part2 = receive(clientSocket);
				}
				clientSocket.Close();
			}
			catch (Exception ex)
			{
				string s = ex.ToString();
			}
			return ret;
		}

		private string receive(Socket clientSocket)
		{
			byte[] data = new byte[1];

			byte[] receive = new byte[3];
            int len3 = 0;
            while (len3 < 3)
            {
                len3 += clientSocket.Receive(receive, len3, 3 - len3, SocketFlags.None);
            }
			int len = (int)receive[0] + ((int)receive[1] << 8) + ((int)receive[2] << 16);
            
			byte[] receiveBuff = new byte[len];
            int len2 = 0;
            while (len2 < len)
            {
                len2 += clientSocket.Receive(receiveBuff, len2, len - len2, SocketFlags.None);
            }
			string s = Encoding.Default.GetString(receiveBuff);

			return s;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			string part2;
			string dir = Send(12, 0, false, out part2);
			textBox1.Text = dir;
		}

		private void button2_Click(object sender, EventArgs e)
		{
			string content;
			string file = Send(13, (byte)int.Parse(textBox2.Text), true, out content);

			textBox2.Text = file;
			textBox3.Text = content;
			DATA = content;
			//string dir = Send(12, 0, false);

		}

        private void button3_Click(object sender, EventArgs e)
        {
            string part2;
            string file = Send(11, (byte)int.Parse(textBox2.Text), false, out part2);
            textBox2.Text = file;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string part2;
            string current = Send(10, 0, false, out part2);
            textBox3.Text = current;
        }

		private void button5_Click(object sender, EventArgs e)
		{
			Form2 f = new Form2();
			f.Show();
		}


	}
}
