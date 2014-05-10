using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CSharpTestSection6Demo1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(axHelloCompositeControl1.Description);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            axHelloCompositeControl1.Description = "在C#中修改了描述属性，我是复合控件！";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            axHelloCompositeControl1.EnableButton(0, false);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            axHelloCompositeControl1.EnableButton(0, true);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            axHelloCompositeControl1.EnableButton(1, false);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            axHelloCompositeControl1.EnableButton(1, true);
        }

        private void axHelloCompositeControl1_OnClickBtn(object sender, AxSection6Demo1Lib._IHelloCompositeControlEvents_OnClickBtnEvent e)
        {
            String msg = "被点击";
            msg += e.lClickCountSum;
            msg += "次！";

            if (e.lBtnIndex == 0)
            {
                msg = "Button1" + msg;
            }
            else if (e.lBtnIndex == 1)
            {
                msg = "Button2" + msg;
            }

            MessageBox.Show(msg);
        }
    }
}