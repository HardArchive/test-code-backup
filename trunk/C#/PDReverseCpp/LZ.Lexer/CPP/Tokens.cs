namespace LZ.Lexer.CPP
{
    using System;
    using System.Collections;

    public sealed class Tokens
    {
        public const int Abstract = 0x30;
        public const int As = 0x31;
        public static BitArray AssgnOps = NewSet(new int[] { 3, 0x26, 0x27, 40, 0x29, 0x2a, 0x2b, 0x2c, 0x2e });
        public const int Assign = 3;
        public static BitArray AssnStartOp = NewSet(new int[] { 4, 5, 0x18, 0x1b, 6 });
        public const int Base = 50;
        public const int BitwiseAnd = 0x1c;
        public const int BitwiseAndAssign = 0x2b;
        public const int BitwiseComplement = 0x1b;
        public const int BitwiseOr = 0x1d;
        public const int BitwiseOrAssign = 0x2c;
        public const int Bool = 0x33;
        public const int Break = 0x34;
        public const int Byte = 0x35;
        public const int Case = 0x36;
        public static BitArray CastFollower = NewSet(new int[] { 
            1, 2, 20, 0x58, 110, 50, 0x59, 0x39, 0x75, 0x72, 0x68, 0x3f, 4, 0x18, 0x1b, 0x1f, 
            0x20, 0x70, 0x47
         });
        public const int Catch = 0x37;
        public const int Char = 0x38;
        public const int Checked = 0x39;
        public const int Class = 0x3a;
        public const int CloseCurlyBrace = 0x11;
        public const int CloseParenthesis = 0x15;
        public const int CloseSquareBracket = 0x13;
        public const int Colon = 9;
        public const int Comma = 14;
        public const int Const = 0x3b;
        public const int Continue = 60;
        public const int Decimal = 0x3d;
        public const int Decrement = 0x20;
        public const int Default = 0x3e;
        public const int Delegate = 0x3f;
        public const int Div = 7;
        public const int DivAssign = 0x29;
        public const int Do = 0x40;
        public const int Dot = 15;
        public const int Double = 0x41;
        public const int DoubleColon = 10;
        public const int DoubleQuestion = 13;
        public const int Else = 0x42;
        public const int Enum = 0x43;
        public const int EOF = 0;
        public const int Equal = 0x21;
        public const int Event = 0x44;
        public const int Explicit = 0x45;
        public const int Extern = 70;
        public const int False = 0x47;
        public const int Finally = 0x48;
        public const int Fixed = 0x49;
        public const int Float = 0x4a;
        public const int For = 0x4b;
        public const int Foreach = 0x4c;
        public const int Goto = 0x4d;
        public const int GreaterEqual = 0x23;
        public const int GreaterThan = 0x16;
        public const int Identifier = 1;
        public const int If = 0x4e;
        public const int Implicit = 0x4f;
        public const int In = 80;
        public const int Increment = 0x1f;
        public const int Int = 0x51;
        public const int Interface = 0x52;
        public const int Internal = 0x53;
        public const int Is = 0x54;
        public const int LessEqual = 0x24;
        public const int LessThan = 0x17;
        public const int Literal = 2;
        public const int Lock = 0x55;
        public const int LogicalAnd = 0x19;
        public const int LogicalOr = 0x1a;
        public const int Long = 0x56;
        public const int maxToken = 0x7d;
        public const int Minus = 5;
        public const int MinusAssign = 0x27;
        public const int Mod = 8;
        public const int ModAssign = 0x2a;
        public const int Namespace = 0x57;
        public const int New = 0x58;
        public const int Not = 0x18;
        public const int NotEqual = 0x22;
        public const int Null = 0x59;
        public const int Object = 90;
        public const int OpenCurlyBrace = 0x10;
        public const int OpenParenthesis = 20;
        public const int OpenSquareBracket = 0x12;
        public const int Operator = 0x5b;
        public const int Out = 0x5c;
        public static BitArray OverloadableBinaryOp = NewSet(new int[] { 4, 5, 6, 7, 8, 0x1c, 0x1d, 30, 0x25, 0x21, 0x22, 0x16, 0x17, 0x23, 0x24 });
        public static BitArray OverloadableUnaryOp = NewSet(new int[] { 4, 0x18, 0x1b, 0x1f, 0x20, 0x70, 0x47 });
        public const int Override = 0x5d;
        public const int Params = 0x5e;
        public const int Plus = 4;
        public const int PlusAssign = 0x26;
        public const int Pointer = 0x2f;
        public const int Private = 0x5f;
        public const int Protected = 0x60;
        public const int Public = 0x61;
        public const int Question = 12;
        public const int Readonly = 0x62;
        public const int Ref = 0x63;
        public const int Return = 100;
        public const int Sbyte = 0x65;
        public const int Sealed = 0x66;
        public const int Semicolon = 11;
        public const int ShiftLeft = 0x25;
        public const int ShiftLeftAssign = 0x2e;
        public const int Short = 0x67;
        public const int Sizeof = 0x68;
        public const int Stackalloc = 0x69;
        public const int Static = 0x6a;
        public const int String = 0x6b;
        public const int Struct = 0x6c;
        public const int Switch = 0x6d;
        public const int This = 110;
        public const int Throw = 0x6f;
        public const int Times = 6;
        public const int TimesAssign = 40;
        private static string[] tokenList = new string[] { 
            "<EOF>", "<Identifier>", "<Literal>", "=", "+", "-", "*", "/", "%", ":", "::", ";", "?", "??", ",", ".", 
            "{", "}", "[", "]", "(", ")", ">", "<", "!", "&&", "||", "~", "&", "|", "^", "++", 
            "--", "==", "!=", ">=", "<=", "<<", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", "->", 
            "abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue", "decimal", "default", "delegate", 
            "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto", "if", "implicit", 
            "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", 
            "protected", "public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct", "switch", "this", "throw", 
            "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void", "volatile", "while"
         };
        public const int True = 0x70;
        public const int Try = 0x71;
        public static BitArray TypeDeclarationKW = NewSet(new int[] { 0x3a, 0x52, 0x6c, 0x43, 0x3f });
        public static BitArray TypeKW = NewSet(new int[] { 0x38, 0x33, 90, 0x6b, 0x65, 0x35, 0x67, 0x77, 0x51, 0x73, 0x56, 0x74, 0x4a, 0x41, 0x3d });
        public const int Typeof = 0x72;
        public const int Uint = 0x73;
        public const int Ulong = 0x74;
        public static BitArray UnaryHead = NewSet(new int[] { 4, 5, 0x18, 0x1b, 6, 0x1f, 0x20, 0x1c });
        public static BitArray UnaryOp = NewSet(new int[] { 4, 5, 0x18, 0x1b, 6, 0x1f, 0x20, 0x1c });
        public const int Unchecked = 0x75;
        public const int Unsafe = 0x76;
        public const int Ushort = 0x77;
        public const int Using = 120;
        public const int Virtual = 0x79;
        public const int Void = 0x7a;
        public const int Volatile = 0x7b;
        public const int While = 0x7c;
        public const int Xor = 30;
        public const int XorAssign = 0x2d;

        public static string GetTokenString(int token)
        {
            if ((token < 0) || (token >= tokenList.Length))
            {
                throw new NotSupportedException("Unknown token:" + token);
            }
            return tokenList[token];
        }

        private static BitArray NewSet(params int[] values)
        {
            BitArray array = new BitArray(0x7d);
            foreach (int num in values)
            {
                array[num] = true;
            }
            return array;
        }
    }
}
