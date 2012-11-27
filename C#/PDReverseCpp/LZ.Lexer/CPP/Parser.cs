namespace ICSharpCode.NRefactory.Parser.CPP
{
    using ICSharpCode.NRefactory.Parser;
    using ICSharpCode.NRefactory.Parser.AST;
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Drawing;
    using System.Runtime.InteropServices;
    using System.Text;

    internal class Parser : AbstractParser
    {
        private string assemblyName;
        private const int maxT = 0x7d;
        private StringBuilder qualidentBuilder;
        private static bool[,] set = new bool[,] { 
            { 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                true, false, false, false, false, false, false, false, false, false, true, false, false, false, false, true, 
                false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, true, false, false, false, true, true, false, false, false, false, false, false, true, 
                true, true, false, false, false, false, true, false, false, false, true, false, true, false, false, false, 
                false, false, false, false, false, false, true, false, false, false, false, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                true, false, false, false, false, false, false, false, false, false, true, false, false, false, false, true, 
                false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, true, false, false, false, false, true, false, false, false, false, false, false, true, 
                true, true, false, false, false, false, true, false, false, false, true, false, true, false, false, false, 
                false, false, false, false, false, false, true, false, false, false, false, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, false, false, false, true, false, false, false, false, false, false, true, 
                true, true, false, false, false, false, true, false, false, false, true, false, false, false, false, false, 
                false, false, false, false, false, false, true, false, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, false, false, false, true, false, false, 
                false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, false, false, false, false, false
             }, { 
                false, true, true, false, true, true, true, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, true, false, false, false, true, false, false, true, true, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, true, false, true, false, false, true, true, false, false, false, true, false, true, 
                false, true, false, false, false, false, false, true, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, true, true, true, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, true, false, false, true, false, false, true, false, 
                true, false, true, true, true, true, false, true, false, false, false, false, false, false, false
             }, { 
                false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, true, true, true, true, true, true, false, true, true, true, true, false, 
                false, true, false, true, false, true, true, true, false, true, true, false, true, true, true, false, 
                false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, 
                false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, false, false, false, true, false, false, 
                false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, false, false, true, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, false, false, false, true, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, true, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, false, false, false, true, false, false, 
                false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, false, false, true, false, true, false, true, false, 
                false, false, false, true, false, true, false, true, false, false, false, true, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, true, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                true, false, false, true, false, true, false, false, true, false, true, true, false, true, false, true, 
                false, true, false, true, true, true, true, false, false, false, true, false, false, false, false, true, 
                false, true, true, true, false, false, true, false, true, false, true, false, false, true, false, true, 
                true, true, true, false, false, true, true, true, false, false, true, true, true, false, false, false, 
                false, false, false, true, true, false, true, true, false, true, true, true, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, true, 
                true, true, true, false, false, false, true, false, false, false, true, false, false, false, false, false, 
                false, false, false, false, false, false, true, false, false, true, false, true, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                true, false, false, true, false, true, false, false, true, false, true, true, false, true, false, true, 
                false, true, false, true, true, true, true, false, false, false, true, false, false, false, false, true, 
                false, true, true, true, false, false, true, false, true, false, true, false, false, true, false, true, 
                true, true, true, false, false, true, true, true, false, false, true, true, true, false, false, false, 
                false, false, false, true, true, false, true, true, false, true, true, true, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, false, false, false, true, false, false, 
                false, true, false, false, true, false, false, false, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, true, false, true, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, false, false, false, true, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, true, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, false, false, false, true, false, false, 
                false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, false, false, true, false, true, false, false, false, 
                false, false, false, true, false, true, false, true, false, false, false, true, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, true, false, false, false, false
             }, 
            { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, true, true, false, true, false, true, 
                false, true, false, true, true, true, false, false, false, false, true, false, false, false, false, true, 
                false, true, true, false, false, false, true, false, false, false, true, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, false, false, false, true, true, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, true, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, true, 
                false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, false, false, false, true, false, false, 
                false, true, false, false, true, false, false, false, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, false, false, true, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, false, false, false, true, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, true, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, true, false, false, true, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, false, false, false, false, false
             }, { 
                false, true, true, false, true, true, true, false, false, false, false, true, false, false, false, false, 
                true, false, false, false, true, false, false, false, true, false, false, true, true, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, true, true, true, false, false, true, true, false, true, true, true, false, true, 
                true, true, false, false, false, false, false, true, false, true, true, true, true, true, true, false, 
                false, true, false, false, false, true, true, false, true, true, true, false, false, false, false, false, 
                false, false, false, false, true, true, false, true, true, false, false, true, false, true, true, true, 
                true, true, true, true, true, true, true, true, true, false, false, false, true, false, false
             }, { 
                false, true, true, false, true, true, true, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, true, false, false, false, true, false, false, true, true, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, true, false, true, false, false, true, true, false, false, false, true, false, true, 
                false, true, false, false, false, false, false, true, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, true, true, true, false, true, false, false, false, 
                false, false, false, true, false, true, false, true, true, false, false, true, false, false, true, false, 
                true, false, true, true, true, true, false, true, false, false, false, false, false, false, false
             }, { 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, true, true, false, true, true, true, false, false, false, false, true, false, false, false, false, 
                true, false, false, false, true, false, false, false, true, false, false, true, true, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, true, true, true, false, false, true, true, false, false, true, true, false, true, 
                true, true, false, false, false, false, false, true, false, true, true, true, true, true, true, false, 
                false, true, false, false, false, true, true, false, true, true, true, false, false, false, false, false, 
                false, false, false, false, true, true, false, true, true, false, false, true, false, true, true, true, 
                true, true, true, true, true, true, true, true, true, false, false, false, true, false, false
             }, { 
                false, true, true, false, true, true, true, false, false, false, false, false, false, false, false, false, 
                true, false, false, false, true, false, false, false, true, false, false, true, true, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, true, true, false, true, false, false, true, true, false, false, false, true, true, true, 
                false, true, false, false, false, false, false, true, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, true, true, true, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, true, true, false, true, false, false, true, false, 
                true, false, true, true, true, true, false, true, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, true, false, false, true, true, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, true, false, true, false, false, true, false, false, false, false, true, false, false, 
                false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, false, 
                false, true, false, false, false, false, true, false, false, false, true, false, false, false, false, false, 
                false, false, false, false, false, true, false, true, false, false, false, true, false, false, false, false, 
                false, false, false, true, true, false, false, true, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, 
                false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, true, 
                true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, 
                false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, 
                false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }, { 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, 
                false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
             }
         };
        private const bool T = true;
        private const bool x = false;

        public Parser(ILexer lexer) : base(lexer)
        {
            this.qualidentBuilder = new StringBuilder();
        }

        private void AccessorDecls(out PropertyGetRegion getBlock, out PropertySetRegion setBlock)
        {
            ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            getBlock = null;
            setBlock = null;
            Modifiers m = null;
            while (this.la.kind == 0x12)
            {
                this.AttributeSection(out section);
                attributes.Add(section);
            }
            if (((this.la.kind == 0x53) || (this.la.kind == 0x5f)) || (this.la.kind == 0x60))
            {
                this.AccessorModifiers(out m);
            }
            if (this.IdentIsGet())
            {
                this.GetAccessorDecl(out getBlock, attributes);
                if (m != null)
                {
                    getBlock.Modifier = m.Modifier;
                }
                if (this.StartOf(0x16))
                {
                    attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
                    m = null;
                    while (this.la.kind == 0x12)
                    {
                        this.AttributeSection(out section);
                        attributes.Add(section);
                    }
                    if (((this.la.kind == 0x53) || (this.la.kind == 0x5f)) || (this.la.kind == 0x60))
                    {
                        this.AccessorModifiers(out m);
                    }
                    this.SetAccessorDecl(out setBlock, attributes);
                    if (m != null)
                    {
                        setBlock.Modifier = m.Modifier;
                    }
                }
            }
            else if (this.IdentIsSet())
            {
                this.SetAccessorDecl(out setBlock, attributes);
                if (m != null)
                {
                    setBlock.Modifier = m.Modifier;
                }
                if (this.StartOf(0x16))
                {
                    attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
                    m = null;
                    while (this.la.kind == 0x12)
                    {
                        this.AttributeSection(out section);
                        attributes.Add(section);
                    }
                    if (((this.la.kind == 0x53) || (this.la.kind == 0x5f)) || (this.la.kind == 0x60))
                    {
                        this.AccessorModifiers(out m);
                    }
                    this.GetAccessorDecl(out getBlock, attributes);
                    if (m != null)
                    {
                        getBlock.Modifier = m.Modifier;
                    }
                }
            }
            else if (this.la.kind == 1)
            {
                base.lexer.NextToken();
                this.Error("get or set accessor declaration expected");
            }
            else
            {
                base.SynErr(0xa2);
            }
        }

        private void AccessorModifiers(out Modifiers m)
        {
            m = new Modifiers();
            if (this.la.kind == 0x5f)
            {
                base.lexer.NextToken();
                m.Add(Modifier.Private, this.t.Location);
            }
            else if (this.la.kind == 0x60)
            {
                base.lexer.NextToken();
                m.Add(Modifier.Protected, this.t.Location);
                if (this.la.kind == 0x53)
                {
                    base.lexer.NextToken();
                    m.Add(Modifier.Internal, this.t.Location);
                }
            }
            else if (this.la.kind == 0x53)
            {
                base.lexer.NextToken();
                m.Add(Modifier.Internal, this.t.Location);
                if (this.la.kind == 0x60)
                {
                    base.lexer.NextToken();
                    m.Add(Modifier.Protected, this.t.Location);
                }
            }
            else
            {
                base.SynErr(0x9e);
            }
        }

        private void AddAccessorDecl(out ICSharpCode.NRefactory.Parser.AST.Statement stmt)
        {
            stmt = null;
            base.Expect(1);
            if (this.t.val != "add")
            {
                this.Error("add expected");
            }
            this.Block(out stmt);
        }

        private void AdditiveExpr(ref Expression outExpr)
        {
            BinaryOperatorType none = BinaryOperatorType.None;
            this.MultiplicativeExpr(ref outExpr);
            while ((this.la.kind == 4) || (this.la.kind == 5))
            {
                Expression expression;
                if (this.la.kind == 4)
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.Add;
                }
                else
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.Subtract;
                }
                this.UnaryExpr(out expression);
                this.MultiplicativeExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, none, expression);
            }
        }

        private void AndExpr(ref Expression outExpr)
        {
            this.EqualityExpr(ref outExpr);
            while (this.la.kind == 0x1c)
            {
                Expression expression;
                base.lexer.NextToken();
                this.UnaryExpr(out expression);
                this.EqualityExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, BinaryOperatorType.BitwiseAnd, expression);
            }
        }

        private void AnonymousMethodExpr(out Expression outExpr)
        {
            AnonymousMethodExpression expression = new AnonymousMethodExpression();
            expression.StartLocation = this.t.Location;
            List<ParameterDeclarationExpression> parameter = new List<ParameterDeclarationExpression>();
            outExpr = expression;
            if (this.la.kind == 20)
            {
                base.lexer.NextToken();
                if (this.StartOf(10))
                {
                    this.FormalParameterList(parameter);
                    expression.Parameters = parameter;
                }
                base.Expect(0x15);
            }
            if (base.compilationUnit != null)
            {
                ICSharpCode.NRefactory.Parser.AST.Statement statement;
                this.Block(out statement);
                expression.Body = (BlockStatement) statement;
            }
            else
            {
                base.Expect(0x10);
                base.lexer.SkipCurrentBlock();
                base.Expect(0x11);
            }
            expression.EndLocation = this.t.Location;
        }

        private void Argument(out Expression argumentexpr)
        {
            Expression expression = null;
            Expression expression2 = null;
            FieldDirection none = FieldDirection.None;
            if ((this.la.kind == 0x5c) || (this.la.kind == 0x63))
            {
                if (this.la.kind == 0x63)
                {
                    base.lexer.NextToken();
                    none = FieldDirection.Ref;
                }
                else
                {
                    base.lexer.NextToken();
                    none = FieldDirection.Out;
                }
            }
            this.Expr(out expression);
            argumentexpr = (none != FieldDirection.None) ? expression2 : expression;
        }

        private void ArrayInitializer(out Expression outExpr)
        {
            Expression initializerExpression = null;
            ArrayInitializerExpression expression2 = new ArrayInitializerExpression();
            base.Expect(0x10);
            if (this.StartOf(0x18))
            {
                this.VariableInitializer(out initializerExpression);
                if (initializerExpression != null)
                {
                    expression2.CreateExpressions.Add(initializerExpression);
                }
                while (this.NotFinalComma())
                {
                    base.Expect(14);
                    this.VariableInitializer(out initializerExpression);
                    if (initializerExpression != null)
                    {
                        expression2.CreateExpressions.Add(initializerExpression);
                    }
                }
                if (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                }
            }
            base.Expect(0x11);
            outExpr = expression2;
        }

        private void AssignmentOperator(out AssignmentOperatorType op)
        {
            op = AssignmentOperatorType.None;
            if (this.la.kind == 3)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.Assign;
            }
            else if (this.la.kind == 0x26)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.Add;
            }
            else if (this.la.kind == 0x27)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.Subtract;
            }
            else if (this.la.kind == 40)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.Multiply;
            }
            else if (this.la.kind == 0x29)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.Divide;
            }
            else if (this.la.kind == 0x2a)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.Modulus;
            }
            else if (this.la.kind == 0x2b)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.BitwiseAnd;
            }
            else if (this.la.kind == 0x2c)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.BitwiseOr;
            }
            else if (this.la.kind == 0x2d)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.ExclusiveOr;
            }
            else if (this.la.kind == 0x2e)
            {
                base.lexer.NextToken();
                op = AssignmentOperatorType.ShiftLeft;
            }
            else if ((this.la.kind == 0x16) && (this.Peek(1).kind == 0x23))
            {
                base.Expect(0x16);
                base.Expect(0x23);
                op = AssignmentOperatorType.ShiftRight;
            }
            else
            {
                base.SynErr(0xa9);
            }
        }

        private void Attribute(out ICSharpCode.NRefactory.Parser.AST.Attribute attribute)
        {
            string str;
            string val = null;
            if ((this.la.kind == 1) && (this.Peek(1).kind == 10))
            {
                base.lexer.NextToken();
                val = this.t.val;
                base.Expect(10);
            }
            this.Qualident(out str);
            List<Expression> positional = new List<Expression>();
            List<NamedArgumentExpression> named = new List<NamedArgumentExpression>();
            string name = ((val != null) && (val != "global")) ? (val + "." + str) : str;
            if (this.la.kind == 20)
            {
                this.AttributeArguments(positional, named);
            }
            attribute = new ICSharpCode.NRefactory.Parser.AST.Attribute(name, positional, named);
        }

        private void AttributeArguments(List<Expression> positional, List<NamedArgumentExpression> named)
        {
            bool flag = false;
            string name = "";
            base.Expect(20);
            if (this.StartOf(5))
            {
                Expression expression;
                if (this.IsAssignment())
                {
                    flag = true;
                    base.lexer.NextToken();
                    name = this.t.val;
                    base.Expect(3);
                }
                this.Expr(out expression);
                if (expression != null)
                {
                    if (name == "")
                    {
                        positional.Add(expression);
                    }
                    else
                    {
                        named.Add(new NamedArgumentExpression(name, expression));
                        name = "";
                    }
                }
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    if (this.IsAssignment())
                    {
                        flag = true;
                        base.Expect(1);
                        name = this.t.val;
                        base.Expect(3);
                    }
                    else if (this.StartOf(5))
                    {
                        if (flag)
                        {
                            this.Error("no positional argument after named argument");
                        }
                    }
                    else
                    {
                        base.SynErr(0x80);
                    }
                    this.Expr(out expression);
                    if (expression != null)
                    {
                        if (name == "")
                        {
                            positional.Add(expression);
                        }
                        else
                        {
                            named.Add(new NamedArgumentExpression(name, expression));
                            name = "";
                        }
                    }
                }
            }
            base.Expect(0x15);
        }

        private void AttributeSection(out ICSharpCode.NRefactory.Parser.AST.AttributeSection section)
        {
            ICSharpCode.NRefactory.Parser.AST.Attribute attribute;
            string attributeTarget = "";
            List<ICSharpCode.NRefactory.Parser.AST.Attribute> attributes = new List<ICSharpCode.NRefactory.Parser.AST.Attribute>();
            base.Expect(0x12);
            Point location = this.t.Location;
            if (this.IsLocalAttrTarget())
            {
                if (this.la.kind == 0x44)
                {
                    base.lexer.NextToken();
                    attributeTarget = "event";
                }
                else if (this.la.kind == 100)
                {
                    base.lexer.NextToken();
                    attributeTarget = "return";
                }
                else
                {
                    base.lexer.NextToken();
                    if ((((this.t.val != "field") || (this.t.val != "method")) || ((this.t.val != "module") || (this.t.val != "param"))) || ((this.t.val != "property") || (this.t.val != "type")))
                    {
                        this.Error("attribute target specifier (event, return, field,method, module, param, property, or type) expected");
                    }
                    attributeTarget = this.t.val;
                }
                base.Expect(9);
            }
            this.Attribute(out attribute);
            attributes.Add(attribute);
            while (this.NotFinalComma())
            {
                base.Expect(14);
                this.Attribute(out attribute);
                attributes.Add(attribute);
            }
            if (this.la.kind == 14)
            {
                base.lexer.NextToken();
            }
            base.Expect(0x13);
            section = new ICSharpCode.NRefactory.Parser.AST.AttributeSection(attributeTarget, attributes);
            section.StartLocation = location;
            section.EndLocation = this.t.EndLocation;
        }

        private void Block(out ICSharpCode.NRefactory.Parser.AST.Statement stmt)
        {
            base.Expect(0x10);
            BlockStatement block = new BlockStatement();
            block.StartLocation = this.t.EndLocation;
            base.compilationUnit.BlockStart(block);
            if (!base.parseMethodContents)
            {
                base.lexer.SkipCurrentBlock();
            }
            while (this.StartOf(20))
            {
                this.Statement();
            }
            base.Expect(0x11);
            stmt = block;
            block.EndLocation = this.t.EndLocation;
            base.compilationUnit.BlockEnd();
        }

        private bool CatchAndLPar()
        {
            return ((this.la.kind == 0x37) && (this.Peek(1).kind == 20));
        }

        private void CatchClauses(out List<CatchClause> catchClauses)
        {
            ICSharpCode.NRefactory.Parser.AST.Statement statement;
            catchClauses = new List<CatchClause>();
            base.Expect(0x37);
            if (this.la.kind == 0x10)
            {
                this.Block(out statement);
                catchClauses.Add(new CatchClause(statement));
            }
            else if (this.la.kind == 20)
            {
                TypeReference reference;
                base.lexer.NextToken();
                this.ClassType(out reference, false);
                string variableName = null;
                if (this.la.kind == 1)
                {
                    base.lexer.NextToken();
                    variableName = this.t.val;
                }
                base.Expect(0x15);
                this.Block(out statement);
                catchClauses.Add(new CatchClause(reference, variableName, statement));
                while (this.IsTypedCatch())
                {
                    base.Expect(0x37);
                    base.Expect(20);
                    this.ClassType(out reference, false);
                    variableName = null;
                    if (this.la.kind == 1)
                    {
                        base.lexer.NextToken();
                        variableName = this.t.val;
                    }
                    base.Expect(0x15);
                    this.Block(out statement);
                    catchClauses.Add(new CatchClause(reference, variableName, statement));
                }
                if (this.la.kind == 0x37)
                {
                    base.lexer.NextToken();
                    this.Block(out statement);
                    catchClauses.Add(new CatchClause(statement));
                }
            }
            else
            {
                base.SynErr(0xb2);
            }
        }

        private void ClassBase(out List<TypeReference> names)
        {
            TypeReference reference;
            names = new List<TypeReference>();
            base.Expect(9);
            this.ClassType(out reference, false);
            if (reference != null)
            {
                names.Add(reference);
            }
            while (this.la.kind == 14)
            {
                base.lexer.NextToken();
                this.TypeName(out reference, false);
                if (reference != null)
                {
                    names.Add(reference);
                }
            }
        }

        private void ClassBody()
        {
            base.Expect(0x10);
            while (this.StartOf(11))
            {
                List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
                Modifiers m = new Modifiers();
                while (this.la.kind == 0x12)
                {
                    ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
                    this.AttributeSection(out section);
                    attributes.Add(section);
                }
                while (this.StartOf(12))
                {
                    this.MemberModifier(m);
                }
                this.ClassMemberDecl(m, attributes);
            }
            base.Expect(0x11);
        }

        private void ClassMemberDecl(Modifiers m, List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes)
        {
            ICSharpCode.NRefactory.Parser.AST.Statement stmt = null;
            if (this.StartOf(0x10))
            {
                this.StructMemberDecl(m, attributes);
            }
            else if (this.la.kind == 0x1b)
            {
                m.Check(Modifier.Destructors);
                Point location = this.t.Location;
                base.lexer.NextToken();
                base.Expect(1);
                DestructorDeclaration childNode = new DestructorDeclaration(this.t.val, m.Modifier, attributes);
                childNode.Modifier = m.Modifier;
                childNode.StartLocation = m.GetDeclarationLocation(location);
                base.Expect(20);
                base.Expect(0x15);
                childNode.EndLocation = this.t.EndLocation;
                if (this.la.kind == 0x10)
                {
                    this.Block(out stmt);
                }
                else if (this.la.kind == 11)
                {
                    base.lexer.NextToken();
                }
                else
                {
                    base.SynErr(0x8a);
                }
                childNode.Body = (BlockStatement) stmt;
                base.compilationUnit.AddChild(childNode);
            }
            else
            {
                base.SynErr(0x8b);
            }
        }

        private void ClassType(out TypeReference typeRef, bool canBeUnbound)
        {
            typeRef = null;
            if (this.la.kind == 1)
            {
                TypeReference reference;
                this.TypeName(out reference, canBeUnbound);
                typeRef = reference;
            }
            else if (this.la.kind == 90)
            {
                base.lexer.NextToken();
                typeRef = new TypeReference("object");
            }
            else if (this.la.kind == 0x6b)
            {
                base.lexer.NextToken();
                typeRef = new TypeReference("string");
            }
            else
            {
                base.SynErr(0x88);
            }
        }

        private void ConditionalAndExpr(ref Expression outExpr)
        {
            this.InclusiveOrExpr(ref outExpr);
            while (this.la.kind == 0x19)
            {
                Expression expression;
                base.lexer.NextToken();
                this.UnaryExpr(out expression);
                this.InclusiveOrExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, BinaryOperatorType.LogicalAnd, expression);
            }
        }

        private void ConditionalOrExpr(ref Expression outExpr)
        {
            this.ConditionalAndExpr(ref outExpr);
            while (this.la.kind == 0x1a)
            {
                Expression expression;
                base.lexer.NextToken();
                this.UnaryExpr(out expression);
                this.ConditionalAndExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, BinaryOperatorType.LogicalOr, expression);
            }
        }

        private void ConstructorInitializer(out ICSharpCode.NRefactory.Parser.AST.ConstructorInitializer ci)
        {
            ci = new ICSharpCode.NRefactory.Parser.AST.ConstructorInitializer();
            base.Expect(9);
            if (this.la.kind == 50)
            {
                base.lexer.NextToken();
                ci.ConstructorInitializerType = ConstructorInitializerType.Base;
            }
            else if (this.la.kind == 110)
            {
                base.lexer.NextToken();
                ci.ConstructorInitializerType = ConstructorInitializerType.This;
            }
            else
            {
                base.SynErr(160);
            }
            base.Expect(20);
            if (this.StartOf(0x15))
            {
                Expression expression;
                this.Argument(out expression);
                if (expression != null)
                {
                    ci.Arguments.Add(expression);
                }
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    this.Argument(out expression);
                    if (expression != null)
                    {
                        ci.Arguments.Add(expression);
                    }
                }
            }
            base.Expect(0x15);
        }

        private void CS()
        {
            base.lexer.NextToken();
            base.compilationUnit = new CompilationUnit();
            while (this.la.kind == 120)
            {
                this.UsingDirective();
            }
            while (this.IsGlobalAttrTarget())
            {
                this.GlobalAttributeSection();
            }
            while (this.StartOf(1))
            {
                this.NamespaceMemberDecl();
            }
            base.Expect(0);
        }

        private bool DotAndIdent()
        {
            return ((this.la.kind == 15) && (this.Peek(1).kind == 1));
        }

        private void EmbeddedStatement(out ICSharpCode.NRefactory.Parser.AST.Statement statement)
        {
            TypeReference type = null;
            Expression expr = null;
            ICSharpCode.NRefactory.Parser.AST.Statement statement2 = null;
            statement = null;
            if (this.la.kind == 0x10)
            {
                this.Block(out statement);
            }
            else if (this.la.kind == 11)
            {
                base.lexer.NextToken();
                statement = new EmptyStatement();
            }
            else if (this.UnCheckedAndLBrace())
            {
                ICSharpCode.NRefactory.Parser.AST.Statement statement3;
                bool flag = true;
                if (this.la.kind == 0x39)
                {
                    base.lexer.NextToken();
                }
                else if (this.la.kind == 0x75)
                {
                    base.lexer.NextToken();
                    flag = false;
                }
                else
                {
                    base.SynErr(170);
                }
                this.Block(out statement3);
                statement = flag ? ((ICSharpCode.NRefactory.Parser.AST.Statement) new CheckedStatement(statement3)) : ((ICSharpCode.NRefactory.Parser.AST.Statement) new UncheckedStatement(statement3));
            }
            else if (this.la.kind == 0x4e)
            {
                base.lexer.NextToken();
                ICSharpCode.NRefactory.Parser.AST.Statement statement4 = null;
                base.Expect(20);
                this.Expr(out expr);
                base.Expect(0x15);
                this.EmbeddedStatement(out statement2);
                if (this.la.kind == 0x42)
                {
                    base.lexer.NextToken();
                    this.EmbeddedStatement(out statement4);
                }
                statement = (statement4 != null) ? new IfElseStatement(expr, statement2, statement4) : new IfElseStatement(expr, statement2);
                if ((statement4 is IfElseStatement) && ((statement4 as IfElseStatement).TrueStatement.Count == 1))
                {
                    (statement as IfElseStatement).ElseIfSections.Add(new ElseIfSection((statement4 as IfElseStatement).Condition, (statement4 as IfElseStatement).TrueStatement[0]));
                    (statement as IfElseStatement).ElseIfSections.AddRange((statement4 as IfElseStatement).ElseIfSections);
                    (statement as IfElseStatement).FalseStatement = (statement4 as IfElseStatement).FalseStatement;
                }
            }
            else if (this.la.kind == 0x6d)
            {
                base.lexer.NextToken();
                List<ICSharpCode.NRefactory.Parser.AST.SwitchSection> switchSections = new List<ICSharpCode.NRefactory.Parser.AST.SwitchSection>();
                base.Expect(20);
                this.Expr(out expr);
                base.Expect(0x15);
                base.Expect(0x10);
                while ((this.la.kind == 0x36) || (this.la.kind == 0x3e))
                {
                    ICSharpCode.NRefactory.Parser.AST.SwitchSection section;
                    this.SwitchSection(out section);
                    switchSections.Add(section);
                }
                base.Expect(0x11);
                statement = new SwitchStatement(expr, switchSections);
            }
            else if (this.la.kind == 0x7c)
            {
                base.lexer.NextToken();
                base.Expect(20);
                this.Expr(out expr);
                base.Expect(0x15);
                this.EmbeddedStatement(out statement2);
                statement = new DoLoopStatement(expr, statement2, ConditionType.While, ConditionPosition.Start);
            }
            else if (this.la.kind == 0x40)
            {
                base.lexer.NextToken();
                this.EmbeddedStatement(out statement2);
                base.Expect(0x7c);
                base.Expect(20);
                this.Expr(out expr);
                base.Expect(0x15);
                base.Expect(11);
                statement = new DoLoopStatement(expr, statement2, ConditionType.While, ConditionPosition.End);
            }
            else if (this.la.kind == 0x4b)
            {
                base.lexer.NextToken();
                List<ICSharpCode.NRefactory.Parser.AST.Statement> initializer = null;
                List<ICSharpCode.NRefactory.Parser.AST.Statement> iterator = null;
                base.Expect(20);
                if (this.StartOf(5))
                {
                    this.ForInitializer(out initializer);
                }
                base.Expect(11);
                if (this.StartOf(5))
                {
                    this.Expr(out expr);
                }
                base.Expect(11);
                if (this.StartOf(5))
                {
                    this.ForIterator(out iterator);
                }
                base.Expect(0x15);
                this.EmbeddedStatement(out statement2);
                statement = new ForStatement(initializer, expr, iterator, statement2);
            }
            else if (this.la.kind == 0x4c)
            {
                base.lexer.NextToken();
                base.Expect(20);
                this.Type(out type);
                base.Expect(1);
                string val = this.t.val;
                Point location = this.t.Location;
                base.Expect(80);
                this.Expr(out expr);
                base.Expect(0x15);
                this.EmbeddedStatement(out statement2);
                statement = new ForeachStatement(type, val, expr, statement2);
                statement.EndLocation = this.t.EndLocation;
            }
            else if (this.la.kind == 0x34)
            {
                base.lexer.NextToken();
                base.Expect(11);
                statement = new BreakStatement();
            }
            else if (this.la.kind == 60)
            {
                base.lexer.NextToken();
                base.Expect(11);
                statement = new ContinueStatement();
            }
            else if (this.la.kind == 0x4d)
            {
                this.GotoStatement(out statement);
            }
            else if (this.IsYieldStatement())
            {
                base.Expect(1);
                if (this.la.kind == 100)
                {
                    base.lexer.NextToken();
                    this.Expr(out expr);
                    statement = new YieldStatement(new ReturnStatement(expr));
                }
                else if (this.la.kind == 0x34)
                {
                    base.lexer.NextToken();
                    statement = new YieldStatement(new BreakStatement());
                }
                else
                {
                    base.SynErr(0xab);
                }
                base.Expect(11);
            }
            else if (this.la.kind == 100)
            {
                base.lexer.NextToken();
                if (this.StartOf(5))
                {
                    this.Expr(out expr);
                }
                base.Expect(11);
                statement = new ReturnStatement(expr);
            }
            else if (this.la.kind == 0x6f)
            {
                base.lexer.NextToken();
                if (this.StartOf(5))
                {
                    this.Expr(out expr);
                }
                base.Expect(11);
                statement = new ThrowStatement(expr);
            }
            else if (this.StartOf(5))
            {
                this.StatementExpr(out statement);
                base.Expect(11);
            }
            else if (this.la.kind == 0x71)
            {
                this.TryStatement(out statement);
            }
            else if (this.la.kind == 0x55)
            {
                base.lexer.NextToken();
                base.Expect(20);
                this.Expr(out expr);
                base.Expect(0x15);
                this.EmbeddedStatement(out statement2);
                statement = new LockStatement(expr, statement2);
            }
            else if (this.la.kind == 120)
            {
                ICSharpCode.NRefactory.Parser.AST.Statement stmt = null;
                base.lexer.NextToken();
                base.Expect(20);
                this.ResourceAcquisition(out stmt);
                base.Expect(0x15);
                this.EmbeddedStatement(out statement2);
                statement = new UsingStatement(stmt, statement2);
            }
            else if (this.la.kind == 0x76)
            {
                base.lexer.NextToken();
                this.Block(out statement2);
                statement = new UnsafeStatement(statement2);
            }
            else if (this.la.kind == 0x49)
            {
                base.lexer.NextToken();
                base.Expect(20);
                this.Type(out type);
                if (type.PointerNestingLevel == 0)
                {
                    this.Error("can only fix pointer types");
                }
                List<VariableDeclaration> pointerDeclarators = new List<VariableDeclaration>(1);
                base.Expect(1);
                string name = this.t.val;
                base.Expect(3);
                this.Expr(out expr);
                pointerDeclarators.Add(new VariableDeclaration(name, expr));
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    base.Expect(1);
                    name = this.t.val;
                    base.Expect(3);
                    this.Expr(out expr);
                    pointerDeclarators.Add(new VariableDeclaration(name, expr));
                }
                base.Expect(0x15);
                this.EmbeddedStatement(out statement2);
                statement = new FixedStatement(type, pointerDeclarators, statement2);
            }
            else
            {
                base.SynErr(0xac);
            }
        }

        private void EnumBody()
        {
            base.Expect(0x10);
            if ((this.la.kind == 1) || (this.la.kind == 0x12))
            {
                FieldDeclaration declaration;
                this.EnumMemberDecl(out declaration);
                base.compilationUnit.AddChild(declaration);
                while (this.NotFinalComma())
                {
                    base.Expect(14);
                    this.EnumMemberDecl(out declaration);
                    base.compilationUnit.AddChild(declaration);
                }
                if (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                }
            }
            base.Expect(0x11);
        }

        private void EnumMemberDecl(out FieldDeclaration f)
        {
            Expression expr = null;
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            ICSharpCode.NRefactory.Parser.AST.AttributeSection section = null;
            VariableDeclaration item = null;
            while (this.la.kind == 0x12)
            {
                this.AttributeSection(out section);
                attributes.Add(section);
            }
            base.Expect(1);
            f = new FieldDeclaration(attributes);
            item = new VariableDeclaration(this.t.val);
            f.Fields.Add(item);
            f.StartLocation = this.t.Location;
            if (this.la.kind == 3)
            {
                base.lexer.NextToken();
                this.Expr(out expr);
                item.Initializer = expr;
            }
        }

        private void EqualityExpr(ref Expression outExpr)
        {
            BinaryOperatorType none = BinaryOperatorType.None;
            this.RelationalExpr(ref outExpr);
            while ((this.la.kind == 0x21) || (this.la.kind == 0x22))
            {
                Expression expression;
                if (this.la.kind == 0x22)
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.InEquality;
                }
                else
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.Equality;
                }
                this.UnaryExpr(out expression);
                this.RelationalExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, none, expression);
            }
        }

        public void Error(string s)
        {
            if (base.errDist >= 2)
            {
                base.errors.Error(this.la.line, this.la.col, s);
            }
            base.errDist = 0;
        }

        private void EventAccessorDecls(out EventAddRegion addBlock, out EventRemoveRegion removeBlock)
        {
            ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
            ICSharpCode.NRefactory.Parser.AST.Statement statement;
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            addBlock = null;
            removeBlock = null;
            while (this.la.kind == 0x12)
            {
                this.AttributeSection(out section);
                attributes.Add(section);
            }
            if (this.IdentIsAdd())
            {
                addBlock = new EventAddRegion(attributes);
                this.AddAccessorDecl(out statement);
                attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
                addBlock.Block = (BlockStatement) statement;
                while (this.la.kind == 0x12)
                {
                    this.AttributeSection(out section);
                    attributes.Add(section);
                }
                this.RemoveAccessorDecl(out statement);
                removeBlock = new EventRemoveRegion(attributes);
                removeBlock.Block = (BlockStatement) statement;
            }
            else if (this.IdentIsRemove())
            {
                this.RemoveAccessorDecl(out statement);
                removeBlock = new EventRemoveRegion(attributes);
                removeBlock.Block = (BlockStatement) statement;
                attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
                while (this.la.kind == 0x12)
                {
                    this.AttributeSection(out section);
                    attributes.Add(section);
                }
                this.AddAccessorDecl(out statement);
                addBlock = new EventAddRegion(attributes);
                addBlock.Block = (BlockStatement) statement;
            }
            else if (this.la.kind == 1)
            {
                base.lexer.NextToken();
                this.Error("add or remove accessor declaration expected");
            }
            else
            {
                base.SynErr(0x9f);
            }
        }

        private void ExclusiveOrExpr(ref Expression outExpr)
        {
            this.AndExpr(ref outExpr);
            while (this.la.kind == 30)
            {
                Expression expression;
                base.lexer.NextToken();
                this.UnaryExpr(out expression);
                this.AndExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, BinaryOperatorType.ExclusiveOr, expression);
            }
        }

        protected void ExpectWeak(int n, int follow)
        {
            if (base.lexer.LookAhead.kind == n)
            {
                base.lexer.NextToken();
            }
            else
            {
                base.SynErr(n);
                while (!this.StartOf(follow))
                {
                    base.lexer.NextToken();
                }
            }
        }

        private void Expr(out Expression expr)
        {
            AssignmentOperatorType type;
            expr = null;
            Expression expression = null;
            Expression expression2 = null;
            this.UnaryExpr(out expr);
            if (this.StartOf(6))
            {
                this.AssignmentOperator(out type);
                this.Expr(out expression);
                expr = new AssignmentExpression(expr, type, expression);
            }
            else if ((this.la.kind == 0x16) && (this.Peek(1).kind == 0x23))
            {
                this.AssignmentOperator(out type);
                this.Expr(out expression);
                expr = new AssignmentExpression(expr, type, expression);
            }
            else if (this.StartOf(7))
            {
                this.ConditionalOrExpr(ref expr);
                if (this.la.kind == 13)
                {
                    base.lexer.NextToken();
                    this.Expr(out expression);
                    expr = new BinaryOperatorExpression(expr, BinaryOperatorType.NullCoalescing, expression);
                }
                if (this.la.kind == 12)
                {
                    base.lexer.NextToken();
                    this.Expr(out expression);
                    base.Expect(9);
                    this.Expr(out expression2);
                    expr = new ConditionalExpression(expr, expression, expression2);
                }
            }
            else
            {
                base.SynErr(0x81);
            }
        }

        private void FixedParameter(out ParameterDeclarationExpression p)
        {
            TypeReference reference;
            ParamModifier @in = ParamModifier.In;
            Point location = this.t.Location;
            if ((this.la.kind == 0x5c) || (this.la.kind == 0x63))
            {
                if (this.la.kind == 0x63)
                {
                    base.lexer.NextToken();
                    @in = ParamModifier.Ref;
                }
                else
                {
                    base.lexer.NextToken();
                    @in = ParamModifier.Out;
                }
            }
            this.Type(out reference);
            base.Expect(1);
            p = new ParameterDeclarationExpression(reference, this.t.val, @in);
            p.StartLocation = location;
            p.EndLocation = this.t.Location;
        }

        private void ForInitializer(out List<ICSharpCode.NRefactory.Parser.AST.Statement> initializer)
        {
            ICSharpCode.NRefactory.Parser.AST.Statement statement;
            initializer = new List<ICSharpCode.NRefactory.Parser.AST.Statement>();
            if (this.IsLocalVarDecl())
            {
                this.LocalVariableDecl(out statement);
                initializer.Add(statement);
            }
            else if (this.StartOf(5))
            {
                this.StatementExpr(out statement);
                initializer.Add(statement);
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    this.StatementExpr(out statement);
                    initializer.Add(statement);
                }
            }
            else
            {
                base.SynErr(0xad);
            }
        }

        private void ForIterator(out List<ICSharpCode.NRefactory.Parser.AST.Statement> iterator)
        {
            ICSharpCode.NRefactory.Parser.AST.Statement statement;
            iterator = new List<ICSharpCode.NRefactory.Parser.AST.Statement>();
            this.StatementExpr(out statement);
            iterator.Add(statement);
            while (this.la.kind == 14)
            {
                base.lexer.NextToken();
                this.StatementExpr(out statement);
                iterator.Add(statement);
            }
        }

        private void FormalParameterList(List<ParameterDeclarationExpression> parameter)
        {
            ParameterDeclarationExpression expression;
            ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> list = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            while (this.la.kind == 0x12)
            {
                this.AttributeSection(out section);
                list.Add(section);
            }
            if (!this.StartOf(15))
            {
                if (this.la.kind == 0x5e)
                {
                    this.ParameterArray(out expression);
                    expression.Attributes = list;
                    parameter.Add(expression);
                }
                else
                {
                    base.SynErr(0x87);
                }
            }
            else
            {
                this.FixedParameter(out expression);
                bool flag = false;
                expression.Attributes = list;
                parameter.Add(expression);
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    list = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
                    if (flag)
                    {
                        this.Error("params array must be at end of parameter list");
                    }
                    while (this.la.kind == 0x12)
                    {
                        this.AttributeSection(out section);
                        list.Add(section);
                    }
                    if (this.StartOf(15))
                    {
                        this.FixedParameter(out expression);
                        expression.Attributes = list;
                        parameter.Add(expression);
                    }
                    else
                    {
                        if (this.la.kind == 0x5e)
                        {
                            this.ParameterArray(out expression);
                            flag = true;
                            expression.Attributes = list;
                            parameter.Add(expression);
                            continue;
                        }
                        base.SynErr(0x86);
                    }
                }
            }
        }

        private void GetAccessorDecl(out PropertyGetRegion getBlock, List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes)
        {
            ICSharpCode.NRefactory.Parser.AST.Statement stmt = null;
            base.Expect(1);
            if (this.t.val != "get")
            {
                this.Error("get expected");
            }
            Point location = this.t.Location;
            if (this.la.kind == 0x10)
            {
                this.Block(out stmt);
            }
            else if (this.la.kind == 11)
            {
                base.lexer.NextToken();
            }
            else
            {
                base.SynErr(0xa5);
            }
            getBlock = new PropertyGetRegion((BlockStatement) stmt, attributes);
            getBlock.StartLocation = location;
            getBlock.EndLocation = this.t.EndLocation;
        }

        private TypeReferenceExpression GetTypeReferenceExpression(Expression expr, List<TypeReference> genericTypes)
        {
            TypeReferenceExpression expression = expr as TypeReferenceExpression;
            if (expression != null)
            {
                return new TypeReferenceExpression(new TypeReference(expression.TypeReference.Type, expression.TypeReference.PointerNestingLevel, expression.TypeReference.RankSpecifier, genericTypes));
            }
            StringBuilder b = new StringBuilder();
            if (!this.WriteFullTypeName(b, expr))
            {
                while (expr is FieldReferenceExpression)
                {
                    expr = ((FieldReferenceExpression) expr).TargetObject;
                }
                expression = expr as TypeReferenceExpression;
                if (expression != null)
                {
                    TypeReference typeReference = expression.TypeReference;
                    if (typeReference.GenericTypes.Count == 0)
                    {
                        typeReference = typeReference.Clone();
                        typeReference.Type = typeReference.Type + "." + b.ToString();
                        typeReference.GenericTypes.AddRange(genericTypes);
                    }
                    else
                    {
                        typeReference = new InnerClassTypeReference(typeReference, b.ToString(), genericTypes);
                    }
                    return new TypeReferenceExpression(typeReference);
                }
            }
            return new TypeReferenceExpression(new TypeReference(b.ToString(), 0, null, genericTypes));
        }

        private void GlobalAttributeSection()
        {
            ICSharpCode.NRefactory.Parser.AST.Attribute attribute;
            base.Expect(0x12);
            Point location = this.t.Location;
            base.Expect(1);
            if (this.t.val != "assembly")
            {
                this.Error("global attribute target specifier (\"assembly\") expected");
            }
            string val = this.t.val;
            List<ICSharpCode.NRefactory.Parser.AST.Attribute> attributes = new List<ICSharpCode.NRefactory.Parser.AST.Attribute>();
            base.Expect(9);
            this.Attribute(out attribute);
            attributes.Add(attribute);
            while (this.NotFinalComma())
            {
                base.Expect(14);
                this.Attribute(out attribute);
                attributes.Add(attribute);
            }
            if (this.la.kind == 14)
            {
                base.lexer.NextToken();
            }
            base.Expect(0x13);
            ICSharpCode.NRefactory.Parser.AST.AttributeSection childNode = new ICSharpCode.NRefactory.Parser.AST.AttributeSection(val, attributes);
            childNode.StartLocation = location;
            childNode.EndLocation = this.t.EndLocation;
            base.compilationUnit.AddChild(childNode);
        }

        private void GotoStatement(out ICSharpCode.NRefactory.Parser.AST.Statement stmt)
        {
            stmt = null;
            base.Expect(0x4d);
            if (this.la.kind == 1)
            {
                base.lexer.NextToken();
                stmt = new ICSharpCode.NRefactory.Parser.AST.GotoStatement(this.t.val);
                base.Expect(11);
            }
            else if (this.la.kind == 0x36)
            {
                Expression expression;
                base.lexer.NextToken();
                this.Expr(out expression);
                base.Expect(11);
                stmt = new GotoCaseStatement(expression);
            }
            else if (this.la.kind == 0x3e)
            {
                base.lexer.NextToken();
                base.Expect(11);
                stmt = new GotoCaseStatement(null);
            }
            else
            {
                base.SynErr(0xae);
            }
        }

        private bool GuessTypeCast()
        {
            this.StartPeek();
            Token pt = this.Peek();
            if (!this.IsTypeNameForTypeCast(ref pt))
            {
                return false;
            }
            if (pt.kind != 0x15)
            {
                return false;
            }
            pt = this.Peek();
            return (Tokens.CastFollower[pt.kind] || (Tokens.TypeKW[pt.kind] && (base.lexer.Peek().kind == 15)));
        }

        private bool IdentAndAsgn()
        {
            return ((this.la.kind == 1) && (this.Peek(1).kind == 3));
        }

        private bool IdentAndColon()
        {
            return ((this.la.kind == 1) && (this.Peek(1).kind == 9));
        }

        private bool IdentAndCommaOrAsgnOrSColon()
        {
            int kind = this.Peek(1).kind;
            if (this.la.kind != 1)
            {
                return false;
            }
            if ((kind != 14) && (kind != 3))
            {
                return (kind == 11);
            }
            return true;
        }

        private bool IdentAndLPar()
        {
            return ((this.la.kind == 1) && (this.Peek(1).kind == 20));
        }

        private bool IdentIsAdd()
        {
            return ((this.la.kind == 1) && (this.la.val == "add"));
        }

        private bool IdentIsGet()
        {
            return ((this.la.kind == 1) && (this.la.val == "get"));
        }

        private bool IdentIsRemove()
        {
            return ((this.la.kind == 1) && (this.la.val == "remove"));
        }

        private bool IdentIsSet()
        {
            return ((this.la.kind == 1) && (this.la.val == "set"));
        }

        private bool IdentIsWhere()
        {
            return ((this.la.kind == 1) && (this.la.val == "where"));
        }

        private void InclusiveOrExpr(ref Expression outExpr)
        {
            this.ExclusiveOrExpr(ref outExpr);
            while (this.la.kind == 0x1d)
            {
                Expression expression;
                base.lexer.NextToken();
                this.UnaryExpr(out expression);
                this.ExclusiveOrExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, BinaryOperatorType.BitwiseOr, expression);
            }
        }

        private void IntegralType(out string name)
        {
            name = "";
            switch (this.la.kind)
            {
                case 0x35:
                    base.lexer.NextToken();
                    name = "byte";
                    return;

                case 0x38:
                    base.lexer.NextToken();
                    name = "char";
                    return;

                case 0x51:
                    base.lexer.NextToken();
                    name = "int";
                    return;

                case 0x65:
                    base.lexer.NextToken();
                    name = "sbyte";
                    return;

                case 0x67:
                    base.lexer.NextToken();
                    name = "short";
                    return;

                case 0x56:
                    base.lexer.NextToken();
                    name = "long";
                    return;

                case 0x73:
                    base.lexer.NextToken();
                    name = "uint";
                    return;

                case 0x74:
                    base.lexer.NextToken();
                    name = "ulong";
                    return;

                case 0x77:
                    base.lexer.NextToken();
                    name = "ushort";
                    return;
            }
            base.SynErr(0x85);
        }

        private void InterfaceAccessors(out PropertyGetRegion getBlock, out PropertySetRegion setBlock)
        {
            ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            getBlock = null;
            setBlock = null;
            PropertyGetSetRegion region = null;
            while (this.la.kind == 0x12)
            {
                this.AttributeSection(out section);
                attributes.Add(section);
            }
            Point location = this.la.Location;
            if (this.IdentIsGet())
            {
                base.Expect(1);
                getBlock = new PropertyGetRegion(null, attributes);
            }
            else if (this.IdentIsSet())
            {
                base.Expect(1);
                setBlock = new PropertySetRegion(null, attributes);
            }
            else if (this.la.kind == 1)
            {
                base.lexer.NextToken();
                this.Error("set or get expected");
            }
            else
            {
                base.SynErr(0xa3);
            }
            base.Expect(11);
            if (getBlock != null)
            {
                getBlock.StartLocation = location;
                getBlock.EndLocation = this.t.EndLocation;
            }
            if (setBlock != null)
            {
                setBlock.StartLocation = location;
                setBlock.EndLocation = this.t.EndLocation;
            }
            attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            if ((this.la.kind == 1) || (this.la.kind == 0x12))
            {
                while (this.la.kind == 0x12)
                {
                    this.AttributeSection(out section);
                    attributes.Add(section);
                }
                location = this.la.Location;
                if (this.IdentIsGet())
                {
                    base.Expect(1);
                    if (getBlock != null)
                    {
                        this.Error("get already declared");
                    }
                    else
                    {
                        getBlock = new PropertyGetRegion(null, attributes);
                        region = getBlock;
                    }
                }
                else if (this.IdentIsSet())
                {
                    base.Expect(1);
                    if (setBlock != null)
                    {
                        this.Error("set already declared");
                    }
                    else
                    {
                        setBlock = new PropertySetRegion(null, attributes);
                        region = setBlock;
                    }
                }
                else if (this.la.kind == 1)
                {
                    base.lexer.NextToken();
                    this.Error("set or get expected");
                }
                else
                {
                    base.SynErr(0xa4);
                }
                base.Expect(11);
                if (region != null)
                {
                    region.StartLocation = location;
                    region.EndLocation = this.t.EndLocation;
                }
            }
        }

        private void InterfaceBase(out List<TypeReference> names)
        {
            TypeReference reference;
            names = new List<TypeReference>();
            base.Expect(9);
            this.TypeName(out reference, false);
            if (reference != null)
            {
                names.Add(reference);
            }
            while (this.la.kind == 14)
            {
                base.lexer.NextToken();
                this.TypeName(out reference, false);
                if (reference != null)
                {
                    names.Add(reference);
                }
            }
        }

        private void InterfaceBody()
        {
            base.Expect(0x10);
            while (this.StartOf(14))
            {
                this.InterfaceMemberDecl();
            }
            base.Expect(0x11);
        }

        private void InterfaceMemberDecl()
        {
            string val;
            Modifier none = Modifier.None;
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            List<ParameterDeclarationExpression> parameter = new List<ParameterDeclarationExpression>();
            Point location = new Point(-1, -1);
            List<TemplateDefinition> templates = new List<TemplateDefinition>();
            while (this.la.kind == 0x12)
            {
                ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
                this.AttributeSection(out section);
                attributes.Add(section);
            }
            if (this.la.kind == 0x58)
            {
                base.lexer.NextToken();
                none = Modifier.VBInterfaceEnums;
                location = this.t.Location;
            }
            if (this.NotVoidPointer())
            {
                base.Expect(0x7a);
                if (location.X == -1)
                {
                    location = this.t.Location;
                }
                base.Expect(1);
                val = this.t.val;
                if (this.la.kind == 0x17)
                {
                    this.TypeParameterList(templates);
                }
                base.Expect(20);
                if (this.StartOf(10))
                {
                    this.FormalParameterList(parameter);
                }
                base.Expect(0x15);
                while (this.IdentIsWhere())
                {
                    this.TypeParameterConstraintsClause(templates);
                }
                base.Expect(11);
                MethodDeclaration childNode = new MethodDeclaration(val, none, new TypeReference("void"), parameter, attributes);
                childNode.StartLocation = location;
                childNode.EndLocation = this.t.EndLocation;
                childNode.Templates = templates;
                base.compilationUnit.AddChild(childNode);
            }
            else if (this.StartOf(0x12))
            {
                TypeReference reference;
                if (this.StartOf(9))
                {
                    PropertyGetRegion region;
                    PropertySetRegion region2;
                    this.Type(out reference);
                    if (location.X == -1)
                    {
                        location = this.t.Location;
                    }
                    if (this.la.kind == 1)
                    {
                        base.lexer.NextToken();
                        val = this.t.val;
                        Point endLocation = this.t.EndLocation;
                        if ((this.la.kind == 20) || (this.la.kind == 0x17))
                        {
                            if (this.la.kind == 0x17)
                            {
                                this.TypeParameterList(templates);
                            }
                            base.Expect(20);
                            if (this.StartOf(10))
                            {
                                this.FormalParameterList(parameter);
                            }
                            base.Expect(0x15);
                            while (this.IdentIsWhere())
                            {
                                this.TypeParameterConstraintsClause(templates);
                            }
                            base.Expect(11);
                            MethodDeclaration declaration2 = new MethodDeclaration(val, none, reference, parameter, attributes);
                            declaration2.StartLocation = location;
                            declaration2.EndLocation = this.t.EndLocation;
                            declaration2.Templates = templates;
                            base.compilationUnit.AddChild(declaration2);
                        }
                        else if (this.la.kind == 0x10)
                        {
                            PropertyDeclaration declaration3 = new PropertyDeclaration(val, reference, none, attributes);
                            base.compilationUnit.AddChild(declaration3);
                            base.lexer.NextToken();
                            Point point3 = this.t.Location;
                            this.InterfaceAccessors(out region, out region2);
                            base.Expect(0x11);
                            declaration3.GetRegion = region;
                            declaration3.SetRegion = region2;
                            declaration3.StartLocation = location;
                            declaration3.EndLocation = endLocation;
                            declaration3.BodyStart = point3;
                            declaration3.BodyEnd = this.t.EndLocation;
                        }
                        else
                        {
                            base.SynErr(0x98);
                        }
                    }
                    else if (this.la.kind == 110)
                    {
                        base.lexer.NextToken();
                        base.Expect(0x12);
                        this.FormalParameterList(parameter);
                        base.Expect(0x13);
                        Point point4 = this.t.EndLocation;
                        IndexerDeclaration declaration4 = new IndexerDeclaration(reference, parameter, none, attributes);
                        base.compilationUnit.AddChild(declaration4);
                        base.Expect(0x10);
                        Point point5 = this.t.Location;
                        this.InterfaceAccessors(out region, out region2);
                        base.Expect(0x11);
                        declaration4.GetRegion = region;
                        declaration4.SetRegion = region2;
                        declaration4.StartLocation = location;
                        declaration4.EndLocation = point4;
                        declaration4.BodyStart = point5;
                        declaration4.BodyEnd = this.t.EndLocation;
                    }
                    else
                    {
                        base.SynErr(0x99);
                    }
                }
                else
                {
                    base.lexer.NextToken();
                    if (location.X == -1)
                    {
                        location = this.t.Location;
                    }
                    this.Type(out reference);
                    base.Expect(1);
                    EventDeclaration declaration5 = new EventDeclaration(reference, this.t.val, none, attributes, null);
                    base.compilationUnit.AddChild(declaration5);
                    base.Expect(11);
                    declaration5.StartLocation = location;
                    declaration5.EndLocation = this.t.EndLocation;
                }
            }
            else
            {
                base.SynErr(0x9a);
            }
        }

        private bool IsAssignment()
        {
            return this.IdentAndAsgn();
        }

        private bool IsDims()
        {
            return this.LBrackAndCommaOrRBrack();
        }

        private bool IsExplicitInterfaceImplementation()
        {
            this.StartPeek();
            Token la = this.la;
            la = this.Peek();
            return (((la.kind == 15) || (la.kind == 10)) || (((la.kind == 0x17) && this.SkipGeneric(ref la)) && (la.kind == 15)));
        }

        private bool IsGenericFollowedBy(int token)
        {
            Token la = this.la;
            if (la.kind != 0x17)
            {
                return false;
            }
            this.StartPeek();
            return (this.SkipGeneric(ref la) && (la.kind == token));
        }

        private bool IsGlobalAttrTarget()
        {
            Token token = this.Peek(1);
            return (((this.la.kind == 0x12) && (token.kind == 1)) && (token.val == "assembly"));
        }

        private bool IsLabel()
        {
            return this.IdentAndColon();
        }

        private bool IsLocalAttrTarget()
        {
            int kind = this.la.kind;
            string val = this.la.val;
            if (((kind != 0x44) && (kind != 100)) && ((kind != 1) || (((!(val == "field") && !(val == "method")) && (!(val == "module") && !(val == "param"))) && (!(val == "property") && !(val == "type")))))
            {
                return false;
            }
            return (this.Peek(1).kind == 9);
        }

        private bool IsLocalVarDecl()
        {
            if (!this.IsYieldStatement())
            {
                string str;
                if ((Tokens.TypeKW[this.la.kind] && (this.Peek(1).kind != 15)) || (this.la.kind == 0x7a))
                {
                    return true;
                }
                this.StartPeek();
                Token la = this.la;
                if (this.IsQualident(ref la, out str) && this.SkipGeneric(ref la))
                {
                    while (la.kind == 15)
                    {
                        la = this.Peek();
                        if (!this.IsQualident(ref la, out str))
                        {
                            return false;
                        }
                        if (!this.SkipGeneric(ref la))
                        {
                            return false;
                        }
                    }
                    return ((this.SkipQuestionMark(ref la) && this.IsPointerOrDims(ref la)) && (la.kind == 1));
                }
            }
            return false;
        }

        private bool IsNotYieldStatement()
        {
            return !this.IsYieldStatement();
        }

        private bool IsPointer()
        {
            return (this.la.kind == 6);
        }

        private bool IsPointerOrDims()
        {
            return this.TimesOrLBrackAndCommaOrRBrack();
        }

        private bool IsPointerOrDims(ref Token pt)
        {
            while (true)
            {
                if (pt.kind == 0x12)
                {
                    do
                    {
                        pt = this.Peek();
                    }
                    while (pt.kind == 14);
                    if (pt.kind != 0x13)
                    {
                        return false;
                    }
                }
                else if (pt.kind != 6)
                {
                    return true;
                }
                pt = this.Peek();
            }
        }

        private bool IsQualident(ref Token pt, out string qualident)
        {
            if (pt.kind == 1)
            {
                this.qualidentBuilder.Length = 0;
                this.qualidentBuilder.Append(pt.val);
                pt = this.Peek();
                while ((pt.kind == 15) || (pt.kind == 10))
                {
                    pt = this.Peek();
                    if (pt.kind != 1)
                    {
                        qualident = string.Empty;
                        return false;
                    }
                    this.qualidentBuilder.Append('.');
                    this.qualidentBuilder.Append(pt.val);
                    pt = this.Peek();
                }
                qualident = this.qualidentBuilder.ToString();
                return true;
            }
            qualident = string.Empty;
            return false;
        }

        private bool IsShiftRight()
        {
            Token token = this.Peek(1);
            return ((this.la.kind == 0x16) && (token.kind == 0x16));
        }

        private bool IsSimpleTypeCast()
        {
            base.lexer.StartPeek();
            Token pt = base.lexer.Peek();
            if (!this.IsTypeKWForTypeCast(ref pt))
            {
                return false;
            }
            if (pt.kind == 12)
            {
                pt = base.lexer.Peek();
            }
            return (pt.kind == 0x15);
        }

        private bool IsTypeCast()
        {
            if (this.la.kind != 20)
            {
                return false;
            }
            return (this.IsSimpleTypeCast() || this.GuessTypeCast());
        }

        private bool IsTypedCatch()
        {
            return this.CatchAndLPar();
        }

        private bool IsTypeKWForTypeCast(ref Token pt)
        {
            if (Tokens.TypeKW[pt.kind])
            {
                pt = base.lexer.Peek();
                return (((pt.kind != 6) && (pt.kind != 0x12)) || this.IsPointerOrDims(ref pt));
            }
            if (pt.kind == 0x7a)
            {
                pt = base.lexer.Peek();
                return this.IsPointerOrDims(ref pt);
            }
            return false;
        }

        private bool IsTypeNameForTypeCast(ref Token pt)
        {
            if (pt.kind != 1)
            {
                return false;
            }
            pt = this.Peek();
            if (pt.kind == 10)
            {
                pt = this.Peek();
                if (pt.kind != 1)
                {
                    return false;
                }
                pt = this.Peek();
            }
            while (pt.kind == 15)
            {
                pt = this.Peek();
                if (pt.kind != 1)
                {
                    return false;
                }
                pt = this.Peek();
            }
            if (pt.kind == 0x17)
            {
                do
                {
                    pt = this.Peek();
                    if (!this.IsTypeNameOrKWForTypeCast(ref pt))
                    {
                        return false;
                    }
                }
                while (pt.kind == 14);
                if (pt.kind != 0x16)
                {
                    return false;
                }
                pt = this.Peek();
            }
            if (pt.kind == 12)
            {
                pt = this.Peek();
            }
            return (((pt.kind != 6) && (pt.kind != 0x12)) || this.IsPointerOrDims(ref pt));
        }

        private bool IsTypeNameOrKWForTypeCast(ref Token pt)
        {
            return (this.IsTypeKWForTypeCast(ref pt) || this.IsTypeNameForTypeCast(ref pt));
        }

        private bool IsTypeReferenceExpression(Expression expr)
        {
            if (expr is TypeReferenceExpression)
            {
                return (((TypeReferenceExpression) expr).TypeReference.GenericTypes.Count == 0);
            }
            while (expr is FieldReferenceExpression)
            {
                expr = ((FieldReferenceExpression) expr).TargetObject;
                if (expr is TypeReferenceExpression)
                {
                    return true;
                }
            }
            return (expr is IdentifierExpression);
        }

        private bool IsVarDecl()
        {
            return this.IdentAndCommaOrAsgnOrSColon();
        }

        private bool IsYieldStatement()
        {
            if ((this.la.kind != 1) || !(this.la.val == "yield"))
            {
                return false;
            }
            if (this.Peek(1).kind != 100)
            {
                return (this.Peek(1).kind == 0x34);
            }
            return true;
        }

        private bool LBrackAndCommaOrRBrack()
        {
            int kind = this.Peek(1).kind;
            if (this.la.kind != 0x12)
            {
                return false;
            }
            if (kind != 14)
            {
                return (kind == 0x13);
            }
            return true;
        }

        private void LocalVariableDecl(out ICSharpCode.NRefactory.Parser.AST.Statement stmt)
        {
            TypeReference reference;
            VariableDeclaration var = null;
            this.Type(out reference);
            LocalVariableDeclaration declaration2 = new LocalVariableDeclaration(reference);
            declaration2.StartLocation = this.t.Location;
            this.LocalVariableDeclarator(out var);
            declaration2.Variables.Add(var);
            while (this.la.kind == 14)
            {
                base.lexer.NextToken();
                this.LocalVariableDeclarator(out var);
                declaration2.Variables.Add(var);
            }
            stmt = declaration2;
        }

        private void LocalVariableDeclarator(out VariableDeclaration var)
        {
            Expression initializerExpression = null;
            base.Expect(1);
            var = new VariableDeclaration(this.t.val);
            if (this.la.kind == 3)
            {
                base.lexer.NextToken();
                this.VariableInitializer(out initializerExpression);
                var.Initializer = initializerExpression;
            }
        }

        private void MemberModifier(Modifiers m)
        {
            switch (this.la.kind)
            {
                case 0x58:
                    base.lexer.NextToken();
                    m.Add(Modifier.VBInterfaceEnums, this.t.Location);
                    return;

                case 0x5d:
                    base.lexer.NextToken();
                    m.Add(Modifier.Override, this.t.Location);
                    return;

                case 0x5f:
                    base.lexer.NextToken();
                    m.Add(Modifier.Private, this.t.Location);
                    return;

                case 0x60:
                    base.lexer.NextToken();
                    m.Add(Modifier.Protected, this.t.Location);
                    return;

                case 0x61:
                    base.lexer.NextToken();
                    m.Add(Modifier.Public, this.t.Location);
                    return;

                case 0x62:
                    base.lexer.NextToken();
                    m.Add(Modifier.ReadOnly, this.t.Location);
                    return;

                case 0x53:
                    base.lexer.NextToken();
                    m.Add(Modifier.Internal, this.t.Location);
                    return;

                case 0x30:
                    base.lexer.NextToken();
                    m.Add(Modifier.Dim, this.t.Location);
                    return;

                case 70:
                    base.lexer.NextToken();
                    m.Add(Modifier.Extern, this.t.Location);
                    return;

                case 0x66:
                    base.lexer.NextToken();
                    m.Add(Modifier.Sealed, this.t.Location);
                    return;

                case 0x6a:
                    base.lexer.NextToken();
                    m.Add(Modifier.Static, this.t.Location);
                    return;

                case 0x79:
                    base.lexer.NextToken();
                    m.Add(Modifier.Virtual, this.t.Location);
                    return;

                case 0x7b:
                    base.lexer.NextToken();
                    m.Add(Modifier.Volatile, this.t.Location);
                    return;

                case 0x76:
                    base.lexer.NextToken();
                    m.Add(Modifier.Unsafe, this.t.Location);
                    return;
            }
            base.SynErr(0x89);
        }

        private void MultiplicativeExpr(ref Expression outExpr)
        {
            BinaryOperatorType none = BinaryOperatorType.None;
            while (((this.la.kind == 6) || (this.la.kind == 7)) || (this.la.kind == 8))
            {
                Expression expression;
                if (this.la.kind == 6)
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.Multiply;
                }
                else if (this.la.kind == 7)
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.Divide;
                }
                else
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.Modulus;
                }
                this.UnaryExpr(out expression);
                outExpr = new BinaryOperatorExpression(outExpr, none, expression);
            }
        }

        private void NamespaceMemberDecl()
        {
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            Modifiers m = new Modifiers();
            if (this.la.kind == 0x57)
            {
                string str;
                base.lexer.NextToken();
                Point location = this.t.Location;
                this.Qualident(out str);
                INode childNode = new NamespaceDeclaration(str);
                childNode.StartLocation = location;
                base.compilationUnit.AddChild(childNode);
                base.compilationUnit.BlockStart(childNode);
                base.Expect(0x10);
                while (this.la.kind == 120)
                {
                    this.UsingDirective();
                }
                while (this.StartOf(1))
                {
                    this.NamespaceMemberDecl();
                }
                base.Expect(0x11);
                if (this.la.kind == 11)
                {
                    base.lexer.NextToken();
                }
                childNode.EndLocation = this.t.EndLocation;
                base.compilationUnit.BlockEnd();
            }
            else if (this.StartOf(2))
            {
                while (this.la.kind == 0x12)
                {
                    ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
                    this.AttributeSection(out section);
                    attributes.Add(section);
                }
                while (this.StartOf(3))
                {
                    this.TypeModifier(m);
                }
                this.TypeDecl(m, attributes);
            }
            else
            {
                base.SynErr(0x7e);
            }
        }

        private void NonArrayType(out TypeReference type)
        {
            int num = 0;
            type = null;
            if (((this.la.kind == 1) || (this.la.kind == 90)) || (this.la.kind == 0x6b))
            {
                this.ClassType(out type, false);
            }
            else if (this.StartOf(4))
            {
                string str;
                this.SimpleType(out str);
                type = new TypeReference(str);
            }
            else if (this.la.kind == 0x7a)
            {
                base.lexer.NextToken();
                base.Expect(6);
                num = 1;
                type = new TypeReference("void");
            }
            else
            {
                base.SynErr(0x7f);
            }
            if (this.la.kind == 12)
            {
                this.NullableQuestionMark(ref type);
            }
            while (this.IsPointer())
            {
                base.Expect(6);
                num++;
            }
            if (type != null)
            {
                type.PointerNestingLevel = num;
            }
        }

        private bool NotFinalComma()
        {
            int kind = this.Peek(1).kind;
            return (((this.la.kind == 14) && (kind != 0x11)) && (kind != 0x13));
        }

        private bool NotVoidPointer()
        {
            return ((this.la.kind == 0x7a) && (this.Peek(1).kind != 6));
        }

        private void NullableQuestionMark(ref TypeReference typeRef)
        {
            List<TypeReference> genericTypes = new List<TypeReference>(1);
            base.Expect(12);
            if (typeRef != null)
            {
                genericTypes.Add(typeRef);
            }
            typeRef = new TypeReference("System.Nullable", genericTypes);
        }

        private void OverloadableOperator(out OverloadableOperatorType op)
        {
            op = OverloadableOperatorType.None;
            switch (this.la.kind)
            {
                case 4:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Add;
                    return;

                case 5:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Subtract;
                    return;

                case 6:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Multiply;
                    return;

                case 7:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Divide;
                    return;

                case 8:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Modulus;
                    return;

                case 0x16:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.GreaterThan;
                    if (this.la.kind == 0x16)
                    {
                        base.lexer.NextToken();
                        op = OverloadableOperatorType.ShiftRight;
                    }
                    return;

                case 0x17:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.LessThan;
                    return;

                case 0x18:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Not;
                    return;

                case 0x1b:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.BitNot;
                    return;

                case 0x1c:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.BitwiseAnd;
                    return;

                case 0x1d:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.BitwiseOr;
                    return;

                case 30:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.ExclusiveOr;
                    return;

                case 0x1f:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Increment;
                    return;

                case 0x20:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Decrement;
                    return;

                case 0x21:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.Equality;
                    return;

                case 0x22:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.InEquality;
                    return;

                case 0x23:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.GreaterThanOrEqual;
                    return;

                case 0x24:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.LessThanOrEqual;
                    return;

                case 0x25:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.ShiftLeft;
                    return;

                case 0x47:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.False;
                    return;

                case 0x70:
                    base.lexer.NextToken();
                    op = OverloadableOperatorType.True;
                    return;
            }
            base.SynErr(0xa1);
        }

        private void ParameterArray(out ParameterDeclarationExpression p)
        {
            TypeReference reference;
            base.Expect(0x5e);
            this.Type(out reference);
            base.Expect(1);
            p = new ParameterDeclarationExpression(reference, this.t.val, ParamModifier.Params);
        }

        public override void Parse()
        {
            this.CS();
        }

        public override Expression ParseExpression()
        {
            Expression expression;
            base.lexer.NextToken();
            this.Expr(out expression);
            return expression;
        }

        private Token Peek()
        {
            return base.lexer.Peek();
        }

        private Token Peek(int n)
        {
            base.lexer.StartPeek();
            Token la = this.la;
            while (n > 0)
            {
                la = base.lexer.Peek();
                n--;
            }
            return la;
        }

        private void PrimaryExpr(out Expression pexpr)
        {
            TypeReference typeReference = null;
            List<TypeReference> types = null;
            Expression expression;
            bool flag = false;
            pexpr = null;
            if (this.la.kind == 0x70)
            {
                base.lexer.NextToken();
                pexpr = new PrimitiveExpression(true, "true");
            }
            else if (this.la.kind == 0x47)
            {
                base.lexer.NextToken();
                pexpr = new PrimitiveExpression(false, "false");
            }
            else if (this.la.kind == 0x59)
            {
                base.lexer.NextToken();
                pexpr = new PrimitiveExpression(null, "null");
            }
            else if (this.la.kind == 2)
            {
                base.lexer.NextToken();
                pexpr = new PrimitiveExpression(this.t.literalValue, this.t.val);
            }
            else if ((this.la.kind == 1) && (this.Peek(1).kind == 10))
            {
                base.Expect(1);
                typeReference = new TypeReference(this.t.val);
                base.Expect(10);
                pexpr = new TypeReferenceExpression(typeReference);
                base.Expect(1);
                if (typeReference.Type == "global")
                {
                    typeReference.IsGlobal = true;
                    typeReference.Type = this.t.val ?? "?";
                }
                else
                {
                    typeReference.Type = typeReference.Type + "." + (this.t.val ?? "?");
                }
            }
            else if (this.la.kind == 1)
            {
                base.lexer.NextToken();
                pexpr = new IdentifierExpression(this.t.val);
            }
            else if (this.la.kind == 20)
            {
                base.lexer.NextToken();
                this.Expr(out expression);
                base.Expect(0x15);
                pexpr = new ParenthesizedExpression(expression);
            }
            else if (!this.StartOf(0x1a))
            {
                if (this.la.kind == 110)
                {
                    base.lexer.NextToken();
                    pexpr = new ThisReferenceExpression();
                }
                else if (this.la.kind == 50)
                {
                    base.lexer.NextToken();
                    Expression targetObject = new BaseReferenceExpression();
                    if (this.la.kind == 15)
                    {
                        base.lexer.NextToken();
                        base.Expect(1);
                        targetObject = new FieldReferenceExpression(targetObject, this.t.val);
                    }
                    else if (this.la.kind == 0x12)
                    {
                        base.lexer.NextToken();
                        this.Expr(out expression);
                        List<Expression> indices = new List<Expression>();
                        if (expression != null)
                        {
                            indices.Add(expression);
                        }
                        while (this.la.kind == 14)
                        {
                            base.lexer.NextToken();
                            this.Expr(out expression);
                            if (expression != null)
                            {
                                indices.Add(expression);
                            }
                        }
                        base.Expect(0x13);
                        targetObject = new IndexerExpression(targetObject, indices);
                    }
                    else
                    {
                        base.SynErr(0xb3);
                    }
                    pexpr = targetObject;
                }
                else if (this.la.kind == 0x58)
                {
                    base.lexer.NextToken();
                    this.NonArrayType(out typeReference);
                    List<Expression> parameters = new List<Expression>();
                    if (this.la.kind == 20)
                    {
                        base.lexer.NextToken();
                        ObjectCreateExpression expression3 = new ObjectCreateExpression(typeReference, parameters);
                        if (this.StartOf(0x15))
                        {
                            this.Argument(out expression);
                            if (expression != null)
                            {
                                parameters.Add(expression);
                            }
                            while (this.la.kind == 14)
                            {
                                base.lexer.NextToken();
                                this.Argument(out expression);
                                if (expression != null)
                                {
                                    parameters.Add(expression);
                                }
                            }
                        }
                        base.Expect(0x15);
                        pexpr = expression3;
                    }
                    else if (this.la.kind == 0x12)
                    {
                        base.lexer.NextToken();
                        flag = true;
                        ArrayCreateExpression expression4 = new ArrayCreateExpression(typeReference);
                        pexpr = expression4;
                        int item = 0;
                        List<int> list4 = new List<int>();
                        if ((this.la.kind == 14) || (this.la.kind == 0x13))
                        {
                            while (this.la.kind == 14)
                            {
                                base.lexer.NextToken();
                                item++;
                            }
                            base.Expect(0x13);
                            list4.Add(item);
                            item = 0;
                            while (this.la.kind == 0x12)
                            {
                                base.lexer.NextToken();
                                while (this.la.kind == 14)
                                {
                                    base.lexer.NextToken();
                                    item++;
                                }
                                base.Expect(0x13);
                                list4.Add(item);
                                item = 0;
                            }
                            expression4.CreateType.RankSpecifier = list4.ToArray();
                            this.ArrayInitializer(out expression);
                            expression4.ArrayInitializer = (ArrayInitializerExpression) expression;
                        }
                        else if (this.StartOf(5))
                        {
                            this.Expr(out expression);
                            if (expression != null)
                            {
                                parameters.Add(expression);
                            }
                            while (this.la.kind == 14)
                            {
                                base.lexer.NextToken();
                                item++;
                                this.Expr(out expression);
                                if (expression != null)
                                {
                                    parameters.Add(expression);
                                }
                            }
                            base.Expect(0x13);
                            list4.Add(item);
                            expression4.Arguments = parameters;
                            for (item = 0; this.la.kind == 0x12; item = 0)
                            {
                                base.lexer.NextToken();
                                while (this.la.kind == 14)
                                {
                                    base.lexer.NextToken();
                                    item++;
                                }
                                base.Expect(0x13);
                                list4.Add(item);
                            }
                            expression4.CreateType.RankSpecifier = list4.ToArray();
                            if (this.la.kind == 0x10)
                            {
                                this.ArrayInitializer(out expression);
                                expression4.ArrayInitializer = (ArrayInitializerExpression) expression;
                            }
                        }
                        else
                        {
                            base.SynErr(180);
                        }
                    }
                    else
                    {
                        base.SynErr(0xb5);
                    }
                }
                else if (this.la.kind == 0x72)
                {
                    base.lexer.NextToken();
                    base.Expect(20);
                    if (this.NotVoidPointer())
                    {
                        base.Expect(0x7a);
                        typeReference = new TypeReference("void");
                    }
                    else if (this.StartOf(9))
                    {
                        this.TypeWithRestriction(out typeReference, true, true);
                    }
                    else
                    {
                        base.SynErr(0xb6);
                    }
                    base.Expect(0x15);
                    pexpr = new TypeOfExpression(typeReference);
                }
                else if ((this.la.kind == 0x3e) && (this.Peek(1).kind == 20))
                {
                    base.Expect(0x3e);
                    base.Expect(20);
                    this.Type(out typeReference);
                    base.Expect(0x15);
                    pexpr = new DefaultValueExpression(typeReference);
                }
                else if (this.la.kind == 0x68)
                {
                    base.lexer.NextToken();
                    base.Expect(20);
                    this.Type(out typeReference);
                    base.Expect(0x15);
                    pexpr = new SizeOfExpression(typeReference);
                }
                else if (this.la.kind == 0x39)
                {
                    base.lexer.NextToken();
                    base.Expect(20);
                    this.Expr(out expression);
                    base.Expect(0x15);
                    pexpr = new CheckedExpression(expression);
                }
                else if (this.la.kind == 0x75)
                {
                    base.lexer.NextToken();
                    base.Expect(20);
                    this.Expr(out expression);
                    base.Expect(0x15);
                    pexpr = new UncheckedExpression(expression);
                }
                else if (this.la.kind == 0x3f)
                {
                    base.lexer.NextToken();
                    this.AnonymousMethodExpr(out expression);
                    pexpr = expression;
                }
                else
                {
                    base.SynErr(0xb7);
                }
            }
            else
            {
                string typeName = null;
                switch (this.la.kind)
                {
                    case 0x3d:
                        base.lexer.NextToken();
                        typeName = "decimal";
                        break;

                    case 0x41:
                        base.lexer.NextToken();
                        typeName = "double";
                        break;

                    case 0x4a:
                        base.lexer.NextToken();
                        typeName = "float";
                        break;

                    case 0x33:
                        base.lexer.NextToken();
                        typeName = "bool";
                        break;

                    case 0x35:
                        base.lexer.NextToken();
                        typeName = "byte";
                        break;

                    case 0x38:
                        base.lexer.NextToken();
                        typeName = "char";
                        break;

                    case 0x51:
                        base.lexer.NextToken();
                        typeName = "int";
                        break;

                    case 0x56:
                        base.lexer.NextToken();
                        typeName = "long";
                        break;

                    case 90:
                        base.lexer.NextToken();
                        typeName = "object";
                        break;

                    case 0x65:
                        base.lexer.NextToken();
                        typeName = "sbyte";
                        break;

                    case 0x67:
                        base.lexer.NextToken();
                        typeName = "short";
                        break;

                    case 0x6b:
                        base.lexer.NextToken();
                        typeName = "string";
                        break;

                    case 0x73:
                        base.lexer.NextToken();
                        typeName = "uint";
                        break;

                    case 0x74:
                        base.lexer.NextToken();
                        typeName = "ulong";
                        break;

                    case 0x77:
                        base.lexer.NextToken();
                        typeName = "ushort";
                        break;
                }
                this.t.val = "";
                base.Expect(15);
                base.Expect(1);
                pexpr = new FieldReferenceExpression(new TypeReferenceExpression(typeName), this.t.val);
            }
            while ((this.StartOf(0x1b) || (this.IsGenericFollowedBy(15) && this.IsTypeReferenceExpression(pexpr))) || this.IsGenericFollowedBy(20))
            {
                if ((this.la.kind == 0x1f) || (this.la.kind == 0x20))
                {
                    if (this.la.kind == 0x1f)
                    {
                        base.lexer.NextToken();
                        pexpr = new UnaryOperatorExpression(pexpr, UnaryOperatorType.PostIncrement);
                    }
                    else if (this.la.kind == 0x20)
                    {
                        base.lexer.NextToken();
                        pexpr = new UnaryOperatorExpression(pexpr, UnaryOperatorType.PostDecrement);
                    }
                    else
                    {
                        base.SynErr(0xb8);
                    }
                }
                else
                {
                    if (this.la.kind == 0x2f)
                    {
                        base.lexer.NextToken();
                        base.Expect(1);
                        pexpr = new PointerReferenceExpression(pexpr, this.t.val);
                        continue;
                    }
                    if (this.la.kind == 15)
                    {
                        base.lexer.NextToken();
                        base.Expect(1);
                        pexpr = new FieldReferenceExpression(pexpr, this.t.val);
                        continue;
                    }
                    if (this.IsGenericFollowedBy(15) && this.IsTypeReferenceExpression(pexpr))
                    {
                        this.TypeArgumentList(out types, false);
                        base.Expect(15);
                        base.Expect(1);
                        pexpr = new FieldReferenceExpression(this.GetTypeReferenceExpression(pexpr, types), this.t.val);
                        continue;
                    }
                    if (this.la.kind == 20)
                    {
                        base.lexer.NextToken();
                        List<Expression> arguments = new List<Expression>();
                        if (this.StartOf(0x15))
                        {
                            this.Argument(out expression);
                            if (expression != null)
                            {
                                arguments.Add(expression);
                            }
                            while (this.la.kind == 14)
                            {
                                base.lexer.NextToken();
                                this.Argument(out expression);
                                if (expression != null)
                                {
                                    arguments.Add(expression);
                                }
                            }
                        }
                        base.Expect(0x15);
                        pexpr = new InvocationExpression(pexpr, arguments);
                        continue;
                    }
                    if (this.IsGenericFollowedBy(20))
                    {
                        this.TypeArgumentList(out types, false);
                        base.Expect(20);
                        List<Expression> list6 = new List<Expression>();
                        if (this.StartOf(0x15))
                        {
                            this.Argument(out expression);
                            if (expression != null)
                            {
                                list6.Add(expression);
                            }
                            while (this.la.kind == 14)
                            {
                                base.lexer.NextToken();
                                this.Argument(out expression);
                                if (expression != null)
                                {
                                    list6.Add(expression);
                                }
                            }
                        }
                        base.Expect(0x15);
                        pexpr = new InvocationExpression(pexpr, list6, types);
                        continue;
                    }
                    if (flag)
                    {
                        this.Error("element access not allow on array creation");
                    }
                    List<Expression> list7 = new List<Expression>();
                    base.lexer.NextToken();
                    this.Expr(out expression);
                    if (expression != null)
                    {
                        list7.Add(expression);
                    }
                    while (this.la.kind == 14)
                    {
                        base.lexer.NextToken();
                        this.Expr(out expression);
                        if (expression != null)
                        {
                            list7.Add(expression);
                        }
                    }
                    base.Expect(0x13);
                    pexpr = new IndexerExpression(pexpr, list7);
                }
            }
        }

        private void Qualident(out string qualident)
        {
            base.Expect(1);
            this.qualidentBuilder.Length = 0;
            this.qualidentBuilder.Append(this.t.val);
            while (this.DotAndIdent())
            {
                base.Expect(15);
                base.Expect(1);
                this.qualidentBuilder.Append('.');
                this.qualidentBuilder.Append(this.t.val);
            }
            qualident = this.qualidentBuilder.ToString();
        }

        private void RelationalExpr(ref Expression outExpr)
        {
            BinaryOperatorType none = BinaryOperatorType.None;
            this.ShiftExpr(ref outExpr);
            while (this.StartOf(0x1c))
            {
                if (this.StartOf(0x1d))
                {
                    Expression expression;
                    if (this.la.kind == 0x17)
                    {
                        base.lexer.NextToken();
                        none = BinaryOperatorType.LessThan;
                    }
                    else if (this.la.kind == 0x16)
                    {
                        base.lexer.NextToken();
                        none = BinaryOperatorType.GreaterThan;
                    }
                    else if (this.la.kind == 0x24)
                    {
                        base.lexer.NextToken();
                        none = BinaryOperatorType.LessThanOrEqual;
                    }
                    else if (this.la.kind == 0x23)
                    {
                        base.lexer.NextToken();
                        none = BinaryOperatorType.GreaterThanOrEqual;
                    }
                    else
                    {
                        base.SynErr(0xba);
                    }
                    this.UnaryExpr(out expression);
                    this.ShiftExpr(ref expression);
                    outExpr = new BinaryOperatorExpression(outExpr, none, expression);
                }
                else
                {
                    TypeReference reference;
                    if (this.la.kind == 0x54)
                    {
                        base.lexer.NextToken();
                        this.TypeWithRestriction(out reference, false, false);
                        if ((this.la.kind == 12) && !Tokens.CastFollower[this.Peek(1).kind])
                        {
                            this.NullableQuestionMark(ref reference);
                        }
                        outExpr = new TypeOfIsExpression(outExpr, reference);
                    }
                    else
                    {
                        if (this.la.kind == 0x31)
                        {
                            base.lexer.NextToken();
                            this.TypeWithRestriction(out reference, false, false);
                            if ((this.la.kind == 12) && !Tokens.CastFollower[this.Peek(1).kind])
                            {
                                this.NullableQuestionMark(ref reference);
                            }
                            outExpr = new CastExpression(reference, outExpr, CastType.TryCast);
                            continue;
                        }
                        base.SynErr(0xbb);
                    }
                }
            }
        }

        private void RemoveAccessorDecl(out ICSharpCode.NRefactory.Parser.AST.Statement stmt)
        {
            stmt = null;
            base.Expect(1);
            if (this.t.val != "remove")
            {
                this.Error("remove expected");
            }
            this.Block(out stmt);
        }

        private void ResourceAcquisition(out ICSharpCode.NRefactory.Parser.AST.Statement stmt)
        {
            stmt = null;
            if (this.IsLocalVarDecl())
            {
                this.LocalVariableDecl(out stmt);
            }
            else if (this.StartOf(5))
            {
                Expression expression;
                this.Expr(out expression);
                stmt = new StatementExpression(expression);
            }
            else
            {
                base.SynErr(0xb0);
            }
        }

        private void SetAccessorDecl(out PropertySetRegion setBlock, List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes)
        {
            ICSharpCode.NRefactory.Parser.AST.Statement stmt = null;
            base.Expect(1);
            if (this.t.val != "set")
            {
                this.Error("set expected");
            }
            Point location = this.t.Location;
            if (this.la.kind == 0x10)
            {
                this.Block(out stmt);
            }
            else if (this.la.kind == 11)
            {
                base.lexer.NextToken();
            }
            else
            {
                base.SynErr(0xa6);
            }
            setBlock = new PropertySetRegion((BlockStatement) stmt, attributes);
            setBlock.StartLocation = location;
            setBlock.EndLocation = this.t.EndLocation;
        }

        private void ShiftExpr(ref Expression outExpr)
        {
            BinaryOperatorType none = BinaryOperatorType.None;
            this.AdditiveExpr(ref outExpr);
            while ((this.la.kind == 0x25) || this.IsShiftRight())
            {
                Expression expression;
                if (this.la.kind == 0x25)
                {
                    base.lexer.NextToken();
                    none = BinaryOperatorType.ShiftLeft;
                }
                else
                {
                    base.Expect(0x16);
                    base.Expect(0x16);
                    none = BinaryOperatorType.ShiftRight;
                }
                this.UnaryExpr(out expression);
                this.AdditiveExpr(ref expression);
                outExpr = new BinaryOperatorExpression(outExpr, none, expression);
            }
        }

        private void SimpleType(out string name)
        {
            name = string.Empty;
            if (this.StartOf(0x13))
            {
                this.IntegralType(out name);
            }
            else if (this.la.kind == 0x4a)
            {
                base.lexer.NextToken();
                name = "float";
            }
            else if (this.la.kind == 0x41)
            {
                base.lexer.NextToken();
                name = "double";
            }
            else if (this.la.kind == 0x3d)
            {
                base.lexer.NextToken();
                name = "decimal";
            }
            else if (this.la.kind == 0x33)
            {
                base.lexer.NextToken();
                name = "bool";
            }
            else
            {
                base.SynErr(0x9d);
            }
        }

        private bool SkipGeneric(ref Token pt)
        {
            if (pt.kind == 0x17)
            {
                int num = 1;
                while (num != 0)
                {
                    pt = this.Peek();
                    if (pt.kind == 0x16)
                    {
                        num--;
                    }
                    else
                    {
                        if (pt.kind == 0x17)
                        {
                            num++;
                            continue;
                        }
                        if (((pt.kind == 11) || (pt.kind == 0x10)) || ((pt.kind == 0x11) || (pt.kind == 0)))
                        {
                            return false;
                        }
                    }
                }
                pt = this.Peek();
            }
            return true;
        }

        private bool SkipQuestionMark(ref Token pt)
        {
            if (pt.kind == 12)
            {
                pt = this.Peek();
            }
            return true;
        }

        protected bool StartOf(int s)
        {
            return set[s, base.lexer.LookAhead.kind];
        }

        private void StartPeek()
        {
            base.lexer.StartPeek();
        }

        private void Statement()
        {
            ICSharpCode.NRefactory.Parser.AST.Statement stmt = null;
            Point location = this.la.Location;
            if (this.IsLabel())
            {
                base.Expect(1);
                base.compilationUnit.AddChild(new LabelStatement(this.t.val));
                base.Expect(9);
                this.Statement();
            }
            else if (this.la.kind == 0x3b)
            {
                TypeReference reference;
                Expression expression;
                base.lexer.NextToken();
                this.Type(out reference);
                LocalVariableDeclaration childNode = new LocalVariableDeclaration(reference, Modifier.Const);
                string name = null;
                childNode.StartLocation = this.t.Location;
                base.Expect(1);
                name = this.t.val;
                base.Expect(3);
                this.Expr(out expression);
                childNode.Variables.Add(new VariableDeclaration(name, expression));
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    base.Expect(1);
                    name = this.t.val;
                    base.Expect(3);
                    this.Expr(out expression);
                    childNode.Variables.Add(new VariableDeclaration(name, expression));
                }
                base.Expect(11);
                base.compilationUnit.AddChild(childNode);
            }
            else if (this.IsLocalVarDecl())
            {
                this.LocalVariableDecl(out stmt);
                base.Expect(11);
                base.compilationUnit.AddChild(stmt);
            }
            else if (this.StartOf(0x17))
            {
                this.EmbeddedStatement(out stmt);
                base.compilationUnit.AddChild(stmt);
            }
            else
            {
                base.SynErr(0xa8);
            }
            if (stmt != null)
            {
                stmt.StartLocation = location;
                stmt.EndLocation = this.t.EndLocation;
            }
        }

        private void StatementExpr(out ICSharpCode.NRefactory.Parser.AST.Statement stmt)
        {
            Expression expression;
            this.Expr(out expression);
            stmt = new StatementExpression(expression);
        }

        private void StructBody()
        {
            base.Expect(0x10);
            while (this.StartOf(13))
            {
                List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
                Modifiers m = new Modifiers();
                while (this.la.kind == 0x12)
                {
                    ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
                    this.AttributeSection(out section);
                    attributes.Add(section);
                }
                while (this.StartOf(12))
                {
                    this.MemberModifier(m);
                }
                this.StructMemberDecl(m, attributes);
            }
            base.Expect(0x11);
        }

        private void StructInterfaces(out List<TypeReference> names)
        {
            TypeReference reference;
            names = new List<TypeReference>();
            base.Expect(9);
            this.TypeName(out reference, false);
            if (reference != null)
            {
                names.Add(reference);
            }
            while (this.la.kind == 14)
            {
                base.lexer.NextToken();
                this.TypeName(out reference, false);
                if (reference != null)
                {
                    names.Add(reference);
                }
            }
        }

        private void StructMemberDecl(Modifiers m, List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes)
        {
            string name = null;
            TypeReference reference;
            List<ParameterDeclarationExpression> parameter = new List<ParameterDeclarationExpression>();
            ICSharpCode.NRefactory.Parser.AST.Statement stmt = null;
            List<VariableDeclaration> fieldDeclaration = new List<VariableDeclaration>();
            List<TemplateDefinition> templates = new List<TemplateDefinition>();
            TypeReference typeRef = null;
            if (this.la.kind == 0x3b)
            {
                Expression expression;
                m.Check(Modifier.VBInterfacs);
                base.lexer.NextToken();
                Point location = this.t.Location;
                this.Type(out reference);
                base.Expect(1);
                FieldDeclaration childNode = new FieldDeclaration(attributes, reference, m.Modifier | Modifier.Const);
                childNode.StartLocation = m.GetDeclarationLocation(location);
                VariableDeclaration item = new VariableDeclaration(this.t.val);
                childNode.Fields.Add(item);
                base.Expect(3);
                this.Expr(out expression);
                item.Initializer = expression;
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    base.Expect(1);
                    item = new VariableDeclaration(this.t.val);
                    childNode.Fields.Add(item);
                    base.Expect(3);
                    this.Expr(out expression);
                    item.Initializer = expression;
                }
                base.Expect(11);
                childNode.EndLocation = this.t.EndLocation;
                base.compilationUnit.AddChild(childNode);
            }
            else if (this.NotVoidPointer())
            {
                m.Check(Modifier.PropertysEventsMethods);
                base.Expect(0x7a);
                Point keywordLocation = this.t.Location;
                if (this.IsExplicitInterfaceImplementation())
                {
                    this.TypeName(out typeRef, false);
                    if ((this.la.kind != 15) || (this.Peek(1).kind != 110))
                    {
                        name = TypeReference.StripLastIdentifierFromType(ref typeRef);
                    }
                }
                else if (this.la.kind == 1)
                {
                    base.lexer.NextToken();
                    name = this.t.val;
                }
                else
                {
                    base.SynErr(140);
                }
                if (this.la.kind == 0x17)
                {
                    this.TypeParameterList(templates);
                }
                base.Expect(20);
                if (this.StartOf(10))
                {
                    this.FormalParameterList(parameter);
                }
                base.Expect(0x15);
                MethodDeclaration declaration3 = new MethodDeclaration(name, m.Modifier, new TypeReference("void"), parameter, attributes);
                declaration3.StartLocation = m.GetDeclarationLocation(keywordLocation);
                declaration3.EndLocation = this.t.EndLocation;
                declaration3.Templates = templates;
                if (typeRef != null)
                {
                    declaration3.InterfaceImplementations.Add(new InterfaceImplementation(typeRef, name));
                }
                base.compilationUnit.AddChild(declaration3);
                base.compilationUnit.BlockStart(declaration3);
                while (this.IdentIsWhere())
                {
                    this.TypeParameterConstraintsClause(templates);
                }
                if (this.la.kind == 0x10)
                {
                    this.Block(out stmt);
                }
                else if (this.la.kind == 11)
                {
                    base.lexer.NextToken();
                }
                else
                {
                    base.SynErr(0x8d);
                }
                base.compilationUnit.BlockEnd();
                declaration3.Body = (BlockStatement) stmt;
            }
            else if (this.la.kind == 0x44)
            {
                m.Check(Modifier.PropertysEventsMethods);
                base.lexer.NextToken();
                EventDeclaration declaration4 = new EventDeclaration(null, null, m.Modifier, attributes, null);
                declaration4.StartLocation = this.t.Location;
                base.compilationUnit.AddChild(declaration4);
                base.compilationUnit.BlockStart(declaration4);
                EventAddRegion addBlock = null;
                EventRemoveRegion removeBlock = null;
                this.Type(out reference);
                declaration4.TypeReference = reference;
                if (this.IsExplicitInterfaceImplementation())
                {
                    this.TypeName(out typeRef, false);
                    name = TypeReference.StripLastIdentifierFromType(ref typeRef);
                    declaration4.InterfaceImplementations.Add(new InterfaceImplementation(typeRef, name));
                }
                else if (this.la.kind == 1)
                {
                    base.lexer.NextToken();
                    name = this.t.val;
                }
                else
                {
                    base.SynErr(0x8e);
                }
                declaration4.Name = name;
                declaration4.EndLocation = this.t.EndLocation;
                if (this.la.kind == 0x10)
                {
                    base.lexer.NextToken();
                    declaration4.BodyStart = this.t.Location;
                    this.EventAccessorDecls(out addBlock, out removeBlock);
                    base.Expect(0x11);
                    declaration4.BodyEnd = this.t.EndLocation;
                }
                if (this.la.kind == 11)
                {
                    base.lexer.NextToken();
                }
                base.compilationUnit.BlockEnd();
                declaration4.AddRegion = addBlock;
                declaration4.RemoveRegion = removeBlock;
            }
            else if (this.IdentAndLPar())
            {
                m.Check(Modifier.StaticConstructors | Modifier.Visibility);
                base.Expect(1);
                string val = this.t.val;
                Point point3 = this.t.Location;
                base.Expect(20);
                if (this.StartOf(10))
                {
                    m.Check(Modifier.Constructors);
                    this.FormalParameterList(parameter);
                }
                base.Expect(0x15);
                ICSharpCode.NRefactory.Parser.AST.ConstructorInitializer ci = null;
                if (this.la.kind == 9)
                {
                    m.Check(Modifier.Constructors);
                    this.ConstructorInitializer(out ci);
                }
                ConstructorDeclaration declaration5 = new ConstructorDeclaration(val, m.Modifier, parameter, ci, attributes);
                declaration5.StartLocation = point3;
                declaration5.EndLocation = this.t.EndLocation;
                if (this.la.kind == 0x10)
                {
                    this.Block(out stmt);
                }
                else if (this.la.kind == 11)
                {
                    base.lexer.NextToken();
                }
                else
                {
                    base.SynErr(0x8f);
                }
                declaration5.Body = (BlockStatement) stmt;
                base.compilationUnit.AddChild(declaration5);
            }
            else if ((this.la.kind == 0x45) || (this.la.kind == 0x4f))
            {
                m.Check(Modifier.Operators);
                if (m.isNone)
                {
                    this.Error("at least one modifier must be set");
                }
                bool flag = true;
                Point empty = Point.Empty;
                if (this.la.kind == 0x4f)
                {
                    base.lexer.NextToken();
                    empty = this.t.Location;
                }
                else
                {
                    base.lexer.NextToken();
                    flag = false;
                    empty = this.t.Location;
                }
                base.Expect(0x5b);
                this.Type(out reference);
                TypeReference typeReference = reference;
                base.Expect(20);
                this.Type(out reference);
                base.Expect(1);
                string parameterName = this.t.val;
                base.Expect(0x15);
                Point point5 = this.t.Location;
                if (this.la.kind == 0x10)
                {
                    this.Block(out stmt);
                }
                else if (this.la.kind == 11)
                {
                    base.lexer.NextToken();
                    stmt = null;
                }
                else
                {
                    base.SynErr(0x90);
                }
                List<ParameterDeclarationExpression> parameters = new List<ParameterDeclarationExpression>();
                parameters.Add(new ParameterDeclarationExpression(reference, parameterName));
                OperatorDeclaration declaration6 = new OperatorDeclaration(m.Modifier, attributes, parameters, typeReference, flag ? ConversionType.Implicit : ConversionType.Explicit);
                declaration6.Body = (BlockStatement) stmt;
                declaration6.StartLocation = m.GetDeclarationLocation(empty);
                declaration6.EndLocation = point5;
                base.compilationUnit.AddChild(declaration6);
            }
            else if (this.StartOf(0x11))
            {
                this.TypeDecl(m, attributes);
            }
            else if (this.StartOf(9))
            {
                this.Type(out reference);
                Point point6 = this.t.Location;
                if (this.la.kind == 0x5b)
                {
                    OverloadableOperatorType type;
                    TypeReference reference4;
                    m.Check(Modifier.Operators);
                    if (m.isNone)
                    {
                        this.Error("at least one modifier must be set");
                    }
                    base.lexer.NextToken();
                    this.OverloadableOperator(out type);
                    TypeReference reference5 = null;
                    string str4 = null;
                    base.Expect(20);
                    this.Type(out reference4);
                    base.Expect(1);
                    string str5 = this.t.val;
                    if (this.la.kind == 14)
                    {
                        base.lexer.NextToken();
                        this.Type(out reference5);
                        base.Expect(1);
                        str4 = this.t.val;
                    }
                    else if (this.la.kind != 0x15)
                    {
                        base.SynErr(0x91);
                    }
                    Point point7 = this.t.Location;
                    base.Expect(0x15);
                    if (this.la.kind == 0x10)
                    {
                        this.Block(out stmt);
                    }
                    else if (this.la.kind == 11)
                    {
                        base.lexer.NextToken();
                    }
                    else
                    {
                        base.SynErr(0x92);
                    }
                    List<ParameterDeclarationExpression> list5 = new List<ParameterDeclarationExpression>();
                    list5.Add(new ParameterDeclarationExpression(reference4, str5));
                    if (reference5 != null)
                    {
                        list5.Add(new ParameterDeclarationExpression(reference5, str4));
                    }
                    OperatorDeclaration declaration7 = new OperatorDeclaration(m.Modifier, attributes, list5, reference, type);
                    declaration7.Body = (BlockStatement) stmt;
                    declaration7.StartLocation = m.GetDeclarationLocation(point6);
                    declaration7.EndLocation = point7;
                    base.compilationUnit.AddChild(declaration7);
                }
                else if (this.IsVarDecl())
                {
                    m.Check(Modifier.Fields);
                    FieldDeclaration declaration8 = new FieldDeclaration(attributes, reference, m.Modifier);
                    declaration8.StartLocation = m.GetDeclarationLocation(point6);
                    this.VariableDeclarator(fieldDeclaration);
                    while (this.la.kind == 14)
                    {
                        base.lexer.NextToken();
                        this.VariableDeclarator(fieldDeclaration);
                    }
                    base.Expect(11);
                    declaration8.EndLocation = this.t.EndLocation;
                    declaration8.Fields = fieldDeclaration;
                    base.compilationUnit.AddChild(declaration8);
                }
                else if (this.la.kind == 110)
                {
                    PropertyGetRegion region3;
                    PropertySetRegion region4;
                    m.Check(Modifier.Indexers);
                    base.lexer.NextToken();
                    base.Expect(0x12);
                    this.FormalParameterList(parameter);
                    base.Expect(0x13);
                    Point endLocation = this.t.EndLocation;
                    base.Expect(0x10);
                    IndexerDeclaration declaration9 = new IndexerDeclaration(reference, parameter, m.Modifier, attributes);
                    declaration9.StartLocation = point6;
                    declaration9.EndLocation = endLocation;
                    declaration9.BodyStart = this.t.Location;
                    this.AccessorDecls(out region3, out region4);
                    base.Expect(0x11);
                    declaration9.BodyEnd = this.t.EndLocation;
                    declaration9.GetRegion = region3;
                    declaration9.SetRegion = region4;
                    base.compilationUnit.AddChild(declaration9);
                }
                else if (this.la.kind == 1)
                {
                    if (this.IsExplicitInterfaceImplementation())
                    {
                        this.TypeName(out typeRef, false);
                        if ((this.la.kind != 15) || (this.Peek(1).kind != 110))
                        {
                            name = TypeReference.StripLastIdentifierFromType(ref typeRef);
                        }
                    }
                    else if (this.la.kind == 1)
                    {
                        base.lexer.NextToken();
                        name = this.t.val;
                    }
                    else
                    {
                        base.SynErr(0x93);
                    }
                    Point point9 = this.t.EndLocation;
                    if (((this.la.kind == 0x10) || (this.la.kind == 20)) || (this.la.kind == 0x17))
                    {
                        if ((this.la.kind == 20) || (this.la.kind == 0x17))
                        {
                            m.Check(Modifier.PropertysEventsMethods);
                            if (this.la.kind == 0x17)
                            {
                                this.TypeParameterList(templates);
                            }
                            base.Expect(20);
                            if (this.StartOf(10))
                            {
                                this.FormalParameterList(parameter);
                            }
                            base.Expect(0x15);
                            MethodDeclaration declaration10 = new MethodDeclaration(name, m.Modifier, reference, parameter, attributes);
                            if (typeRef != null)
                            {
                                declaration10.InterfaceImplementations.Add(new InterfaceImplementation(typeRef, name));
                            }
                            declaration10.StartLocation = m.GetDeclarationLocation(point6);
                            declaration10.EndLocation = this.t.EndLocation;
                            declaration10.Templates = templates;
                            base.compilationUnit.AddChild(declaration10);
                            while (this.IdentIsWhere())
                            {
                                this.TypeParameterConstraintsClause(templates);
                            }
                            if (this.la.kind == 0x10)
                            {
                                this.Block(out stmt);
                            }
                            else if (this.la.kind == 11)
                            {
                                base.lexer.NextToken();
                            }
                            else
                            {
                                base.SynErr(0x94);
                            }
                            declaration10.Body = (BlockStatement) stmt;
                        }
                        else
                        {
                            PropertyGetRegion region5;
                            PropertySetRegion region6;
                            base.lexer.NextToken();
                            PropertyDeclaration declaration11 = new PropertyDeclaration(name, reference, m.Modifier, attributes);
                            if (typeRef != null)
                            {
                                declaration11.InterfaceImplementations.Add(new InterfaceImplementation(typeRef, name));
                            }
                            declaration11.StartLocation = m.GetDeclarationLocation(point6);
                            declaration11.EndLocation = point9;
                            declaration11.BodyStart = this.t.Location;
                            this.AccessorDecls(out region5, out region6);
                            base.Expect(0x11);
                            declaration11.GetRegion = region5;
                            declaration11.SetRegion = region6;
                            declaration11.BodyEnd = this.t.EndLocation;
                            base.compilationUnit.AddChild(declaration11);
                        }
                    }
                    else if (this.la.kind == 15)
                    {
                        PropertyGetRegion region7;
                        PropertySetRegion region8;
                        m.Check(Modifier.Indexers);
                        base.lexer.NextToken();
                        base.Expect(110);
                        base.Expect(0x12);
                        this.FormalParameterList(parameter);
                        base.Expect(0x13);
                        IndexerDeclaration declaration12 = new IndexerDeclaration(reference, parameter, m.Modifier, attributes);
                        declaration12.StartLocation = m.GetDeclarationLocation(point6);
                        declaration12.EndLocation = this.t.EndLocation;
                        if (typeRef != null)
                        {
                            declaration12.InterfaceImplementations.Add(new InterfaceImplementation(typeRef, "this"));
                        }
                        base.Expect(0x10);
                        Point point10 = this.t.Location;
                        this.AccessorDecls(out region7, out region8);
                        base.Expect(0x11);
                        declaration12.BodyStart = point10;
                        declaration12.BodyEnd = this.t.EndLocation;
                        declaration12.GetRegion = region7;
                        declaration12.SetRegion = region8;
                        base.compilationUnit.AddChild(declaration12);
                    }
                    else
                    {
                        base.SynErr(0x95);
                    }
                }
                else
                {
                    base.SynErr(150);
                }
            }
            else
            {
                base.SynErr(0x97);
            }
        }

        private void SwitchLabel(out CaseLabel label)
        {
            Expression expr = null;
            label = null;
            if (this.la.kind == 0x36)
            {
                base.lexer.NextToken();
                this.Expr(out expr);
                base.Expect(9);
                label = new CaseLabel(expr);
            }
            else if (this.la.kind == 0x3e)
            {
                base.lexer.NextToken();
                base.Expect(9);
                label = new CaseLabel();
            }
            else
            {
                base.SynErr(0xb1);
            }
        }

        private void SwitchSection(out ICSharpCode.NRefactory.Parser.AST.SwitchSection stmt)
        {
            CaseLabel label;
            ICSharpCode.NRefactory.Parser.AST.SwitchSection block = new ICSharpCode.NRefactory.Parser.AST.SwitchSection();
            this.SwitchLabel(out label);
            block.SwitchLabels.Add(label);
            while ((this.la.kind == 0x36) || (this.la.kind == 0x3e))
            {
                this.SwitchLabel(out label);
                block.SwitchLabels.Add(label);
            }
            base.compilationUnit.BlockStart(block);
            this.Statement();
            while (this.StartOf(20))
            {
                this.Statement();
            }
            base.compilationUnit.BlockEnd();
            stmt = block;
        }

        protected override void SynErr(int line, int col, int errorNumber)
        {
            string str;
            base.errors.count++;
            switch (errorNumber)
            {
                case 0:
                    str = "EOF expected";
                    break;

                case 1:
                    str = "ident expected";
                    break;

                case 2:
                    str = "Literal expected";
                    break;

                case 3:
                    str = "\"=\" expected";
                    break;

                case 4:
                    str = "\"+\" expected";
                    break;

                case 5:
                    str = "\"-\" expected";
                    break;

                case 6:
                    str = "\"*\" expected";
                    break;

                case 7:
                    str = "\"/\" expected";
                    break;

                case 8:
                    str = "\"%\" expected";
                    break;

                case 9:
                    str = "\":\" expected";
                    break;

                case 10:
                    str = "\"::\" expected";
                    break;

                case 11:
                    str = "\";\" expected";
                    break;

                case 12:
                    str = "\"?\" expected";
                    break;

                case 13:
                    str = "\"??\" expected";
                    break;

                case 14:
                    str = "\",\" expected";
                    break;

                case 15:
                    str = "\".\" expected";
                    break;

                case 0x10:
                    str = "\"{\" expected";
                    break;

                case 0x11:
                    str = "\"}\" expected";
                    break;

                case 0x12:
                    str = "\"[\" expected";
                    break;

                case 0x13:
                    str = "\"]\" expected";
                    break;

                case 20:
                    str = "\"(\" expected";
                    break;

                case 0x15:
                    str = "\")\" expected";
                    break;

                case 0x16:
                    str = "\">\" expected";
                    break;

                case 0x17:
                    str = "\"<\" expected";
                    break;

                case 0x18:
                    str = "\"!\" expected";
                    break;

                case 0x19:
                    str = "\"&&\" expected";
                    break;

                case 0x1a:
                    str = "\"||\" expected";
                    break;

                case 0x1b:
                    str = "\"~\" expected";
                    break;

                case 0x1c:
                    str = "\"&\" expected";
                    break;

                case 0x1d:
                    str = "\"|\" expected";
                    break;

                case 30:
                    str = "\"^\" expected";
                    break;

                case 0x1f:
                    str = "\"++\" expected";
                    break;

                case 0x20:
                    str = "\"--\" expected";
                    break;

                case 0x21:
                    str = "\"==\" expected";
                    break;

                case 0x22:
                    str = "\"!=\" expected";
                    break;

                case 0x23:
                    str = "\">=\" expected";
                    break;

                case 0x24:
                    str = "\"<=\" expected";
                    break;

                case 0x25:
                    str = "\"<<\" expected";
                    break;

                case 0x26:
                    str = "\"+=\" expected";
                    break;

                case 0x27:
                    str = "\"-=\" expected";
                    break;

                case 40:
                    str = "\"*=\" expected";
                    break;

                case 0x29:
                    str = "\"/=\" expected";
                    break;

                case 0x2a:
                    str = "\"%=\" expected";
                    break;

                case 0x2b:
                    str = "\"&=\" expected";
                    break;

                case 0x2c:
                    str = "\"|=\" expected";
                    break;

                case 0x2d:
                    str = "\"^=\" expected";
                    break;

                case 0x2e:
                    str = "\"<<=\" expected";
                    break;

                case 0x2f:
                    str = "\"->\" expected";
                    break;

                case 0x30:
                    str = "\"abstract\" expected";
                    break;

                case 0x31:
                    str = "\"as\" expected";
                    break;

                case 50:
                    str = "\"base\" expected";
                    break;

                case 0x33:
                    str = "\"bool\" expected";
                    break;

                case 0x34:
                    str = "\"break\" expected";
                    break;

                case 0x35:
                    str = "\"byte\" expected";
                    break;

                case 0x36:
                    str = "\"case\" expected";
                    break;

                case 0x37:
                    str = "\"catch\" expected";
                    break;

                case 0x38:
                    str = "\"char\" expected";
                    break;

                case 0x39:
                    str = "\"checked\" expected";
                    break;

                case 0x3a:
                    str = "\"class\" expected";
                    break;

                case 0x3b:
                    str = "\"const\" expected";
                    break;

                case 60:
                    str = "\"continue\" expected";
                    break;

                case 0x3d:
                    str = "\"decimal\" expected";
                    break;

                case 0x3e:
                    str = "\"default\" expected";
                    break;

                case 0x3f:
                    str = "\"delegate\" expected";
                    break;

                case 0x40:
                    str = "\"do\" expected";
                    break;

                case 0x41:
                    str = "\"double\" expected";
                    break;

                case 0x42:
                    str = "\"else\" expected";
                    break;

                case 0x43:
                    str = "\"enum\" expected";
                    break;

                case 0x44:
                    str = "\"event\" expected";
                    break;

                case 0x45:
                    str = "\"explicit\" expected";
                    break;

                case 70:
                    str = "\"extern\" expected";
                    break;

                case 0x47:
                    str = "\"false\" expected";
                    break;

                case 0x48:
                    str = "\"finally\" expected";
                    break;

                case 0x49:
                    str = "\"fixed\" expected";
                    break;

                case 0x4a:
                    str = "\"float\" expected";
                    break;

                case 0x4b:
                    str = "\"for\" expected";
                    break;

                case 0x4c:
                    str = "\"foreach\" expected";
                    break;

                case 0x4d:
                    str = "\"goto\" expected";
                    break;

                case 0x4e:
                    str = "\"if\" expected";
                    break;

                case 0x4f:
                    str = "\"implicit\" expected";
                    break;

                case 80:
                    str = "\"in\" expected";
                    break;

                case 0x51:
                    str = "\"int\" expected";
                    break;

                case 0x52:
                    str = "\"interface\" expected";
                    break;

                case 0x53:
                    str = "\"internal\" expected";
                    break;

                case 0x54:
                    str = "\"is\" expected";
                    break;

                case 0x55:
                    str = "\"lock\" expected";
                    break;

                case 0x56:
                    str = "\"long\" expected";
                    break;

                case 0x57:
                    str = "\"namespace\" expected";
                    break;

                case 0x58:
                    str = "\"new\" expected";
                    break;

                case 0x59:
                    str = "\"null\" expected";
                    break;

                case 90:
                    str = "\"object\" expected";
                    break;

                case 0x5b:
                    str = "\"operator\" expected";
                    break;

                case 0x5c:
                    str = "\"out\" expected";
                    break;

                case 0x5d:
                    str = "\"override\" expected";
                    break;

                case 0x5e:
                    str = "\"params\" expected";
                    break;

                case 0x5f:
                    str = "\"private\" expected";
                    break;

                case 0x60:
                    str = "\"protected\" expected";
                    break;

                case 0x61:
                    str = "\"public\" expected";
                    break;

                case 0x62:
                    str = "\"readonly\" expected";
                    break;

                case 0x63:
                    str = "\"ref\" expected";
                    break;

                case 100:
                    str = "\"return\" expected";
                    break;

                case 0x65:
                    str = "\"sbyte\" expected";
                    break;

                case 0x66:
                    str = "\"sealed\" expected";
                    break;

                case 0x67:
                    str = "\"short\" expected";
                    break;

                case 0x68:
                    str = "\"sizeof\" expected";
                    break;

                case 0x69:
                    str = "\"stackalloc\" expected";
                    break;

                case 0x6a:
                    str = "\"static\" expected";
                    break;

                case 0x6b:
                    str = "\"string\" expected";
                    break;

                case 0x6c:
                    str = "\"struct\" expected";
                    break;

                case 0x6d:
                    str = "\"switch\" expected";
                    break;

                case 110:
                    str = "\"this\" expected";
                    break;

                case 0x6f:
                    str = "\"throw\" expected";
                    break;

                case 0x70:
                    str = "\"true\" expected";
                    break;

                case 0x71:
                    str = "\"try\" expected";
                    break;

                case 0x72:
                    str = "\"typeof\" expected";
                    break;

                case 0x73:
                    str = "\"uint\" expected";
                    break;

                case 0x74:
                    str = "\"ulong\" expected";
                    break;

                case 0x75:
                    str = "\"unchecked\" expected";
                    break;

                case 0x76:
                    str = "\"unsafe\" expected";
                    break;

                case 0x77:
                    str = "\"ushort\" expected";
                    break;

                case 120:
                    str = "\"using\" expected";
                    break;

                case 0x79:
                    str = "\"virtual\" expected";
                    break;

                case 0x7a:
                    str = "\"void\" expected";
                    break;

                case 0x7b:
                    str = "\"volatile\" expected";
                    break;

                case 0x7c:
                    str = "\"while\" expected";
                    break;

                case 0x7d:
                    str = "??? expected";
                    break;

                case 0x7e:
                    str = "invalid NamespaceMemberDecl";
                    break;

                case 0x7f:
                    str = "invalid NonArrayType";
                    break;

                case 0x80:
                    str = "invalid AttributeArguments";
                    break;

                case 0x81:
                    str = "invalid Expr";
                    break;

                case 130:
                    str = "invalid TypeModifier";
                    break;

                case 0x83:
                    str = "invalid TypeDecl";
                    break;

                case 0x84:
                    str = "invalid TypeDecl";
                    break;

                case 0x85:
                    str = "invalid IntegralType";
                    break;

                case 0x86:
                    str = "invalid FormalParameterList";
                    break;

                case 0x87:
                    str = "invalid FormalParameterList";
                    break;

                case 0x88:
                    str = "invalid ClassType";
                    break;

                case 0x89:
                    str = "invalid MemberModifier";
                    break;

                case 0x8a:
                    str = "invalid ClassMemberDecl";
                    break;

                case 0x8b:
                    str = "invalid ClassMemberDecl";
                    break;

                case 140:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x8d:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x8e:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x8f:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x90:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x91:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x92:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x93:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x94:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x95:
                    str = "invalid StructMemberDecl";
                    break;

                case 150:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x97:
                    str = "invalid StructMemberDecl";
                    break;

                case 0x98:
                    str = "invalid InterfaceMemberDecl";
                    break;

                case 0x99:
                    str = "invalid InterfaceMemberDecl";
                    break;

                case 0x9a:
                    str = "invalid InterfaceMemberDecl";
                    break;

                case 0x9b:
                    str = "invalid TypeWithRestriction";
                    break;

                case 0x9c:
                    str = "invalid TypeWithRestriction";
                    break;

                case 0x9d:
                    str = "invalid SimpleType";
                    break;

                case 0x9e:
                    str = "invalid AccessorModifiers";
                    break;

                case 0x9f:
                    str = "invalid EventAccessorDecls";
                    break;

                case 160:
                    str = "invalid ConstructorInitializer";
                    break;

                case 0xa1:
                    str = "invalid OverloadableOperator";
                    break;

                case 0xa2:
                    str = "invalid AccessorDecls";
                    break;

                case 0xa3:
                    str = "invalid InterfaceAccessors";
                    break;

                case 0xa4:
                    str = "invalid InterfaceAccessors";
                    break;

                case 0xa5:
                    str = "invalid GetAccessorDecl";
                    break;

                case 0xa6:
                    str = "invalid SetAccessorDecl";
                    break;

                case 0xa7:
                    str = "invalid VariableInitializer";
                    break;

                case 0xa8:
                    str = "invalid Statement";
                    break;

                case 0xa9:
                    str = "invalid AssignmentOperator";
                    break;

                case 170:
                    str = "invalid EmbeddedStatement";
                    break;

                case 0xab:
                    str = "invalid EmbeddedStatement";
                    break;

                case 0xac:
                    str = "invalid EmbeddedStatement";
                    break;

                case 0xad:
                    str = "invalid ForInitializer";
                    break;

                case 0xae:
                    str = "invalid GotoStatement";
                    break;

                case 0xaf:
                    str = "invalid TryStatement";
                    break;

                case 0xb0:
                    str = "invalid ResourceAcquisition";
                    break;

                case 0xb1:
                    str = "invalid SwitchLabel";
                    break;

                case 0xb2:
                    str = "invalid CatchClauses";
                    break;

                case 0xb3:
                    str = "invalid PrimaryExpr";
                    break;

                case 180:
                    str = "invalid PrimaryExpr";
                    break;

                case 0xb5:
                    str = "invalid PrimaryExpr";
                    break;

                case 0xb6:
                    str = "invalid PrimaryExpr";
                    break;

                case 0xb7:
                    str = "invalid PrimaryExpr";
                    break;

                case 0xb8:
                    str = "invalid PrimaryExpr";
                    break;

                case 0xb9:
                    str = "invalid TypeArgumentList";
                    break;

                case 0xba:
                    str = "invalid RelationalExpr";
                    break;

                case 0xbb:
                    str = "invalid RelationalExpr";
                    break;

                case 0xbc:
                    str = "invalid TypeParameterConstraintsClauseBase";
                    break;

                default:
                    str = "error " + errorNumber;
                    break;
            }
            base.errors.Error(line, col, str);
        }

        private bool TimesOrLBrackAndCommaOrRBrack()
        {
            if (this.la.kind != 6)
            {
                return this.LBrackAndCommaOrRBrack();
            }
            return true;
        }

        private void TryStatement(out ICSharpCode.NRefactory.Parser.AST.Statement tryStatement)
        {
            ICSharpCode.NRefactory.Parser.AST.Statement stmt = null;
            ICSharpCode.NRefactory.Parser.AST.Statement statement2 = null;
            List<CatchClause> catchClauses = null;
            base.Expect(0x71);
            this.Block(out stmt);
            if (this.la.kind == 0x37)
            {
                this.CatchClauses(out catchClauses);
                if (this.la.kind == 0x48)
                {
                    base.lexer.NextToken();
                    this.Block(out statement2);
                }
            }
            else if (this.la.kind == 0x48)
            {
                base.lexer.NextToken();
                this.Block(out statement2);
            }
            else
            {
                base.SynErr(0xaf);
            }
            tryStatement = new TryCatchStatement(stmt, catchClauses, statement2);
        }

        private void Type(out TypeReference type)
        {
            this.TypeWithRestriction(out type, true, false);
        }

        private void TypeArgumentList(out List<TypeReference> types, bool canBeUnbound)
        {
            types = new List<TypeReference>();
            TypeReference type = null;
            base.Expect(0x17);
            if (canBeUnbound && ((this.la.kind == 0x16) || (this.la.kind == 14)))
            {
                types.Add(TypeReference.Null);
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    types.Add(TypeReference.Null);
                }
            }
            else if (this.StartOf(9))
            {
                this.Type(out type);
                types.Add(type);
                while (this.la.kind == 14)
                {
                    base.lexer.NextToken();
                    this.Type(out type);
                    types.Add(type);
                }
            }
            else
            {
                base.SynErr(0xb9);
            }
            base.Expect(0x16);
        }

        private void TypeDecl(Modifiers m, List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes)
        {
            List<TypeReference> list;
            List<TemplateDefinition> templates;
            List<ParameterDeclarationExpression> parameter = new List<ParameterDeclarationExpression>();
            if (this.la.kind == 0x3a)
            {
                m.Check(Modifier.Classes);
                base.lexer.NextToken();
                TypeDeclaration childNode = new TypeDeclaration(m.Modifier, attributes);
                templates = childNode.Templates;
                base.compilationUnit.AddChild(childNode);
                base.compilationUnit.BlockStart(childNode);
                childNode.StartLocation = m.GetDeclarationLocation(this.t.Location);
                childNode.Type = ICSharpCode.NRefactory.Parser.AST.ClassType.Class;
                base.Expect(1);
                childNode.Name = this.t.val;
                if (this.la.kind == 0x17)
                {
                    this.TypeParameterList(templates);
                }
                if (this.la.kind == 9)
                {
                    this.ClassBase(out list);
                    childNode.BaseTypes = list;
                }
                while (this.IdentIsWhere())
                {
                    this.TypeParameterConstraintsClause(templates);
                }
                this.ClassBody();
                if (this.la.kind == 11)
                {
                    base.lexer.NextToken();
                }
                childNode.EndLocation = this.t.Location;
                base.compilationUnit.BlockEnd();
            }
            else if (this.StartOf(8))
            {
                m.Check(Modifier.StructsInterfacesEnumsDelegates);
                if (this.la.kind == 0x6c)
                {
                    base.lexer.NextToken();
                    TypeDeclaration declaration2 = new TypeDeclaration(m.Modifier, attributes);
                    templates = declaration2.Templates;
                    declaration2.StartLocation = m.GetDeclarationLocation(this.t.Location);
                    base.compilationUnit.AddChild(declaration2);
                    base.compilationUnit.BlockStart(declaration2);
                    declaration2.Type = ICSharpCode.NRefactory.Parser.AST.ClassType.Struct;
                    base.Expect(1);
                    declaration2.Name = this.t.val;
                    if (this.la.kind == 0x17)
                    {
                        this.TypeParameterList(templates);
                    }
                    if (this.la.kind == 9)
                    {
                        this.StructInterfaces(out list);
                        declaration2.BaseTypes = list;
                    }
                    while (this.IdentIsWhere())
                    {
                        this.TypeParameterConstraintsClause(templates);
                    }
                    this.StructBody();
                    if (this.la.kind == 11)
                    {
                        base.lexer.NextToken();
                    }
                    declaration2.EndLocation = this.t.Location;
                    base.compilationUnit.BlockEnd();
                }
                else if (this.la.kind == 0x52)
                {
                    base.lexer.NextToken();
                    TypeDeclaration declaration3 = new TypeDeclaration(m.Modifier, attributes);
                    templates = declaration3.Templates;
                    base.compilationUnit.AddChild(declaration3);
                    base.compilationUnit.BlockStart(declaration3);
                    declaration3.StartLocation = m.GetDeclarationLocation(this.t.Location);
                    declaration3.Type = ICSharpCode.NRefactory.Parser.AST.ClassType.Interface;
                    base.Expect(1);
                    declaration3.Name = this.t.val;
                    if (this.la.kind == 0x17)
                    {
                        this.TypeParameterList(templates);
                    }
                    if (this.la.kind == 9)
                    {
                        this.InterfaceBase(out list);
                        declaration3.BaseTypes = list;
                    }
                    while (this.IdentIsWhere())
                    {
                        this.TypeParameterConstraintsClause(templates);
                    }
                    this.InterfaceBody();
                    if (this.la.kind == 11)
                    {
                        base.lexer.NextToken();
                    }
                    declaration3.EndLocation = this.t.Location;
                    base.compilationUnit.BlockEnd();
                }
                else if (this.la.kind == 0x43)
                {
                    base.lexer.NextToken();
                    TypeDeclaration declaration4 = new TypeDeclaration(m.Modifier, attributes);
                    base.compilationUnit.AddChild(declaration4);
                    base.compilationUnit.BlockStart(declaration4);
                    declaration4.StartLocation = m.GetDeclarationLocation(this.t.Location);
                    declaration4.Type = ICSharpCode.NRefactory.Parser.AST.ClassType.Enum;
                    base.Expect(1);
                    declaration4.Name = this.t.val;
                    if (this.la.kind == 9)
                    {
                        string str;
                        base.lexer.NextToken();
                        this.IntegralType(out str);
                        declaration4.BaseTypes.Add(new TypeReference(str));
                    }
                    this.EnumBody();
                    if (this.la.kind == 11)
                    {
                        base.lexer.NextToken();
                    }
                    declaration4.EndLocation = this.t.Location;
                    base.compilationUnit.BlockEnd();
                }
                else
                {
                    base.lexer.NextToken();
                    DelegateDeclaration declaration5 = new DelegateDeclaration(m.Modifier, attributes);
                    templates = declaration5.Templates;
                    declaration5.StartLocation = m.GetDeclarationLocation(this.t.Location);
                    if (this.NotVoidPointer())
                    {
                        base.Expect(0x7a);
                        declaration5.ReturnType = new TypeReference("void", 0, null);
                    }
                    else if (this.StartOf(9))
                    {
                        TypeReference reference;
                        this.Type(out reference);
                        declaration5.ReturnType = reference;
                    }
                    else
                    {
                        base.SynErr(0x83);
                    }
                    base.Expect(1);
                    declaration5.Name = this.t.val;
                    if (this.la.kind == 0x17)
                    {
                        this.TypeParameterList(templates);
                    }
                    base.Expect(20);
                    if (this.StartOf(10))
                    {
                        this.FormalParameterList(parameter);
                        declaration5.Parameters = parameter;
                    }
                    base.Expect(0x15);
                    while (this.IdentIsWhere())
                    {
                        this.TypeParameterConstraintsClause(templates);
                    }
                    base.Expect(11);
                    declaration5.EndLocation = this.t.Location;
                    base.compilationUnit.AddChild(declaration5);
                }
            }
            else
            {
                base.SynErr(0x84);
            }
        }

        private void TypeModifier(Modifiers m)
        {
            switch (this.la.kind)
            {
                case 0x53:
                    base.lexer.NextToken();
                    m.Add(Modifier.Internal, this.t.Location);
                    return;

                case 0x58:
                    base.lexer.NextToken();
                    m.Add(Modifier.VBInterfaceEnums, this.t.Location);
                    return;

                case 1:
                    base.lexer.NextToken();
                    if (!(this.t.val == "partial"))
                    {
                        break;
                    }
                    m.Add(Modifier.Partial, this.t.Location);
                    return;

                case 0x30:
                    base.lexer.NextToken();
                    m.Add(Modifier.Dim, this.t.Location);
                    return;

                case 0x5f:
                    base.lexer.NextToken();
                    m.Add(Modifier.Private, this.t.Location);
                    return;

                case 0x60:
                    base.lexer.NextToken();
                    m.Add(Modifier.Protected, this.t.Location);
                    return;

                case 0x61:
                    base.lexer.NextToken();
                    m.Add(Modifier.Public, this.t.Location);
                    return;

                case 0x66:
                    base.lexer.NextToken();
                    m.Add(Modifier.Sealed, this.t.Location);
                    return;

                case 0x6a:
                    base.lexer.NextToken();
                    m.Add(Modifier.Static, this.t.Location);
                    return;

                case 0x76:
                    base.lexer.NextToken();
                    m.Add(Modifier.Unsafe, this.t.Location);
                    return;

                default:
                    base.SynErr(130);
                    break;
            }
        }

        private void TypeName(out TypeReference typeRef, bool canBeUnbound)
        {
            List<TypeReference> types = null;
            string val = null;
            string str2;
            if ((this.la.kind == 1) && (this.Peek(1).kind == 10))
            {
                base.lexer.NextToken();
                val = this.t.val;
                base.Expect(10);
            }
            this.Qualident(out str2);
            if (this.la.kind == 0x17)
            {
                this.TypeArgumentList(out types, canBeUnbound);
            }
            if (val == null)
            {
                typeRef = new TypeReference(str2, types);
            }
            else if (val == "global")
            {
                typeRef = new TypeReference(str2, types);
                typeRef.IsGlobal = true;
            }
            else
            {
                typeRef = new TypeReference(val + "." + str2, types);
            }
            while (this.DotAndIdent())
            {
                base.Expect(15);
                types = null;
                this.Qualident(out str2);
                if (this.la.kind == 0x17)
                {
                    this.TypeArgumentList(out types, canBeUnbound);
                }
                typeRef = new InnerClassTypeReference(typeRef, str2, types);
            }
        }

        private void TypeParameterConstraintsClause(List<TemplateDefinition> templates)
        {
            TypeReference reference;
            string val = "";
            base.Expect(1);
            if (this.t.val != "where")
            {
                this.Error("where expected");
            }
            base.Expect(1);
            val = this.t.val;
            base.Expect(9);
            this.TypeParameterConstraintsClauseBase(out reference);
            TemplateDefinition definition = null;
            foreach (TemplateDefinition definition2 in templates)
            {
                if (definition2.Name == val)
                {
                    definition = definition2;
                    break;
                }
            }
            if (definition != null)
            {
                definition.Bases.Add(reference);
            }
            while (this.la.kind == 14)
            {
                base.lexer.NextToken();
                this.TypeParameterConstraintsClauseBase(out reference);
                definition = null;
                foreach (TemplateDefinition definition3 in templates)
                {
                    if (definition3.Name == val)
                    {
                        definition = definition3;
                        break;
                    }
                }
                if (definition != null)
                {
                    definition.Bases.Add(reference);
                }
            }
        }

        private void TypeParameterConstraintsClauseBase(out TypeReference type)
        {
            type = null;
            if (this.la.kind == 0x6c)
            {
                base.lexer.NextToken();
                type = new TypeReference("struct");
            }
            else if (this.la.kind == 0x3a)
            {
                base.lexer.NextToken();
                type = new TypeReference("struct");
            }
            else if (this.la.kind == 0x58)
            {
                base.lexer.NextToken();
                base.Expect(20);
                base.Expect(0x15);
                type = new TypeReference("struct");
            }
            else if (this.StartOf(9))
            {
                TypeReference reference;
                this.Type(out reference);
                type = reference;
            }
            else
            {
                base.SynErr(0xbc);
            }
        }

        private void TypeParameterList(List<TemplateDefinition> templates)
        {
            ICSharpCode.NRefactory.Parser.AST.AttributeSection section;
            List<ICSharpCode.NRefactory.Parser.AST.AttributeSection> attributes = new List<ICSharpCode.NRefactory.Parser.AST.AttributeSection>();
            base.Expect(0x17);
            while (this.la.kind == 0x12)
            {
                this.AttributeSection(out section);
                attributes.Add(section);
            }
            base.Expect(1);
            templates.Add(new TemplateDefinition(this.t.val, attributes));
            while (this.la.kind == 14)
            {
                base.lexer.NextToken();
                while (this.la.kind == 0x12)
                {
                    this.AttributeSection(out section);
                    attributes.Add(section);
                }
                base.Expect(1);
                templates.Add(new TemplateDefinition(this.t.val, attributes));
            }
            base.Expect(0x16);
        }

        private void TypeWithRestriction(out TypeReference type, bool allowNullable, bool canBeUnbound)
        {
            int num = 0;
            type = null;
            if (((this.la.kind == 1) || (this.la.kind == 90)) || (this.la.kind == 0x6b))
            {
                this.ClassType(out type, canBeUnbound);
            }
            else if (this.StartOf(4))
            {
                string str;
                this.SimpleType(out str);
                type = new TypeReference(str);
            }
            else if (this.la.kind == 0x7a)
            {
                base.lexer.NextToken();
                base.Expect(6);
                num = 1;
                type = new TypeReference("void");
            }
            else
            {
                base.SynErr(0x9b);
            }
            List<int> list = new List<int>();
            if (allowNullable && (this.la.kind == 12))
            {
                this.NullableQuestionMark(ref type);
            }
            while (this.IsPointerOrDims())
            {
                int item = 0;
                if (this.la.kind == 6)
                {
                    base.lexer.NextToken();
                    num++;
                }
                else
                {
                    if (this.la.kind == 0x12)
                    {
                        base.lexer.NextToken();
                        while (this.la.kind == 14)
                        {
                            base.lexer.NextToken();
                            item++;
                        }
                        base.Expect(0x13);
                        list.Add(item);
                        continue;
                    }
                    base.SynErr(0x9c);
                }
            }
            if (type != null)
            {
                type.RankSpecifier = list.ToArray();
                type.PointerNestingLevel = num;
            }
        }

        private void UnaryExpr(out Expression uExpr)
        {
            TypeReference type = null;
            Expression expression;
            ArrayList list = new ArrayList();
            uExpr = null;
            while (this.StartOf(0x19) || this.IsTypeCast())
            {
                if (this.la.kind == 4)
                {
                    base.lexer.NextToken();
                    list.Add(new UnaryOperatorExpression(UnaryOperatorType.Plus));
                }
                else
                {
                    if (this.la.kind == 5)
                    {
                        base.lexer.NextToken();
                        list.Add(new UnaryOperatorExpression(UnaryOperatorType.Minus));
                        continue;
                    }
                    if (this.la.kind == 0x18)
                    {
                        base.lexer.NextToken();
                        list.Add(new UnaryOperatorExpression(UnaryOperatorType.Not));
                        continue;
                    }
                    if (this.la.kind == 0x1b)
                    {
                        base.lexer.NextToken();
                        list.Add(new UnaryOperatorExpression(UnaryOperatorType.BitNot));
                        continue;
                    }
                    if (this.la.kind == 6)
                    {
                        base.lexer.NextToken();
                        list.Add(new UnaryOperatorExpression(UnaryOperatorType.Star));
                        continue;
                    }
                    if (this.la.kind == 0x1f)
                    {
                        base.lexer.NextToken();
                        list.Add(new UnaryOperatorExpression(UnaryOperatorType.Increment));
                        continue;
                    }
                    if (this.la.kind == 0x20)
                    {
                        base.lexer.NextToken();
                        list.Add(new UnaryOperatorExpression(UnaryOperatorType.Decrement));
                        continue;
                    }
                    if (this.la.kind == 0x1c)
                    {
                        base.lexer.NextToken();
                        list.Add(new UnaryOperatorExpression(UnaryOperatorType.BitWiseAnd));
                        continue;
                    }
                    base.Expect(20);
                    this.Type(out type);
                    base.Expect(0x15);
                    list.Add(new CastExpression(type));
                }
            }
            this.PrimaryExpr(out expression);
            for (int i = 0; i < list.Count; i++)
            {
                Expression expression2 = ((i + 1) < list.Count) ? ((Expression) list[i + 1]) : expression;
                if (list[i] is CastExpression)
                {
                    ((CastExpression) list[i]).Expression = expression2;
                }
                else
                {
                    ((UnaryOperatorExpression) list[i]).Expression = expression2;
                }
            }
            if (list.Count > 0)
            {
                uExpr = (Expression) list[0];
            }
            else
            {
                uExpr = expression;
            }
        }

        private bool UnCheckedAndLBrace()
        {
            return ((this.la.kind == 0x39) || ((this.la.kind == 0x75) && (this.Peek(1).kind == 0x10)));
        }

        private void UsingDirective()
        {
            string qualident = null;
            TypeReference type = null;
            base.Expect(120);
            Point location = this.t.Location;
            this.Qualident(out qualident);
            if (this.la.kind == 3)
            {
                base.lexer.NextToken();
                this.NonArrayType(out type);
            }
            base.Expect(11);
            if ((qualident != null) && (qualident.Length > 0))
            {
                INode node;
                if (type != null)
                {
                    node = new UsingDeclaration(qualident, type);
                }
                else
                {
                    node = new UsingDeclaration(qualident);
                }
                node.StartLocation = location;
                node.EndLocation = this.t.EndLocation;
                base.compilationUnit.AddChild(node);
            }
        }

        private void VariableDeclarator(List<VariableDeclaration> fieldDeclaration)
        {
            Expression initializerExpression = null;
            base.Expect(1);
            VariableDeclaration item = new VariableDeclaration(this.t.val);
            if (this.la.kind == 3)
            {
                base.lexer.NextToken();
                this.VariableInitializer(out initializerExpression);
                item.Initializer = initializerExpression;
            }
            fieldDeclaration.Add(item);
        }

        private void VariableInitializer(out Expression initializerExpression)
        {
            TypeReference type = null;
            Expression expr = null;
            initializerExpression = null;
            if (this.StartOf(5))
            {
                this.Expr(out initializerExpression);
            }
            else if (this.la.kind == 0x10)
            {
                this.ArrayInitializer(out initializerExpression);
            }
            else if (this.la.kind == 0x69)
            {
                base.lexer.NextToken();
                this.Type(out type);
                base.Expect(0x12);
                this.Expr(out expr);
                base.Expect(0x13);
                initializerExpression = new StackAllocExpression(type, expr);
            }
            else if (this.la.kind == 0x3e)
            {
                base.lexer.NextToken();
                base.Expect(20);
                this.Type(out type);
                base.Expect(0x15);
                initializerExpression = new DefaultValueExpression(type);
            }
            else
            {
                base.SynErr(0xa7);
            }
        }

        protected bool WeakSeparator(int n, int syFol, int repFol)
        {
            bool[] flagArray = new bool[0x7e];
            if (base.lexer.LookAhead.kind == n)
            {
                base.lexer.NextToken();
                return true;
            }
            if (this.StartOf(repFol))
            {
                return false;
            }
            for (int i = 0; i <= 0x7d; i++)
            {
                flagArray[i] = (set[syFol, i] || set[repFol, i]) || set[0, i];
            }
            base.SynErr(n);
            while (!flagArray[base.lexer.LookAhead.kind])
            {
                base.lexer.NextToken();
            }
            return this.StartOf(syFol);
        }

        private bool WriteFullTypeName(StringBuilder b, Expression expr)
        {
            FieldReferenceExpression expression = expr as FieldReferenceExpression;
            if (expression != null)
            {
                bool flag = this.WriteFullTypeName(b, expression.TargetObject);
                if (b.Length > 0)
                {
                    b.Append('.');
                }
                b.Append(expression.FieldName);
                return flag;
            }
            if (expr is IdentifierExpression)
            {
                b.Append(((IdentifierExpression) expr).Identifier);
                return true;
            }
            return false;
        }

        public string ContainingAssembly
        {
            set
            {
                this.assemblyName = value;
            }
        }

        private Token la
        {
            [DebuggerStepThrough]
            get
            {
                return base.lexer.LookAhead;
            }
        }

        private Token t
        {
            [DebuggerStepThrough]
            get
            {
                return base.lexer.Token;
            }
        }
    }
}
