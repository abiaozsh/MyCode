using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using filetool;

namespace FileTool
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}

		public class ListData { 
			public string key;
			public string val;
		}

		public List<ListData> list;

		private void Form2_Load(object sender, EventArgs e)
		{
			dataGridView1.Columns.Add("key", "key");
			dataGridView1.Columns.Add("val", "val");

			dataGridView1.Rows.Clear();

			foreach (var item in list)
			{
				DataGridViewRow row = new DataGridViewRow();
				row.CreateCells(dataGridView1, item.key, item.val);
				dataGridView1.Rows.Add(row);
			}
			Form1.AutoSizeColumn(dataGridView1);
		}
	}
}
