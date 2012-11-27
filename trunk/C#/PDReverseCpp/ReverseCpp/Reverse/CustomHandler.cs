namespace LZ.Reverse
{
    using PdOOM;
    using System;

    internal class CustomHandler
    {
        protected object[] _atts;
        private string _disp;
        private string _name;
        private BaseObject _pdObj;

        protected CustomHandler(object[] atts, string aName, BaseObject anObj)
        {
            this._atts = atts;
            this._name = aName;
            this._pdObj = anObj;
        }

        public virtual void Convert()
        {
            if (LZ.Reverse.Info._bDebug)
            {
                this.Display();
            }
        }

        public void Display()
        {
            if (this._disp != "")
            {
                LZ.Reverse.Info.Write(new string[] { this._disp });
            }
        }

        protected virtual bool Process(object anAttr)
        {
            return false;
        }

        protected void ProcessAttrs()
        {
            if (this._atts != null)
            {
                this._disp = "";
                bool flag = true;
                for (int i = 0; i < this._atts.Length; i++)
                {
                    if (!this.Process(this._atts[i]))
                    {
                        if (flag)
                        {
                            this._disp = this._disp + "The attributes for " + this._name + " are:\n";
                            flag = false;
                        }
                        this._disp = this._disp + "   of type " + this._atts[i].ToString() + "\n";
                    }
                }
            }
        }

        public BaseObject PdObj
        {
            set
            {
                this._pdObj = value;
            }
        }
    }
}

