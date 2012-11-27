namespace LZ.Lexer
{
    using System;
    using System.Drawing;

    public class BlankLine : AbstractSpecial
    {
        public BlankLine(Point point) : base(point)
        {
        }

        //public override object AcceptVisitor(ISpecialVisitor visitor, object data)
        //{
        //    return visitor.Visit(this, data);
        //}
    }
}
