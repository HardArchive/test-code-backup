namespace LZ.Reverse
{
    using PdCommon;
    using PdOOM;
    using System;
    using System.Collections;
    using System.IO;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Diagnostics;
    using System.Collections.Generic;

    internal class ReverserNew
    {
        class ProcessTypeTemp
        {
            public AttributeInfo Attribute = new AttributeInfo();
            public MethordInfo Methord = new MethordInfo();
            public ObjectInfo Type = new ObjectInfo();
            public ObjectInfo Parameter = new ObjectInfo();

            public void Reset()
            {

            }
        };

        private Application _app;
        private bool _bAllTypes;
        ////private Hashtable _mapDelegate;
        private Model _mdl;
        private Recursion _recur;
        ////private Hashtable _tabAssemblies;
        private Stack _work;
        /// <summary>
        /// 语法解析器
        /// </summary>
        private LZ.Lexer.CPP.Lexer _lexer;

        private ReverserNew()
        {
        }

        public ReverserNew(bool bAllTypes, Recursion recur, bool bDebug, int nVersion)
        {
            Application anApp = new ApplicationClass();
            anApp.Locked = true;
            this.Initialize(anApp, bAllTypes, recur, bDebug, nVersion);
        }

        public ReverserNew(Application anApp, bool bAllTypes, Recursion recur, bool bDebug, int nVersion)
        {
            this.Initialize(anApp, bAllTypes, recur, bDebug, nVersion);
        }

        private void AddClassifier(TypeInfo aType, bool bComplete)
        {
            bool bCreate = true;
            TypeMapping mapping = TypeMapping.Retrieve(aType, ref bCreate);
            if (bCreate || bComplete)
            {
                PdOOM.BasePackage package = null;
                Classifier classifier;
                if (bCreate)
                {
                    bool flag2;
                    PdOOM.NamedObject container = this.GetContainer(aType, out flag2);
                    if (flag2)
                    {
                        package = (PdOOM.BasePackage)container;
                    }
                    else
                    {
                        package = NamespaceMapping.RetrieveFromChildName("").Package;
                    }
                    int kind = 0x18112063;  // aType.IsInterface ? 0x18112064 : 0x18112063;
                    classifier = (Classifier)container.CreateObject(kind, "", -1, true);
                    mapping.Classifier = classifier;
                    try
                    {
                        classifier.Name = classifier.Code = aType.Code; // TypeInfoHelper.GetTypeName(aType);
                    }
                    catch (COMException)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { "*** exception while naming the type \"{0}\"", /*TypeInfoHelper.GetTypeFullName(aType)*/aType.Code });
                        }
                    }
                    classifier.Visibility = GetTypeVisibility(aType.Modifier);

                    if (!bComplete)
                    {
                        return;
                    }
                }
                else
                {
                    classifier = mapping.Classifier;
                    ////package = mapping.Classifier.Package;
                    package = NamespaceMapping.RetrieveFromChildName("").Package;
                }
                mapping.Complete = true;
                classifier.Comment = aType.Comment;


                if (aType.IsClass && aType.IsAbstract)
                {
                    classifier.Abstract = true;
                }
                if (aType.IsValueType)
                {
                    classifier.Stereotype = "structure";
                }
                if (aType.IsGenericType)
                {
                    classifier.Stereotype = "template";
                }
                // 处理继承或实现
                foreach(TypeInfo ti in aType.Parents)
                {
                    ti.IsClass = true;
                    this.CreateGeneralization(classifier, ti, package);
                }
                InfluenceTable aInfTable = new InfluenceTable();

                foreach (AttributeInfo ai in aType.Attributes)
                {
                    if (package == null)//(!aAttrTable.Contains(info5))
                    {
                        ai.ToString();
                    }
                    else
                    {
                        this.CreateAttribute(aType, classifier, ai, package);
                    }
                }

                foreach (MethordInfo mi in aType.Methords)
                {
                    //if (!aAttrTable.Contains(info5))
                    {
                        this.CreateMethod(aType, classifier, mi, package, null, aInfTable);
                    }
                }
            }
        }

        private void AddEnumeration(Type aType)
        {
            ////if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            ////{
            ////    aType = aType.GetGenericTypeDefinition();
            ////}
            ////bool bCreate = true;
            ////TypeMapping mapping = TypeMapping.Retrieve(aType, ref bCreate);
            ////if (bCreate)
            ////{
            ////    bool flag2;
            ////    PdOOM.BasePackage package;
            ////    PdOOM.NamedObject container = this.GetContainer(aType, out flag2);
            ////    if (flag2)
            ////    {
            ////        package = (PdOOM.BasePackage) container;
            ////    }
            ////    else
            ////    {
            ////        package = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType)).Package;
            ////    }
            ////    Class innerCsfr = (Class) package.CreateObject(0x18112063, "", -1, true);
            ////    innerCsfr.Stereotype = "enumeration";
            ////    mapping.Classifier = innerCsfr;
            ////    mapping.Complete = true;
            ////    if (!flag2)
            ////    {
            ////        Classifier classifier = (Classifier) container;
            ////        if (classifier != null)
            ////        {
            ////            classifier.AddInnerClassifier(innerCsfr);
            ////        }
            ////        else if (LZ.Reverse.Info._bDebug)
            ////        {
            ////            LZ.Reverse.Info.Write(new string[] { "*** null parent for \"{0}\"", aType.FullName });
            ////        }
            ////    }
            ////    innerCsfr.Name = innerCsfr.Code = TypeInfoHelper.GetTypeName(aType);
            ////    innerCsfr.Visibility = GetTypeVisibility(aType);
            ////    Type underlyingType = Enum.GetUnderlyingType(aType);
            ////    TypeMapping mapping3 = TypeMapping.Retrieve(underlyingType);
            ////    string name = underlyingType.Name;
            ////    if (name != "Int32")
            ////    {
            ////        innerCsfr.SetExtendedAttribute(LZ.Reverse.Info._bVBNet ? "VB.NET.EnumDataType" : "C#.EnumDataType", mapping3.Name);
            ////    }
            ////    string[] names = Enum.GetNames(aType);
            ////    Array values = Enum.GetValues(aType);
            ////    ulong num = 0;
            ////    for (int i = 0; i < names.Length; i++)
            ////    {
            ////        ulong num3;
            ////        PdOOM.Attribute attribute = (PdOOM.Attribute) innerCsfr.CreateObject(0x18112065, "", -1, true);
            ////        attribute.Name = attribute.Code = names[i];
            ////        attribute.DataType = mapping3.Name;
            ////        attribute.Visibility = "+";
            ////        attribute.Frozen = "F";
            ////        switch (name)
            ////        {
            ////            case "Byte":
            ////                num3 = (byte) values.GetValue(i);
            ////                break;

            ////            case "SByte":
            ////                num3 = (ulong) ((sbyte) values.GetValue(i));
            ////                break;

            ////            case "Int16":
            ////                num3 = (ulong) ((short) values.GetValue(i));
            ////                break;

            ////            case "UInt16":
            ////                num3 = (ushort) values.GetValue(i);
            ////                break;

            ////            case "Int32":
            ////                num3 = (ulong) ((int) values.GetValue(i));
            ////                break;

            ////            case "UInt32":
            ////                num3 = (uint) values.GetValue(i);
            ////                break;

            ////            case "Int64":
            ////                num3 = (ulong) ((long) values.GetValue(i));
            ////                break;

            ////            case "UInt64":
            ////                num3 = (ulong) values.GetValue(i);
            ////                break;

            ////            default:
            ////                num3 = 0;
            ////                break;
            ////        }
            ////        if (num3 != num)
            ////        {
            ////            attribute.InitialValue = num3.ToString();
            ////        }
            ////        num = num3 + 1;
            ////    }
            ////}
        }

        private void AddParameter(Operation aMethod, ObjectInfo aParam, PdOOM.BasePackage aPckg)
        {
            Parameter aPrm = (Parameter)aMethod.CreateObject(0x1811206d, "", -1, true);
            string str = "I";    // aParam.IsOut ? "O" : "I";
            ////Type parameterType = aParam.Type;
            ////if (parameterType.IsGenericType && !parameterType.IsGenericTypeDefinition)
            ////{
            ////    parameterType = parameterType.GetGenericTypeDefinition();
            ////}
            if (aParam.IsConst)
            {
                aPrm.SetExtendedAttribute("C++.const", "true");
            }
            if (aParam.IsByRef)
            {
                aPrm.SetExtendedAttribute("C++.ref", "true");
            }
            if (!aParam.IsConst && (aParam.IsByRef || aParam.IsPointer))
            {
                str = "IO";
            }
            if (aParam.IsArray)
            {
                aPrm.Array = true;
            }
            aPrm.SetExtendedAttribute("C++.init", aParam.Default);

            TypeInfo tiField = new TypeInfo();
            tiField.Code = tiField.Name = aParam.Type;
            tiField.IsClass = true;
            tiField.Comment = ((PdOOM.NamedObject)aMethod.Parent).Comment;
            this.AddClassifier(tiField, false);
            bool bCreate = true;
            TypeMapping aType = TypeMapping.Retrieve(tiField, ref bCreate);
            if (aType.HasClassifier())
            {
                PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), aType);
                aPrm.UseQualifiedDataType = false;
                aPrm.DataTypeObject = obj2;
            }
            aPrm.ParameterType = str;
            aPrm.DataType = aParam.FullType;
            aPrm.Name = aPrm.Code = aParam.Name;
            CustomHandlerParam param = new CustomHandlerParam(aParam, aPrm);
            param.Convert();
            if (param.Params)
            {
                aPrm.SetExtendedAttribute("C++.params", "true");
            }
        }

        public void Close()
        {
            ProperRef.Model = null;
            NamespaceMapping.Model = null;
            TypeMapping.Clear();
            this._mdl = null;
            this._app = null;
        }

        private void CreateAttribute(TypeInfo ti, Classifier aCls, AttributeInfo aField, PdOOM.BasePackage aPckg)
        {
            CustomHandlerField field = new CustomHandlerField(aField, null);
            if (!field.ThroughProperty)
            {
                PdOOM.Attribute attribute = (PdOOM.Attribute)aCls.CreateObject(0x18112065, "", -1, true);
                if (aField.IsStatic)
                {
                    attribute.Static = true;
                }
                try
                {
                    attribute.Name = attribute.Code = aField.Name;
                }
                catch (COMException)
                {
                    if (LZ.Reverse.Info._bDebug)
                    {
                        LZ.Reverse.Info.Write(new string[] { "*** exception while naming the attribute \"{1}\" on the type \"{0}\"", aField.Code, aField.Name });
                    }
                }
                attribute.Visibility = GetTypeVisibility(aField.Modifier);
                //attribute.Persistent = !aField.IsNotSerialized;
                attribute.Volatile = aField.IsVolatile;
                if (aField.IsPointer)
                {
                    attribute.Multiplicity = "*";
                }
                field.PdObj = attribute;
                field.Convert();

                TypeInfo tiField = new TypeInfo();
                ////System.Text.RegularExpressions.Regex reg = new System.Text.RegularExpressions.Regex("[a-zA-Z_][a-zA-Z_0-9]*");
                ////System.Text.RegularExpressions.Match m = reg.Match(aField.Type.Replace("unsigned",""));
                tiField.Code = tiField.Name = aField.Type;
                tiField.IsClass = true;
                tiField.Comment = aCls.Comment;
                this.AddClassifier(tiField, false);
                bool bCreate = true;
                TypeMapping mapping = TypeMapping.Retrieve(tiField, ref bCreate);
                if (mapping.HasClassifier())
                {
                    PdOOM.NamedObject objClassB = (PdOOM.NamedObject)ProperRef.Link(NamespaceMapping.Retrieve(aPckg), mapping);
                    PdOOM.NamedObject objClassA = (PdOOM.NamedObject)attribute.Parent;
                    attribute.UseQualifiedDataType = false;
                    attribute.DataTypeObject = objClassA;

                    Association ass = (Association)aPckg.CreateObject((int)PdOOM_Classes.cls_Association, "", -1, true);
                    ass.ClassA = objClassA;
                    ass.ClassB = objClassB;
                    try
                    {
                        ass.Name = ass.Code = string.Format("ass_{0}_2_{1}_{2}", objClassA.Code, objClassB.Code, attribute.Code);
                        if (tiField.Code == aField.FullType)
                        {
                            ass.RoleBMultiplicity = "1..1";
                            ass.RoleAIndicator = aField.IsPointer ? "A" : "C";
                        }
                        ass.RoleAMultiplicity = "";
                        ass.RoleBName = aField.Name;
                        ass.RoleANavigability = false;
                        ass.RoleBNavigability = false;
                    }
                    catch (COMException)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { "*** exception while naming the attribute \"{1}\" on the type \"{0}\"", aField.Code, aField.Name });
                        }
                    }

                }
                ////else
                {
                    attribute.DataType = aField.FullType;   // mapping.Name;
                }
            }
        }

        private void CreateGeneralization(PdOOM.NamedObject obj, TypeInfo aParentType, PdOOM.BasePackage aPckg)
        {
            this.AddClassifier(aParentType, false);
            ////if (aParentType.IsGenericType && !aParentType.IsGenericTypeDefinition)
            ////{
            ////    aParentType = aParentType.GetGenericTypeDefinition();
            ////}
            bool bCreate = true;
            TypeMapping aType = TypeMapping.Retrieve(aParentType, ref bCreate);
            PdOOM.BaseObject objParent = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), aType);
            Generalization generalization = (Generalization)aPckg.CreateObject(0x18112067, "", -1, true);
            generalization.ParentObject = objParent;
            generalization.ChildObject = obj;

            try
            {
                generalization.Name = generalization.Code = string.Format("gen_{0}_2_{1}", obj.Code, aParentType.Code); // TypeInfoHelper.GetTypeName(aType);
            }
            catch (COMException)
            {
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** exception while naming the Generalization \"{0}\"", generalization.Name });
                }
            }

        }

        private void CreateMethod(TypeInfo aType, Classifier aCls, MethordInfo aMethod, PdOOM.BasePackage aPckg, Operation anOper, InfluenceTable anInfTable)
        {
            Operation operation;
            if (anOper != null)
            {
                operation = anOper;
            }
            else
            {
                operation = (Operation)aCls.CreateObject(0x18112066, "", -1, true);
            }
            operation.Static = aMethod.IsStatic;
            operation.Visibility = GetTypeVisibility(aMethod.Modifier);
            operation.ReturnType = aMethod.Type;
            //operation.Final = aMethod.IsFinal;
            if (aMethod.IsGenericMethod)
            {
                operation.Generic = true;
                foreach (TypeParameter parameter in operation.TypeParameters)
                {
                    parameter.delete();
                }
                ////foreach (Type type in aMethod.GetGenericArguments())
                ////{
                ////    TypeParameter parameter2 = (TypeParameter)operation.CreateObject(0x1811207d, "", -1, true);
                ////    this.ProcessType(type, false);
                ////    TypeMapping mapping = TypeMapping.Retrieve(type);
                ////    parameter2.Name = parameter2.Code = mapping.Name;
                ////    operation.TypeParameters.Add(parameter2);
                ////}
            }
            if (aMethod.IsVirtual)
            {
                operation.SetExtendedAttribute("C++.virtual", "true");
            }
            if (aMethod.IsAbstract)
            {
                operation.Abstract = true;
            }
            try
            {
                operation.Code = aMethod.Code;
                operation.Name = aMethod.Name;
            }
            catch (COMException)
            {
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** exception while naming the operation \"{1}\" on the type \"{0}\"", aType.Code, aMethod.Name });
                }
            }

            new CustomHandlerMethod(aMethod, operation).Convert();
            // 添加参数
            foreach (ObjectInfo type in aMethod.Parameters)
            {
                AddParameter(operation, type, aPckg);
            }

            // 防止有重载方法前面改名失败
            if (aMethod.Parameters.Count > 0 && operation.Code != aMethod.Code)
            {
                try
                {
                    operation.Code = aMethod.Code;
                    operation.Name = aMethod.Name;
                }
                catch (COMException)
                {
                    if (LZ.Reverse.Info._bDebug)
                    {
                        LZ.Reverse.Info.Write(new string[] { "*** exception while naming the operation \"{1}\" on the type \"{0}\"", aType.Code, aMethod.Name });
                    }
                }
            }
        }

        private void CreateRealization(PdOOM.BaseObject obj, Type aParentType, PdOOM.BasePackage aPckg)
        {
            ////this.ProcessType(aParentType, false);
            ////if (aParentType.IsGenericType && !aParentType.IsGenericTypeDefinition)
            ////{
            ////    aParentType = aParentType.GetGenericTypeDefinition();
            ////}
            ////TypeMapping aType = TypeMapping.Retrieve(aParentType);
            ////PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), aType);
            ////Realization realization = (Realization) aPckg.CreateObject(0x18112068, "", -1, true);
            ////realization.Interface = obj2;
            ////realization.Class = obj;
        }

        ~ReverserNew()
        {
            this.Close();
        }

        private PdOOM.NamedObject GetContainer(Type aType, out bool bPackage)
        {
            ////Type declaringType = aType.DeclaringType;
            ////if (declaringType != null)
            ////{
            ////    this.ProcessType(declaringType, false);
            ////    TypeMapping mapping = TypeMapping.Retrieve(declaringType);
            ////    bPackage = false;
            ////    return mapping.Classifier;
            ////}
            NamespaceMapping mapping2 = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType));
            bPackage = true;
            return mapping2.Package;
        }

        private PdOOM.NamedObject GetContainer(TypeInfo aType, out bool bPackage)
        {
            //////Type declaringType = aType.Code;    //aType.DeclaringType;
            //////if (declaringType != null)
            //////{
            ////    bool bCreate = false;
            ////    //this.ProcessType(aType, false);
            ////    TypeMapping mapping = TypeMapping.Retrieve(aType, ref bCreate);
            ////    bPackage = false;
            ////    return mapping.Classifier;
            //////}
            NamespaceMapping mapping2 = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType));
            bPackage = true;
            return mapping2.Package;
        }

        private static string GetTypeVisibility(string aTypeVisibility)
        {
            string sVisibility = "+";
            switch(aTypeVisibility)
            {
                case "private":
                    sVisibility = "-";
                    break;
                case "protected":
                    sVisibility = "#";
                    break;
                case "public":
                    sVisibility = "+";
                    break;
            }

            return sVisibility;
        }


        private void Initialize(Application anApp, bool bAllTypes, Recursion recur, bool bDebug, int nVersion)
        {
            this._work = new Stack();

            this._app = anApp;
            string languageName = "C++";
            LZ.Reverse.Info._nVersion = nVersion;
            this._bAllTypes = bAllTypes;
            this._recur = recur;
            LZ.Reverse.Info._bDebug = bDebug;
            this._mdl = (Model) this._app.CreateModel(0x18112060, "|Language=" + languageName + "|Diagram=ClassDiagram", OpenModelFlags.omf_Default);
            this._mdl.Name = "Reverse";
            this._mdl.Code = "Reverse";
            NamespaceMapping.Model = this._mdl;
            ProperRef.Model = this._mdl;
            TypeMapping.Clear();
        }

        LZ.Lexer.Token tokenCurrent;


        private string GetTokenText(LZ.Lexer.Token token)
        {
            if (token == null) return "";

            StringBuilder sb = new StringBuilder();
            string word = token.val;
            return word;
        }

        private string NextTokenText()
        {
            if (tokenCurrent == null) return "";

            tokenCurrent = tokenCurrent.next;
            return GetTokenText(tokenCurrent);
        }

        private string ReadToLineEnd()
        {
            string word = "";
            int nLine = tokenCurrent.line;
            while (nLine == tokenCurrent.line)
            {
                word = NextTokenText();
            }

            return word;
        }

        private bool ReadToMethordStart(StringBuilder sb)
        {
            bool bIsFind = false;
            string word = "";
            while (tokenCurrent != null && word != "{")
            {
                word = NextTokenText();
                if (word == "{")
                {
                    bIsFind = true;
                    break;
                }
                if (word == ";")
                    break;
            }

            return bIsFind;
        }

        private void ReadMethordBody(string currentWord, StringBuilder sb)
        {
            string word = "";
            Stack<string> stack = new Stack<string>();
            if (currentWord == "{" || ReadToMethordStart(sb))
            {
                stack.Push("{");
                word = NextTokenText();
                if (word == "}")
                {
                    stack.Pop();
                }
                while (stack.Count > 0)
                {
                    word = NextTokenText();
                    switch (word)
                    {
                        case "}":
                            stack.Pop();
                            break;
                        case "{":
                            stack.Push("{");
                            break;
                    }
                }
            }
        }

        private void ReadParentInfo(TypeInfo ti, string currentWord)
        {
            string word = NextTokenText();
            while (tokenCurrent != null && "{" != GetTokenText(tokenCurrent.next))
            {
                switch (word)
                {
                    case "private":
                    case "protected":
                    case "public":
                        word = NextTokenText();
                        break;
                    default:
                        break;
                }

                TypeInfo obj = new TypeInfo();
                obj.Code = word;
                obj.Name = word;
                ti.Parents.Add(obj);

                word = GetTokenText(tokenCurrent.next);
                if (word == ",")
                {
                    word = NextTokenText();
                    word = NextTokenText();
                }
            }
        }

        private void ChangeObjectInfo(ObjectInfo obj, string word, string sModifier)
        {
            if (string.IsNullOrEmpty(obj.Type))
            {
                obj.Modifier = sModifier;
                switch (word)
                {
                    case "unsigned":
                        obj.FullType += word;
                        break;
                    case "volitale":
                        obj.IsVolatile = true;
                        obj.FullType += word;
                        break;
                    case "const":
                        obj.IsConst = true;
                        break;
                    case "inline":
                        break;
                    default:
                        obj.Type = word;
                        obj.FullType += word;

                        string wordNext = GetTokenText(tokenCurrent.next);
                        switch (wordNext)
                        {
                            case "<":
                                word = NextTokenText();
                                obj.IsGenericType = true;
                                // 处理类似于THash<int, tagUser>之类的定义
                                while (tokenCurrent != null)
                                {
                                    obj.FullType += word;

                                    if (word == ">") break;
                                    word = NextTokenText();
                                }
                                break;
                            case "[":
                                obj.IsArray = true;
                                word = NextTokenText();
                                while (tokenCurrent != null)
                                {
                                    obj.FullType += word;

                                    if (word == "]") break;
                                    word = NextTokenText();
                                }
                                break;
                            case "*":
                                obj.IsArray = true;
                                obj.IsPointer = true;
                                word = NextTokenText();
                                break;
                            case "&":
                                obj.IsByRef = true;
                                word = NextTokenText();
                                break;
                        }
                        break;
                }
            }
            else if (string.IsNullOrEmpty(obj.Code))
            {
                switch (word)
                {
                    case "*":
                        obj.IsArray = true;
                        obj.IsPointer = true;
                        word = NextTokenText();
                        break;
                    case "&":
                        obj.IsByRef = true;
                        word = NextTokenText();
                        break;
                    case "<":
                        obj.FullType += word;
                        break;
                    default:
                        obj.Code = obj.Name = word;
                        break;
                }
            }
        }

        private void ProcessAType(ref TypeInfo ti, ref string sModifier, bool bIsValueType)
        {
            string word = NextTokenText();
            ti.Name = ti.Code = word;
            word = NextTokenText();
            string wordNext = GetTokenText(tokenCurrent.next);


            if (word == ";" || word == "*" || ";" == wordNext || wordNext == "*")
            {
                ti.IsClass = false;
                return;
            }
            ti.IsClass = true;

            ObjectInfo objTemp = new ObjectInfo(), paramObj = new ObjectInfo();
            List<ObjectInfo> paramList = new List<ObjectInfo>();
            bool isTypeBody = false, isMethord = false;
            Stack<int> stack = new Stack<int>();
            stack.Push(0);
            while (stack.Count > 0)
            {
                switch(word)
                {
                    case "operator":
                        ReadMethordBody(word, null);
                        objTemp.Clear();
                        break;
                    case "volatile":
                        objTemp.IsVolatile = true;
                        break;
                    case "static":
                        objTemp.IsStatic = true;
                        break;
                    case "virtual":
                        objTemp.IsVirtual = true;
                        objTemp.Virtual = word;
                        break;
                    case "WINAPI":
                    case "afx_msg":
                        break;
                    case "private":
                    case "protected":
                    case "public":
                        sModifier = word;
                        break;
                    case "union":
                        objTemp.Type = word;
                        ReadMethordBody(word, null);
                        break;
                    case ":":
                        if (!isTypeBody)
                        {
                            // 处理继承
                            ReadParentInfo(ti, word);
                        }
                        break;
                    case "(":
                        isMethord = true;
                        paramList.Clear();
                        paramObj.Clear();
                        break;
                    case ")":
                        if (isMethord && tokenCurrent.next != null && tokenCurrent.next.next != null && ("=" == GetTokenText(tokenCurrent.next)) && ("0" == GetTokenText(tokenCurrent.next.next)))
                        {
                            objTemp.IsAbstract = true;
                            ti.IsAbstract = true;
                            tokenCurrent = tokenCurrent.next.next;
                        }
                        if (isMethord && wordNext == ":")
                        {
                            // 构造函数后有参数初始化的
                            ReadMethordBody(word, objTemp.Implementation);

                            if (isTypeBody)
                            {
                                if (!string.IsNullOrEmpty(paramObj.Code))
                                {
                                    ObjectInfo param = new ObjectInfo();
                                    param.Store(paramObj);
                                    paramList.Add(param);
                                }
                                paramObj.Clear();

                                // 方法或属性定义结束
                                ti.AddMethord(objTemp, paramList);

                                objTemp.Clear();
                                isMethord = false;
                            }
                        }
                        break;
                    case "}":
                        stack.Pop();
                        break;
                    case "{":
                    case ";":
                    case ",":
                        if (isTypeBody && isMethord)    //  && (word == "," || word == ";")
                        {
                            if (!string.IsNullOrEmpty(paramObj.Code))
                            {
                                ObjectInfo param = new ObjectInfo();
                                param.Store(paramObj);
                                paramList.Add(param);
                            }
                            paramObj.Clear();
                        }

                        if (isTypeBody && word != ",") //  || word == ";"
                        {
                            // 方法或属性定义结束
                            if (isMethord)
                            {
                                // 跳过方法体
                                if (word == "{")
                                {
                                    ReadMethordBody(word, objTemp.Implementation);
                                }

                                ti.AddMethord(objTemp, paramList);
                            }
                            else
                            {
                                ti.AddAttribute(objTemp);
                            }

                            objTemp.Clear();
                            isMethord = false;

                            break;
                        }

                        // 类体定义，方法或属性
                        if (!isTypeBody && word == "{")
                        {
                            isTypeBody = true;
                        }
                        break;
                    case "DECLARE_DYNCREATE":
                    case "DECLARE_MESSAGE_MAP":
                        // 读取到行尾
                        word = ReadToLineEnd();
                        wordNext = GetTokenText(tokenCurrent);
                        continue;
                    default:
                        if (word == "~" && GetTokenText(tokenCurrent.next) == ti.Code)
                        {
                            // 析构函数
                            objTemp.Modifier = sModifier;
                            objTemp.Code = objTemp.Name = word + ti.Code;
                            word = NextTokenText();
                        }
                        else
                        {
                            if (isMethord)
                            {
                                if (word == "=")
                                {
                                    // 忽略参数的默认值
                                    wordNext = GetTokenText(tokenCurrent.next);
                                    Stack sTemp = new Stack();
                                    while ((wordNext != "," && wordNext != ")") || sTemp.Count > 0)
                                    {
                                        switch (wordNext)
                                        {
                                            case "(":
                                                sTemp.Push("(");
                                                break;
                                            case ")":
                                                if (sTemp.Count > 0) sTemp.Pop();
                                                break;
                                        }
                                        word = NextTokenText();
                                        paramObj.Default += word;
                                        if (tokenCurrent == null || tokenCurrent.next == null)
                                        {
                                            break;
                                        }
                                        wordNext = GetTokenText(tokenCurrent.next);
                                    }
                                }
                                else
                                {
                                    // 一般应该为参数
                                    ChangeObjectInfo(paramObj, word, sModifier);
                                }
                            }
                            else
                            {
                                // 可能为构造函数或属性定义【指针】
                                ChangeObjectInfo(objTemp, word, sModifier);
                            }
                        }

                        break;
                }

                if (tokenCurrent == null ||tokenCurrent.next == null)
                {
                    break;
                }
                word = NextTokenText();
                wordNext = GetTokenText(tokenCurrent.next);
            }
        }

        private string ReadTemplate()
        {
            string templateText = "";
            string word = tokenCurrent == null || tokenCurrent.next == null ? "" : GetTokenText(tokenCurrent.next);
            if ("<" == word)
            {
                word = NextTokenText();
                templateText += word;
                word = NextTokenText();
                while(tokenCurrent != null && ">" != word)
                {
                    templateText += word;
                    word = NextTokenText();
                }
                templateText += word;
            }

            return templateText;
        }

        public void ProcessAFile(string aName, Encoding encode)
        {

            Debug.WriteLine(string.Format("reverse file: {0}", aName));

            TextReader reader = new StreamReader(aName, encode);
            _lexer = new LZ.Lexer.CPP.Lexer(reader);

            LZ.Lexer.Token tokenFirst = null, token = null;
            int i = 0;
            do
            {
                i++;
                token = _lexer.NextToken();
                if (token.kind == 0) break;
                if (tokenFirst == null)
                {
                    tokenFirst = token;
                }

                //Debug.WriteLine(string.Format("{0}. {1}", i, GetTokenText(token)));
            }
            while (token.kind != 0);

            tokenCurrent = null;
            tokenCurrent = tokenFirst;
            string sModifier = "private";  // 0=private;1=protected;2=public;
            bool isInTypeDef = false, isParameter = false, isGenericType = false;
            while (tokenCurrent != null && tokenCurrent.next != null)
            {
                string word = tokenCurrent.val;
                TypeInfo ti = null;

                switch (word)
                {
                    case "template":
                        isGenericType = true;
                        ReadTemplate();
                        break;
                    case "class":
                        sModifier = "private";
                        ti = new TypeInfo();
                        ti.Comment = aName;
                        ti.IsGenericType = isGenericType;
                        ProcessAType(ref ti, ref sModifier, false);
                        isGenericType = false;
                        ////if (ti.IsClass) AddClassifier(ti, true);
                        if (!this._work.Contains(ti))
                        {
                            this._work.Push(ti);
                        }
                        break;
                    case "struct":
                        // 跳过在类型定义体中的struct关键字
                        if (isParameter) break;

                        ti = new TypeInfo();
                        ti.Comment = aName;
                        sModifier = "public";
                        ti.IsValueType = true;
                        ti.IsGenericType = isGenericType;
                        ProcessAType(ref ti, ref sModifier, true);
                        isGenericType = false;
                        if (ti.IsClass && !this._work.Contains(ti))
                        {
                            this._work.Push(ti);
                        }
                        ////if (ti.IsClass) AddClassifier(ti, true);
                        break;
                    case "typedef":
                        isInTypeDef = true;
                        break;
                    case ";":
                        isInTypeDef = false;
                        break;
                    case "(":
                        isParameter = true;
                        break;
                    case ")":
                        isParameter = false;
                        break;
                    default:
                        break;
                }

                if (tokenCurrent == null) break;
                tokenCurrent = tokenCurrent.next;
            }

            // 关闭文件
            reader.Close();
            _lexer = null;
        }

        ////private void ProcessType(TypeInfo aType, bool bComplete)
        ////{
        ////    if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
        ////    {
        ////        //aType = aType.GetGenericTypeDefinition();
        ////    }

        ////    bool bCreate = false;
        ////    TypeMapping mapping = TypeMapping.Retrieve(aType, ref bCreate);
        ////    if (mapping != null)
        ////    {
        ////        if (!bComplete)
        ////        {
        ////            if (!mapping.Complete && !this._work.Contains(aType))
        ////            {
        ////                switch (this._recur)
        ////                {
        ////                    case Recursion.LIMITED:
        ////                        if ((this._tabAssemblies != null) && this._tabAssemblies.Contains(aType.Assembly))
        ////                        {
        ////                            this._work.Push(aType);
        ////                        }
        ////                        return;

        ////                    case Recursion.FULL:
        ////                        this._work.Push(aType);
        ////                        return;

        ////                    default:
        ////                        return;
        ////                }
        ////            }
        ////            return;
        ////        }
        ////        if (mapping.Complete)
        ////        {
        ////            return;
        ////        }
        ////    }
        ////    if (aType.HasElementType)
        ////    {
        ////        TypeMapping.DefineExtraType(aType);
        ////    }
        ////    else if (aType.IsEnum)
        ////    {
        ////        this.AddEnumeration(aType);
        ////    }
        ////    else if (aType.IsClass)
        ////    {
        ////        if (aType.IsSubclassOf(typeof(Delegate)))
        ////        {
        ////            this.AddDelegate(aType);
        ////        }
        ////        else
        ////        {
        ////            this.AddClassifier(aType, bComplete);
        ////        }
        ////    }
        ////    else if (aType.IsInterface)
        ////    {
        ////        this.AddClassifier(aType, bComplete);
        ////    }
        ////    else if (aType.IsValueType)
        ////    {
        ////        this.AddClassifier(aType, bComplete);
        ////    }
        ////    else if (LZ.Reverse.Info._bDebug)
        ////    {
        ////        LZ.Reverse.Info.Write(new string[] { "*** unsupported Type type for \"{0}\"", aType.FullName });
        ////    }
        ////}

        public void ProcessWork()
        {
            if (this._work != null)
            {
                while (this._work.Count != 0)
                {
                    TypeInfo aType = (TypeInfo)this._work.Pop();
                    LZ.Reverse.Info.Write(new string[] { "...   " + aType.Code });
                    try
                    {
                        this.AddClassifier(aType, true);
                        ////this.ProcessType(aType, true);
                    }
                    catch (Exception exception)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { exception.StackTrace });
                        }
                    }
                }

                this._work.Clear();
            }
        }

        private static bool ShortenOperator(int aNum, ref string aName)
        {
            if (!LZ.Reverse.Info._bVBNet)
            {
                switch (aName)
                {
                    case "op_Addition":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "+";
                        return true;

                    case "op_BitwiseAnd":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "&";
                        return true;

                    case "op_BitwiseOr":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "|";
                        return true;

                    case "op_Decrement":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "--";
                        return true;

                    case "op_Division":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "/";
                        return true;

                    case "op_Equality":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "==";
                        return true;

                    case "op_ExclusiveOr":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "^";
                        return true;

                    case "op_False":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "false";
                        return true;

                    case "op_GreaterThan":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = ">";
                        return true;

                    case "op_GreaterThanOrEqual":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = ">=";
                        return true;

                    case "op_Increment":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "++";
                        return true;

                    case "op_Inequality":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "!=";
                        return true;

                    case "op_LeftShift":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "<<";
                        return true;

                    case "op_LessThan":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "<";
                        return true;

                    case "op_LessThanOrEqual":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "<=";
                        return true;

                    case "op_LogicalNot":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "!";
                        return true;

                    case "op_Modulus":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "%";
                        return true;

                    case "op_Multiply":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "*";
                        return true;

                    case "op_OnesComplement":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "~";
                        return true;

                    case "op_RightShift":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = ">>";
                        return true;

                    case "op_Subtraction":
                        if (aNum != 2)
                        {
                            return false;
                        }
                        aName = "-";
                        return true;

                    case "op_True":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "true";
                        return true;

                    case "op_UnaryNegation":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "-";
                        return true;

                    case "op_UnaryPlus":
                        if (aNum != 1)
                        {
                            return false;
                        }
                        aName = "+";
                        return true;
                }
            }
            return false;
        }

        private class AttributeTable
        {
            private Hashtable _store;

            ////public void Add(FieldInfo aField)
            ////{
            ////    if (aField != null)
            ////    {
            ////        if (this._store == null)
            ////        {
            ////            this._store = new Hashtable();
            ////        }
            ////        this._store.Add(aField, aField);
            ////    }
            ////}

            ////public bool Contains(FieldInfo aField)
            ////{
            ////    if (aField == null)
            ////    {
            ////        return false;
            ////    }
            ////    if (this._store == null)
            ////    {
            ////        return false;
            ////    }
            ////    return this._store.Contains(aField);
            ////}
        }

        private class InfluenceTable
        {
            private Hashtable _store;

            ////public void Add(MethodInfo aMethod, PdOOM.BaseObject anObject)
            ////{
            ////    this.Add(aMethod, anObject, null);
            ////}

            ////public void Add(MethodInfo aMethod, PdOOM.BaseObject anObject, string aStereo)
            ////{
            ////    if ((aMethod != null) && (aMethod != null))
            ////    {
            ////        if (this._store == null)
            ////        {
            ////            this._store = new Hashtable();
            ////        }
            ////        else if (this._store.Contains(aMethod))
            ////        {
            ////            return;
            ////        }
            ////        Info info = new Info();
            ////        info._obj = anObject;
            ////        info._stereo = aStereo;
            ////        this._store.Add(aMethod, info);
            ////    }
            ////}

            ////public bool Contains(MethodInfo aMethod)
            ////{
            ////    if (aMethod == null)
            ////    {
            ////        return false;
            ////    }
            ////    if (this._store == null)
            ////    {
            ////        return false;
            ////    }
            ////    return this._store.Contains(aMethod);
            ////}

            ////public void Impact(MethodInfo aMethod, Operation anOper)
            ////{
            ////    if ((((aMethod != null) && (anOper != null)) && (this._store != null)) && this._store.Contains(aMethod))
            ////    {
            ////        Info info = (Info) this._store[aMethod];
            ////        anOper.InfluentObject = info._obj;
            ////        if (info._stereo != null)
            ////        {
            ////            anOper.Stereotype = info._stereo;
            ////        }
            ////    }
            ////}

            [StructLayout(LayoutKind.Sequential)]
            private struct Info
            {
                public PdOOM.BaseObject _obj;
                public string _stereo;
            }
        }

        private class MethodTable
        {
            private Hashtable _store;

            ////public void Add(MethodInfo aMethod)
            ////{
            ////    if (aMethod != null)
            ////    {
            ////        if (this._store == null)
            ////        {
            ////            this._store = new Hashtable();
            ////        }
            ////        this._store.Add(aMethod, aMethod);
            ////    }
            ////}

            ////public bool Contains(MethodInfo aMethod)
            ////{
            ////    if (aMethod == null)
            ////    {
            ////        return false;
            ////    }
            ////    if (this._store == null)
            ////    {
            ////        return false;
            ////    }
            ////    return this._store.Contains(aMethod);
            ////}
        }
    }
}

