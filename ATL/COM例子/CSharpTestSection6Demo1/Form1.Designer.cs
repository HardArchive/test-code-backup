namespace CSharpTestSection6Demo1
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.axHelloCompositeControl1 = new AxSection6Demo1Lib.AxHelloCompositeControl();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.axHelloCompositeControl1)).BeginInit();
            this.SuspendLayout();
            // 
            // axHelloCompositeControl1
            // 
            this.axHelloCompositeControl1.Enabled = true;
            this.axHelloCompositeControl1.Location = new System.Drawing.Point(27, 23);
            this.axHelloCompositeControl1.Name = "axHelloCompositeControl1";
            this.axHelloCompositeControl1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axHelloCompositeControl1.OcxState")));
            this.axHelloCompositeControl1.Size = new System.Drawing.Size(230, 47);
            this.axHelloCompositeControl1.TabIndex = 0;
            this.axHelloCompositeControl1.OnClickBtn += new AxSection6Demo1Lib._IHelloCompositeControlEvents_OnClickBtnEventHandler(this.axHelloCompositeControl1_OnClickBtn);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(38, 150);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(89, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "属性读";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(160, 150);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(87, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "属性写";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(38, 201);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(89, 23);
            this.button3.TabIndex = 3;
            this.button3.Text = "禁用Button1";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(160, 201);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(87, 23);
            this.button4.TabIndex = 4;
            this.button4.Text = "启用Button1";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(38, 252);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(89, 23);
            this.button5.TabIndex = 5;
            this.button5.Text = "禁用Button2";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(160, 252);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(87, 23);
            this.button6.TabIndex = 6;
            this.button6.Text = "启用Button2";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(527, 358);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.axHelloCompositeControl1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.axHelloCompositeControl1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private AxSection6Demo1Lib.AxHelloCompositeControl axHelloCompositeControl1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
    }
}

