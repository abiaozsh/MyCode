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
using ClassLibrary1;

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
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.dataGridView1 = new System.Windows.Forms.DataGridView();
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fdsaToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.verifyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.compaireToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.sortToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.nameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.sizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.cuntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.reportDupoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
			this.menuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// treeView1
			// 
			this.treeView1.AllowDrop = true;
			this.treeView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.treeView1.Location = new System.Drawing.Point(0, 4);
			this.treeView1.Name = "treeView1";
			this.treeView1.Size = new System.Drawing.Size(370, 397);
			this.treeView1.TabIndex = 4;
			this.treeView1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.Location = new System.Drawing.Point(2, 27);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.treeView1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.dataGridView1);
			this.splitContainer1.Size = new System.Drawing.Size(799, 404);
			this.splitContainer1.SplitterDistance = 373;
			this.splitContainer1.TabIndex = 5;
			// 
			// dataGridView1
			// 
			this.dataGridView1.AllowDrop = true;
			this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView1.Location = new System.Drawing.Point(3, 4);
			this.dataGridView1.Name = "dataGridView1";
			this.dataGridView1.RowTemplate.Height = 23;
			this.dataGridView1.Size = new System.Drawing.Size(416, 397);
			this.dataGridView1.TabIndex = 2;
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fdsaToolStripMenuItem,
            this.verifyToolStripMenuItem,
            this.compaireToolStripMenuItem,
            this.sortToolStripMenuItem,
            this.reportDupoToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(801, 24);
			this.menuStrip1.TabIndex = 7;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fdsaToolStripMenuItem
			// 
			this.fdsaToolStripMenuItem.Name = "fdsaToolStripMenuItem";
			this.fdsaToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
			this.fdsaToolStripMenuItem.Text = "clear";
			this.fdsaToolStripMenuItem.Click += new System.EventHandler(this.fdsaToolStripMenuItem_Click);
			// 
			// verifyToolStripMenuItem
			// 
			this.verifyToolStripMenuItem.Name = "verifyToolStripMenuItem";
			this.verifyToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
			this.verifyToolStripMenuItem.Text = "verify";
			this.verifyToolStripMenuItem.Click += new System.EventHandler(this.verifyToolStripMenuItem_Click);
			// 
			// compaireToolStripMenuItem
			// 
			this.compaireToolStripMenuItem.Name = "compaireToolStripMenuItem";
			this.compaireToolStripMenuItem.Size = new System.Drawing.Size(12, 20);
			// 
			// sortToolStripMenuItem
			// 
			this.sortToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.nameToolStripMenuItem,
            this.sizeToolStripMenuItem,
            this.dateToolStripMenuItem,
            this.cuntToolStripMenuItem});
			this.sortToolStripMenuItem.Name = "sortToolStripMenuItem";
			this.sortToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
			this.sortToolStripMenuItem.Text = "sort";
			// 
			// nameToolStripMenuItem
			// 
			this.nameToolStripMenuItem.Name = "nameToolStripMenuItem";
			this.nameToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.nameToolStripMenuItem.Text = "name";
			this.nameToolStripMenuItem.Click += new System.EventHandler(this.nameToolStripMenuItem_Click);
			// 
			// sizeToolStripMenuItem
			// 
			this.sizeToolStripMenuItem.Name = "sizeToolStripMenuItem";
			this.sizeToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.sizeToolStripMenuItem.Text = "size";
			this.sizeToolStripMenuItem.Click += new System.EventHandler(this.sizeToolStripMenuItem_Click);
			// 
			// dateToolStripMenuItem
			// 
			this.dateToolStripMenuItem.Name = "dateToolStripMenuItem";
			this.dateToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.dateToolStripMenuItem.Text = "date";
			this.dateToolStripMenuItem.Click += new System.EventHandler(this.dateToolStripMenuItem_Click);
			// 
			// cuntToolStripMenuItem
			// 
			this.cuntToolStripMenuItem.Name = "cuntToolStripMenuItem";
			this.cuntToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.cuntToolStripMenuItem.Text = "cunt";
			this.cuntToolStripMenuItem.Click += new System.EventHandler(this.cuntToolStripMenuItem_Click);
			// 
			// reportDupoToolStripMenuItem
			// 
			this.reportDupoToolStripMenuItem.Name = "reportDupoToolStripMenuItem";
			this.reportDupoToolStripMenuItem.Size = new System.Drawing.Size(83, 20);
			this.reportDupoToolStripMenuItem.Text = "report dupo";
			this.reportDupoToolStripMenuItem.Click += new System.EventHandler(this.reportDupoToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(801, 431);
			this.Controls.Add(this.menuStrip1);
			this.Controls.Add(this.splitContainer1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "Form1";
			this.Text = "Form1";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TreeView treeView1;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private ContextMenuStrip contextMenuStrip1;
		private MenuStrip menuStrip1;
		private ToolStripMenuItem fdsaToolStripMenuItem;
		private ToolStripMenuItem verifyToolStripMenuItem;
		private ToolStripMenuItem compaireToolStripMenuItem;
		private ToolStripMenuItem sortToolStripMenuItem;
		private ToolStripMenuItem nameToolStripMenuItem;
		private ToolStripMenuItem sizeToolStripMenuItem;
		private ToolStripMenuItem dateToolStripMenuItem;
		private ToolStripMenuItem cuntToolStripMenuItem;
		private ToolStripMenuItem reportDupoToolStripMenuItem;
		private System.Windows.Forms.DataGridView dataGridView1;


		public Form1()
		{
			InitializeComponent();
		}

		bool sortbyname;
		bool sortbysize;
		bool sortbydate;
		bool sortbycunt;

		private void Form1_Load(object sender, EventArgs e)
		{
			this.treeView1.DragDrop += new DragEventHandler(Form1_DragDrop);
			this.treeView1.DragEnter += new DragEventHandler(Form1_DragEnter);
			dataGridView1.Columns.Add("file", "file");
			dataGridView1.Columns.Add("size", "size");
			dataGridView1.Columns.Add("datetime", "datetime");
			dataGridView1.Columns.Add("md5", "md5");
		}

		void Form1_DragEnter(object sender, DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}

		private void sort(List<TFolder> folderList)
		{
			folderList.Sort(delegate(TFolder tf1, TFolder tf2)
			{
				if (sortbyname)
				{
					return tf1.name.CompareTo(tf2.name);
				}
				if (sortbysize)
				{
					return tf1.size.CompareTo(tf2.size);
				}
				if (sortbydate)
				{
					return tf1.datetime.CompareTo(tf2.datetime);
				}
				if (sortbycunt)
				{
					return tf1.filecount.CompareTo(tf2.filecount);
				}
				return 0;
			});
		}

		private void sort(List<TFile> folderList)
		{
			folderList.Sort(delegate(TFile tf1, TFile tf2)
			{
				if (sortbyname)
				{
					return tf1.name.CompareTo(tf2.name);
				}
				if (sortbysize)
				{
					return tf1.size.CompareTo(tf2.size);
				}
				if (sortbydate)
				{
					return tf1.datetime.CompareTo(tf2.datetime);
				}
				if (sortbycunt)
				{
					return 0;
				}
				return 0;
			});

		}
		List<TRoot> roots = new List<TRoot>();

		void Form1_DragDrop(object sender, DragEventArgs e)
		{
			string[] files = (string[])e.Data.GetData("FileDrop");

			TreeNode tn = new TreeNode();
			TFolder tftemp = new TFolder();
			tftemp.fileList = new List<TFile>();
			tftemp.folderList = new List<TFolder>();
			tftemp.filecount = 0;
			tftemp.size = 0;
			foreach (string file in files)
			{
				TRoot tf = TRoot.load(file);
				roots.Add(tf);
				sort(tf.folderList);

				foreach (TFolder tf2 in tf.folderList)
				{
					tn.Nodes.Add(GetTree(tf2));
				}
				tftemp.fileList.AddRange(tf.fileList);
				tftemp.folderList.AddRange(tf.folderList);
				tftemp.filecount += tf.filecount;
				tftemp.size += tf.size;

			}
			tn.Text = tftemp.name + " (" + tftemp.filecount + "  ;  " + tostring(tftemp.size) + ")";
			tn.Tag = tftemp;

			this.treeView1.Nodes.Add(tn);
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

		private TreeNode GetTree(TFolder tf)
		{
			TreeNode tn = new TreeNode();
			tn.Text = tf.name + " (" + tf.filecount + "  ;  " + tostring(tf.size) + ")";
			tn.Tag = tf;
			sort(tf.folderList);
			foreach (TFolder tf2 in tf.folderList)
			{
				tn.Nodes.Add(GetTree(tf2));
			}
			return tn;
		}

		private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
		{
			//e.Node.Tag
			TFolder tf = (TFolder)e.Node.Tag;
			showFolder(tf);
		}

		private void showFolder(TFolder tf)
		{
			dataGridView1.Rows.Clear();

			sort(tf.folderList);
			foreach (TFolder file in tf.folderList)
			{
				DataGridViewRow row = new DataGridViewRow();
				row.CreateCells(dataGridView1, file, file.filecount + "  ;  " + tostring(file.size), file.getDatetime().ToString("yyyy-MM-dd HH:mm:ss"), "");
				dataGridView1.Rows.Add(row);
			}

			sort(tf.fileList);
			foreach (TFile file in tf.fileList)
			{
				DataGridViewRow row = new DataGridViewRow();
				row.CreateCells(dataGridView1, file, tostring(file.size), file.getDatetime().ToString("yyyy-MM-dd HH:mm:ss"), file.md5);
				dataGridView1.Rows.Add(row);
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{

		}

		private void fdsaToolStripMenuItem_Click(object sender, EventArgs e)
		{
			this.treeView1.Nodes.Clear();
			roots.Clear();
		}

		private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (this.dataGridView1.CurrentRow != null)
			{
				TFile tFile = (TFile)this.dataGridView1.CurrentRow.Cells[0].Value;
				if (tFile.md5 != null)
				{
					string filename = tFile.name;
					TFile currentTFile = tFile.parent;
					while (true)
					{
						if (currentTFile is TRoot)
						{
							filename = ((TRoot)currentTFile).root + "\\" + filename;
							break;
						}
						else
						{
							filename = ((TFolder)currentTFile).name + "\\" + filename;
							currentTFile = currentTFile.parent;
						}
					}
					if (tFile.md5 == TRoot.MD5Encoding(filename))
					{
						MessageBox.Show("OK");
					}
					else
					{
						MessageBox.Show("NG");
					}
				}
			}
		}

		private void nameToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortbyname = true;
			sortbysize = false;
			sortbydate = false;
			sortbycunt = false;
		}

		private void sizeToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortbyname = false;
			sortbysize = true;
			sortbydate = false;
			sortbycunt = false;
		}

		private void dateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortbyname = false;
			sortbysize = false;
			sortbydate = true;
			sortbycunt = false;
		}

		private void cuntToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortbyname = false;
			sortbysize = false;
			sortbydate = false;
			sortbycunt = true;
		}

		private void reportDupoToolStripMenuItem_Click(object sender, EventArgs e)
		{
			List<TFile> files = new List<TFile>();

			List<TFile> dupfiles = new List<TFile>();

			foreach (var item in roots)
			{
				addList(item, files);
			}

			sortbyname = false;
			sortbysize = true;
			sortbydate = false;
			sortbycunt = false;
			sort(files);

			TFile lastfile = files[0];
			foreach (var item in files)
			{
				if (item.size > 1000000)
				{
					if (item.size == lastfile.size)
					{
						dupfiles.Add(lastfile);
						dupfiles.Add(item);
					}
				}

				lastfile = item;
			}

			dataGridView1.Rows.Clear();

			foreach (TFile file in dupfiles)
			{
				DataGridViewRow row = new DataGridViewRow();
				row.CreateCells(dataGridView1, file.name, tostring(file.size), file.getDatetime().ToString("yyyy-MM-dd HH:mm:ss"), file.getFullPath());
				dataGridView1.Rows.Add(row);
			}

		}

		private void addList(TFolder folder, List<TFile> files)
		{
			files.AddRange(folder.fileList);
			foreach (var item in folder.folderList)
			{
				addList(item, files);
			}
		}
	}


}