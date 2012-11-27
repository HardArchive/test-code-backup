namespace LZ.Lexer.CPP
{
    using System;
    using System.Drawing;
    using System.Globalization;
    using System.IO;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Text.RegularExpressions;

    public sealed class Lexer : AbstractLexer
    {
        // Fields
        private char[] escapeSequenceBuffer;
        private char[] identBuffer;
        private const int MAX_IDENTIFIER_LENGTH = 0x200;

        // Methods
        public Lexer(TextReader reader)
            : base(reader)
        {
            this.identBuffer = new char[0x200];
            this.escapeSequenceBuffer = new char[12];
        }



        protected override Token Next()
        {
            int num;
            while ((num = base.ReaderRead()) != -1)
            {
                char ch;
                Token token;
                switch (num)
                {
                    case 0x2f:
                        {
                            int num2 = base.ReaderPeek();
                            if ((num2 != 0x2f) && (num2 != 0x2a))
                            {
                                break;
                            }
                            this.ReadComment();
                            continue;
                        }
                    ////case 60:
                    ////    if (base.ReaderPeek() == 0x25)
                    ////    {
                    ////        this.ReadScriptTag();
                    ////        continue;
                    ////    }
                    ////    token = this.ReadOperator('<');
                    ////    goto Label_0298;

                    case 0x40:
                        {
                            int num5 = base.ReaderRead();
                            if (num5 == -1)
                            {
                                base.errors.Error(base.Line, base.Col, string.Format("EOF after @", new object[0]));
                            }
                            else
                            {
                                int col = base.Col - 1;
                                int line = base.Line;
                                ch = (char)num5;
                                if (ch == '"')
                                {
                                    token = this.ReadVerbatimString();
                                    goto Label_0298;
                                }
                                if (char.IsLetterOrDigit(ch))
                                {
                                    token = new Token(1, col - 1, line, this.ReadIdent(ch));
                                    goto Label_0298;
                                }
                                base.errors.Error(line, col, string.Format("Unexpected char in Lexer.Next() : {0}", ch));
                            }
                            continue;
                        }
                    case 9:
                    case 0x20:
                        {
                            continue;
                        }
                    case 10:
                    case 13:
                        {
                            base.HandleLineEnd((char)num);
                            continue;
                        }
                    case 0x22:
                        token = this.ReadString();
                        goto Label_0298;

                    case 0x23:
                        {
                            Point start = new Point(base.Col - 1, base.Line);
                            string cmd = this.ReadIdent('#');
                            string str2 = base.ReadToEOL();
                            base.specialTracker.AddPreProcessingDirective(cmd, str2.Trim(), start, new Point((start.X + cmd.Length) + str2.Length, start.Y));
                            continue;
                        }
                    //case 0x25:
                    //    {
                    //        if (base.ReaderPeek() != 0x3e)
                    //        {
                    //            goto Label_00ED;
                    //        }
                    //        this.ReadScriptTag();
                    //        continue;
                    //    }
                    case 0x27:
                        token = this.ReadChar();
                        goto Label_0298;

                    default:
                        ch = (char)num;
                        if (char.IsLetter(ch) || (ch == '_'))
                        {
                            int num8 = base.Col - 1;
                            int num9 = base.Line;
                            string keyword = this.ReadIdent(ch);
                            int kind = Keywords.GetToken(keyword);
                            if (kind >= 0)
                            {
                                return new Token(kind, num8, num9, keyword);
                            }
                            return new Token(1, num8, num9, keyword);
                        }
                        if (char.IsDigit(ch))
                        {
                            token = this.ReadDigit(ch, base.Col - 1);
                        }
                        else
                        {
                            token = this.ReadOperator(ch);
                        }
                        goto Label_0298;
                }
                token = this.ReadOperator('/');
                goto Label_0298;
            Label_00ED:
                token = this.ReadOperator('%');
            Label_0298:
                if (token != null)
                {
                    return token;
                }
            }
            return new Token(0, base.Col, base.Line, string.Empty);
        }

        private Token ReadChar()
        {
            int col = base.Col - 1;
            int line = base.Line;
            int num3 = base.ReaderRead();
            if (num3 == -1)
            {
                base.errors.Error(line, col, string.Format("End of file reached inside character literal", new object[0]));
                return null;
            }
            char ch = (char)num3;
            char ch2 = ch;
            string str = string.Empty;
            if (ch == '\\')
            {
                str = this.ReadEscapeSequence(out ch2);
            }
            if (((ushort)base.ReaderRead()) != 0x27)
            {
                base.errors.Error(line, col, string.Format("Char not terminated", new object[0]));
            }
            return new Token(2, col, line, string.Concat(new object[] { "'", ch, str, "'" }), ch2);
        }

        private void ReadComment()
        {
            switch (base.ReaderRead())
            {
                case 0x2a:
                    this.ReadMultiLineComment();
                    return;

                case 0x2f:
                    if (base.ReaderPeek() == 0x2f)
                    {
                        base.ReaderRead();
                        this.ReadSingleLineComment(CommentType.Documentation);
                        return;
                    }
                    this.ReadSingleLineComment(CommentType.SingleLine);
                    return;
            }
            base.errors.Error(base.Line, base.Col, string.Format("Error while reading comment", new object[0]));
        }

        private string ReadCommentToEOL()
        {
            int num;
            if (base.specialCommentHash == null)
            {
                return base.ReadToEOL();
            }
            base.sb.Length = 0;
            StringBuilder builder = new StringBuilder();
            while ((num = base.ReaderRead()) != -1)
            {
                char ch = (char)num;
                if (base.HandleLineEnd(ch))
                {
                    break;
                }
                base.sb.Append(ch);
                if (base.IsIdentifierPart(num))
                {
                    builder.Append(ch);
                }
                else
                {
                    string key = builder.ToString();
                    builder.Length = 0;
                    if (base.specialCommentHash.ContainsKey(key))
                    {
                        Point startPosition = new Point(base.Col, base.Line);
                        string comment = ch + base.ReadToEOL();
                        base.tagComments.Add(new TagComment(key, comment, startPosition, new Point(base.Col, base.Line)));
                        base.sb.Append(comment);
                        break;
                    }
                }
            }
            return base.sb.ToString();
        }

        private Token ReadDigit(char ch, int x)
        {
            Token token2;
            int line = base.Line;
            base.sb.Length = 0;
            base.sb.Append(ch);
            string str = null;
            string str2 = null;
            bool flag = false;
            bool flag2 = false;
            bool flag3 = false;
            bool flag4 = false;
            bool flag5 = false;
            bool flag6 = false;
            char c = (char)base.ReaderPeek();
            if (ch == '.')
            {
                flag5 = true;
                while (char.IsDigit((char)base.ReaderPeek()))
                {
                    base.sb.Append((char)base.ReaderRead());
                }
                c = (char)base.ReaderPeek();
            }
            else if ((ch != '0') || ((c != 'x') && (c != 'X')))
            {
                while (char.IsDigit((char)base.ReaderPeek()))
                {
                    base.sb.Append((char)base.ReaderRead());
                }
                c = (char)base.ReaderPeek();
            }
            else
            {
                base.ReaderRead();
                base.sb.Length = 0;
                while (base.IsHex((char)base.ReaderPeek()))
                {
                    base.sb.Append((char)base.ReaderRead());
                }
                if (base.sb.Length == 0)
                {
                    base.sb.Append('0');
                    base.errors.Error(line, x, "Invalid hexadecimal integer literal");
                }
                flag = true;
                str = "0x";
                c = (char)base.ReaderPeek();
            }
            Token token = null;
            if (c == '.')
            {
                base.ReaderRead();
                c = (char)base.ReaderPeek();
                if (!char.IsDigit(c))
                {
                    token = new Token(15, base.Col - 1, base.Line, ".");
                    c = '.';
                }
                else
                {
                    flag5 = true;
                    if (flag)
                    {
                        base.errors.Error(line, x, string.Format("No hexadecimal floating point values allowed", new object[0]));
                    }
                    base.sb.Append('.');
                    while (char.IsDigit((char)base.ReaderPeek()))
                    {
                        base.sb.Append((char)base.ReaderRead());
                    }
                    c = (char)base.ReaderPeek();
                }
            }
            if ((c == 'e') || (c == 'E'))
            {
                flag5 = true;
                base.sb.Append((char)base.ReaderRead());
                switch (((char)base.ReaderPeek()))
                {
                    case '-':
                    case '+':
                        base.sb.Append((char)base.ReaderRead());
                        break;
                }
                while (char.IsDigit((char)base.ReaderPeek()))
                {
                    base.sb.Append((char)base.ReaderRead());
                }
                flag2 = true;
                c = (char)base.ReaderPeek();
            }
            if ((c == 'f') || (c == 'F'))
            {
                base.ReaderRead();
                str2 = "f";
                flag4 = true;
            }
            else if ((c == 'd') || (c == 'D'))
            {
                base.ReaderRead();
                str2 = "d";
                flag5 = true;
            }
            else if ((c == 'm') || (c == 'M'))
            {
                base.ReaderRead();
                str2 = "m";
                flag6 = true;
            }
            else if (!flag5)
            {
                switch (c)
                {
                    case 'u':
                    case 'U':
                        base.ReaderRead();
                        str2 = "u";
                        flag2 = true;
                        c = (char)base.ReaderPeek();
                        break;
                }
                if ((c == 'l') || (c == 'L'))
                {
                    base.ReaderRead();
                    c = (char)base.ReaderPeek();
                    flag3 = true;
                    if (!flag2 && ((c == 'u') || (c == 'U')))
                    {
                        base.ReaderRead();
                        str2 = "lu";
                        flag2 = true;
                    }
                    else
                    {
                        str2 = flag2 ? "ul" : "l";
                    }
                }
            }
            string s = base.sb.ToString();
            string val = str + s + str2;
            if (flag4)
            {
                try
                {
                    return new Token(2, x, line, val, float.Parse(s, CultureInfo.InvariantCulture));
                }
                catch (Exception)
                {
                    base.errors.Error(line, x, string.Format("Can't parse float {0}", s));
                    return new Token(2, x, line, val, 0f);
                }
            }
            if (flag6)
            {
                try
                {
                    return new Token(2, x, line, val, decimal.Parse(s, NumberStyles.Any, CultureInfo.InvariantCulture));
                }
                catch (Exception)
                {
                    base.errors.Error(line, x, string.Format("Can't parse decimal {0}", s));
                    return new Token(2, x, line, val, 0M);
                }
            }
            if (flag5)
            {
                try
                {
                    return new Token(2, x, line, val, double.Parse(s, CultureInfo.InvariantCulture));
                }
                catch (Exception)
                {
                    base.errors.Error(line, x, string.Format("Can't parse double {0}", s));
                    return new Token(2, x, line, val, 0.0);
                }
            }
            double result = 0.0;
            if (flag)
            {
                result = ulong.Parse(s, NumberStyles.HexNumber);
            }
            else if (!double.TryParse(s, NumberStyles.Integer, (IFormatProvider)null, out result))
            {
                base.errors.Error(line, x, string.Format("Can't parse integral constant {0}", s));
                return new Token(2, x, line, val.ToString(), 0);
            }
            if ((result < -9.2233720368547758E+18) || (result > 9.2233720368547758E+18))
            {
                flag3 = true;
                flag2 = true;
            }
            else if ((result < 0.0) || (result > 4294967295))
            {
                flag3 = true;
            }
            else if ((result < -2147483648.0) || (result > 2147483647.0))
            {
                flag2 = true;
            }
            if (flag3)
            {
                if (flag2)
                {
                    try
                    {
                        token2 = new Token(2, x, line, val, ulong.Parse(s, flag ? NumberStyles.HexNumber : NumberStyles.Number));
                    }
                    catch (Exception)
                    {
                        base.errors.Error(line, x, string.Format("Can't parse unsigned long {0}", s));
                        token2 = new Token(2, x, line, val, (ulong)0L);
                    }
                }
                else
                {
                    try
                    {
                        token2 = new Token(2, x, line, val, long.Parse(s, flag ? NumberStyles.HexNumber : NumberStyles.Number));
                    }
                    catch (Exception)
                    {
                        base.errors.Error(line, x, string.Format("Can't parse long {0}", s));
                        token2 = new Token(2, x, line, val, 0L);
                    }
                }
            }
            else if (flag2)
            {
                try
                {
                    token2 = new Token(2, x, line, val, uint.Parse(s, flag ? NumberStyles.HexNumber : NumberStyles.Number));
                }
                catch (Exception)
                {
                    base.errors.Error(line, x, string.Format("Can't parse unsigned int {0}", s));
                    token2 = new Token(2, x, line, val, 0);
                }
            }
            else
            {
                try
                {
                    token2 = new Token(2, x, line, val, int.Parse(s, flag ? NumberStyles.HexNumber : NumberStyles.Number));
                }
                catch (Exception)
                {
                    base.errors.Error(line, x, string.Format("Can't parse int {0}", s));
                    token2 = new Token(2, x, line, val, 0);
                }
            }
            token2.next = token;
            return token2;
        }

        private string ReadEscapeSequence(out char ch)
        {
            int hexNumber;
            int num = base.ReaderRead();
            if (num == -1)
            {
                base.errors.Error(base.Line, base.Col, string.Format("End of file reached inside escape sequence", new object[0]));
                ch = '\0';
                return string.Empty;
            }
            char digit = (char)num;
            int length = 1;
            this.escapeSequenceBuffer[0] = digit;
            switch (digit)
            {
                case '"':
                    ch = '"';
                    goto Label_0201;

                case '\'':
                    ch = '\'';
                    goto Label_0201;

                case '0':
                    ch = '\0';
                    goto Label_0201;

                case 'n':
                    ch = '\n';
                    goto Label_0201;

                case 'r':
                    ch = '\r';
                    goto Label_0201;

                case 't':
                    ch = '\t';
                    goto Label_0201;

                case 'u':
                case 'x':
                    digit = (char)base.ReaderRead();
                    hexNumber = base.GetHexNumber(digit);
                    this.escapeSequenceBuffer[length++] = digit;
                    if (hexNumber < 0)
                    {
                        base.errors.Error(base.Line, base.Col - 1, string.Format("Invalid char in literal : {0}", digit));
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        if (!base.IsHex((char)base.ReaderPeek()))
                        {
                            break;
                        }
                        digit = (char)base.ReaderRead();
                        int num5 = base.GetHexNumber(digit);
                        this.escapeSequenceBuffer[length++] = digit;
                        hexNumber = (0x10 * hexNumber) + num5;
                    }
                    break;

                case 'v':
                    ch = '\v';
                    goto Label_0201;

                case 'f':
                    ch = '\f';
                    goto Label_0201;

                case 'a':
                    ch = '\a';
                    goto Label_0201;

                case 'b':
                    ch = '\b';
                    goto Label_0201;

                case '\\':
                    ch = '\\';
                    goto Label_0201;

                default:
                    base.errors.Error(base.Line, base.Col, string.Format("Unexpected escape sequence : {0}", digit));
                    ch = '\0';
                    goto Label_0201;
            }
            ch = (char)hexNumber;
        Label_0201:
            return new string(this.escapeSequenceBuffer, 0, length);
        }

        private string ReadIdent(char ch)
        {
            int num;
            int length = 1;
            this.identBuffer[0] = ch;
            while (base.IsIdentifierPart(num = base.ReaderPeek()))
            {
                base.ReaderRead();
                if (length < 0x200)
                {
                    this.identBuffer[length++] = (char)num;
                }
                else
                {
                    base.errors.Error(base.Line, base.Col, string.Format("Identifier too long", new object[0]));
                    while (base.IsIdentifierPart(base.ReaderPeek()))
                    {
                        base.ReaderRead();
                    }
                    break;
                }
            }
            return new string(this.identBuffer, 0, length);
        }

        private void ReadMultiLineComment()
        {
            int num;
            if (base.skipAllComments)
            {
                while ((num = base.ReaderRead()) != -1)
                {
                    char ch = (char)num;
                    if ((ch == '*') && (base.ReaderPeek() == 0x2f))
                    {
                        base.ReaderRead();
                        return;
                    }
                }
            }
            else
            {
                base.specialTracker.StartComment(CommentType.Block, new Point(base.Col, base.Line));
                while ((num = base.ReaderRead()) != -1)
                {
                    char ch2 = (char)num;
                    if (base.HandleLineEnd(ch2))
                    {
                        base.specialTracker.AddChar('\n');
                    }
                    else
                    {
                        if ((ch2 == '*') && (base.ReaderPeek() == 0x2f))
                        {
                            base.ReaderRead();
                            base.specialTracker.FinishComment(new Point(base.Col, base.Line));
                            return;
                        }
                        base.specialTracker.AddChar(ch2);
                    }
                }
                base.specialTracker.FinishComment(new Point(base.Col, base.Line));
            }
            base.errors.Error(base.Line, base.Col, string.Format("Reached EOF before the end of a multiline comment", new object[0]));
        }

        private Token ReadOperator(char ch)
        {
            int col = base.Col - 1;
            int line = base.Line;
            switch (ch)
            {
                case '!':
                    if (base.ReaderPeek() != 0x3d)
                    {
                        return new Token(0x18, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(0x22, col, line, ch);

                case '%':
                    if (base.ReaderPeek() != 0x3d)
                    {
                        return new Token(8, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(0x2a, col, line, ch);

                case '&':
                    switch (base.ReaderPeek())
                    {
                        case 0x26:
                            base.ReaderRead();
                            return new Token(0x19, col, line, ch);

                        case 0x3d:
                            base.ReaderRead();
                            return new Token(0x2b, col, line, ch);
                    }
                    return new Token(0x1c, col, line, ch);

                case '(':
                    return new Token(20, col, line, ch);

                case ')':
                    return new Token(0x15, col, line, ch);

                case '*':
                    if (base.ReaderPeek() != 0x3d)
                    {
                        return new Token(6, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(40, col, line, ch);

                case '+':
                    switch (base.ReaderPeek())
                    {
                        case 0x2b:
                            base.ReaderRead();
                            return new Token(0x1f, col, line, ch);

                        case 0x3d:
                            base.ReaderRead();
                            return new Token(0x26, col, line, ch);
                    }
                    return new Token(4, col, line, ch);

                case ',':
                    return new Token(14, col, line, ch);

                case '-':
                    switch (base.ReaderPeek())
                    {
                        case 0x3d:
                            base.ReaderRead();
                            return new Token(0x27, col, line, ch);

                        case 0x3e:
                            base.ReaderRead();
                            return new Token(0x2f, col, line, ch);

                        case 0x2d:
                            base.ReaderRead();
                            return new Token(0x20, col, line, ch);
                    }
                    return new Token(5, col, line, ch);

                case '.':
                    {
                        int num3 = base.ReaderPeek();
                        if ((num3 > 0) && char.IsDigit((char)num3))
                        {
                            return this.ReadDigit('.', base.Col - 1);
                        }
                        return new Token(15, col, line, ch);
                    }
                case '/':
                    if (base.ReaderPeek() != 0x3d)
                    {
                        return new Token(7, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(0x29, col, line, ch);

                case ':':
                    if (base.ReaderPeek() != 0x3a)
                    {
                        return new Token(9, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(10, col, line, ch);

                case ';':
                    return new Token(11, col, line, ch);

                case '<':
                    switch (base.ReaderPeek())
                    {
                        case 60:
                            base.ReaderRead();
                            if (base.ReaderPeek() == 0x3d)
                            {
                                base.ReaderRead();
                                return new Token(0x2e, col, line, ch);
                            }
                            return new Token(0x25, col, line, ch);

                        case 0x3d:
                            base.ReaderRead();
                            return new Token(0x24, col, line, ch);
                    }
                    return new Token(0x17, col, line, ch);

                case '=':
                    if (base.ReaderPeek() != 0x3d)
                    {
                        return new Token(3, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(0x21, col, line, ch);

                case '>':
                    if (base.ReaderPeek() != 0x3d)
                    {
                        return new Token(0x16, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(0x23, col, line, ch);

                case '?':
                    if (base.ReaderPeek() != 0x3f)
                    {
                        return new Token(12, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(13, col, line, ch);

                case '[':
                    return new Token(0x12, col, line, ch);

                case ']':
                    return new Token(0x13, col, line, ch);

                case '^':
                    if (base.ReaderPeek() != 0x3d)
                    {
                        return new Token(30, col, line, ch);
                    }
                    base.ReaderRead();
                    return new Token(0x2d, col, line, ch);

                case '{':
                    return new Token(0x10, col, line, ch);

                case '|':
                    {
                        int num10 = base.ReaderPeek();
                        if (num10 == 0x3d)
                        {
                            base.ReaderRead();
                            return new Token(0x2c, col, line, ch);
                        }
                        if (num10 != 0x7c)
                        {
                            return new Token(0x1d, col, line, ch);
                        }
                        base.ReaderRead();
                        return new Token(0x1a, col, line, ch);
                    }
                case '}':
                    return new Token(0x11, col, line, ch);

                case '~':
                    return new Token(0x1b, col, line, ch);
            }
            return null;
        }

        private void ReadSingleLineComment(CommentType commentType)
        {
            if (base.skipAllComments)
            {
                base.SkipToEOL();
            }
            else
            {
                base.specialTracker.StartComment(commentType, new Point(base.Col, base.Line));
                base.specialTracker.AddString(this.ReadCommentToEOL());
                base.specialTracker.FinishComment(new Point(base.Col, base.Line));
            }
        }

        private Token ReadString()
        {
            int num3;
            int col = base.Col - 1;
            int line = base.Line;
            base.sb.Length = 0;
            base.originalValue.Length = 0;
            base.originalValue.Append('"');
            bool flag = false, isMultiLineString = false;
            while ((num3 = base.ReaderRead()) != -1)
            {
                char ch = (char)num3;
                if (ch == '"')
                {
                    flag = true;
                    base.originalValue.Append('"');
                    break;
                }
                if (ch == '\\')
                {
                    int chNext = base.ReaderPeek();
                    if (chNext == '\r' || chNext == '\n')
                    {
                        isMultiLineString = true;
                        base.originalValue.Append(ch);
                        base.sb.Append(ch);
                    }
                    else
                    {
                        base.originalValue.Append('\\');
                        base.originalValue.Append(this.ReadEscapeSequence(out ch));
                        base.sb.Append(ch);
                    }
                }
                else if (isMultiLineString)
                {
                    base.originalValue.Append(ch);
                    base.sb.Append(ch);
                    if (ch == '\n')
                    {
                        line++;
                        isMultiLineString = false;
                    }
                }
                else
                {
                    if (ch == '\n')
                    {
                        base.errors.Error(line, col, string.Format("No new line is allowed inside a string literal", new object[0]));
                        break;
                    }
                    base.originalValue.Append(ch);
                    base.sb.Append(ch);
                }
            }
            if (!flag)
            {
                base.errors.Error(line, col, string.Format("End of file reached inside string literal", new object[0]));
            }
            return new Token(2, col, line, base.originalValue.ToString(), base.sb.ToString());
        }

        private Token ReadVerbatimString()
        {
            int num3;
            base.sb.Length = 0;
            base.originalValue.Length = 0;
            base.originalValue.Append("@\"");
            int col = base.Col - 2;
            int line = base.Line;
            while ((num3 = base.ReaderRead()) != -1)
            {
                char ch = (char)num3;
                if (ch == '"')
                {
                    if (base.ReaderPeek() != 0x22)
                    {
                        base.originalValue.Append('"');
                        break;
                    }
                    base.originalValue.Append("\"\"");
                    base.sb.Append('"');
                    base.ReaderRead();
                }
                else if (base.HandleLineEnd(ch))
                {
                    base.sb.Append("\r\n");
                    base.originalValue.Append("\r\n");
                }
                else
                {
                    base.sb.Append(ch);
                    base.originalValue.Append(ch);
                }
            }
            if (num3 == -1)
            {
                base.errors.Error(line, col, string.Format("End of file reached inside verbatim string literal", new object[0]));
            }
            return new Token(2, col, line, base.originalValue.ToString(), base.sb.ToString());
        }

        public override void SkipCurrentBlock()
        {
            int chVal;
            int num = 0;
            while (base.curToken != null)
            {
                if (base.curToken.kind == 0x10)
                {
                    num++;
                }
                else if ((base.curToken.kind == 0x11) && (--num < 0))
                {
                    return;
                }
                base.lastToken = base.curToken;
                base.curToken = base.curToken.next;
            }
            while ((chVal = base.ReaderRead()) != -1)
            {
                switch (chVal)
                {
                    case 0x22:
                        this.ReadString();
                        break;

                    case 0x23:
                        base.SkipToEOL();
                        break;

                    case 13:
                    case 10:
                        base.HandleLineEnd((char)chVal);
                        break;

                    case 0x7b:
                        num++;
                        break;

                    case 0x7d:
                        if (--num >= 0)
                        {
                            break;
                        }
                        base.curToken = new Token(0x11, base.Col, base.Line, (char)num);
                        return;

                    case 0x40:
                        switch (base.ReaderRead())
                        {
                            case -1:
                                base.errors.Error(base.Line, base.Col, string.Format("EOF after @", new object[0]));
                                break;

                            case 0x22:
                                this.ReadVerbatimString();
                                break;
                        }
                        break;

                    case 0x27:
                        this.ReadChar();
                        break;

                    case 0x2f:
                        switch (base.ReaderPeek())
                        {
                            case 0x2f:
                            case 0x2a:
                                this.ReadComment();
                                break;
                        }
                        break;
                }
            }
            base.curToken = new Token(0, base.Col, base.Line, (char)chVal);
        }
    }
}
