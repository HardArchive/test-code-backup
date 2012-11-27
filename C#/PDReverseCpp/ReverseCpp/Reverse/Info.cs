namespace LZ.Reverse
{
    using PdCommon;
    using System;
    using System.Collections;
    using System.IO;

    internal class Info
    {
        public static bool _bDebug;
        public static bool _bVBNet;
        public static int _nVersion;
        private static IWrapper _out;

        static Info()
        {
            ResetOutput();
        }

        public static void ResetOutput()
        {
            _out = new WrapperTextWriter(Console.Out);
        }

        public static void SetOutput(Application anApp)
        {
            _out = null;
            if (anApp != null)
            {
                _out = new WrapperApplication(anApp);
            }
        }

        public static void SetOutput(TextWriter anOutput)
        {
            _out = null;
            if (anOutput != null)
            {
                _out = new WrapperTextWriter(anOutput);
            }
        }

        public static void Write(params string[] str)
        {
            if (_out != null)
            {
                _out.Write(str);
            }
        }

        internal class CancelException : Exception
        {
        }

        private interface IWrapper
        {
            void Write(params string[] str);
        }

        private class WrapperApplication : LZ.Reverse.Info.IWrapper
        {
            private Application _app;

            public WrapperApplication(Application anApp)
            {
                this._app = anApp;
            }

            public void Write(params string[] str)
            {
                if (this._app != null)
                {
                    string str2;
                    if (str.Length > 1)
                    {
                        string format = str[0];
                        ArrayList list = new ArrayList(str);
                        list.RemoveAt(0);
                        str2 = string.Format(format, list.ToArray());
                    }
                    else
                    {
                        str2 = str[0];
                    }
                    this._app.Output(str2);
                }
            }
        }

        private class WrapperTextWriter : LZ.Reverse.Info.IWrapper
        {
            private TextWriter _fil;

            public WrapperTextWriter(TextWriter aWriter)
            {
                this._fil = aWriter;
            }

            public void Write(params string[] str)
            {
                if (this._fil != null)
                {
                    string str2;
                    if (str.Length > 1)
                    {
                        string format = str[0];
                        ArrayList list = new ArrayList(str);
                        list.RemoveAt(0);
                        str2 = string.Format(format, list.ToArray());
                    }
                    else
                    {
                        str2 = str[0];
                    }
                    this._fil.WriteLine(str2);
                }
            }
        }
    }
}

