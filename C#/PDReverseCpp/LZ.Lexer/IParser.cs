namespace LZ.Lexer
{
    //using ICSharpCode.NRefactory.Parser.AST;
    using System;

    public interface IParser : IDisposable
    {
        void Parse();
        Expression ParseExpression();

        ICSharpCode.NRefactory.Parser.AST.CompilationUnit CompilationUnit { get; }

        ICSharpCode.NRefactory.Parser.Errors Errors { get; }

        ILexer Lexer { get; }

        bool ParseMethodBodies { get; set; }
    }
}
