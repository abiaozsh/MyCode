namespace LogTool
{
	partial class Form1
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
			this.button1 = new System.Windows.Forms.Button();
			this.pages = new System.Windows.Forms.TextBox();
			this.search = new System.Windows.Forms.TextBox();
			this.button2 = new System.Windows.Forms.Button();
			this.priceHi = new System.Windows.Forms.TextBox();
			this.listBox1 = new System.Windows.Forms.ListBox();
			this.priceLo = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(269, 216);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 38;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click_1);
			// 
			// pages
			// 
			this.pages.Location = new System.Drawing.Point(12, 93);
			this.pages.Name = "pages";
			this.pages.Size = new System.Drawing.Size(128, 21);
			this.pages.TabIndex = 39;
			// 
			// search
			// 
			this.search.Location = new System.Drawing.Point(12, 12);
			this.search.Name = "search";
			this.search.Size = new System.Drawing.Size(128, 21);
			this.search.TabIndex = 40;
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(38, 120);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(75, 23);
			this.button2.TabIndex = 47;
			this.button2.Text = "button2";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// priceHi
			// 
			this.priceHi.Location = new System.Drawing.Point(12, 66);
			this.priceHi.Name = "priceHi";
			this.priceHi.Size = new System.Drawing.Size(128, 21);
			this.priceHi.TabIndex = 51;
			// 
			// listBox1
			// 
			this.listBox1.FormattingEnabled = true;
			this.listBox1.ItemHeight = 12;
			this.listBox1.Location = new System.Drawing.Point(146, 12);
			this.listBox1.Name = "listBox1";
			this.listBox1.Size = new System.Drawing.Size(438, 148);
			this.listBox1.TabIndex = 52;
			// 
			// priceLo
			// 
			this.priceLo.Location = new System.Drawing.Point(12, 39);
			this.priceLo.Name = "priceLo";
			this.priceLo.Size = new System.Drawing.Size(128, 21);
			this.priceLo.TabIndex = 53;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(726, 337);
			this.Controls.Add(this.priceLo);
			this.Controls.Add(this.listBox1);
			this.Controls.Add(this.priceHi);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.search);
			this.Controls.Add(this.pages);
			this.Controls.Add(this.button1);
			this.Name = "Form1";
			this.Text = "Log";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox pages;
		private System.Windows.Forms.TextBox search;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.TextBox priceHi;
		private System.Windows.Forms.ListBox listBox1;
		private System.Windows.Forms.TextBox priceLo;
	}
}

