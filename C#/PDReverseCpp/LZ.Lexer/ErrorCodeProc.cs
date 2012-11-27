namespace LZ.Lexer
{
    using System;
    using System.Runtime.CompilerServices;

    public delegate void ErrorCodeProc(int line, int col, int n);
}
