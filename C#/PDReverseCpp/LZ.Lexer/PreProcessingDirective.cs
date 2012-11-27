namespace LZ.Lexer
{
    using System;
    using System.Collections.Generic;
    using System.Drawing;

    public class PreProcessingDirective : AbstractSpecial
    {
        private string arg;
        private string cmd;

        public PreProcessingDirective(string cmd, string arg, Point start, Point end) : base(start, end)
        {
            this.cmd = cmd;
            this.arg = arg;
        }

        ////public override object AcceptVisitor(ISpecialVisitor visitor, object data)
        ////{
        ////    return visitor.Visit(this, data);
        ////}

        public static void CSharpToVB(List<ISpecial> list)
        {
            for (int i = 0; i < list.Count; i++)
            {
                if (list[i] is PreProcessingDirective)
                {
                    list[i] = CSharpToVB((PreProcessingDirective) list[i]);
                }
            }
        }

        public static PreProcessingDirective CSharpToVB(PreProcessingDirective dir)
        {
            string cmd = dir.Cmd;
            string arg = dir.Arg;
            string str3 = cmd;
            if (str3 != null)
            {
                if (!(str3 == "#region"))
                {
                    if (str3 == "#endregion")
                    {
                        cmd = "#End";
                        arg = "Region";
                    }
                    else if (str3 == "#endif")
                    {
                        cmd = "#End";
                        arg = "If";
                    }
                    else if (str3 == "#if")
                    {
                        arg = arg + " Then";
                    }
                }
                else if (!arg.StartsWith("\""))
                {
                    arg = "\"" + arg.Trim() + "\"";
                }
            }
            if (cmd.Length > 1)
            {
                cmd = cmd.Substring(0, 2).ToUpperInvariant() + cmd.Substring(2);
            }
            return new PreProcessingDirective(cmd, arg, dir.StartPosition, dir.EndPosition);
        }

        public override string ToString()
        {
            return string.Format("[PreProcessingDirective: Cmd = {0}, Arg = {1}]", this.Cmd, this.Arg);
        }

        public static PreProcessingDirective VBToCSharp(PreProcessingDirective dir)
        {
            string cmd = dir.Cmd.ToLowerInvariant();
            string arg = dir.Arg;
            string str3 = cmd;
            if (str3 != null)
            {
                if (!(str3 == "#end"))
                {
                    if ((str3 == "#if") && arg.ToLowerInvariant().EndsWith(" then"))
                    {
                        arg = arg.Substring(0, arg.Length - 5);
                    }
                }
                else if (arg.ToLowerInvariant().StartsWith("region"))
                {
                    cmd = "#endregion";
                    arg = "";
                }
                else if ("if".Equals(arg, StringComparison.InvariantCultureIgnoreCase))
                {
                    cmd = "#endif";
                    arg = "";
                }
            }
            return new PreProcessingDirective(cmd, arg, dir.StartPosition, dir.EndPosition);
        }

        public static void VBToCSharp(IList<ISpecial> list)
        {
            for (int i = 0; i < list.Count; i++)
            {
                if (list[i] is PreProcessingDirective)
                {
                    list[i] = VBToCSharp((PreProcessingDirective) list[i]);
                }
            }
        }

        public string Arg
        {
            get
            {
                return this.arg;
            }
            set
            {
                this.arg = value;
            }
        }

        public string Cmd
        {
            get
            {
                return this.cmd;
            }
            set
            {
                this.cmd = value;
            }
        }
    }
}
