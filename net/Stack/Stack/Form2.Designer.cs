namespace Stack
{
	partial class Form2
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
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.button2 = new System.Windows.Forms.Button();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.trackBar1 = new System.Windows.Forms.TrackBar();
			this.button3 = new System.Windows.Forms.Button();
			this.button4 = new System.Windows.Forms.Button();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(12, 12);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(96, 31);
			this.button1.TabIndex = 0;
			this.button1.Text = "calc";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(238, 12);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(270, 31);
			this.textBox1.TabIndex = 3;
			this.textBox1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Form2_KeyPress);
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(114, 12);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(118, 31);
			this.button2.TabIndex = 4;
			this.button2.Text = "next";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// pictureBox1
			// 
			this.pictureBox1.Location = new System.Drawing.Point(12, 59);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(640, 480);
			this.pictureBox1.TabIndex = 5;
			this.pictureBox1.TabStop = false;
			this.pictureBox1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseDown);
			this.pictureBox1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseUp);
			// 
			// trackBar1
			// 
			this.trackBar1.Location = new System.Drawing.Point(12, 558);
			this.trackBar1.Name = "trackBar1";
			this.trackBar1.Size = new System.Drawing.Size(640, 42);
			this.trackBar1.TabIndex = 6;
			this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
			// 
			// button3
			// 
			this.button3.Location = new System.Drawing.Point(595, 12);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(75, 23);
			this.button3.TabIndex = 7;
			this.button3.Text = "fromrepo";
			this.button3.UseVisualStyleBackColor = true;
			this.button3.Click += new System.EventHandler(this.button3_Click);
			// 
			// button4
			// 
			this.button4.Location = new System.Drawing.Point(514, 12);
			this.button4.Name = "button4";
			this.button4.Size = new System.Drawing.Size(75, 23);
			this.button4.TabIndex = 8;
			this.button4.Text = "torepo";
			this.button4.UseVisualStyleBackColor = true;
			this.button4.Click += new System.EventHandler(this.button4_Click);
			// 
			// Form2
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(674, 619);
			this.Controls.Add(this.button4);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.trackBar1);
			this.Controls.Add(this.pictureBox1);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.button1);
			this.Name = "Form2";
			this.Text = "Form2";
			this.Load += new System.EventHandler(this.Form2_Load);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}


		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.TrackBar trackBar1;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button4;
	}
}