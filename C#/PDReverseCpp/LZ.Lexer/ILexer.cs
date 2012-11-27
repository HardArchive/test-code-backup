namespace LZ.Lexer
{
    using System;
    using System.Collections.Generic;

    public interface ILexer : IDisposable
    {
        LZ.Lexer.Token NextToken();
        LZ.Lexer.Token Peek();
        void SkipCurrentBlock();
        void StartPeek();

        LZ.Lexer.Errors Errors { get; }

        LZ.Lexer.Token LookAhead { get; }

        bool SkipAllComments { get; set; }

        string[] SpecialCommentTags { get; set; }

        LZ.Lexer.SpecialTracker SpecialTracker { get; }

        List<TagComment> TagComments { get; }

        LZ.Lexer.Token Token { get; }
    }
}
