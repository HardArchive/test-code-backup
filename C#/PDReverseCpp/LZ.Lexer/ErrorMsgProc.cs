namespace LZ.Lexer
{
    using System;
    using System.Runtime.CompilerServices;

    public delegate void ErrorMsgProc(int line, int col, string msg);
}
