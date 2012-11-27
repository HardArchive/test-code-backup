namespace LZ.Lexer
{
    using System;
    using System.Collections.Generic;
    using System.Drawing;
    using System.Text;

    public class SpecialTracker
    {
        private CommentType currentCommentType;
        private List<ISpecial> currentSpecials = new List<ISpecial>();
        private StringBuilder sb = new StringBuilder();
        private Point startPosition;

        public void AddChar(char c)
        {
            this.sb.Append(c);
        }

        public void AddEndOfLine(Point point)
        {
            this.currentSpecials.Add(new BlankLine(point));
        }

        public void AddPreProcessingDirective(string cmd, string arg, Point start, Point end)
        {
            this.currentSpecials.Add(new PreProcessingDirective(cmd, arg, start, end));
        }

        public void AddString(string s)
        {
            this.sb.Append(s);
        }

        public void FinishComment(Point endPosition)
        {
            this.currentSpecials.Add(new Comment(this.currentCommentType, this.sb.ToString(), this.startPosition, endPosition));
        }

        public void InformToken(int kind)
        {
        }

        public List<ISpecial> RetrieveSpecials()
        {
            List<ISpecial> currentSpecials = this.currentSpecials;
            this.currentSpecials = new List<ISpecial>();
            return currentSpecials;
        }

        public void StartComment(CommentType commentType, Point startPosition)
        {
            this.currentCommentType = commentType;
            this.startPosition = startPosition;
            this.sb.Length = 0;
        }

        public List<ISpecial> CurrentSpecials
        {
            get
            {
                return this.currentSpecials;
            }
        }
    }
}
