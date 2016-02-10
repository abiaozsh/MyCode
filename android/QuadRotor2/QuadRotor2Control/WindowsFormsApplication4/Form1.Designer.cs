namespace WindowsFormsApplication4
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
            this.components = new System.ComponentModel.Container();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.pwr1 = new System.Windows.Forms.TrackBar();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.trackBar3 = new System.Windows.Forms.TrackBar();
            this.trackBar4 = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.start1 = new System.Windows.Forms.CheckBox();
            this.start2 = new System.Windows.Forms.CheckBox();
            this.pwr2 = new System.Windows.Forms.TrackBar();
            this.start3 = new System.Windows.Forms.CheckBox();
            this.pwr3 = new System.Windows.Forms.TrackBar();
            this.start4 = new System.Windows.Forms.CheckBox();
            this.pwr4 = new System.Windows.Forms.TrackBar();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.button5 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(12, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(320, 240);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(339, 13);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "cali";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.Red;
            this.button2.Location = new System.Drawing.Point(339, 43);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "reset";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(338, 101);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 3;
            this.button3.Text = "ON";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(338, 131);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 4;
            this.button4.Text = "OFF";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(426, 12);
            this.textBox1.MaxLength = 0;
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(649, 304);
            this.textBox1.TabIndex = 6;
            this.textBox1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox1_KeyPress);
            // 
            // pwr1
            // 
            this.pwr1.Location = new System.Drawing.Point(77, 263);
            this.pwr1.Maximum = 255;
            this.pwr1.Name = "pwr1";
            this.pwr1.Size = new System.Drawing.Size(263, 42);
            this.pwr1.TabIndex = 7;
            this.pwr1.Scroll += new System.EventHandler(this.pwr1_Scroll);
            // 
            // pictureBox2
            // 
            this.pictureBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox2.Location = new System.Drawing.Point(503, 407);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(100, 100);
            this.pictureBox2.TabIndex = 9;
            this.pictureBox2.TabStop = false;
            this.pictureBox2.Click += new System.EventHandler(this.pictureBox2_Click);
            this.pictureBox2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox2_MouseDown);
            this.pictureBox2.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox2_MouseMove);
            this.pictureBox2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox2_MouseUp);
            // 
            // trackBar2
            // 
            this.trackBar2.Location = new System.Drawing.Point(11, 522);
            this.trackBar2.Maximum = 255;
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(761, 42);
            this.trackBar2.TabIndex = 10;
            this.trackBar2.Scroll += new System.EventHandler(this.trackBar2_Scroll);
            // 
            // trackBar3
            // 
            this.trackBar3.Location = new System.Drawing.Point(609, 465);
            this.trackBar3.Maximum = 100;
            this.trackBar3.Minimum = -100;
            this.trackBar3.Name = "trackBar3";
            this.trackBar3.Size = new System.Drawing.Size(97, 42);
            this.trackBar3.TabIndex = 11;
            this.trackBar3.Scroll += new System.EventHandler(this.trackBar3_Scroll);
            // 
            // trackBar4
            // 
            this.trackBar4.Location = new System.Drawing.Point(643, 377);
            this.trackBar4.Maximum = 100;
            this.trackBar4.Minimum = -100;
            this.trackBar4.Name = "trackBar4";
            this.trackBar4.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar4.Size = new System.Drawing.Size(42, 82);
            this.trackBar4.TabIndex = 12;
            this.trackBar4.Scroll += new System.EventHandler(this.trackBar4_Scroll);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(678, 412);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 13;
            this.label4.Text = "label4";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(698, 476);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 12);
            this.label5.TabIndex = 14;
            this.label5.Text = "label5";
            // 
            // start1
            // 
            this.start1.AutoSize = true;
            this.start1.Location = new System.Drawing.Point(11, 263);
            this.start1.Name = "start1";
            this.start1.Size = new System.Drawing.Size(60, 16);
            this.start1.TabIndex = 15;
            this.start1.Text = "start1";
            this.start1.UseVisualStyleBackColor = true;
            this.start1.CheckedChanged += new System.EventHandler(this.start1_CheckedChanged);
            // 
            // start2
            // 
            this.start2.AutoSize = true;
            this.start2.Location = new System.Drawing.Point(11, 311);
            this.start2.Name = "start2";
            this.start2.Size = new System.Drawing.Size(60, 16);
            this.start2.TabIndex = 17;
            this.start2.Text = "start2";
            this.start2.UseVisualStyleBackColor = true;
            this.start2.CheckedChanged += new System.EventHandler(this.start2_CheckedChanged);
            // 
            // pwr2
            // 
            this.pwr2.Location = new System.Drawing.Point(77, 311);
            this.pwr2.Maximum = 255;
            this.pwr2.Name = "pwr2";
            this.pwr2.Size = new System.Drawing.Size(263, 42);
            this.pwr2.TabIndex = 16;
            this.pwr2.Scroll += new System.EventHandler(this.pwr2_Scroll);
            // 
            // start3
            // 
            this.start3.AutoSize = true;
            this.start3.Location = new System.Drawing.Point(11, 359);
            this.start3.Name = "start3";
            this.start3.Size = new System.Drawing.Size(60, 16);
            this.start3.TabIndex = 19;
            this.start3.Text = "start3";
            this.start3.UseVisualStyleBackColor = true;
            this.start3.CheckedChanged += new System.EventHandler(this.start3_CheckedChanged);
            // 
            // pwr3
            // 
            this.pwr3.Location = new System.Drawing.Point(77, 359);
            this.pwr3.Maximum = 255;
            this.pwr3.Name = "pwr3";
            this.pwr3.Size = new System.Drawing.Size(263, 42);
            this.pwr3.TabIndex = 18;
            this.pwr3.Scroll += new System.EventHandler(this.pwr3_Scroll);
            // 
            // start4
            // 
            this.start4.AutoSize = true;
            this.start4.Location = new System.Drawing.Point(11, 407);
            this.start4.Name = "start4";
            this.start4.Size = new System.Drawing.Size(60, 16);
            this.start4.TabIndex = 21;
            this.start4.Text = "start4";
            this.start4.UseVisualStyleBackColor = true;
            this.start4.CheckedChanged += new System.EventHandler(this.start4_CheckedChanged);
            // 
            // pwr4
            // 
            this.pwr4.Location = new System.Drawing.Point(77, 407);
            this.pwr4.Maximum = 255;
            this.pwr4.Name = "pwr4";
            this.pwr4.Size = new System.Drawing.Size(263, 42);
            this.pwr4.TabIndex = 20;
            this.pwr4.Scroll += new System.EventHandler(this.pwr4_Scroll);
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(11, 446);
            this.trackBar1.Maximum = 255;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(321, 42);
            this.trackBar1.TabIndex = 20;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(338, 161);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(81, 21);
            this.textBox2.TabIndex = 22;
            this.textBox2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBox2_KeyDown);
            this.textBox2.KeyUp += new System.Windows.Forms.KeyEventHandler(this.textBox2_KeyUp);
            // 
            // button5
            // 
            this.button5.BackColor = System.Drawing.Color.Red;
            this.button5.Location = new System.Drawing.Point(339, 72);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(75, 23);
            this.button5.TabIndex = 23;
            this.button5.Text = "reset8B";
            this.button5.UseVisualStyleBackColor = false;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 20;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1028, 576);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.start4);
            this.Controls.Add(this.trackBar1);
            this.Controls.Add(this.pwr4);
            this.Controls.Add(this.start3);
            this.Controls.Add(this.pwr3);
            this.Controls.Add(this.start2);
            this.Controls.Add(this.pwr2);
            this.Controls.Add(this.start1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.trackBar4);
            this.Controls.Add(this.trackBar3);
            this.Controls.Add(this.trackBar2);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pwr1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Form1_KeyPress);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pwr4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button4;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.TrackBar pwr1;
		private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.TrackBar trackBar2;
		private System.Windows.Forms.TrackBar trackBar3;
		private System.Windows.Forms.TrackBar trackBar4;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.CheckBox start1;
		private System.Windows.Forms.CheckBox start2;
		private System.Windows.Forms.TrackBar pwr2;
		private System.Windows.Forms.CheckBox start3;
		private System.Windows.Forms.TrackBar pwr3;
		private System.Windows.Forms.CheckBox start4;
		private System.Windows.Forms.TrackBar pwr4;
		private System.Windows.Forms.TrackBar trackBar1;
		private System.Windows.Forms.TextBox textBox2;
		private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Timer timer1;
	}
}

