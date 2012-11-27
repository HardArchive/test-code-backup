namespace LZ.Lexer
{
    using System;
    using System.Drawing;

    public interface ISpecial
    {
        //object AcceptVisitor(ISpecialVisitor visitor, object data);

        Point EndPosition { get; }

        Point StartPosition { get; }
    }
}
