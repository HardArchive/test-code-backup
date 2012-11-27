namespace LZ.Lexer
{
    using System;
    using System.Drawing;

    public abstract class AbstractSpecial : ISpecial
    {
        private Point endPosition;
        private Point startPosition;

        public AbstractSpecial(Point position)
        {
            this.startPosition = position;
            this.endPosition = position;
        }

        public AbstractSpecial(Point startPosition, Point endPosition)
        {
            this.startPosition = startPosition;
            this.endPosition = endPosition;
        }

        ////public abstract object AcceptVisitor(ISpecialVisitor visitor, object data);
        public override string ToString()
        {
            return string.Format("[{0}: Start = {1}, End = {2}]", base.GetType().Name, this.StartPosition, this.EndPosition);
        }

        public Point EndPosition
        {
            get
            {
                return this.endPosition;
            }
            set
            {
                this.endPosition = value;
            }
        }

        public Point StartPosition
        {
            get
            {
                return this.startPosition;
            }
            set
            {
                this.startPosition = value;
            }
        }
    }
}
