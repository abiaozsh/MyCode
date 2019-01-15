using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form0());
		}


		//const string reg_filterroomname = "<[^<>]*>";
		const string reg_filterroomname = "\\([^\\(\\)]*\\)";
		/// <summary>
		/// 去掉尖括号内容
		/// </summary>
		/// <returns></returns>
		public static string FilterRoomName(string roomname)
		{
			if (string.IsNullOrEmpty(roomname))
				return string.Empty;

			for (int i = 0; i < 100; i++)
			{
				if (roomname.IndexOf("(") >= 0 && roomname.IndexOf(")") > 0)
				{
					roomname = Regex.Replace(roomname, reg_filterroomname, ",");
				}
				else
				{
					break;
				}
			}
			return roomname;
		}

	}
}
