namespace LZ.Lexer
{
    using System;
    using System.Drawing;

    public class Token
    {
        public int col;
        public int kind;
        public int line;
        public object literalValue;
        public Token next;
        public string val;

        ////public Token(int kind) : this(kind, 0, 0)
        ////{
        ////}

        ////public Token(int kind, int col, int line) : this(kind, col, line, null)
        ////{
        ////}

        public Token(int kind, int col, int line, char ch)
            : this(kind, col, line, string.Concat(ch), null)
        {
        }

        public Token(int kind, int col, int line, string val) : this(kind, col, line, val, null)
        {
        }

        public Token(int kind, int col, int line, string val, object literalValue)
        {
            this.kind = kind;
            this.col = col;
            this.line = line;
            this.val = val;
            this.literalValue = literalValue;
        }

        public Point EndLocation
        {
            get
            {
                return new Point((this.val == null) ? (this.col + 1) : (this.col + this.val.Length), this.line);
            }
        }

        public Point Location
        {
            get
            {
                return new Point(this.col, this.line);
            }
        }
    }
}
