namespace LZ.Lexer
{
    using System;
    using System.Drawing;

    public class Comment : AbstractSpecial
    {
        private string comment;
        private LZ.Lexer.CommentType commentType;

        public Comment(LZ.Lexer.CommentType commentType, string comment, Point startPosition, Point endPosition) : base(startPosition, endPosition)
        {
            this.commentType = commentType;
            this.comment = comment;
        }

        //public override object AcceptVisitor(ISpecialVisitor visitor, object data)
        //{
        //    return visitor.Visit(this, data);
        //}

        public override string ToString()
        {
            return string.Format("[{0}: Type = {1}, Text = {2}, Start = {3}, End = {4}]", new object[] { base.GetType().Name, this.CommentType, this.CommentText, base.StartPosition, base.EndPosition });
        }

        public string CommentText
        {
            get
            {
                return this.comment;
            }
            set
            {
                this.comment = value;
            }
        }

        public LZ.Lexer.CommentType CommentType
        {
            get
            {
                return this.commentType;
            }
            set
            {
                this.commentType = value;
            }
        }
    }
}
