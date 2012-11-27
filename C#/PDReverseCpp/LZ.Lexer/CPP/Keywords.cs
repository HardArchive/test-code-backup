namespace LZ.Lexer.CPP
{
    using System;

    public class Keywords
    {
        private static readonly string[] keywordList = new string[] { 
            "abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue", "decimal", "default", "delegate", 
            "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto", "if", "implicit", 
            "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", 
            "protected", "public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct", "switch", "this", "throw", 
            "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void", "volatile", "while"
         };
        private static LookupTable keywords = new LookupTable(true);

        static Keywords()
        {
            for (int i = 0; i < keywordList.Length; i++)
            {
                keywords[keywordList[i]] = i + 0x30;
            }
        }

        public static int GetToken(string keyword)
        {
            return keywords[keyword];
        }
    }
}
