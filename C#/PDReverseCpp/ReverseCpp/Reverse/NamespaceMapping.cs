namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Collections;

    internal class NamespaceMapping
    {
        private static Hashtable _idxNmsp = new Hashtable();
        private static PdOOM.Model _mdl;
        private string _name;
        private BasePackage _pdPckg;
        private static Hashtable _tabNmsp = new Hashtable();

        private NamespaceMapping(string aName, BasePackage aPckg)
        {
            this._pdPckg = aPckg;
            this._name = aName;
            _tabNmsp.Add(aName, this);
            _idxNmsp.Add(aPckg, this);
        }

        public static NamespaceMapping Retrieve(BasePackage aPckg)
        {
            if (aPckg == null)
            {
                _idxNmsp.Count.ToString();
            }
            return (NamespaceMapping) _idxNmsp[aPckg];
        }

        public static NamespaceMapping RetrieveFromChildName(string aName)
        {
            string str;
            ArrayList list = new ArrayList(aName.Split(new char[] { '.' }));
            if (list.Count > 0)
            {
                Array array = Array.CreateInstance(typeof(string), (int) (list.Count - 1));
                list.CopyTo(0, array, 0, array.Length);
                str = string.Join(".", (string[]) array);
            }
            else
            {
                str = "";
            }
            return RetrieveFromName(str);
        }

        public static NamespaceMapping RetrieveFromName(string aName)
        {
            NamespaceMapping mapping = (NamespaceMapping) _tabNmsp[aName];
            if (mapping != null)
            {
                return mapping;
            }
            if (_mdl == null)
            {
                return null;
            }
            ArrayList list = new ArrayList(aName.Split(new char[] { '.' }));
            string str = (string) list[list.Count - 1];
            Array array = Array.CreateInstance(typeof(string), (int) (list.Count - 1));
            list.CopyTo(0, array, 0, array.Length);
            BasePackage aPckg = (BasePackage) RetrieveFromName(string.Join(".", (string[]) array))._pdPckg.CreateObject(0x18112061, "", -1, true);
            aPckg.Name = aPckg.Code = str;
            return new NamespaceMapping(aName, aPckg);
        }

        public string FullName
        {
            get
            {
                return this._name;
            }
        }

        public static PdOOM.Model Model
        {
            set
            {
                _tabNmsp.Clear();
                _idxNmsp.Clear();
                _mdl = value;
                if (_mdl != null)
                {
                    new NamespaceMapping("", _mdl);
                }
            }
        }

        public BasePackage Package
        {
            get
            {
                return this._pdPckg;
            }
        }
    }
}

