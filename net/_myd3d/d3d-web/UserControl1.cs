using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace d3d_web
{
	/// <summary>
	/// UserControl1 ��ժҪ˵����
	/// </summary>
	public class UserControl1 : System.Windows.Forms.UserControl
	{
		private System.Windows.Forms.Button button1;
		/// <summary>
		/// ����������������
		/// </summary>
		private System.ComponentModel.Container components = null;

		public UserControl1()
		{
			// �õ����� Windows.Forms ���������������ġ�
			InitializeComponent();

			// TODO: �� InitComponent ���ú�����κγ�ʼ��

		}

		/// <summary>
		/// ������������ʹ�õ���Դ��
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

		#region �����������ɵĴ���
		/// <summary>
		/// �����֧������ķ��� - ��Ҫʹ�ô���༭�� 
		/// �޸Ĵ˷��������ݡ�
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
