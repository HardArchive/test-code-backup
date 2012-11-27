namespace LZ.Lexer
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.IO;
    using System.Text;

    public abstract class AbstractLexer : ILexer, IDisposable
    {
        private int col = 1;
        protected LZ.Lexer.Token curToken;
        protected LZ.Lexer.Errors errors = new LZ.Lexer.Errors();
        protected LZ.Lexer.Token lastToken;
        private int line = 1;
        protected StringBuilder originalValue = new StringBuilder();
        protected LZ.Lexer.Token peekToken;
        private TextReader reader;
        protected StringBuilder sb = new StringBuilder();
        protected bool skipAllComments;
        protected Hashtable specialCommentHash;
        private string[] specialCommentTags;
        protected LZ.Lexer.SpecialTracker specialTracker = new LZ.Lexer.SpecialTracker();
        protected List<TagComment> tagComments = new List<TagComment>();

        public AbstractLexer(TextReader reader)
        {
            this.reader = reader;
        }

        public virtual void Dispose()
        {
            this.reader.Close();
            this.reader = null;
            this.errors = null;
            this.lastToken = this.curToken = (LZ.Lexer.Token) (this.peekToken = null);
            this.specialCommentHash = null;
            this.tagComments = null;
            this.sb = (StringBuilder) (this.originalValue = null);
        }

        protected int GetHexNumber(char digit)
        {
            if (char.IsDigit(digit))
            {
                return (digit - '0');
            }
            if (('A' <= digit) && (digit <= 'F'))
            {
                return ((digit - 'A') + 10);
            }
            if (('a' <= digit) && (digit <= 'f'))
            {
                return ((digit - 'a') + 10);
            }
            this.errors.Error(this.line, this.col, string.Format("Invalid hex number '" + digit + "'", new object[0]));
            return 0;
        }

        protected bool HandleLineEnd(char ch)
        {
            if (this.WasLineEnd(ch))
            {
                this.line++;
                this.col = 1;
                return true;
            }
            return false;
        }

        protected bool IsHex(char digit)
        {
            return ((char.IsDigit(digit) || (('A' <= digit) && (digit <= 'F'))) || (('a' <= digit) && (digit <= 'f')));
        }

        protected bool IsIdentifierPart(int ch)
        {
            if (ch == 0x5f)
            {
                return true;
            }
            if (ch == -1)
            {
                return false;
            }
            return char.IsLetterOrDigit((char) ch);
        }

        protected abstract LZ.Lexer.Token Next();
        public virtual LZ.Lexer.Token NextToken()
        {
            if (this.curToken == null)
            {
                this.curToken = this.Next();
                this.specialTracker.InformToken(this.curToken.kind);
                return this.curToken;
            }
            this.lastToken = this.curToken;
            if (this.curToken.next == null)
            {
                this.curToken.next = this.Next();
                if (this.curToken.next != null)
                {
                    this.specialTracker.InformToken(this.curToken.next.kind);
                }
            }
            this.curToken = this.curToken.next;
            return this.curToken;
        }

        public LZ.Lexer.Token Peek()
        {
            if (this.peekToken.next == null)
            {
                this.peekToken.next = this.Next();
                this.specialTracker.InformToken(this.peekToken.next.kind);
            }
            this.peekToken = this.peekToken.next;
            return this.peekToken;
        }

        protected int ReaderPeek()
        {
            return this.reader.Peek();
        }

        protected int ReaderRead()
        {
            this.col++;
            return this.reader.Read();
        }

        protected string ReadToEOL()
        {
            int num;
            this.sb.Length = 0;
            while ((num = this.reader.Read()) != -1)
            {
                char ch = (char) num;
                if (this.HandleLineEnd(ch))
                {
                    return this.sb.ToString();
                }
                this.sb.Append(ch);
            }
            string str = this.sb.ToString();
            this.col += str.Length;
            return str;
        }

        public virtual void SkipCurrentBlock()
        {
            throw new NotSupportedException();
        }

        protected void SkipToEOL()
        {
            int num;
            while ((num = this.reader.Read()) != -1)
            {
                if (this.HandleLineEnd((char) num))
                {
                    return;
                }
            }
        }

        public void StartPeek()
        {
            this.peekToken = this.curToken;
        }

        protected bool WasLineEnd(char ch)
        {
            if (ch == '\r')
            {
                if (this.reader.Peek() == 10)
                {
                    ch = (char) this.reader.Read();
                    this.col++;
                }
                else
                {
                    ch = '\n';
                }
            }
            return (ch == '\n');
        }

        protected int Col
        {
            get
            {
                return this.col;
            }
        }

        public LZ.Lexer.Errors Errors
        {
            get
            {
                return this.errors;
            }
        }

        protected int Line
        {
            get
            {
                return this.line;
            }
        }

        public LZ.Lexer.Token LookAhead
        {
            get
            {
                return this.curToken;
            }
        }

        public bool SkipAllComments
        {
            get
            {
                return this.skipAllComments;
            }
            set
            {
                this.skipAllComments = value;
            }
        }

        public string[] SpecialCommentTags
        {
            get
            {
                return this.specialCommentTags;
            }
            set
            {
                this.specialCommentTags = value;
                this.specialCommentHash = null;
                if ((this.specialCommentTags != null) && (this.specialCommentTags.Length > 0))
                {
                    this.specialCommentHash = new Hashtable();
                    foreach (string str in this.specialCommentTags)
                    {
                        this.specialCommentHash.Add(str, null);
                    }
                }
            }
        }

        public LZ.Lexer.SpecialTracker SpecialTracker
        {
            get
            {
                return this.specialTracker;
            }
        }

        public List<TagComment> TagComments
        {
            get
            {
                return this.tagComments;
            }
        }

        public LZ.Lexer.Token Token
        {
            get
            {
                return this.lastToken;
            }
        }
    }
}
