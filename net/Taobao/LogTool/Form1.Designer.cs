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
			this.n1 = new System.Windows.Forms.TextBox();
			this.t1 = new System.Windows.Forms.TextBox();
			this.t2 = new System.Windows.Forms.TextBox();
			this.n2 = new System.Windows.Forms.TextBox();
			this.t4 = new System.Windows.Forms.TextBox();
			this.n4 = new System.Windows.Forms.TextBox();
			this.t3 = new System.Windows.Forms.TextBox();
			this.n3 = new System.Windows.Forms.TextBox();
			this.button2 = new System.Windows.Forms.Button();
			this.button3 = new System.Windows.Forms.Button();
			this.button4 = new System.Windows.Forms.Button();
			this.button5 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(300, 159);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 38;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click_1);
			// 
			// n1
			// 
			this.n1.Location = new System.Drawing.Point(12, 39);
			this.n1.Name = "n1";
			this.n1.Size = new System.Drawing.Size(128, 21);
			this.n1.TabIndex = 39;
			// 
			// t1
			// 
			this.t1.Location = new System.Drawing.Point(12, 12);
			this.t1.Name = "t1";
			this.t1.Size = new System.Drawing.Size(128, 21);
			this.t1.TabIndex = 40;
			// 
			// t2
			// 
			this.t2.Location = new System.Drawing.Point(183, 12);
			this.t2.Name = "t2";
			this.t2.Size = new System.Drawing.Size(128, 21);
			this.t2.TabIndex = 42;
			// 
			// n2
			// 
			this.n2.Location = new System.Drawing.Point(183, 39);
			this.n2.Name = "n2";
			this.n2.Size = new System.Drawing.Size(128, 21);
			this.n2.TabIndex = 41;
			// 
			// t4
			// 
			this.t4.Location = new System.Drawing.Point(528, 12);
			this.t4.Name = "t4";
			this.t4.Size = new System.Drawing.Size(128, 21);
			this.t4.TabIndex = 46;
			// 
			// n4
			// 
			this.n4.Location = new System.Drawing.Point(528, 39);
			this.n4.Name = "n4";
			this.n4.Size = new System.Drawing.Size(128, 21);
			this.n4.TabIndex = 45;
			// 
			// t3
			// 
			this.t3.Location = new System.Drawing.Point(357, 12);
			this.t3.Name = "t3";
			this.t3.Size = new System.Drawing.Size(128, 21);
			this.t3.TabIndex = 44;
			// 
			// n3
			// 
			this.n3.Location = new System.Drawing.Point(357, 39);
			this.n3.Name = "n3";
			this.n3.Size = new System.Drawing.Size(128, 21);
			this.n3.TabIndex = 43;
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(36, 77);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(75, 23);
			this.button2.TabIndex = 47;
			this.button2.Text = "button2";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// button3
			// 
			this.button3.Location = new System.Drawing.Point(202, 77);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(75, 23);
			this.button3.TabIndex = 48;
			this.button3.Text = "button3";
			this.button3.UseVisualStyleBackColor = true;
			this.button3.Click += new System.EventHandler(this.button3_Click);
			// 
			// button4
			// 
			this.button4.Location = new System.Drawing.Point(381, 77);
			this.button4.Name = "button4";
			this.button4.Size = new System.Drawing.Size(75, 23);
			this.button4.TabIndex = 49;
			this.button4.Text = "button4";
			this.button4.UseVisualStyleBackColor = true;
			this.button4.Click += new System.EventHandler(this.button4_Click);
			// 
			// button5
			// 
			this.button5.Location = new System.Drawing.Point(554, 77);
			this.button5.Name = "button5";
			this.button5.Size = new System.Drawing.Size(75, 23);
			this.button5.TabIndex = 50;
			this.button5.Text = "button5";
			this.button5.UseVisualStyleBackColor = true;
			this.button5.Click += new System.EventHandler(this.button5_Click);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(726, 219);
			this.Controls.Add(this.button5);
			this.Controls.Add(this.button4);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.t4);
			this.Controls.Add(this.n4);
			this.Controls.Add(this.t3);
			this.Controls.Add(this.n3);
			this.Controls.Add(this.t2);
			this.Controls.Add(this.n2);
			this.Controls.Add(this.t1);
			this.Controls.Add(this.n1);
			this.Controls.Add(this.button1);
			this.Name = "Form1";
			this.Text = "Log";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox n1;
		private System.Windows.Forms.TextBox t1;
		private System.Windows.Forms.TextBox t2;
		private System.Windows.Forms.TextBox n2;
		private System.Windows.Forms.TextBox t4;
		private System.Windows.Forms.TextBox n4;
		private System.Windows.Forms.TextBox t3;
		private System.Windows.Forms.TextBox n3;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button4;
		private System.Windows.Forms.Button button5;
	}
}

