namespace LZ.Reverse
{
    using PdCommon;
    using PdOOM;
    using System;
    using System.Collections;
    using System.IO;
    using System.Reflection;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Diagnostics;
    using System.Collections.Generic;

    internal class Reverser
    {
        private Application _app;
        private bool _bAllTypes;
        private Hashtable _mapDelegate;
        private Model _mdl;
        private Recursion _recur;
        private Hashtable _tabAssemblies;
        private Stack _work;
        private const string DELEGATE_NAME = "PackageDelegates";
        private const string EXA_C_DELEGATE = "C#.DelegateName";
        private const string EXA_C_ENUMTYPE = "C#.EnumDataType";
        private const string EXA_C_NEW = "C#.new";
        private const string EXA_C_OVERRIDE = "C#.override";
        private const string EXA_C_PARAMS = "C#.params";
        private const string EXA_C_VIRTUAL = "C#.virtual";
        private const string EXA_VB_DEFAULT = "VB.NET.Default";
        private const string EXA_VB_DELEGATE = "VB.NET.DelegateName";
        private const string EXA_VB_ENUMTYPE = "VB.NET.EnumDataType";
        private const string EXA_VB_OVERRIDABLE = "VB.NET.Overridable";
        private const string EXA_VB_OVERRIDES = "VB.NET.Overrides";
        private const string EXA_VB_PARAMS = "VB.NET.ParamArray";
        private const string EXA_VB_SHADOWING = "VB.NET.Shadowing";
        public const string LANG_CS = "C#";
        public const string LANG_CS2 = "C# 2.0";
        public const string LANG_VB = "Visual Basic .NET";
        public const string LANG_VB2005 = "Visual Basic 2005";
        private const string MODEL_CODE = "Reverse";
        private const string MODEL_COMMENT = "Reversed assemblies:\r\n";
        private const string MODEL_NAME = "Reverse";
        private const string MSG_CANTLOAD = "*** could not load Assembly: \"{0}\"";
        private const string MSG_EXCP_ATTNAME = "*** exception while naming the attribute \"{1}\" on the type \"{0}\"";
        private const string MSG_EXCP_CLSSNAME = "*** exception while naming the type \"{0}\"";
        private const string MSG_EXCP_DLGTNAME = "*** exception while naming the delegate method \"{0}\"";
        private const string MSG_EXCP_ENUMALLTYPE = "*** exception accessing the list of all types, replaced by the list of exported types \"{0}\"";
        private const string MSG_EXCP_ENUMTYPE = "*** exception during type enumeration \"{0}\"";
        private const string MSG_EXCP_EVENTNAME = "*** exception while naming the event \"{1}\" on the type \"{0}\"";
        private const string MSG_EXCP_MTHDNAME = "*** exception while naming the operation \"{1}\" on the type \"{0}\"";
        private const string MSG_EXCP_PROPNAME = "*** exception while naming the property \"{1}\" on the type \"{0}\"";
        private const string MSG_EXCP_PROPNOGETTER = "*** exception around the setter for the property \"{1}\" on the type \"{0}\"";
        private const string MSG_EXCP_PROPNOSETTER = "*** exception around the setter for the property \"{1}\" on the type \"{0}\"";
        private const string MSG_INVALIDPARENT = "*** null parent for \"{0}\"";
        private const string MSG_INVDELEGATE = "*** invalid delegate mapping for \"{0}\"";
        private const string MSG_MEMBVISIBILITY = "*** unrecognized visibility for member \"{1}\" on type \"{0}\"";
        private const string MSG_PROCESSING = "... processing assembly \"{0}\"";
        private const string MSG_TYPEVISIBILITY = "*** unrecognized visibility for type \"{0}\"";
        private const string MSG_UNSUPPORTEDTYPE = "*** unsupported Type type for \"{0}\"";
        private const string ST_C_EVENT = "Event";
        private const string ST_CLS_DELEGATE = "Delegate";
        private const string ST_CONSTRUCTOR = "Constructor";
        private const string ST_ENUMERATION = "enumeration";
        private const string ST_EVENTADD = "EventAdd";
        private const string ST_EVENTREMOVE = "EventRemove";
        private const string ST_GETTER = "Getter";
        private const string ST_INDEXER = "Indexer";
        private const string ST_MODULE = "Module";
        private const string ST_OPERATOR = "Operator";
        private const string ST_OPR_DELEGATE = "Delegate";
        private const string ST_PROPERTY = "Property";
        private const string ST_SETTER = "Setter";
        private const string ST_STRUCTURE = "structure";
        private const string ST_VB_EVENT = "event";

        private Reverser()
        {
        }

        public Reverser(bool bAllTypes, Recursion recur, bool bDebug, bool bVBNet, int nVersion)
        {
            Application anApp = new ApplicationClass();
            anApp.Locked = true;
            this.Initialize(anApp, bAllTypes, recur, bDebug, bVBNet, nVersion);
        }

        public Reverser(Application anApp, bool bAllTypes, Recursion recur, bool bDebug, bool bVBNet, int nVersion)
        {
            this.Initialize(anApp, bAllTypes, recur, bDebug, bVBNet, nVersion);
        }

        private void AddClassifier(Type aType, bool bComplete)
        {
            bool bCreate = true;
            if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            {
                aType = aType.GetGenericTypeDefinition();
            }
            TypeMapping mapping = TypeMapping.Retrieve(aType, ref bCreate);
            if (bCreate || bComplete)
            {
                PdOOM.BasePackage package;
                Classifier classifier;
                if (bCreate)
                {
                    bool flag2;
                    PdOOM.NamedObject container = this.GetContainer(aType, out flag2);
                    if (flag2)
                    {
                        package = (PdOOM.BasePackage) container;
                    }
                    else
                    {
                        package = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType)).Package;
                    }
                    int kind = aType.IsInterface ? 0x18112064 : 0x18112063;
                    classifier = (Classifier) container.CreateObject(kind, "", -1, true);
                    mapping.Classifier = classifier;
                    try
                    {
                        classifier.Name = classifier.Code = TypeInfoHelper.GetTypeName(aType);
                    }
                    catch (COMException)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { "*** exception while naming the type \"{0}\"", TypeInfoHelper.GetTypeFullName(aType) });
                        }
                    }
                    classifier.Visibility = GetTypeVisibility(aType);
                    if (aType.IsSealed)
                    {
                        classifier.Final = true;
                    }
                    if (aType.IsClass && aType.IsAbstract)
                    {
                        classifier.Abstract = true;
                    }
                    if (aType.IsGenericType && ((aType.IsInterface || aType.IsClass) || aType.IsValueType))
                    {
                        foreach (TypeParameter parameter in classifier.TypeParameters)
                        {
                            parameter.delete();
                        }
                        Type[] genericArguments = aType.GetGenericArguments();
                        for (int i = 0; i < genericArguments.Length; i++)
                        {
                            TypeParameter parameter2 = (TypeParameter) classifier.CreateObject(0x1811207d, "", -1, true);
                            parameter2.Name = parameter2.Code = TypeInfoHelper.GetTypeName(genericArguments[i]);
                            classifier.TypeParameters.Add(parameter2);
                        }
                        classifier.Generic = true;
                        classifier.Name = classifier.Code = TypeInfoHelper.GetTypeName(aType);
                    }
                    if (aType.IsValueType)
                    {
                        classifier.Stereotype = "structure";
                    }
                    if (!bComplete)
                    {
                        return;
                    }
                }
                else
                {
                    classifier = mapping.Classifier;
                    package = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType)).Package;
                }
                mapping.Complete = true;
                new CustomHandlerType(aType, classifier).Convert();
                if (((aType.BaseType != null) && (aType.BaseType != typeof(object))) && (!aType.IsValueType || (aType.BaseType != typeof(ValueType))))
                {
                    this.CreateGeneralization(classifier, aType.BaseType, package);
                }
                Hashtable hashtable = new Hashtable();
                if (aType.BaseType != null)
                {
                    foreach (Type type2 in aType.BaseType.GetInterfaces())
                    {
                        if (!hashtable.Contains(type2))
                        {
                            hashtable.Add(type2, null);
                        }
                    }
                }
                foreach (Type type3 in aType.GetInterfaces())
                {
                    foreach (Type type4 in type3.GetInterfaces())
                    {
                        if (!hashtable.Contains(type4))
                        {
                            hashtable.Add(type4, null);
                        }
                    }
                }
                foreach (Type type5 in aType.GetInterfaces())
                {
                    if (!hashtable.Contains(type5))
                    {
                        if (aType.IsInterface)
                        {
                            this.CreateGeneralization(classifier, type5, package);
                        }
                        else
                        {
                            this.CreateRealization(classifier, type5, package);
                        }
                    }
                }
                AttributeTable aAttrTable = new AttributeTable();
                MethodTable aTable = new MethodTable();
                InfluenceTable aInfTable = new InfluenceTable();
                BindingFlags bindingAttr = BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (PropertyInfo info in aType.GetProperties(bindingAttr))
                {
                    this.CreateProperty(aType, classifier, info, package, ref aTable);
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (PropertyInfo info2 in aType.GetProperties(bindingAttr))
                {
                    this.CreateProperty(aType, classifier, info2, package, ref aTable);
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (EventInfo info3 in aType.GetEvents(bindingAttr))
                {
                    this.CreateEvent(aType, classifier, info3, package, ref aAttrTable, ref aTable, ref aInfTable);
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (EventInfo info4 in aType.GetEvents(bindingAttr))
                {
                    this.CreateEvent(aType, classifier, info4, package, ref aAttrTable, ref aTable, ref aInfTable);
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (FieldInfo info5 in aType.GetFields(bindingAttr))
                {
                    if (!aAttrTable.Contains(info5))
                    {
                        this.CreateAttribute(aType, classifier, info5, package);
                    }
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (FieldInfo info6 in aType.GetFields(bindingAttr))
                {
                    if (!aAttrTable.Contains(info6))
                    {
                        this.CreateAttribute(aType, classifier, info6, package);
                    }
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (ConstructorInfo info7 in aType.GetConstructors(bindingAttr))
                {
                    this.CreateMethod(aType, classifier, info7, package, null, null);
                }
                foreach (MethodInfo info8 in aType.GetMethods(bindingAttr))
                {
                    if (!aTable.Contains(info8))
                    {
                        this.CreateMethod(aType, classifier, info8, package, null, aInfTable);
                    }
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (ConstructorInfo info9 in aType.GetConstructors(bindingAttr))
                {
                    this.CreateMethod(aType, classifier, info9, package, null, null);
                }
                foreach (MethodInfo info10 in aType.GetMethods(bindingAttr))
                {
                    if (!aTable.Contains(info10))
                    {
                        this.CreateMethod(aType, classifier, info10, package, null, aInfTable);
                    }
                }
                Hashtable hashtable2 = new Hashtable();
                bindingAttr = BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (Type type6 in aType.GetNestedTypes(bindingAttr))
                {
                    if (!hashtable2.Contains(type6))
                    {
                        hashtable2.Add(type6, type6);
                        this.ProcessType(type6, true);
                    }
                }
                bindingAttr = BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly;
                if (this._bAllTypes)
                {
                    bindingAttr |= BindingFlags.NonPublic;
                }
                foreach (Type type7 in aType.GetNestedTypes(bindingAttr))
                {
                    if (!hashtable2.Contains(type7))
                    {
                        hashtable2.Add(type7, type7);
                        this.ProcessType(type7, true);
                    }
                }
            }
        }

        private void AddClassifier(TypeInfo aType, bool bComplete)
        {
            bool bCreate = true;
            TypeMapping mapping = TypeMapping.Retrieve(aType, ref bCreate);

            #region Add classifier
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
                        //package = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType)).Package;
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

                    if (aType.IsClass && aType.IsAbstract)
                    {
                        classifier.Abstract = true;
                    }
                    ////if (aType.IsGenericType && ((aType.IsInterface || aType.IsClass) || aType.IsValueType))
                    ////{
                    ////    foreach (TypeParameter parameter in classifier.TypeParameters)
                    ////    {
                    ////        parameter.delete();
                    ////    }
                    ////    Type[] genericArguments = aType.GetGenericArguments();
                    ////    for (int i = 0; i < genericArguments.Length; i++)
                    ////    {
                    ////        TypeParameter parameter2 = (TypeParameter)classifier.CreateObject(0x1811207d, "", -1, true);
                    ////        parameter2.Name = parameter2.Code = TypeInfoHelper.GetTypeName(genericArguments[i]);
                    ////        classifier.TypeParameters.Add(parameter2);
                    ////    }
                    ////    classifier.Generic = true;
                    ////    classifier.Name = classifier.Code = TypeInfoHelper.GetTypeName(aType);
                    ////}
                    if (aType.IsValueType)
                    {
                        classifier.Stereotype = "structure";
                    }
                    if (!bComplete)
                    {
                        return;
                    }
                }
                else
                {
                    classifier = mapping.Classifier;
                    //package = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType)).Package;
                }
                mapping.Complete = true;
                ////new CustomHandlerType(aType, classifier).Convert();
                ////if (((aType.BaseType != null) && (aType.BaseType != typeof(object))) && (!aType.IsValueType || (aType.BaseType != typeof(ValueType))))
                ////{
                ////    this.CreateGeneralization(classifier, aType.BaseType, package);
                ////}

                ////Hashtable hashtable = new Hashtable();
                ////if (aType.BaseType != null)
                ////{
                ////    foreach (Type type2 in aType.BaseType.GetInterfaces())
                ////    {
                ////        if (!hashtable.Contains(type2))
                ////        {
                ////            hashtable.Add(type2, null);
                ////        }
                ////    }
                ////}
                ////foreach (Type type3 in aType.GetInterfaces())
                ////{
                ////    foreach (Type type4 in type3.GetInterfaces())
                ////    {
                ////        if (!hashtable.Contains(type4))
                ////        {
                ////            hashtable.Add(type4, null);
                ////        }
                ////    }
                ////}
                ////foreach (Type type5 in aType.GetInterfaces())
                ////{
                ////    if (!hashtable.Contains(type5))
                ////    {
                ////        if (aType.IsInterface)
                ////        {
                ////            this.CreateGeneralization(classifier, type5, package);
                ////        }
                ////        else
                ////        {
                ////            this.CreateRealization(classifier, type5, package);
                ////        }
                ////    }
                ////}
                AttributeTable aAttrTable = new AttributeTable();
                MethodTable aTable = new MethodTable();
                InfluenceTable aInfTable = new InfluenceTable();

                foreach (AttributeInfo ai in aType.Attributes)
                {
                    //if (!aAttrTable.Contains(info5))
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
            #endregion Add classifier
        }

        private void AddDelegate(Type aType)
        {
            bool flag;
            PdOOM.BasePackage package;
            Classifier classifier;
            if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            {
                aType = aType.GetGenericTypeDefinition();
            }
            TypeMapping.DefineExtraType(aType).Delegate = true;
            PdOOM.NamedObject container = this.GetContainer(aType, out flag);
            if (flag)
            {
                package = (PdOOM.BasePackage) container;
            }
            else
            {
                package = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType)).Package;
            }
            if (flag)
            {
                if (this._mapDelegate == null)
                {
                    this._mapDelegate = new Hashtable();
                }
                classifier = (Classifier) this._mapDelegate[package];
                if (classifier == null)
                {
                    classifier = (Classifier) package.CreateObject(0x18112063, "", -1, true);
                    classifier.Stereotype = "Delegate";
                    try
                    {
                        classifier.Name = classifier.Code = "PackageDelegates";
                    }
                    catch (COMException)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { "*** exception while naming the type \"{0}\"", aType.FullName });
                        }
                    }
                    this._mapDelegate.Add(package, classifier);
                }
            }
            else
            {
                classifier = (Classifier) container;
            }
            Operation aMethod = (Operation) classifier.CreateObject(0x18112066, "", -1, true);
            aMethod.Stereotype = "Delegate";
            MethodInfo method = aType.GetMethod("Invoke");
            if (method != null)
            {
                Type returnType = method.ReturnType;
                if (returnType.IsArray)
                {
                    aMethod.Array = true;
                    returnType = returnType.GetElementType();
                }
                this.ProcessType(returnType, false);
                TypeMapping mapping3 = TypeMapping.Retrieve(returnType);
                if (mapping3.HasClassifier())
                {
                    PdOOM.BaseObject obj3 = ProperRef.Link(NamespaceMapping.Retrieve(package), mapping3);
                    aMethod.UseQualifiedDataType = false;
                    aMethod.ReturnTypeObject = obj3;
                }
                else
                {
                    aMethod.ReturnType = mapping3.Name;
                }
                foreach (ParameterInfo info2 in method.GetParameters())
                {
                    this.AddParameter(aMethod, info2, package);
                }
            }
            try
            {
                aMethod.Name = aMethod.Code = TypeInfoHelper.GetTypeName(aType);
            }
            catch (COMException)
            {
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** exception while naming the delegate method \"{0}\"", aType.FullName });
                }
            }
        }

        private void AddEnumeration(Type aType)
        {
            if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            {
                aType = aType.GetGenericTypeDefinition();
            }
            bool bCreate = true;
            TypeMapping mapping = TypeMapping.Retrieve(aType, ref bCreate);
            if (bCreate)
            {
                bool flag2;
                PdOOM.BasePackage package;
                PdOOM.NamedObject container = this.GetContainer(aType, out flag2);
                if (flag2)
                {
                    package = (PdOOM.BasePackage) container;
                }
                else
                {
                    package = NamespaceMapping.RetrieveFromChildName(TypeInfoHelper.GetTypeNamespace(aType)).Package;
                }
                Class innerCsfr = (Class) package.CreateObject(0x18112063, "", -1, true);
                innerCsfr.Stereotype = "enumeration";
                mapping.Classifier = innerCsfr;
                mapping.Complete = true;
                if (!flag2)
                {
                    Classifier classifier = (Classifier) container;
                    if (classifier != null)
                    {
                        classifier.AddInnerClassifier(innerCsfr);
                    }
                    else if (LZ.Reverse.Info._bDebug)
                    {
                        LZ.Reverse.Info.Write(new string[] { "*** null parent for \"{0}\"", aType.FullName });
                    }
                }
                innerCsfr.Name = innerCsfr.Code = TypeInfoHelper.GetTypeName(aType);
                innerCsfr.Visibility = GetTypeVisibility(aType);
                Type underlyingType = Enum.GetUnderlyingType(aType);
                TypeMapping mapping3 = TypeMapping.Retrieve(underlyingType);
                string name = underlyingType.Name;
                if (name != "Int32")
                {
                    innerCsfr.SetExtendedAttribute("C#.EnumDataType", mapping3.Name);
                }
                string[] names = Enum.GetNames(aType);
                Array values = Enum.GetValues(aType);
                ulong num = 0;
                for (int i = 0; i < names.Length; i++)
                {
                    ulong num3;
                    PdOOM.Attribute attribute = (PdOOM.Attribute) innerCsfr.CreateObject(0x18112065, "", -1, true);
                    attribute.Name = attribute.Code = names[i];
                    attribute.DataType = mapping3.Name;
                    attribute.Visibility = "+";
                    attribute.Frozen = "F";
                    switch (name)
                    {
                        case "Byte":
                            num3 = (byte) values.GetValue(i);
                            break;

                        case "SByte":
                            num3 = (ulong) ((sbyte) values.GetValue(i));
                            break;

                        case "Int16":
                            num3 = (ulong) ((short) values.GetValue(i));
                            break;

                        case "UInt16":
                            num3 = (ushort) values.GetValue(i);
                            break;

                        case "Int32":
                            num3 = (ulong) ((int) values.GetValue(i));
                            break;

                        case "UInt32":
                            num3 = (uint) values.GetValue(i);
                            break;

                        case "Int64":
                            num3 = (ulong) ((long) values.GetValue(i));
                            break;

                        case "UInt64":
                            num3 = (ulong) values.GetValue(i);
                            break;

                        default:
                            num3 = 0;
                            break;
                    }
                    if (num3 != num)
                    {
                        attribute.InitialValue = num3.ToString();
                    }
                    num = num3 + 1;
                }
            }
        }

        private void AddParameter(Operation aMethod, ParameterInfo aParam, PdOOM.BasePackage aPckg)
        {
            Parameter aPrm = (Parameter) aMethod.CreateObject(0x1811206d, "", -1, true);
            string str = aParam.IsOut ? "O" : "I";
            Type parameterType = aParam.ParameterType;
            if (parameterType.IsGenericType && !parameterType.IsGenericTypeDefinition)
            {
                parameterType = parameterType.GetGenericTypeDefinition();
            }
            if (parameterType.IsByRef)
            {
                if (str == "I")
                {
                    str = "IO";
                }
                parameterType = parameterType.GetElementType();
            }
            if (parameterType.IsArray)
            {
                aPrm.Array = true;
                parameterType = parameterType.GetElementType();
            }
            this.ProcessType(parameterType, false);
            TypeMapping aType = TypeMapping.Retrieve(parameterType);
            if (aType.HasClassifier())
            {
                PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), aType);
                aPrm.UseQualifiedDataType = false;
                aPrm.DataTypeObject = obj2;
            }
            else
            {
                aPrm.DataType = aType.Name;
            }
            aPrm.ParameterType = str;
            aPrm.Name = aPrm.Code = aParam.Name;
            CustomHandlerParam param = new CustomHandlerParam(aParam, aPrm);
            param.Convert();
            if (param.Params)
            {
                aPrm.SetExtendedAttribute("C#.params", "true");
            }
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
            ////if (parameterType.IsByRef)
            ////{
            ////    if (str == "I")
            ////    {
            ////        str = "IO";
            ////    }
            ////    parameterType = parameterType.GetElementType();
            ////}
            ////if (parameterType.IsArray)
            ////{
            ////    aPrm.Array = true;
            ////    parameterType = parameterType.GetElementType();
            ////}
            ////this.ProcessType(parameterType, false);
            ////TypeMapping aType = TypeMapping.Retrieve(aParam.Type);
            ////if (aType.HasClassifier())
            ////{
            ////    PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), aParam.Type);
            ////    aPrm.UseQualifiedDataType = false;
            ////    aPrm.DataTypeObject = obj2;
            ////}
            ////else
            {
                aPrm.DataType = aParam.Name;
            }
            aPrm.ParameterType = str;
            aPrm.Name = aPrm.Code = aParam.Name;
            CustomHandlerParam param = new CustomHandlerParam(aParam, aPrm);
            param.Convert();
            if (param.Params)
            {
                aPrm.SetExtendedAttribute("C#.params", "true");
            }
        }

        public void Close()
        {
            this._mapDelegate = null;
            this._tabAssemblies = null;
            ProperRef.Model = null;
            NamespaceMapping.Model = null;
            TypeMapping.Clear();
            this._mdl = null;
            this._app = null;
        }

        private void CreateAttribute(Type aType, Classifier aCls, FieldInfo aField, PdOOM.BasePackage aPckg)
        {
            CustomHandlerField field = new CustomHandlerField(aField, null);
            if (!field.ThroughProperty)
            {
                PdOOM.Attribute attribute = (PdOOM.Attribute) aCls.CreateObject(0x18112065, "", -1, true);
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
                        LZ.Reverse.Info.Write(new string[] { "*** exception while naming the attribute \"{1}\" on the type \"{0}\"", aType.FullName, aField.Name });
                    }
                }
                attribute.Visibility = GetAttributeVisibility(aField);
                attribute.Persistent = !aField.IsNotSerialized;
                field.PdObj = attribute;
                field.Convert();
                Type fieldType = aField.FieldType;
                if (fieldType.IsArray)
                {
                    attribute.Multiplicity = "*";
                    fieldType = fieldType.GetElementType();
                }
                if (fieldType.IsGenericType && !fieldType.IsGenericTypeDefinition)
                {
                    fieldType = fieldType.GetGenericTypeDefinition();
                }
                this.ProcessType(fieldType, false);
                TypeMapping mapping = TypeMapping.Retrieve(fieldType);
                if (mapping.HasClassifier())
                {
                    PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), mapping);
                    attribute.UseQualifiedDataType = false;
                    attribute.DataTypeObject = obj2;
                }
                else
                {
                    attribute.DataType = mapping.Name;
                }
            }
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
                field.PdObj = attribute;
                field.Convert();
                ////Type fieldType = aField.FieldType;
                ////if (fieldType.IsArray)
                ////{
                ////    attribute.Multiplicity = "*";
                ////    fieldType = fieldType.GetElementType();
                ////}
                ////if (fieldType.IsGenericType && !fieldType.IsGenericTypeDefinition)
                ////{
                ////    fieldType = fieldType.GetGenericTypeDefinition();
                ////}
                ////this.ProcessType(fieldType, false);
                ////TypeMapping mapping = TypeMapping.Retrieve(fieldType);
                ////if (mapping.HasClassifier())
                ////{
                ////    PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), mapping);
                ////    attribute.UseQualifiedDataType = false;
                ////    attribute.DataTypeObject = obj2;
                ////}
                ////else
                {
                    attribute.DataType = ti.Code;   // mapping.Name;
                }
            }
        }


        private void CreateEvent(Type aType, Classifier aCls, EventInfo anEvent, PdOOM.BasePackage aPckg, ref AttributeTable aAttrTable, ref MethodTable aMthTable, ref InfluenceTable aInfTable)
        {
            if (!LZ.Reverse.Info._bVBNet)
            {
                bool flag;
                bool flag2;
                if (aType.GetField(anEvent.Name, BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly) != null)
                {
                    flag = false;
                    flag2 = false;
                }
                else
                {
                    flag = true;
                    flag2 = true;
                }
                if (flag)
                {
                    PdOOM.Attribute anObject = (PdOOM.Attribute) aCls.CreateObject(0x18112065, "", -1, true);
                    anObject.Stereotype = "Event";
                    try
                    {
                        anObject.Name = anObject.Code = anEvent.Name;
                    }
                    catch (COMException)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { "*** exception while naming the event \"{1}\" on the type \"{0}\"", aType.FullName, anEvent.Name });
                        }
                    }
                    Type eventHandlerType = anEvent.EventHandlerType;
                    this.ProcessType(eventHandlerType, false);
                    TypeMapping mapping = TypeMapping.Retrieve(eventHandlerType);
                    anObject.DataType = mapping.Name;
                    if (!mapping.Delegate && LZ.Reverse.Info._bDebug)
                    {
                        LZ.Reverse.Info.Write(new string[] { "*** invalid delegate mapping for \"{0}\"", eventHandlerType.FullName });
                    }
                    MethodInfo addMethod = anEvent.GetAddMethod(true);
                    anObject.Visibility = GetMethodVisibility(addMethod);
                    if (flag2)
                    {
                        aInfTable.Add(addMethod, anObject, "EventAdd");
                        addMethod = anEvent.GetRemoveMethod(true);
                        aInfTable.Add(addMethod, anObject, "EventRemove");
                    }
                }
                if (!flag2)
                {
                    MethodInfo aMethod = anEvent.GetAddMethod(true);
                    aMthTable.Add(aMethod);
                    aMethod = anEvent.GetRemoveMethod(true);
                    aMthTable.Add(aMethod);
                }
            }
        }

        private void CreateGeneralization(PdOOM.BaseObject obj, Type aParentType, PdOOM.BasePackage aPckg)
        {
            this.ProcessType(aParentType, false);
            if (aParentType.IsGenericType && !aParentType.IsGenericTypeDefinition)
            {
                aParentType = aParentType.GetGenericTypeDefinition();
            }
            TypeMapping aType = TypeMapping.Retrieve(aParentType);
            PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), aType);
            Generalization generalization = (Generalization) aPckg.CreateObject(0x18112067, "", -1, true);
            generalization.ParentObject = obj2;
            generalization.ChildObject = obj;
        }

        private void CreateMethod(Type aType, Classifier aCls, MethodBase aMethod, PdOOM.BasePackage aPckg, Operation anOper, InfluenceTable anInfTable)
        {
            Operation operation;
            if (anOper != null)
            {
                operation = anOper;
            }
            else
            {
                operation = (Operation) aCls.CreateObject(0x18112066, "", -1, true);
            }
            operation.Static = aMethod.IsStatic;
            operation.Visibility = GetMethodVisibility(aMethod);
            operation.Final = aMethod.IsFinal;
            if (aMethod.IsGenericMethod)
            {
                operation.Generic = true;
                foreach (TypeParameter parameter in operation.TypeParameters)
                {
                    parameter.delete();
                }
                foreach (Type type in aMethod.GetGenericArguments())
                {
                    TypeParameter parameter2 = (TypeParameter) operation.CreateObject(0x1811207d, "", -1, true);
                    this.ProcessType(type, false);
                    TypeMapping mapping = TypeMapping.Retrieve(type);
                    parameter2.Name = parameter2.Code = mapping.Name;
                    operation.TypeParameters.Add(parameter2);
                }
            }
            if (aMethod.IsVirtual)
            {
                if (LZ.Reverse.Info._bVBNet)
                {
                    operation.SetExtendedAttribute("VB.NET.Overridable", "true");
                }
                else
                {
                    operation.SetExtendedAttribute("C#.virtual", "true");
                }
            }
            if (aMethod.IsAbstract)
            {
                operation.Abstract = true;
            }
            new CustomHandlerMethod(aMethod, operation).Convert();
            if (anOper == null)
            {
                foreach (ParameterInfo info in aMethod.GetParameters())
                {
                    this.AddParameter(operation, info, aPckg);
                }
            }
            bool flag = aMethod.IsConstructor || aMethod.GetType().IsSubclassOf(typeof(ConstructorInfo));
            if (!flag)
            {
                MethodInfo info2 = (MethodInfo) aMethod;
                Type returnType = info2.ReturnType;
                if (returnType.IsArray)
                {
                    operation.Array = true;
                    returnType = returnType.GetElementType();
                }
                this.ProcessType(returnType, false);
                TypeMapping mapping2 = TypeMapping.Retrieve(returnType);
                if (mapping2.HasClassifier())
                {
                    PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), mapping2);
                    operation.UseQualifiedDataType = false;
                    operation.ReturnTypeObject = obj2;
                }
                else
                {
                    operation.ReturnType = mapping2.Name;
                }
                if ((anInfTable != null) && anInfTable.Contains(info2))
                {
                    anInfTable.Impact(info2, operation);
                }
                if (LZ.Reverse.Info._bVBNet)
                {
                    new CustomHandlerReturn(info2, anOper).Convert();
                }
                bool flag2 = false;
                if (!info2.IsHideBySig && info2.GetBaseDefinition().Equals(info2))
                {
                    string name = info2.Name;
                    if (!aType.BaseType.Equals(aType))
                    {
                        BindingFlags bindingAttr = BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
                        bool flag3 = false;
                        foreach (MethodInfo info3 in aType.GetMethods(bindingAttr))
                        {
                            if (info3.Name == name)
                            {
                                flag3 = true;
                                break;
                            }
                        }
                        if (flag3)
                        {
                            flag2 = true;
                        }
                    }
                }
                if (info2.GetBaseDefinition().Equals(info2))
                {
                    if (flag2)
                    {
                        if (LZ.Reverse.Info._bVBNet)
                        {
                            operation.SetExtendedAttribute("VB.NET.Shadowing", "Shadows");
                        }
                        else
                        {
                            operation.SetExtendedAttribute("C#.new", "true");
                        }
                    }
                    else if (aMethod.IsHideBySig && LZ.Reverse.Info._bVBNet)
                    {
                        operation.SetExtendedAttribute("VB.NET.Shadowing", "Overloads");
                    }
                }
                else if (LZ.Reverse.Info._bVBNet)
                {
                    operation.SetExtendedAttribute("VB.NET.Overrides", "true");
                }
                else
                {
                    operation.SetExtendedAttribute("C#.override", "true");
                }
            }
            if (flag)
            {
                operation.Stereotype = "Constructor";
                operation.ReturnType = "";
                operation.InfluentObject = aCls;
                operation.Automatic = true;
                try
                {
                    if (LZ.Reverse.Info._bVBNet)
                    {
                        operation.Name = operation.Code = "New";
                    }
                    else
                    {
                        operation.Name = operation.Code = TypeInfoHelper.GetTypeName(aType);
                    }
                    return;
                }
                catch (COMException)
                {
                    if (LZ.Reverse.Info._bDebug)
                    {
                        LZ.Reverse.Info.Write(new string[] { "*** exception while naming the operation \"{1}\" on the type \"{0}\"", aType.FullName, aMethod.Name });
                    }
                    return;
                }
            }
            if (anOper == null)
            {
                string aName = aMethod.Name;
                if (!LZ.Reverse.Info._bVBNet && ShortenOperator(aMethod.GetParameters().Length, ref aName))
                {
                    operation.Stereotype = "Operator";
                }
                try
                {
                    operation.Name = operation.Code = aName;
                }
                catch (COMException)
                {
                    if (LZ.Reverse.Info._bDebug)
                    {
                        LZ.Reverse.Info.Write(new string[] { "*** exception while naming the operation \"{1}\" on the type \"{0}\"", aType.FullName, aMethod.Name });
                    }
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
            //operation.ReturnType = aMethod.Type;
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
                operation.SetExtendedAttribute("C#.virtual", "true");
            }
            if (aMethod.IsAbstract)
            {
                operation.Abstract = true;
            }
            new CustomHandlerMethod(aMethod, operation).Convert();
            if (anOper == null)
            {
                foreach (ObjectInfo info in aMethod.Parameters)
                {
                    //this.AddParameter(operation, info, aPckg);
                }
            }

            foreach (ObjectInfo type in aMethod.Parameters)
            {
                AddParameter(operation, type, null);
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
            ////bool flag = false;  // aMethod.IsConstructor || aMethod.GetType().IsSubclassOf(typeof(ConstructorInfo));
            ////if (!flag)
            ////{
            ////    MethodInfo info2 = (MethodInfo)aMethod;
            ////    Type returnType = info2.ReturnType;
            ////    if (returnType.IsArray)
            ////    {
            ////        operation.Array = true;
            ////        returnType = returnType.GetElementType();
            ////    }
            ////    this.ProcessType(returnType, false);
            ////    TypeMapping mapping2 = TypeMapping.Retrieve(returnType);
            ////    if (mapping2.HasClassifier())
            ////    {
            ////        PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), mapping2);
            ////        operation.UseQualifiedDataType = false;
            ////        operation.ReturnTypeObject = obj2;
            ////    }
            ////    else
            ////    {
            ////        operation.ReturnType = mapping2.Name;
            ////    }
            ////    if ((anInfTable != null) && anInfTable.Contains(info2))
            ////    {
            ////        anInfTable.Impact(info2, operation);
            ////    }
            ////    if (LZ.Reverse.Info._bVBNet)
            ////    {
            ////        new CustomHandlerReturn(info2, anOper).Convert();
            ////    }
            ////    bool flag2 = false;
            ////    if (!info2.IsHideBySig && info2.GetBaseDefinition().Equals(info2))
            ////    {
            ////        string name = info2.Name;
            ////        if (!aType.BaseType.Equals(aType))
            ////        {
            ////            BindingFlags bindingAttr = BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
            ////            bool flag3 = false;
            ////            foreach (MethodInfo info3 in aType.GetMethods(bindingAttr))
            ////            {
            ////                if (info3.Name == name)
            ////                {
            ////                    flag3 = true;
            ////                    break;
            ////                }
            ////            }
            ////            if (flag3)
            ////            {
            ////                flag2 = true;
            ////            }
            ////        }
            ////    }
            ////    //if (info2.GetBaseDefinition().Equals(info2))
            ////    //{
            ////    //    if (flag2)
            ////    //    {
            ////    //        if (LZ.Reverse.Info._bVBNet)
            ////    //        {
            ////    //            operation.SetExtendedAttribute("VB.NET.Shadowing", "Shadows");
            ////    //        }
            ////    //        else
            ////    //        {
            ////    //            operation.SetExtendedAttribute("C#.new", "true");
            ////    //        }
            ////    //    }
            ////    //    else if (aMethod.IsHideBySig && LZ.Reverse.Info._bVBNet)
            ////    //    {
            ////    //        operation.SetExtendedAttribute("VB.NET.Shadowing", "Overloads");
            ////    //    }
            ////    //}
            ////    else
            ////    {
            ////        operation.SetExtendedAttribute("C#.override", "true");
            ////    }
            ////}
            ////if (flag)
            ////{
            ////    operation.Stereotype = "Constructor";
            ////    operation.ReturnType = "";
            ////    operation.InfluentObject = aCls;
            ////    operation.Automatic = true;
            ////    try
            ////    {
            ////        if (LZ.Reverse.Info._bVBNet)
            ////        {
            ////            operation.Name = operation.Code = "New";
            ////        }
            ////        else
            ////        {
            ////            operation.Name = operation.Code = TypeInfoHelper.GetTypeName(aType);
            ////        }
            ////        return;
            ////    }
            ////    catch (COMException)
            ////    {
            ////        if (LZ.Reverse.Info._bDebug)
            ////        {
            ////            LZ.Reverse.Info.Write(new string[] { "*** exception while naming the operation \"{1}\" on the type \"{0}\"", aType.FullName, aMethod.Name });
            ////        }
            ////        return;
            ////    }
            ////}
            ////if (anOper == null)
            ////{
            ////    string aName = aMethod.Name;
            ////    if (!LZ.Reverse.Info._bVBNet && ShortenOperator(aMethod.GetParameters().Length, ref aName))
            ////    {
            ////        operation.Stereotype = "Operator";
            ////    }
            ////    try
            ////    {
            ////        operation.Name = operation.Code = aName;
            ////    }
            ////    catch (COMException)
            ////    {
            ////        if (LZ.Reverse.Info._bDebug)
            ////        {
            ////            LZ.Reverse.Info.Write(new string[] { "*** exception while naming the operation \"{1}\" on the type \"{0}\"", aType.FullName, aMethod.Name });
            ////        }
            ////    }
            ////}
        }

        private void CreateProperty(Type aType, Classifier aCls, PropertyInfo aProperty, PdOOM.BasePackage aPckg, ref MethodTable aTable)
        {
            bool flag = aProperty.GetIndexParameters().GetLength(0) != 0;
            PdOOM.Attribute anAttr = (PdOOM.Attribute) aCls.CreateObject(0x18112065, "", -1, true);
            if (aProperty.CanRead)
            {
                if (aProperty.CanWrite)
                {
                    anAttr.Frozen = "C";
                }
                else
                {
                    anAttr.Frozen = "R";
                }
            }
            else
            {
                anAttr.Frozen = "A";
            }
            anAttr.Stereotype = LZ.Reverse.Info._bVBNet ? "Property" : (flag ? "Indexer" : "Property");
            Operation anOper = null;
            Operation operation2 = null;
            foreach (PdOOM.BaseObject obj2 in anAttr.DependentObjects)
            {
                if (obj2.ClassKind == 0x18112065)
                {
                    obj2.delete();
                }
                else if (obj2.ClassKind == 0x18112066)
                {
                    Operation operation3 = (Operation) obj2;
                    if (operation3.Stereotype == "Setter")
                    {
                        operation2 = operation3;
                        continue;
                    }
                    if (operation3.Stereotype == "Getter")
                    {
                        anOper = operation3;
                    }
                }
            }
            if (aProperty.CanRead)
            {
                anAttr.Visibility = GetMethodVisibility(aProperty.GetGetMethod(true));
            }
            else if (aProperty.CanWrite)
            {
                anAttr.Visibility = GetMethodVisibility(aProperty.GetSetMethod(true));
            }
            else
            {
                anAttr.Visibility = "-";
            }
            Type propertyType = aProperty.PropertyType;
            if (propertyType.IsArray)
            {
                anAttr.Multiplicity = "*";
                propertyType = propertyType.GetElementType();
            }
            this.ProcessType(propertyType, false);
            TypeMapping mapping = TypeMapping.Retrieve(propertyType);
            if (mapping.HasClassifier())
            {
                PdOOM.BaseObject obj3 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), mapping);
                anAttr.UseQualifiedDataType = false;
                anAttr.DataTypeObject = obj3;
            }
            else
            {
                anAttr.DataType = mapping.Name;
            }
            try
            {
                anAttr.Name = anAttr.Code = aProperty.Name;
            }
            catch (COMException)
            {
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** exception while naming the property \"{1}\" on the type \"{0}\"", aType.FullName, aProperty.Name });
                }
            }
            if (aProperty.CanRead)
            {
                if (anOper != null)
                {
                    MethodInfo getMethod = aProperty.GetGetMethod(true);
                    aTable.Add(getMethod);
                    this.CreateMethod(aType, aCls, getMethod, aPckg, anOper, null);
                }
                else if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** exception around the setter for the property \"{1}\" on the type \"{0}\"", aType.FullName, aProperty.Name });
                }
            }
            if (aProperty.CanWrite)
            {
                if (operation2 != null)
                {
                    MethodInfo setMethod = aProperty.GetSetMethod(true);
                    aTable.Add(setMethod);
                    this.CreateMethod(aType, aCls, setMethod, aPckg, operation2, null);
                }
                else if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** exception around the setter for the property \"{1}\" on the type \"{0}\"", aType.FullName, aProperty.Name });
                }
            }
            CustomHandlerProperty property = new CustomHandlerProperty(aProperty, anAttr);
            property.Convert();
            if (LZ.Reverse.Info._bVBNet && property.Default)
            {
                anAttr.SetExtendedAttribute("VB.NET.Default", "true");
            }
            bool flag2 = false;
            if (LZ.Reverse.Info._bVBNet)
            {
                bool flag3 = false;
                if (aProperty.CanRead && aProperty.GetGetMethod(true).IsHideBySig)
                {
                    flag3 = true;
                }
                else if (aProperty.CanWrite && aProperty.GetSetMethod(true).IsHideBySig)
                {
                    flag3 = true;
                }
                if (flag3 && !flag2)
                {
                    anAttr.SetExtendedAttribute("VB.NET.Shadowing", "Overloads");
                }
            }
        }

        private void CreateRealization(PdOOM.BaseObject obj, Type aParentType, PdOOM.BasePackage aPckg)
        {
            this.ProcessType(aParentType, false);
            if (aParentType.IsGenericType && !aParentType.IsGenericTypeDefinition)
            {
                aParentType = aParentType.GetGenericTypeDefinition();
            }
            TypeMapping aType = TypeMapping.Retrieve(aParentType);
            PdOOM.BaseObject obj2 = ProperRef.Link(NamespaceMapping.Retrieve(aPckg), aType);
            Realization realization = (Realization) aPckg.CreateObject(0x18112068, "", -1, true);
            realization.Interface = obj2;
            realization.Class = obj;
        }

        ~Reverser()
        {
            this.Close();
        }

        private static string GetAttributeVisibility(FieldInfo aField)
        {
            if (aField.IsPublic)
            {
                return "+";
            }
            if (aField.IsPrivate)
            {
                return "-";
            }
            if (aField.IsFamily)
            {
                return "#";
            }
            if (aField.IsFamilyOrAssembly || aField.IsFamilyAndAssembly)
            {
                return "r";
            }
            if (!aField.IsAssembly && LZ.Reverse.Info._bDebug)
            {
                LZ.Reverse.Info.Write(new string[] { "*** unrecognized visibility for member \"{1}\" on type \"{0}\"", aField.DeclaringType.FullName, aField.Name });
            }
            return "i";
        }

        private PdOOM.NamedObject GetContainer(Type aType, out bool bPackage)
        {
            Type declaringType = aType.DeclaringType;
            if (declaringType != null)
            {
                this.ProcessType(declaringType, false);
                TypeMapping mapping = TypeMapping.Retrieve(declaringType);
                bPackage = false;
                return mapping.Classifier;
            }
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

        ////public static int GetDotNetVersion(string sLang)
        ////{
        ////    if (!(sLang == "C# 2.0") && !(sLang == "Visual Basic 2005"))
        ////    {
        ////        return 1;
        ////    }
        ////    return 2;
        ////}

        public static string GetLanguageName(bool bVBNet, int nVersion)
        {
            return "C++";
        }

        private static string GetMethodVisibility(MethodBase aMethod)
        {
            if (aMethod.IsPublic)
            {
                return "+";
            }
            if (aMethod.IsPrivate)
            {
                return "-";
            }
            if (aMethod.IsFamily)
            {
                return "#";
            }
            if (aMethod.IsFamilyOrAssembly || aMethod.IsFamilyAndAssembly)
            {
                return "r";
            }
            if (!aMethod.IsAssembly && LZ.Reverse.Info._bDebug)
            {
                LZ.Reverse.Info.Write(new string[] { "*** unrecognized visibility for member \"{1}\" on type \"{0}\"", aMethod.DeclaringType.FullName, aMethod.Name });
            }
            return "i";
        }

        private static string GetTypeVisibility(Type aType)
        {
            if (aType.IsPublic || aType.IsNestedPublic)
            {
                return "+";
            }
            if (aType.IsNestedPrivate)
            {
                return "-";
            }
            if (aType.IsNestedFamily)
            {
                return "#";
            }
            if (aType.IsNestedFamANDAssem || aType.IsNestedFamORAssem)
            {
                return "r";
            }
            if (!aType.IsNestedAssembly)
            {
                if (aType.IsNotPublic)
                {
                    return "i";
                }
                if (LZ.Reverse.Info._bDebug)
                {
                    LZ.Reverse.Info.Write(new string[] { "*** unrecognized visibility for type \"{0}\"", aType.FullName });
                }
            }
            return "i";
        }

        private static string GetTypeVisibility(string aTypeVisibility)
        {
            string sVisibility = "i";
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


        private void Initialize(Application anApp, bool bAllTypes, Recursion recur, bool bDebug, bool bVBNet, int nVersion)
        {
            this._app = anApp;
            string languageName = GetLanguageName(bVBNet, nVersion);
            LZ.Reverse.Info._bVBNet = bVBNet;
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


        private string GetTokenText(string[] list, LZ.Lexer.Token token)
        {
            if (token == null) return "";

            StringBuilder sb = new StringBuilder();
            if (token.EndLocation.Y == token.Location.Y)
            {
                sb.Append(list[token.line - 1]);
            }
            else
            {
                for (int i = token.Location.Y; i < token.EndLocation.Y; i++ )
                {
                    if (i == token.EndLocation.Y - 1)
                    {
                        sb.Append(list[i]);
                    }
                    else
                    {
                        sb.Append(list[i]).Append("\n");
                    }
                }
            }
            int nLen = token.EndLocation.X - token.Location.X;
            string word = nLen <= 0 ? "" : sb.ToString().Substring(token.Location.X - 1, nLen);
            return word;
        }

        private string NextTokenText(string[] list)
        {
            tokenCurrent = tokenCurrent.next;
            return GetTokenText(list, tokenCurrent);
        }

        private string ReadToLineEnd(string[] list)
        {
            string word = "";
            int nLine = tokenCurrent.line;
            while (nLine == tokenCurrent.line)
            {
                word = NextTokenText(list);
            }

            return word;
        }


        private bool ReadToMethordStart(string[] list)
        {
            bool bIsFind = false;
            string word = "";
            while (word != "{")
            {
                word = NextTokenText(list);
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

        private void ReadMethordBody(string[] list, string currentWord)
        {
            string word = "";
            Stack<string> stack = new Stack<string>();
            if (currentWord == "{" || ReadToMethordStart(list))
            {
                stack.Push("{");
                word = NextTokenText(list);
                if (word == "}")
                {
                    stack.Pop();
                }
                while (stack.Count > 0)
                {
                    word = NextTokenText(list);
                    if (word == "}")
                    {
                        stack.Pop();
                    }
                }
            }
        }

        private void ProcessAType(ref TypeInfo ti, ref string sModifier, string[] list, bool bIsValueType)
        {
            string word = NextTokenText(list);
            ti = new TypeInfo();
            ti.Name = ti.Code = word;
            word = NextTokenText(list);
            string wordNext = GetTokenText(list, tokenCurrent);


            if (word == ";")
            {
                ti.IsClass = false;
                return;
            }
            ti.IsClass = true;

            ObjectInfo obj = null, objTemp = new ObjectInfo(), paramObj = new ObjectInfo();
            MethordInfo mi = null;
            List<ObjectInfo> paramList = new List<ObjectInfo>();
            AttributeInfo attr = null;
            bool isTypeBody = false, isMethord = false, isOperator = false;
            Stack<int> stack = new Stack<int>();
            stack.Push(0);
            while (stack.Count > 0)
            {
                switch(word)
                {
                    case "operator":
                        ReadMethordBody(list, word);
                        objTemp.Clear();
                        break;
                    case "virtual":
                    case "afx_msg":
                        mi = new MethordInfo();
                        mi.Virtual = word;
                        break;
                    case "private":
                    case "protected":
                    case "public":
                        sModifier = word;
                        break;
                    case ":":
                        if (isTypeBody)
                        {
                            
                        }
                        else
                        {
                            word = NextTokenText(list);
                            ////obj = new ObjectInfo();
                            ////switch (word)
                            ////{
                            ////    case "private":
                            ////    case "protected":
                            ////    case "public":
                            ////        obj.Modifier = word;
                            ////        word = NextTokenText(list);
                            ////        break;
                            ////    default:
                            ////        break;
                            ////}
                            ////obj.Code = word;
                            ////obj.Name = word;
                            ////ti.Parents.Add(obj);
                        }
                        break;
                    case "(":
                        if (mi == null)  mi = new MethordInfo();
                        isMethord = true;
                        paramList.Clear();
                        paramObj.Clear();
                        break;
                    case ")":
                        break;
                    case "}":
                        stack.Pop();
                        break;
                    case "{":
                        if (isTypeBody)
                        {
                            // 这里应该是方法

                            // 方法或属性定义结束
                            if (isMethord)
                            {
                                if (mi != null)
                                {
                                    mi.Store(objTemp);
                                    if (!string.IsNullOrEmpty(paramObj.Code))
                                    {
                                        ObjectInfo param = new ObjectInfo();
                                        param.Store(paramObj);
                                        paramList.Add(param);

                                        if (mi.Parameters == null) mi.Parameters = new List<ObjectInfo>();
                                        mi.Parameters.AddRange(paramList);
                                    }
                                    ti.Methords.Add(mi);
                                }
                            }
                            else
                            {
                                if (!string.IsNullOrEmpty(objTemp.Code))
                                {
                                    attr = new AttributeInfo();
                                    attr.Store(objTemp);
                                    ti.Attributes.Add(attr);
                                }
                            }
                            objTemp.Clear();
                            mi = null;
                            attr = null;
                            isMethord = false;

                            ReadMethordBody(list, word);

                            break;
                        }

                        // 类体定义，方法或属性
                        if (!isTypeBody)
                        {
                            isTypeBody = true;
                        }
                        break;
                    case "DECLARE_DYNCREATE":
                    case "DECLARE_MESSAGE_MAP":
                        // 读取到行尾
                        word = ReadToLineEnd(list);
                        wordNext = GetTokenText(list, tokenCurrent);
                        continue;

                    case ";":
                        // 方法或属性定义结束
                        if (isMethord)
                        {
                            if (mi != null)
                            {
                                mi.Store(objTemp);
                                if (!string.IsNullOrEmpty(paramObj.Code))
                                {
                                    ObjectInfo param = new ObjectInfo();
                                    param.Store(paramObj);
                                    paramList.Add(param);

                                    if (mi.Parameters == null) mi.Parameters = new List<ObjectInfo>();
                                    mi.Parameters.AddRange(paramList);
                                }
                                ti.Methords.Add(mi);
                            }
                        }
                        else
                        {
                            if (!string.IsNullOrEmpty(objTemp.Code))
                            {
                                attr = new AttributeInfo();
                                attr.Store(objTemp);
                                ti.Attributes.Add(attr);
                            }
                        }
                        objTemp.Clear();
                        mi = null;
                        attr = null;
                        isMethord = false;
                        break;
                    case ",":
                        if (isMethord)
                        {
                            if (!string.IsNullOrEmpty(paramObj.Code))
                            {
                                ObjectInfo param = new ObjectInfo();
                                param.Store(paramObj);
                                paramList.Add(param);
                            }
                            paramObj.Clear();
                        }
                        break;
                    default:
                        if (word == ti.Code)
                        {
                            // 构造函数
                            objTemp.Modifier = sModifier;
                            objTemp.Code = objTemp.Name = word;
                        }
                        else if(word == "~" && GetTokenText(list, tokenCurrent.next) == ti.Code)
                        {
                            // 析构函数
                            objTemp.Modifier = sModifier;
                            objTemp.Code = objTemp.Name = word + ti.Code;
                            word = NextTokenText(list);
                        }
                        else
                        {
                            if (string.IsNullOrEmpty(objTemp.Type))
                            {
                                objTemp.Modifier = sModifier;
                                switch (word)
                                {
                                    case "const":
                                        objTemp.IsConst = true;
                                        break;
                                    case "inline":
                                        break;
                                    default:
                                        objTemp.Type = word;
                                        break;
                                }
                            }
                            else if (string.IsNullOrEmpty(objTemp.Code))
                            {
                                switch (word)
                                {
                                    case "*":
                                    case "&":
                                        objTemp.Type += word;
                                        break;
                                    default:
                                        objTemp.Code = objTemp.Name = word;
                                        break;
                                }
                            }
                            else if (isMethord)
                            {
                                // 一般应该为参数

                                if (string.IsNullOrEmpty(paramObj.Type))
                                {
                                    paramObj.Modifier = sModifier;

                                    if (word == "const")
                                    {
                                        paramObj.IsConst = true;
                                    }
                                    else
                                    {
                                        paramObj.Type = word;
                                    }
                                }
                                else if (string.IsNullOrEmpty(paramObj.Code))
                                {
                                    switch(word)
                                    {
                                        case "*":
                                        case "&":
                                            paramObj.Type += word;
                                            break;
                                        default:
                                            paramObj.Code = paramObj.Name = word;
                                            break;
                                    }
                                }
                            }
                        }

                        break;
                }

                if (tokenCurrent.next == null)
                {
                    break;
                }
                word = NextTokenText(list);
                wordNext = GetTokenText(list, tokenCurrent.next);
            }
        }

        public void ProcessAssembly(string aName, Encoding encode)
        {
            TextReader reader = new StreamReader(aName, encode);
            StringBuilder sb = new StringBuilder();
            sb.Append(reader.ReadToEnd());
            reader.Close();
            reader = new StreamReader(aName, encode);
            LZ.Lexer.CPP.Lexer lex = new LZ.Lexer.CPP.Lexer(reader);

            string[] list = sb.ToString().Split('\n');
            LZ.Lexer.Token tokenFirst = null, token = null;
            int i = 0;
            do
            {
                i++;
                token = lex.NextToken();
                if (token.kind == 0) break;
                if (tokenFirst == null)
                {
                    tokenFirst = token;
                }

                Debug.WriteLine(string.Format("{0}. {1}", i, GetTokenText(list, token)));
            }
            while (token.kind != 0);

            tokenCurrent = null;
            this._work = new Stack();
            tokenCurrent = tokenFirst;
            string sModifier = "private";  // 0=private;1=protected;2=public;
            while (tokenCurrent != null && tokenCurrent.next != null)
            {
                string line = list[tokenCurrent.line - 1];
                int nLen = tokenCurrent.EndLocation.X - tokenCurrent.Location.X;
                string word = line.Substring(tokenCurrent.Location.X - 1, nLen);


                TypeInfo ti = null;

                switch (word)
                {
                    case "class":
                        sModifier = "private";

                        ProcessAType(ref ti, ref sModifier, list, false);
                        if (ti.IsClass) AddClassifier(ti, true);
                        ////if (!this._work.Contains(ti))
                        ////{
                        ////    this._work.Push(ti);
                        ////}
                        break;
                    case "struct":
                        sModifier = "public";
                        ProcessAType(ref ti, ref sModifier, list, true);
                        if (ti.IsClass) AddClassifier(ti, true);
                        break;
                    default:
                        break;
                }
                ////this._work = new Stack();
                ////foreach (Type type in types)
                ////{
                ////    if (!this._work.Contains(type))
                ////    {
                ////        this._work.Push(type);
                ////    }
                ////}
                ////this.ProcessWork();


                tokenCurrent = tokenCurrent.next;
            }
            reader.Close();

            ////Assembly assembly;
            ////try
            ////{
            ////    assembly = Assembly.LoadFrom(aName);
            ////    LZ.Reverse.Info.Write(new string[] { "... processing assembly \"{0}\"", aName });
            ////}
            ////catch (FileNotFoundException)
            ////{
            ////    LZ.Reverse.Info.Write(new string[] { "*** could not load Assembly: \"{0}\"", aName });
            ////    return;
            ////}
            ////catch (FileLoadException)
            ////{
            ////    LZ.Reverse.Info.Write(new string[] { "*** could not load Assembly: \"{0}\"", aName });
            ////    return;
            ////}
            ////catch (Exception)
            ////{
            ////    LZ.Reverse.Info.Write(new string[] { "*** could not load Assembly: \"{0}\"", aName });
            ////    return;
            ////}
            ////if (this._mdl.Comment == "")
            ////{
            ////    this._mdl.Comment = "Reversed assemblies:\r\n";
            ////}
            ////this._mdl.Comment = string.Concat(new object[] { this._mdl.Comment, aName, " (", assembly.GetName(), ")\r\n" });
            ////Type[] types = new Type[0];
            ////try
            ////{
            ////    if (this._bAllTypes)
            ////    {
            ////        try
            ////        {
            ////            types = assembly.GetTypes();
            ////        }
            ////        catch (ReflectionTypeLoadException exception)
            ////        {
            ////            if (LZ.Reverse.Info._bDebug)
            ////            {
            ////                LZ.Reverse.Info.Write(new string[] { "*** exception accessing the list of all types, replaced by the list of exported types \"{0}\"", exception.ToString() });
            ////            }
            ////            types = assembly.GetExportedTypes();
            ////            if (this._recur == Recursion.NONE)
            ////            {
            ////                this._recur = Recursion.LIMITED;
            ////            }
            ////        }
            ////    }
            ////    else
            ////    {
            ////        types = assembly.GetExportedTypes();
            ////    }
            ////}
            ////catch (Exception exception2)
            ////{
            ////    if (LZ.Reverse.Info._bDebug)
            ////    {
            ////        LZ.Reverse.Info.Write(new string[] { "*** exception during type enumeration \"{0}\"", exception2.ToString() });
            ////    }
            ////}
            ////this._work = new Stack();
            ////foreach (Type type in types)
            ////{
            ////    if (!this._work.Contains(type))
            ////    {
            ////        this._work.Push(type);
            ////    }
            ////}
            ////this.ProcessWork();
        }

        private void ProcessType(Type aType, bool bComplete)
        {
            if (aType.IsGenericType && !aType.IsGenericTypeDefinition)
            {
                aType = aType.GetGenericTypeDefinition();
            }
            TypeMapping mapping = TypeMapping.Retrieve(aType);
            if (mapping != null)
            {
                if (!bComplete)
                {
                    if (!mapping.Complete && !this._work.Contains(aType))
                    {
                        switch (this._recur)
                        {
                            case Recursion.LIMITED:
                                if ((this._tabAssemblies != null) && this._tabAssemblies.Contains(aType.Assembly))
                                {
                                    this._work.Push(aType);
                                }
                                return;

                            case Recursion.FULL:
                                this._work.Push(aType);
                                return;

                            default:
                                return;
                        }
                    }
                    return;
                }
                if (mapping.Complete)
                {
                    return;
                }
            }
            if (aType.HasElementType)
            {
                TypeMapping.DefineExtraType(aType);
            }
            else if (aType.IsEnum)
            {
                this.AddEnumeration(aType);
            }
            else if (aType.IsClass)
            {
                if (aType.IsSubclassOf(typeof(Delegate)))
                {
                    this.AddDelegate(aType);
                }
                else
                {
                    this.AddClassifier(aType, bComplete);
                }
            }
            else if (aType.IsInterface)
            {
                this.AddClassifier(aType, bComplete);
            }
            else if (aType.IsValueType)
            {
                this.AddClassifier(aType, bComplete);
            }
            else if (LZ.Reverse.Info._bDebug)
            {
                LZ.Reverse.Info.Write(new string[] { "*** unsupported Type type for \"{0}\"", aType.FullName });
            }
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

        public void ProcessTypeList(IEnumerator anEnum)
        {
            if (anEnum != null)
            {
                anEnum.Reset();
                this._work = new Stack();
                while (anEnum.MoveNext())
                {
                    string current = (string) anEnum.Current;
                    Type type = Type.GetType(current);
                    if ((type != null) && !this._work.Contains(type))
                    {
                        this._work.Push(type);
                    }
                }
                this.ProcessWork();
            }
        }

        private void ProcessWork()
        {
            if (this._work != null)
            {
                while (this._work.Count != 0)
                {
                    Type aType = (Type) this._work.Pop();
                    LZ.Reverse.Info.Write(new string[] { "...   " + aType.FullName });
                    try
                    {
                        this.ProcessType(aType, true);
                    }
                    catch (Exception exception)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { exception.StackTrace });
                        }
                    }
                }
                this._work = null;
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

            public void Add(FieldInfo aField)
            {
                if (aField != null)
                {
                    if (this._store == null)
                    {
                        this._store = new Hashtable();
                    }
                    this._store.Add(aField, aField);
                }
            }

            public bool Contains(FieldInfo aField)
            {
                if (aField == null)
                {
                    return false;
                }
                if (this._store == null)
                {
                    return false;
                }
                return this._store.Contains(aField);
            }
        }

        private class InfluenceTable
        {
            private Hashtable _store;

            public void Add(MethodInfo aMethod, PdOOM.BaseObject anObject)
            {
                this.Add(aMethod, anObject, null);
            }

            public void Add(MethodInfo aMethod, PdOOM.BaseObject anObject, string aStereo)
            {
                if ((aMethod != null) && (aMethod != null))
                {
                    if (this._store == null)
                    {
                        this._store = new Hashtable();
                    }
                    else if (this._store.Contains(aMethod))
                    {
                        return;
                    }
                    Info info = new Info();
                    info._obj = anObject;
                    info._stereo = aStereo;
                    this._store.Add(aMethod, info);
                }
            }

            public bool Contains(MethodInfo aMethod)
            {
                if (aMethod == null)
                {
                    return false;
                }
                if (this._store == null)
                {
                    return false;
                }
                return this._store.Contains(aMethod);
            }

            public void Impact(MethodInfo aMethod, Operation anOper)
            {
                if ((((aMethod != null) && (anOper != null)) && (this._store != null)) && this._store.Contains(aMethod))
                {
                    Info info = (Info) this._store[aMethod];
                    anOper.InfluentObject = info._obj;
                    if (info._stereo != null)
                    {
                        anOper.Stereotype = info._stereo;
                    }
                }
            }

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

            public void Add(MethodInfo aMethod)
            {
                if (aMethod != null)
                {
                    if (this._store == null)
                    {
                        this._store = new Hashtable();
                    }
                    this._store.Add(aMethod, aMethod);
                }
            }

            public bool Contains(MethodInfo aMethod)
            {
                if (aMethod == null)
                {
                    return false;
                }
                if (this._store == null)
                {
                    return false;
                }
                return this._store.Contains(aMethod);
            }
        }
    }
}

