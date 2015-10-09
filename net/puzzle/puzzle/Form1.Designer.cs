namespace puzzle
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
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.listBox1 = new System.Windows.Forms.ListBox();
			this.button2 = new System.Windows.Forms.Button();
			this.textBox3 = new System.Windows.Forms.TextBox();
			this.button3 = new System.Windows.Forms.Button();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			this.SuspendLayout();
			// 
			// pictureBox1
			// 
			this.pictureBox1.Location = new System.Drawing.Point(13, 13);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(200, 200);
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
			this.pictureBox1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseDown);
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(234, 12);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(392, 21);
			this.textBox1.TabIndex = 1;
			this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(12, 219);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 2;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// textBox2
			// 
			this.textBox2.Location = new System.Drawing.Point(12, 277);
			this.textBox2.MaxLength = 0;
			this.textBox2.Multiline = true;
			this.textBox2.Name = "textBox2";
			this.textBox2.Size = new System.Drawing.Size(200, 179);
			this.textBox2.TabIndex = 3;
			// 
			// listBox1
			// 
			this.listBox1.FormattingEnabled = true;
			this.listBox1.ItemHeight = 12;
			this.listBox1.Items.AddRange(new object[] {
            "\'000000000000100F,0,0,0, ,2",
            "\'0000000000203C00,0,0,1, ,2",
            "\'0000000020780000,0,0,2, ,2",
            "\'0000001078000000,0,0,3, ,2",
            "\'0000043C00000000,0,0,4, ,2",
            "\'00008F0000000000,0,0,5,x,2",
            "\'0009E00000000000,0,0,6, ,2",
            "\'000000000000201E,0,1,0, ,2",
            "\'0000000000407800,0,1,1, ,2",
            "\'0000000040F00000,0,1,2, ,2",
            "\'00000020F0000000,0,1,3, ,2",
            "\'0000087800000000,0,1,4,x,2",
            "\'00011E0000000000,0,1,5, ,2",
            "\'000000000000403C,0,2,0, ,2",
            "\'000000000080F000,0,2,1, ,2",
            "\'0000000081E00000,0,2,2, ,2",
            "\'00000041E0000000,0,2,3,x,2",
            "\'000010F000000000,0,2,4, ,2",
            "\'0000000000008078,0,3,0, ,2",
            "\'000000000101E000,0,3,1, ,2",
            "\'0000000103C00000,0,3,2,x,2",
            "\'00000083C0000000,0,3,3, ,2",
            "\'00000000000100F0,0,4,0, ,2",
            "\'000000000203C000,0,4,1,x,2",
            "\'0000000207800000,0,4,2, ,2",
            "\'00000000000201E0,0,5,0,x,2",
            "\'0000000004078000,0,5,1, ,2",
            "\'00000000000403C0,0,6,0, ,2",
            "\'0000000010180802,1,0,0,x,2",
            "\'0000000818100800,1,0,1, ,2",
            "\'0000020C10100000,1,0,2, ,2",
            "\'0000430810000000,1,0,3, ,2",
            "\'0004620800000000,1,0,4,x,2",
            "\'0026420000000000,1,0,5,x,2",
            "\'0000000020301004,1,1,0, ,2",
            "\'0000001030201000,1,1,1, ,2",
            "\'0000041820200000,1,1,2, ,2",
            "\'0000861020000000,1,1,3, ,2",
            "\'0008C41000000000,1,1,4, ,2",
            "\'0000000040602008,1,2,0, ,2",
            "\'0000002060402000,1,2,1, ,2",
            "\'0000083040400000,1,2,2, ,2",
            "\'00010C2040000000,1,2,3, ,2",
            "\'0000000080C04010,1,3,0, ,2",
            "\'00000040C0804000,1,3,1, ,2",
            "\'0000106080800000,1,3,2, ,2",
            "\'0000000101808020,1,4,0, ,2",
            "\'0000008181008000,1,4,1, ,2",
            "\'0000000203010040,1,5,0,x,2",
            "\'0000000000003C02,2,0,0,x,2",
            "\'0000000000780800,2,0,1, ,2",
            "\'0000000078100000,2,0,2, ,2",
            "\'0000003C10000000,2,0,3, ,2",
            "\'00000F0800000000,2,0,4, ,2",
            "\'0001E20000000000,2,0,5,x,2",
            "\'0000000000007804,2,1,0, ,2",
            "\'0000000000F01000,2,1,1, ,2",
            "\'00000000F0200000,2,1,2, ,2",
            "\'0000007820000000,2,1,3, ,2",
            "\'00001E1000000000,2,1,4, ,2",
            "\'000000000000F008,2,2,0, ,2",
            "\'0000000001E02000,2,2,1, ,2",
            "\'00000001E0400000,2,2,2, ,2",
            "\'000000F040000000,2,2,3, ,2",
            "\'000000000001E010,2,3,0, ,2",
            "\'0000000003C04000,2,3,1, ,2",
            "\'00000003C0800000,2,3,2, ,2",
            "\'000000000003C020,2,4,0, ,2",
            "\'0000000007808000,2,4,1, ,2",
            "\'0000000000078040,2,5,0,x,2",
            "\'0000000008080C01,3,0,0, ,2",
            "\'0000000408180400,3,0,1, ,2",
            "\'0000010418080000,3,0,2, ,2",
            "\'0000210C08000000,3,0,3, ,2",
            "\'0002230400000000,3,0,4, ,2",
            "\'0012610000000000,3,0,5, ,2",
            "\'0056200000000000,3,0,6,x,2",
            "\'0000000010101802,3,1,0, ,2",
            "\'0000000810300800,3,1,1, ,2",
            "\'0000020830100000,3,1,2, ,2",
            "\'0000421810000000,3,1,3, ,2",
            "\'0004460800000000,3,1,4, ,2",
            "\'0024C20000000000,3,1,5,x,2",
            "\'0000000020203004,3,2,0, ,2",
            "\'0000001020601000,3,2,1, ,2",
            "\'0000041060200000,3,2,2, ,2",
            "\'0000843020000000,3,2,3, ,2",
            "\'00088C1000000000,3,2,4,x,2",
            "\'0000000040406008,3,3,0, ,2",
            "\'0000002040C02000,3,3,1, ,2",
            "\'00000820C0400000,3,3,2, ,2",
            "\'0001086040000000,3,3,3,x,2",
            "\'000000008080C010,3,4,0, ,2",
            "\'0000004081804000,3,4,1, ,2",
            "\'0000104180800000,3,4,2,x,2",
            "\'0000000101018020,3,5,0, ,2",
            "\'0000008103008000,3,5,1,x,2",
            "\'0000000202030040,3,6,0,x,2",
            "\'0000000000003C04,4,0,0,x,2",
            "\'0000000000781000,4,0,1, ,2",
            "\'0000000078200000,4,0,2, ,2",
            "\'0000003C20000000,4,0,3, ,2",
            "\'00000F1000000000,4,0,4, ,2",
            "\'0001E40000000000,4,0,5,x,2",
            "\'0000000000007808,4,1,0, ,2",
            "\'0000000000F02000,4,1,1, ,2",
            "\'00000000F0400000,4,1,2, ,2",
            "\'0000007840000000,4,1,3, ,2",
            "\'00001E2000000000,4,1,4, ,2",
            "\'000000000000F010,4,2,0, ,2",
            "\'0000000001E04000,4,2,1, ,2",
            "\'00000001E0800000,4,2,2, ,2",
            "\'000000F080000000,4,2,3, ,2",
            "\'000000000001E020,4,3,0, ,2",
            "\'0000000003C08000,4,3,1, ,2",
            "\'00000003C1000000,4,3,2, ,2",
            "\'000000000003C040,4,4,0,x,2",
            "\'0000000007810000,4,4,1, ,2",
            "\'0000000000078080,4,5,0,x,2",
            "\'0000000008180401,5,0,0, ,2",
            "\'0000000418080400,5,0,1, ,2",
            "\'0000010C08080000,5,0,2, ,2",
            "\'0000230408000000,5,0,3, ,2",
            "\'0002610400000000,5,0,4, ,2",
            "\'0016210000000000,5,0,5,x,2",
            "\'0072200000000000,5,0,6, ,2",
            "\'0000000010300802,5,1,0, ,2",
            "\'0000000830100800,5,1,1, ,2",
            "\'0000021810100000,5,1,2, ,2",
            "\'0000460810000000,5,1,3, ,2",
            "\'0004C20800000000,5,1,4,x,2",
            "\'002C420000000000,5,1,5, ,2",
            "\'0000000020601004,5,2,0, ,2",
            "\'0000001060201000,5,2,1, ,2",
            "\'0000043020200000,5,2,2, ,2",
            "\'00008C1020000000,5,2,3,x,2",
            "\'0009841000000000,5,2,4, ,2",
            "\'0000000040C02008,5,3,0, ,2",
            "\'00000020C0402000,5,3,1, ,2",
            "\'0000086040400000,5,3,2,x,2",
            "\'0001182040000000,5,3,3, ,2",
            "\'0000000081804010,5,4,0, ,2",
            "\'0000004180804000,5,4,1,x,2",
            "\'000010C080800000,5,4,2, ,2",
            "\'0000000103008020,5,5,0,x,2",
            "\'0000008301008000,5,5,1, ,2",
            "\'0000000206010040,5,6,0, ,2",
            "\'000000000000080F,6,0,0, ,2",
            "\'0000000000103C00,6,0,1, ,2",
            "\'0000000010780000,6,0,2, ,2",
            "\'0000000878000000,6,0,3, ,2",
            "\'0000023C00000000,6,0,4, ,2",
            "\'00004F0000000000,6,0,5, ,2",
            "\'0005E00000000000,6,0,6,x,2",
            "\'000000000000101E,6,1,0, ,2",
            "\'0000000000207800,6,1,1, ,2",
            "\'0000000020F00000,6,1,2, ,2",
            "\'00000010F0000000,6,1,3, ,2",
            "\'0000047800000000,6,1,4, ,2",
            "\'00009E0000000000,6,1,5,x,2",
            "\'000000000000203C,6,2,0, ,2",
            "\'000000000040F000,6,2,1, ,2",
            "\'0000000041E00000,6,2,2, ,2",
            "\'00000021E0000000,6,2,3, ,2",
            "\'000008F000000000,6,2,4,x,2",
            "\'0000000000004078,6,3,0, ,2",
            "\'000000000081E000,6,3,1, ,2",
            "\'0000000083C00000,6,3,2, ,2",
            "\'00000043C0000000,6,3,3,x,2",
            "\'00000000000080F0,6,4,0, ,2",
            "\'000000000103C000,6,4,1, ,2",
            "\'0000000107800000,6,4,2,x,2",
            "\'00000000000101E0,6,5,0,x,2",
            "\'0000000002078000,6,5,1,x,2",
            "\'00000000000203C0,6,6,0,x,2",
            "\'0000000010100C02,7,0,0,x,2",
            "\'0000000810180800,7,0,1, ,2",
            "\'0000020818100000,7,0,2, ,2",
            "\'0000420C10000000,7,0,3, ,2",
            "\'0004430800000000,7,0,4,x,2",
            "\'0024620000000000,7,0,5,x,2",
            "\'0000000020201804,7,1,0, ,2",
            "\'0000001020301000,7,1,1, ,2",
            "\'0000041030200000,7,1,2, ,2",
            "\'0000841820000000,7,1,3, ,2",
            "\'0008861000000000,7,1,4, ,2",
            "\'0000000040403008,7,2,0, ,2",
            "\'0000002040602000,7,2,1, ,2",
            "\'0000082060400000,7,2,2, ,2",
            "\'0001083040000000,7,2,3, ,2",
            "\'0000000080806010,7,3,0, ,2",
            "\'0000004080C04000,7,3,1, ,2",
            "\'00001040C0800000,7,3,2, ,2",
            "\'000000010100C020,7,4,0,?,2",
            "\'0000008101808000,7,4,1,?,2",
            "\'0000000202018040,7,5,0,x,2"});
			this.listBox1.Location = new System.Drawing.Point(234, 39);
			this.listBox1.Name = "listBox1";
			this.listBox1.Size = new System.Drawing.Size(392, 172);
			this.listBox1.TabIndex = 4;
			this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(13, 248);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(75, 23);
			this.button2.TabIndex = 5;
			this.button2.Text = "init";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// textBox3
			// 
			this.textBox3.Location = new System.Drawing.Point(93, 217);
			this.textBox3.Name = "textBox3";
			this.textBox3.Size = new System.Drawing.Size(392, 21);
			this.textBox3.TabIndex = 6;
			this.textBox3.TextChanged += new System.EventHandler(this.textBox3_TextChanged);
			// 
			// button3
			// 
			this.button3.Location = new System.Drawing.Point(93, 248);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(75, 23);
			this.button3.TabIndex = 7;
			this.button3.Text = "button3";
			this.button3.UseVisualStyleBackColor = true;
			this.button3.Click += new System.EventHandler(this.button3_Click);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(662, 479);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.textBox3);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.listBox1);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.pictureBox1);
			this.Name = "Form1";
			this.Text = "Form1";
			this.Load += new System.EventHandler(this.Form1_Load);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.ListBox listBox1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.TextBox textBox3;
		private System.Windows.Forms.Button button3;
    }
}

