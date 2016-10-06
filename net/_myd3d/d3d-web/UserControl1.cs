using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace d3d_web
{
	/// <summary>
	/// UserControl1 的摘要说明。
	/// </summary>
	public class UserControl1 : System.Windows.Forms.UserControl
	{
		private System.Windows.Forms.Button button1;
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public UserControl1()
		{
			// 该调用是 Windows.Forms 窗体设计器所必需的。
			InitializeComponent();

			// TODO: 在 InitComponent 调用后添加任何初始化

		}

		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
					components.Dispose();
			}
			base.Dispose( disposing );
		}

		#region 组件设计器生成的代码
		/// <summary>
		/// 设计器支持所需的方法 - 不要使用代码编辑器 
		/// 修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
			this.button1 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(24, 32);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(144, 80);
			this.button1.TabIndex = 0;
			this.button1.Text = "button1";
			// 
			// UserControl1
			// 
			this.Controls.Add(this.button1);
			this.Name = "UserControl1";
			this.Size = new System.Drawing.Size(368, 280);
			this.ResumeLayout(false);

		}
		#endregion
	}
}
