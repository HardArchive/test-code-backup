namespace LZ.Lexer
{
    using System;
    using System.Drawing;

    public class TagComment : Comment
    {
        private string tag;

        public TagComment(string tag, string comment, Point startPosition, Point endPosition) : base(CommentType.SingleLine, comment, startPosition, endPosition)
        {
            this.tag = tag;
        }

        public string Tag
        {
            get
            {
                return this.tag;
            }
            set
            {
                this.tag = value;
            }
        }
    }
}
