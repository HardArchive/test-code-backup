namespace LZ.Lexer
{
    using System;
    using System.Text;

    public class Errors
    {
        public int count;
        public ErrorMsgProc Error;
        private StringBuilder errorText = new StringBuilder();
        public ErrorCodeProc SemErr;
        public ErrorCodeProc SynErr;

        public Errors()
        {
            this.SynErr = new ErrorCodeProc(this.DefaultCodeError);
            this.SemErr = new ErrorCodeProc(this.DefaultCodeError);
            this.Error = new ErrorMsgProc(this.DefaultMsgError);
        }

        private void DefaultCodeError(int line, int col, int n)
        {
            this.errorText.AppendLine(string.Format("-- line {0} col {1}: error {2}", line, col, n));
            this.count++;
        }

        private void DefaultMsgError(int line, int col, string s)
        {
            this.errorText.AppendLine(string.Format("-- line {0} col {1}: {2}", line, col, s));
            this.count++;
        }

        public string ErrorOutput
        {
            get
            {
                return this.errorText.ToString();
            }
        }
    }
}
