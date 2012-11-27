namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Collections;

    internal class TypeMapping
    {
        private bool _bComplete;
        private bool _bDelegate;
        private bool _bModule;
        private bool _classifier;
        private string _name;
        private PdOOM.Classifier _pdClss;
        private static Hashtable _tabCheck;
        private static Hashtable _tabType;
        private Type _type;
        private const string MSG_DUPLICATENAME = "*** there are several classifiers \"{0}\"";
        private const string MSG_NOTCLASSIFIER = "*** \"{0}\" is not a classifier";

        static TypeMapping()
        {
            Init();
        }

        private TypeMapping(string aName)
        {
            this._type = null;
            this._classifier = true;
            string[] strArray = aName.Split(new char[] { '.' });
            strArray = strArray[strArray.Length - 1].Split(new char[] { '+' });
            string str = strArray[strArray.Length - 1];
            this._name = str;
            this._bComplete = true;
            this._bDelegate = false;
            this._bModule = false;
            if (_tabCheck.Contains(aName))
            {
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** there are several classifiers \"{0}\"", aName });
                }
            }
            else
            {
                _tabCheck.Add(aName, this);
            }
        }

        private TypeMapping(Type aType)
        {
            this._type = aType;
            this._classifier = ((this._type.IsClass || this._type.IsInterface) || this._type.IsEnum) || this._type.IsValueType;
            this._name = TypeInfoHelper.GetTypeName(aType);
            this._bComplete = false;
            this._bDelegate = false;
            this._bModule = false;
            if (this._type.IsClass)
            {
                foreach (object obj2 in this._type.GetCustomAttributes(false))
                {
                    if (obj2.GetType().FullName == "Microsoft.VisualBasic.CompilerServices.StandardModuleAttribute")
                    {
                        this._bModule = true;
                        break;
                    }
                }
            }
            _tabType.Add(aType, this);
            if (_tabCheck.Contains(TypeInfoHelper.GetTypeFullName(aType)))
            {
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** there are several classifiers \"{0}\"", this._type.FullName });
                }
            }
            else
            {
                _tabCheck.Add(TypeInfoHelper.GetTypeFullName(aType), this);
            }
        }

        private TypeMapping(TypeInfo aType)
        {
            //this._type = aType;
            this._classifier = aType.IsClass;   // ((this._type.IsClass || this._type.IsInterface) || this._type.IsEnum) || this._type.IsValueType;
            this._name = aType.Code;//TypeInfoHelper.GetTypeName(aType);
            this._bComplete = false;
            this._bDelegate = false;
            this._bModule = false;
            if (aType.IsClass)
            {
                ////foreach (object obj2 in this._type.GetCustomAttributes(false))
                ////{
                ////    if (obj2.GetType().FullName == "Microsoft.VisualBasic.CompilerServices.StandardModuleAttribute")
                ////    {
                ////        this._bModule = true;
                ////        break;
                ////    }
                ////}
            }
            _tabType.Add(aType, this);
            if (_tabCheck.Contains(aType.Code))
            {
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** there are several classifiers \"{0}\"", this._type.FullName });
                }
            }
            else
            {
                _tabCheck.Add(aType.Code, this);
            }
        }

        public static void Clear()
        {
            Init();
        }

        public static TypeMapping DefineExtraType(Type aType)
        {
            return DefineExtraType(aType, "");
        }

        public static TypeMapping DefineExtraType(string aName, PdOOM.Classifier aCls)
        {
            if (aName == "")
            {
                return null;
            }
            if (aCls == null)
            {
                return null;
            }
            if (_tabCheck.Contains(aName))
            {
                return (TypeMapping) _tabCheck[aName];
            }
            TypeMapping mapping = new TypeMapping(aName);
            mapping.Classifier = aCls;
            return mapping;
        }

        public static TypeMapping DefineExtraType(Type aType, string aName)
        {
            TypeMapping mapping = (TypeMapping) _tabType[aType];
            if (mapping == null)
            {
                mapping = new TypeMapping(aType);
                if (aName != "")
                {
                    mapping._name = aName;
                }
                mapping.Complete = true;
            }
            return mapping;
        }

        public static TypeMapping DefineExtraType(string aName)
        {
            TypeInfo aType = new TypeInfo();
            aType.Code = aType.Name = aName;
            aType.Modifier = "public";
            TypeMapping mapping = (TypeMapping)_tabType[aType];
            if (mapping == null)
            {
                mapping = new TypeMapping(aType);
                if (aName != "")
                {
                    mapping._name = aName;
                }
                mapping.Complete = true;
            }
            return mapping;
        }

        public bool HasClassifier()
        {
            return (this._pdClss != null);
        }

        private static void Init()
        {
            _tabType = new Hashtable();
            _tabCheck = new Hashtable();

            LZTools.Xml.XmlOperate xml = new LZTools.Xml.XmlOperate();
            string sPath = string.Format("{0}\\types.xml", System.IO.Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath));
            xml.LoadXmlFile(sPath);
            string[] typeList = xml.GetNodeTextArray("//config/item");
            foreach (string t in typeList)
            {
                DefineExtraType(t);
            }
            xml.CloseXmlReader();
        }

        public bool IsClassifier()
        {
            return this._classifier;
        }

        public bool IsModule()
        {
            return this._bModule;
        }

        public static TypeMapping Retrieve(Type aType)
        {
            bool bCreate = false;
            if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            {
                aType = aType.GetGenericTypeDefinition();
            }
            if (aType.IsGenericParameter)
            {
                return DefineExtraType(aType, aType.Name);
            }
            return Retrieve(aType, ref bCreate);
        }

        public static TypeMapping Retrieve(Type aType, ref bool bCreate)
        {
            if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            {
                aType = aType.GetGenericTypeDefinition();
            }
            if (aType.IsGenericParameter)
            {
                return DefineExtraType(aType, aType.Name);
            }
            TypeMapping mapping = (TypeMapping) _tabType[aType];
            if (mapping != null)
            {
                bCreate = false;
                return mapping;
            }
            if (!bCreate)
            {
                bCreate = false;
                return null;
            }
            mapping = (TypeMapping) _tabCheck[aType.FullName];
            if (mapping != null)
            {
                _tabType[aType] = mapping;
                bCreate = false;
                return mapping;
            }
            mapping = new TypeMapping(aType);
            bCreate = true;
            return mapping;
        }

        public static TypeMapping Retrieve(TypeInfo aType, ref bool bCreate)
        {
            if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            {
                //aType = aType.GetGenericTypeDefinition();
            }
            ////if (aType.IsGenericParameter)
            ////{
            ////    return DefineExtraType(aType, aType.Name);
            ////}
            TypeMapping mapping = (TypeMapping)_tabType[aType];
            if (mapping != null)
            {
                bCreate = false;
                return mapping;
            }
            if (!bCreate)
            {
                bCreate = false;
                return null;
            }
            mapping = (TypeMapping)_tabCheck[aType.Code];
            if (mapping != null)
            {
                _tabType[aType] = mapping;
                bCreate = false;
                return mapping;
            }
            mapping = new TypeMapping(aType);
            mapping._classifier = aType.IsClass;
            bCreate = true;
            return mapping;
        }

        public PdOOM.Classifier Classifier
        {
            get
            {
                return this._pdClss;
            }
            set
            {
                if (this.IsClassifier())
                {
                    this._pdClss = value;
                }
                else if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** \"{0}\" is not a classifier", this._type.FullName });
                }
            }
        }

        public bool Complete
        {
            get
            {
                return this._bComplete;
            }
            set
            {
                if (value)
                {
                    this._bComplete = true;
                }
            }
        }

        public bool Delegate
        {
            get
            {
                return this._bDelegate;
            }
            set
            {
                if (value)
                {
                    this._bDelegate = true;
                }
            }
        }

        public string FullName
        {
            get
            {
                return this._name;
            }
        }

        public string Name
        {
            get
            {
                return this._name;
            }
        }
    }
}

