using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace LZ.Reverse
{

    public partial class Form1 : Form
    {
        private List<string> files = new List<string>();

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (files.Count == 0) return;

            ReverserNew reverser;
            try
            {
                reverser = new ReverserNew(true, 0, true, 2);
            }
            catch (COMException exception)
            {
                LZ.Reverse.Info.Write(new string[] { "*** unable to connect with PowerDesigner: \"{0}\"", exception.Message });
                return;
            }

            foreach (string sFile in files)
            {
                reverser.ProcessAFile(sFile, Encoding.GetEncoding("GB2312"));
            }

            // 处理各个文件
            reverser.ProcessWork();

            reverser.Close();
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                StringBuilder sb = new StringBuilder();
                foreach(string sFile in ofd.FileNames)
                {
                    if (!files.Contains(sFile))
                    {
                        files.Add(sFile);
                        sb.AppendLine(sFile);
                    }
                }
                txtFiles.Text += sb.ToString();
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            files.Clear();
            txtFiles.Clear();
        }
    }


    #region 词法分析
    public class LexicalAnalysis
    {
        /// <summary>   
        /// 记录字符位置   
        /// </summary>   
        int i = 0, j = 0;

        /// <summary>   
        /// 行号   
        /// </summary>   
        public static int LineNo = 1;

        /// <summary>   
        /// 记录识别出的单词      
        /// </summary>   
        string token = "";

        /// <summary>   
        /// 记录每一次得到的完整信息   
        /// </summary>   
        string text2 = "";

        /// <summary>   
        /// 用来记录错误信息   
        /// </summary>   
        string text3 = "";

        /// <summary>   
        /// 记录异常信息   
        /// </summary>   
        string wrong = "";

        /// <summary>   
        /// 错误字段的个数   
        /// </summary>   
        public static int errors = 0;

        /// <summary>   
        /// 记录错误的详细信息   
        /// </summary>   
        public static string AllWrongText = "";

        /// <summary>   
        /// 标记成对出现的界符   
        /// </summary>   
        int flag1 = 0, flag2 = 0, flag3, flag4, flag5, flag6 = 0;

        /// <summary>   
        /// 关键字(部分)   
        /// </summary>   
        string[] keyWords = new string[32]{"auto", "double", "int","struct", "break", "else",  
               "long","switch","case","enum","register","typedef","char","extern","return",  
               "union","const","float","short","unsigned","continue","for","signed","void","default",  
               "goto","sizeof","volatile","do","if","while","static" };

        /// <summary>   
        /// 运算符   
        /// </summary>   
        string[] operatorWords = new string[28]{"+","-","*","/","%",">","<",">=","<=","==", "-=",  
               "+=","*=","/=","!=","=","%=","&","&&","|","||","!","++","--","~","<<",">>","?:"};

        /// <summary>   
        /// 边界符   
        /// </summary>   
        string[] boundWords = new string[24] { "{", "}", "[", "]", ";", ",", ".",   
               "(", ")", ":", "\"", "#", ">", "<", "\'", "?","+","-","*","/","%","!","=","~" };

        /// <summary>   
        /// 词法分析   
        /// </summary>   
        /// <param name="str">所要分析的句子</param>   
        /// <returns>经过分析之后的句子</returns>   
        public string Analysis(string str)
        {
            string space = "";                         //空格数   
            if (str.Length == 0)                           //判断字符串是否为空   
                return "";
            try
            {
                while (str[i] != '\0')      //读入字符判断，空格、字母、数字、界符   
                {
                    if (str[i] == ' ' || str[i] == '\t' || str[i] == '\r')
                    {
                        i++;                                     //跳过无意义的字符   
                    }
                    else if (str[i] == '\n')        //如果是换行符，则行号加1   
                    {
                        LineNo++;
                        i++;
                    }

                    else if (IsLetter(str[i]))      //如果是字母   
                    {
                        i = RecogniseLetter(str, i);
                        for (j = 0; j < keyWords.Length; j++)
                        {
                            if (token.CompareTo(keyWords[j]) == 0)
                            {
                                break;
                            }
                        }

                        if (j >= keyWords.Length)                    //是普通的标志符   
                        {
                            for (int m = 0; m < 12 - token.Length; m++)
                            {
                                space = space + " ";
                            }
                            text2 = text2 + LineNo.ToString() + ":  " + token + space + "标识符" + "\r\n";
                            token = "";
                            space = "";
                        }

                        if (j < keyWords.Length)                             //是保留字   
                        {
                            for (int m = 0; m < 12 - token.Length; m++)
                                space = space + " ";
                            text2 = text2 + LineNo.ToString() + ":  " + keyWords[j] + space + "关键字" + "\r\n"; ;
                            token = "";
                            space = "";
                        }
                    }//end else if   

                    else if (IsDigit(str[i]))       //如果是数字   
                    {
                        i = RecogniseDigit(str, i);
                        for (int m = 0; m < 12 - token.Length; m++)
                        {
                            space = space + " ";
                        }
                        text2 = text2 + LineNo.ToString() + ":  " + token + space + "常量" + "\r\n";
                        token = "";
                        space = "";
                    }//end else if   

                    else if (IsBound(str[i]))                  //识别界符   
                    {
                        i = RecogniseBound(str, i);
                        for (int m = 0; m < 12 - token.Length; m++)
                        {
                            space = space + " ";
                        }
                        text2 = text2 + LineNo.ToString() + ":  " + token + space + "界符" + "\r\n"; ;
                        token = "";
                        space = "";
                    }//end else if   

                    else if (IsOperator(str[i]))
                    {
                        i = RecogniseOperator(str, i);
                        for (int m = 0; m < 12 - token.Length; m++)
                        {
                            space = space + " ";
                        }
                        text2 = text2 + LineNo.ToString() + ":  " + token + space + "运算符" + "\r\n";
                        token = "";
                        space = "";
                    }//end else if   

                    else
                    {
                        error(0); i++;
                    }
                }//end while   
            }//end try   

            catch (DivideByZeroException e1)
            {
                wrong = e1.Message;
            }
            catch (IndexOutOfRangeException e2)
            {

                wrong = e2.Message;
            }
            catch (Exception e)
            {

                wrong = e.Message;
            }

            return text2;
        }

        /// <summary>   
        /// 判断字符是不是字母   
        /// </summary>   
        /// <param name="ch">待定字符</param>   
        /// <returns>是否</returns>   
        public bool IsLetter(char ch)
        {
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                return true;
            }
            else
                return false;
        }

        /// <summary>   
        /// 判断字符是不是数字   
        /// </summary>   
        /// <param name="ch">待定字符</param>   
        /// <returns>是否</returns>   
        public bool IsDigit(char ch)
        {
            if (ch >= '0' && ch <= '9')
            {
                return true;
            }
            else
                return false;
        }

        /// <summary>   
        /// 判断字符是不是下划线   
        /// </summary>   
        /// <param name="ch">待定字符</param>   
        /// <returns>是否</returns>   
        public bool IsUnderline(char ch)
        {
            if (ch == '_')
                return true;
            else return false;
        }

        /// <summary>   
        /// 判断字符是不是边界符   
        /// </summary>   
        /// <param name="ch">待定字符</param>   
        /// <returns></returns>   
        public bool IsBound(char ch)
        {
            for (int j = 0; j < boundWords.Length; j++)
            {
                if (ch.CompareTo(boundWords[j][0]) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>   
        /// 判断字符是不是操作符   
        /// </summary>   
        /// <param name="ch">待定字符</param>   
        /// <returns>是否</returns>   
        public bool IsOperator(char ch)
        {
            for (int i = 0; i < operatorWords.Length; i++)
            {
                if (ch == operatorWords[i][0])
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>   
        /// 识别整个操作符   
        /// </summary>   
        /// <param name="str">字符串</param>   
        /// <param name="i">字符串中字符的位置</param>   
        /// <returns>字符位置</returns>   
        private int RecogniseOperator(string str, int i)
        {
            char state = '0';
            string temp = "";

            while (state != '2')
            {
                switch (state)
                {
                    case '0':
                        temp += str[i];
                        i++;
                        state = '1';
                        break;

                    case '1':
                        //判断为双个运算符    
                        if (str.Substring(i - 1, 2) == "++" || str.Substring(i - 1, 2) == "--" || str.Substring(i - 1, 2) == "<<" || str.Substring(i - 1, 2) == ">>" || str.Substring(i - 1, 2) == "+=" || str.Substring(i - 1, 2) == "-=" || str.Substring(i - 1, 2) == "*=" || str.Substring(i - 1, 2) == "/=" || str.Substring(i - 1, 2) == "!=" || str.Substring(i - 1, 2) == "%=")
                        {
                            temp += str[i];
                            i++;
                            state = '2';
                        }
                        //三目运算符   
                        if (str[i - 1] == '?' && str[i] == ':')
                        {
                            temp += str[i];
                            i++;
                            state = '2';
                        }
                        //不是运算符   
                        else
                        {
                            state = '2';
                        }
                        break;
                }//end switch   
            }//end while   

            token = temp;
            return i;
        }

        /// <summary>   
        /// 识别普通的标识符   
        /// </summary>   
        /// <param name="str">用来识别的字符串</param>   
        /// <param name="i">字符串中字符的位置</param>   
        /// <returns>字符位置</returns>   
        public int RecogniseLetter(string str, int i)                          //识别单词   
        {
            char state = '0';
            string temp = "";                             //记录单词   
            while (state != '2')
            {
                switch (state)
                {
                    case '0':
                        if (IsLetter(str[i]))
                        {
                            state = '1';
                            temp = temp + str[i];
                            i++;
                        }
                        break;

                    case '1':
                        if (IsLetter(str[i]) || IsDigit(str[i]) || IsUnderline(str[i]))
                        {
                            state = '1';
                            temp = temp + str[i];
                            i++;
                        }
                        else
                            state = '2';
                        break;
                }//end switch   
            }//end while   

            token = temp;                   //记录识别的字符串   
            return i;
        }

        /// <summary>   
        /// 识别整个常熟   
        /// </summary>   
        /// <param name="str">待测字符串</param>   
        /// <param name="i">字符串中字符的位置</param>   
        /// <returns>字符位置</returns>   
        public int RecogniseDigit(string str, int i)                                //识别常数   
        {
            char state = '0';
            string temp = "";

            while (state != '2')
            {
                switch (state)
                {
                    case '0':
                        if (IsDigit(str[i]))
                        {
                            temp += str[i];
                            state = '1';
                            i++;
                        }
                        break;

                    case '1':
                        if (IsDigit(str[i]))
                        {
                            temp += str[i];
                            state = '1';
                            i++;
                        }
                        else if (str[i] == '.' && IsDigit(str[i + 1])) //有小数点的数                            
                        {
                            temp += str[i];
                            state = '1';
                            i++;
                        }
                        else
                            state = '2';
                        break;
                }//end switch                     
            }//end while   

            token = temp;
            return i;
        }

        /// <summary>   
        /// 识别标识符   
        /// </summary>   
        /// <param name="str">待测字符串</param>   
        /// <param name="i">字符串中字符的位置</param>   
        /// <returns>字符位置</returns>   
        public int RecogniseBound(string str, int i)
        {
            string temp = "";
            for (int k = 0; k < boundWords.Length; k++)
            {
                if (str[i].CompareTo(boundWords[k][0]) == 0)
                {
                    temp += str[i];
                    i++;
                    break;
                }
                else
                    continue;
            }//end for   

            token = temp;

            //这些标志数是用来等会做错误判断用的（没有成双出现就是错误）   
            if (token == "{" || token == "}")
                flag1++;

            else if (token == "[" || token == "]")
                flag2++;

            else if (token == "(" || token == ")")
                flag3++;

            else if (token == "<" || token == ">")
                flag4++;

            else if (token == "\'")
                flag5++;

            else if (token == "\"")
                flag6++;

            return i;
        }

        /// <summary>   
        /// 得到已经识别的字符串   
        /// </summary>   
        /// <param name="str">字符串</param>   
        /// <param name="k">字符位置</param>   
        /// <returns>字符位置</returns>   
        public int GetetToken(string str, int k)
        {
            switch (k)
            {
                case 1:
                    for (int i = 0; i < keyWords.Length; i++)  //关键字   
                    {
                        if (str == keyWords[i])
                            return i;
                    }
                    break;

                case 2:
                    for (int i = 0; i < operatorWords.Length; i++)  //运算符   
                    {
                        if (str == operatorWords[i])
                            return i + 32;   //+32的原因是keyWords中有32个关键字   
                    }
                    break;

                case 3:
                    for (int i = 0; i < boundWords.Length; i++) //界符   
                    {
                        if (str == boundWords[i])
                            return i + 60;   //+60的原因是keyWords+operatorWords有60个   
                    }
                    break;
            }//end switch   

            return 0;
        }

        /// <summary>   
        /// 错误个数   
        /// </summary>   
        /// <returns>错误信息</returns>   
        public string ErrorNO()
        {
            error(1);
            text3 = errors.ToString() + "   errors";
            return text3;
        }

        /// <summary>   
        /// 错误信息   
        /// </summary>   
        /// <param name="k"></param>   
        public void error(int k)
        {
            switch (k)
            {
                case 0:    //输入了非法字符   
                    AllWrongText = AllWrongText + LineNo.ToString() + ": " + "非法字符" + "\r\n";
                    errors++;
                    break;

                case 1:   //界符不匹配   
                    if (flag1 % 2 != 0)
                    {
                        AllWrongText = AllWrongText + "{ 不匹配" + "\r\n";
                        errors++;
                    }
                    if (flag2 % 2 != 0)
                    {
                        AllWrongText = AllWrongText + "[ 不匹配" + "\r\n";
                        errors++;
                    }
                    if (flag3 % 2 != 0)
                    {
                        AllWrongText = AllWrongText + "( 不匹配" + "\r\n";
                        errors++;
                    }
                    if (flag4 % 2 != 0)
                    {
                        AllWrongText = AllWrongText + "< 不匹配" + "\r\n";
                        errors++;
                    }
                    if (flag5 % 2 != 0)
                    {
                        AllWrongText = AllWrongText + "' 不匹配" + "\r\n";
                        errors++;
                    }
                    if (flag6 % 2 != 0)
                    {
                        AllWrongText = AllWrongText + " \" 不匹配" + "\r\n";
                        errors++;
                    }
                    break;
            }//end switch   
        }

    }
    #endregion   

}
