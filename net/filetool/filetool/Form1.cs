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
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.dataGridView1 = new System.Windows.Forms.DataGridView();
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.createToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.updateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.md5ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.addMd5ListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.makeMd5ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.stopMakeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.hashReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.sortToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.nameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.sizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.cuntToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.reportDupoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.findToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.verifyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.makeAvatarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
			this.treeView1.Size = new System.Drawing.Size(370, 314);
			this.treeView1.TabIndex = 4;
			this.treeView1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.Location = new System.Drawing.Point(2, 28);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.treeView1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.dataGridView1);
			this.splitContainer1.Size = new System.Drawing.Size(799, 321);
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
			this.dataGridView1.Size = new System.Drawing.Size(416, 314);
			this.dataGridView1.TabIndex = 2;
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
			// 
			// menuStrip1
			// 
			this.menuStrip1.AllowDrop = true;
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.md5ToolStripMenuItem,
            this.sortToolStripMenuItem,
            this.reportDupoToolStripMenuItem,
            this.findToolStripMenuItem,
            this.verifyToolStripMenuItem,
            this.makeAvatarToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(801, 25);
			this.menuStrip1.TabIndex = 7;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createToolStripMenuItem1,
            this.updateToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 21);
			this.fileToolStripMenuItem.Text = "file";
			// 
			// createToolStripMenuItem1
			// 
			this.createToolStripMenuItem1.Name = "createToolStripMenuItem1";
			this.createToolStripMenuItem1.Size = new System.Drawing.Size(117, 22);
			this.createToolStripMenuItem1.Text = "create";
			this.createToolStripMenuItem1.Click += new System.EventHandler(this.createToolStripMenuItem1_Click);
			// 
			// updateToolStripMenuItem
			// 
			this.updateToolStripMenuItem.Name = "updateToolStripMenuItem";
			this.updateToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
			this.updateToolStripMenuItem.Text = "update";
			this.updateToolStripMenuItem.Click += new System.EventHandler(this.updateToolStripMenuItem_Click);
			// 
			// openToolStripMenuItem
			// 
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
			this.openToolStripMenuItem.Text = "open";
			this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click_1);
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
			this.saveToolStripMenuItem.Text = "save";
			this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// md5ToolStripMenuItem
			// 
			this.md5ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addMd5ListToolStripMenuItem,
            this.makeMd5ToolStripMenuItem,
            this.stopMakeToolStripMenuItem,
            this.hashReportToolStripMenuItem});
			this.md5ToolStripMenuItem.Name = "md5ToolStripMenuItem";
			this.md5ToolStripMenuItem.Size = new System.Drawing.Size(46, 21);
			this.md5ToolStripMenuItem.Text = "md5";
			// 
			// addMd5ListToolStripMenuItem
			// 
			this.addMd5ListToolStripMenuItem.Name = "addMd5ListToolStripMenuItem";
			this.addMd5ListToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.addMd5ListToolStripMenuItem.Text = "addMd5List";
			this.addMd5ListToolStripMenuItem.Click += new System.EventHandler(this.addMd5ListToolStripMenuItem_Click);
			// 
			// makeMd5ToolStripMenuItem
			// 
			this.makeMd5ToolStripMenuItem.Name = "makeMd5ToolStripMenuItem";
			this.makeMd5ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.makeMd5ToolStripMenuItem.Text = "makeMd5";
			this.makeMd5ToolStripMenuItem.Click += new System.EventHandler(this.makeMd5ToolStripMenuItem_Click_1);
			// 
			// stopMakeToolStripMenuItem
			// 
			this.stopMakeToolStripMenuItem.Name = "stopMakeToolStripMenuItem";
			this.stopMakeToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.stopMakeToolStripMenuItem.Text = "stopMake";
			this.stopMakeToolStripMenuItem.Click += new System.EventHandler(this.stopMakeToolStripMenuItem_Click);
			// 
			// hashReportToolStripMenuItem
			// 
			this.hashReportToolStripMenuItem.Name = "hashReportToolStripMenuItem";
			this.hashReportToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.hashReportToolStripMenuItem.Text = "hashReport";
			this.hashReportToolStripMenuItem.Click += new System.EventHandler(this.hashReportToolStripMenuItem_Click_1);
			// 
			// sortToolStripMenuItem
			// 
			this.sortToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.nameToolStripMenuItem,
            this.sizeToolStripMenuItem,
            this.dateToolStripMenuItem,
            this.cuntToolStripMenuItem});
			this.sortToolStripMenuItem.Name = "sortToolStripMenuItem";
			this.sortToolStripMenuItem.Size = new System.Drawing.Size(43, 21);
			this.sortToolStripMenuItem.Text = "sort";
			// 
			// nameToolStripMenuItem
			// 
			this.nameToolStripMenuItem.Name = "nameToolStripMenuItem";
			this.nameToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
			this.nameToolStripMenuItem.Text = "name";
			this.nameToolStripMenuItem.Click += new System.EventHandler(this.nameToolStripMenuItem_Click);
			// 
			// sizeToolStripMenuItem
			// 
			this.sizeToolStripMenuItem.Name = "sizeToolStripMenuItem";
			this.sizeToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
			this.sizeToolStripMenuItem.Text = "size";
			this.sizeToolStripMenuItem.Click += new System.EventHandler(this.sizeToolStripMenuItem_Click);
			// 
			// dateToolStripMenuItem
			// 
			this.dateToolStripMenuItem.Name = "dateToolStripMenuItem";
			this.dateToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
			this.dateToolStripMenuItem.Text = "date";
			this.dateToolStripMenuItem.Click += new System.EventHandler(this.dateToolStripMenuItem_Click);
			// 
			// cuntToolStripMenuItem
			// 
			this.cuntToolStripMenuItem.Name = "cuntToolStripMenuItem";
			this.cuntToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
			this.cuntToolStripMenuItem.Text = "cunt";
			this.cuntToolStripMenuItem.Click += new System.EventHandler(this.cuntToolStripMenuItem_Click);
			// 
			// reportDupoToolStripMenuItem
			// 
			this.reportDupoToolStripMenuItem.Name = "reportDupoToolStripMenuItem";
			this.reportDupoToolStripMenuItem.Size = new System.Drawing.Size(92, 21);
			this.reportDupoToolStripMenuItem.Text = "report dupo";
			this.reportDupoToolStripMenuItem.Click += new System.EventHandler(this.reportDupoToolStripMenuItem_Click);
			// 
			// findToolStripMenuItem
			// 
			this.findToolStripMenuItem.Name = "findToolStripMenuItem";
			this.findToolStripMenuItem.Size = new System.Drawing.Size(42, 21);
			this.findToolStripMenuItem.Text = "find";
			this.findToolStripMenuItem.Click += new System.EventHandler(this.findToolStripMenuItem_Click);
			// 
			// verifyToolStripMenuItem
			// 
			this.verifyToolStripMenuItem.Name = "verifyToolStripMenuItem";
			this.verifyToolStripMenuItem.Size = new System.Drawing.Size(51, 21);
			this.verifyToolStripMenuItem.Text = "verify";
			this.verifyToolStripMenuItem.Click += new System.EventHandler(this.verifyToolStripMenuItem_Click);
			// 
			// makeAvatarToolStripMenuItem
			// 
			this.makeAvatarToolStripMenuItem.Name = "makeAvatarToolStripMenuItem";
			this.makeAvatarToolStripMenuItem.Size = new System.Drawing.Size(89, 21);
			this.makeAvatarToolStripMenuItem.Text = "makeAvatar";
			this.makeAvatarToolStripMenuItem.Click += new System.EventHandler(this.makeAvatarToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this.AllowDrop = true;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(801, 349);
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
		private ToolStripMenuItem sortToolStripMenuItem;
		private ToolStripMenuItem nameToolStripMenuItem;
		private ToolStripMenuItem sizeToolStripMenuItem;
		private ToolStripMenuItem dateToolStripMenuItem;
		private ToolStripMenuItem cuntToolStripMenuItem;
		private ToolStripMenuItem reportDupoToolStripMenuItem;
		private ToolStripMenuItem findToolStripMenuItem;
		private ToolStripMenuItem fileToolStripMenuItem;
		private ToolStripMenuItem openToolStripMenuItem;
		private ToolStripMenuItem saveToolStripMenuItem;
		private ToolStripMenuItem md5ToolStripMenuItem;
		private ToolStripMenuItem makeMd5ToolStripMenuItem;
		private ToolStripMenuItem hashReportToolStripMenuItem;
		private ToolStripMenuItem createToolStripMenuItem1;
		private ToolStripMenuItem updateToolStripMenuItem;
		private System.Windows.Forms.DataGridView dataGridView1;

		TRoot root;
		string filePath;

		enum SortBy
		{
			name,
			size,
			date,
			cunt
		}
		SortBy sortby;
		private ToolStripMenuItem addMd5ListToolStripMenuItem;
		private ToolStripMenuItem stopMakeToolStripMenuItem;
		private ToolStripMenuItem verifyToolStripMenuItem;
		private ToolStripMenuItem makeAvatarToolStripMenuItem;
		bool sortbyMd5;

		public Form1()
		{
			InitializeComponent();
		}

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
				if (sortby == SortBy.name)
				{
					return tf1.name.CompareTo(tf2.name);
				}
				else if (sortby == SortBy.size)
				{
					return tf1.size.CompareTo(tf2.size);
				}
				else if (sortby == SortBy.date)
				{
					return tf1.datetime.CompareTo(tf2.datetime);
				}
				else if (sortby == SortBy.cunt)
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
				if (sortbyMd5)
				{
					int result = tf1.size.CompareTo(tf2.size);
					if (result != 0)
					{
						return result;
					}
					string md5a = "";
					if (root.md5List.ContainsKey(tf1.getFullPath()))
					{
						md5a = root.md5List[tf1.getFullPath()];
					}

					string md5b = "";
					if (root.md5List.ContainsKey(tf2.getFullPath()))
					{
						md5b = root.md5List[tf2.getFullPath()];
					}

					return md5a.CompareTo(md5b);
				}
				else if (sortby == SortBy.name)
				{
					return tf1.name.CompareTo(tf2.name);
				}
				else if (sortby == SortBy.size)
				{
					return tf1.size.CompareTo(tf2.size);
				}
				else if (sortby == SortBy.date)
				{
					return tf1.datetime.CompareTo(tf2.datetime);
				}
				else if (sortby == SortBy.cunt)
				{
					return 0;
				}
				return 0;
			});
		}

		void Form1_DragDrop(object sender, DragEventArgs e)
		{
			string[] files = (string[])e.Data.GetData("FileDrop");
			TRoot tf = TRoot.load(files[0]);
			filePath = files[0];
			refresh(tf);
		}

		void refresh(TRoot tf)
		{
			TreeNode tn = new TreeNode();
			TFolder tftemp = new TFolder();
			tftemp.fileList = new List<TFile>();
			tftemp.folderList = new List<TFolder>();
			tftemp.filecount = 0;
			tftemp.size = 0;

			root = tf;
			sort(tf.folderList);

			foreach (TFolder tf2 in tf.folderList)
			{
				tn.Nodes.Add(GetTree(tf2));
			}
			tftemp.fileList.AddRange(tf.fileList);
			tftemp.folderList.AddRange(tf.folderList);
			tftemp.filecount += tf.filecount;
			tftemp.size += tf.size;

			tn.Text = tftemp.name + " (" + tftemp.filecount + "  ;  " + tostring(tftemp.size) + ")";
			tn.Tag = tftemp;
			this.treeView1.Nodes.Clear();
			this.treeView1.Nodes.Add(tn);

			this.Text = root.path;
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
				string md5 = "";
				if (root.md5List.ContainsKey(file.getFullPath()))
				{
					md5 = root.md5List[file.getFullPath()];
				}
				row.CreateCells(dataGridView1, file, tostring(file.size), file.getDatetime().ToString("yyyy-MM-dd HH:mm:ss"), md5);
				dataGridView1.Rows.Add(row);
			}
			AutoSizeColumn(dataGridView1);

		}

		private void nameToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortby = SortBy.name;
		}

		private void sizeToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortby = SortBy.size;
		}

		private void dateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortby = SortBy.date;
		}

		private void cuntToolStripMenuItem_Click(object sender, EventArgs e)
		{
			sortby = SortBy.cunt;
		}

		private void reportDupoToolStripMenuItem_Click(object sender, EventArgs e)
		{
			List<TFile> files = new List<TFile>();

			List<TFile> dupfiles = new List<TFile>();

			if (treeView1.SelectedNode == null)
			{
				MessageBox.Show("select");
				return;
			}

			TFolder tf = (TFolder)treeView1.SelectedNode.Tag;

			foreach (var item in tf.folderList)
			{
				addList(item, files);
			}

			sortbyMd5 = true;
			sort(files);
			sortbyMd5 = false;

			TFile lastfile = files[0];
			foreach (var item in files)
			{
				string md5a = "";
				if (root.md5List.ContainsKey(item.getFullPath()))
				{
					md5a = root.md5List[item.getFullPath()];
				}

				string md5b = "";
				if (root.md5List.ContainsKey(lastfile.getFullPath()))
				{
					md5b = root.md5List[lastfile.getFullPath()];
				}

				if (md5a == md5b)
				{
					dupfiles.Add(lastfile);
					dupfiles.Add(item);
				}

				lastfile = item;
			}

			dataGridView1.Rows.Clear();

			foreach (TFile file in dupfiles)
			{
				DataGridViewRow row = new DataGridViewRow();
				string md5 = "";
				if (root.md5List.ContainsKey(file.getFullPath()))
				{
					md5 = root.md5List[file.getFullPath()];
				}
				row.CreateCells(dataGridView1, file.getFullPath(), tostring(file.size), file.getDatetime().ToString("yyyy-MM-dd HH:mm:ss"), md5);
				dataGridView1.Rows.Add(row);
			}
			AutoSizeColumn(dataGridView1);
		}

		private void addList(TFolder folder, List<TFile> files)
		{
			files.AddRange(folder.fileList);
			foreach (var item in folder.folderList)
			{
				addList(item, files);
			}
		}

		private void findToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//find(roots[0], "bbtTheme.mp3");
		}

		void find(TFolder folder, string findtxt)
		{
			foreach (var item in folder.fileList)
			{
				if (item.name.Contains(findtxt))
				{
					MessageBox.Show(item.getFullPath());
				}
			}
			foreach (var item in folder.folderList)
			{
				find(item, findtxt);
			}
		}

		private void openToolStripMenuItem_Click_1(object sender, EventArgs e)
		{
			OpenFileDialog d = new OpenFileDialog();
			d.InitialDirectory = Application.ExecutablePath.Replace("FileTool.EXE", "");
			d.ShowDialog();
			if (d.FileNames.Length > 0)
			{
				TRoot tf = TRoot.load(d.FileNames[0]);
				filePath = d.FileNames[0];
				refresh(tf);
			}
		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(filePath))
			{
				OpenFileDialog d = new OpenFileDialog();
				d.InitialDirectory = Application.ExecutablePath.Replace("FileTool.EXE", "");
				d.ShowDialog();
				if (d.FileNames.Length > 0)
				{
					filePath = d.FileNames[0];
				}
			}
			root.save(filePath);
		}

		Md5Maker md5 = new Md5Maker();
		static void make(Md5Maker md5, TRoot root)
		{
			md5.makeMd5(root);
		}
		private void makeMd5ToolStripMenuItem_Click_1(object sender, EventArgs e)
		{
			Action<Md5Maker, TRoot> a = make;
			a.BeginInvoke(md5, root, null, null);
		}

		private void stopMakeToolStripMenuItem_Click(object sender, EventArgs e)
		{
			md5.stop = true;
		}

		private void hashReportToolStripMenuItem_Click_1(object sender, EventArgs e)
		{
			if (root != null && root.md5List != null)
			{
				Form2 form2 = new Form2();
				form2.list = new List<Form2.ListData>();
				foreach (var key in root.md5List.Keys)
				{
					form2.list.Add(new Form2.ListData() { key = key, val = root.md5List[key] });
				}

				form2.ShowDialog();
			}

		}

		private void createToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			FolderBrowserDialog d = new FolderBrowserDialog();
			d.ShowDialog();
			string path = d.SelectedPath;

			TRoot tf = TRoot.GetFolder(path);
			filePath = null;
			refresh(tf);
		}

		private void updateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			root.UpdateFolder();
			refresh(root);
		}

		private void addMd5ListToolStripMenuItem_Click(object sender, EventArgs e)
		{
			root.addMd5List(root);
		}

		private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (this.dataGridView1.CurrentRow != null)
			{
				TFile tFile = (TFile)this.dataGridView1.CurrentRow.Cells[0].Value;
				string md5 = "";
				if (root.md5List.ContainsKey(tFile.getFullPath()))
				{
					md5 = root.md5List[tFile.getFullPath()];
				}
				if (!string.IsNullOrEmpty(md5))
				{
					string filename = tFile.name;
					TFile currentTFile = tFile.parent;

					if (md5 == Md5Maker.MD5Encoding(root.path + tFile.getFullPath()))
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

		private void makeAvatarToolStripMenuItem_Click(object sender, EventArgs e)
		{
			FolderBrowserDialog d = new FolderBrowserDialog();
			d.ShowDialog();
			string path = d.SelectedPath + "\\";

			root.makeAvatar(root, path);
		}
	}


}