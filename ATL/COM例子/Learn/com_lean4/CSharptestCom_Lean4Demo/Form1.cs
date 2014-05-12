using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using com_lean4Lib;

namespace CSharptestCom_Lean4Demo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            //创建简单对象
            HelloSimpleObjectClass helloSpObj = new HelloSimpleObjectClass();

            //加法方法测试
            int iSum = helloSpObj.Sumlong(9, 99);

            //读描述属性
            String strA = helloSpObj.Description;

            //写描述属性
            helloSpObj.Description = "我在C#中运行，我是新的描述属性！";

            //读描述属性
            String strB = helloSpObj.Description;
        }
    }
}
