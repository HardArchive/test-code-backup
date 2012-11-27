namespace LZ.Lexer
{
    using System;
    using System.Collections.Generic;

    public interface ILexerEx : ILexer, IDisposable
    {
        List<string> IncludeFiles { get; }
    }
}
