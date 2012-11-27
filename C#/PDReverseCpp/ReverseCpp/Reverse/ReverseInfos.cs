using System;
using System.Collections.Generic;
using System.Text;

namespace LZ.Reverse
{
    /// <summary>
    /// 类型数据
    /// </summary>
    public class TypeInfo: ObjectInfo
    {
        public TypeInfo()
        {
            Attributes = new List<AttributeInfo>();
            Methords = new List<MethordInfo>();
            Parents = new List<TypeInfo>();

            Namespace = null;
        }

        public void AddAttribute(ObjectInfo objTemp)
        {
            if (!string.IsNullOrEmpty(objTemp.Code))
            {
                AttributeInfo attr = new AttributeInfo();
                attr.Store(objTemp);
                this.Attributes.Add(attr);
            }
        }

        public void AddMethord(ObjectInfo objTemp, List<ObjectInfo> paramList)
        {
            MethordInfo mi = new MethordInfo();

            mi.Store(objTemp);
            if (paramList.Count > 0)
            {
                if (mi.Parameters == null) mi.Parameters = new List<ObjectInfo>();
                mi.Parameters.AddRange(paramList);
            }
            if (string.IsNullOrEmpty(mi.Code) && mi.Type == this.Code)
            {
                // 构造函数
                mi.Code = mi.Name = mi.Type;
                mi.Type = "";
            }
            this.Methords.Add(mi);
        }

        public string Namespace;

        /// <summary>
        /// 
        /// </summary>
        public List<TypeInfo> Parents;

        /// <summary>
        /// 
        /// </summary>
        public List<AttributeInfo> Attributes;

        /// <summary>
        /// 
        /// </summary>
        public List<MethordInfo> Methords;
    }

    /// <summary>
    /// 方法类型数据
    /// </summary>
    public class MethordInfo : ObjectInfo
    {
        public List<ObjectInfo> Parameters;

        public MethordInfo()
        {
            Parameters = new List<ObjectInfo>();
        }
    }

    /// <summary>
    /// 属性类型数据
    /// </summary>
    public class AttributeInfo: ObjectInfo
    {
    }
    
    /// <summary>
    /// 对象信息
    /// </summary>
    public class ObjectInfo
    {
        /// <summary>
        /// 名称
        /// </summary>
        public string Name;
        
        /// <summary>
        /// 编码
        /// </summary>
        public string Code;
        
        /// <summary>
        /// 注释
        /// </summary>
        public string Comment;

        /// <summary>
        /// 类型
        /// </summary>
        public string Type;
        /// <summary>
        /// 类型附加信息
        /// </summary>
        public string FullType;

        /// <summary>
        /// 访问符
        /// </summary>
        public string Modifier;

        /// <summary>
        /// 访问符
        /// </summary>
        public string Virtual;

        /// <summary>
        /// 默认值
        /// </summary>
        public string Default;


        /// <summary>
        /// 实现内容，主要是指定操作
        /// </summary>
        public StringBuilder Implementation;


        public bool IsStatic;
        public bool IsConst;
        public bool IsArray;
        public bool IsAbstract;
        public bool IsClass;
        public bool IsInterface;
        public bool IsGenericType;
        public bool IsValueType;
        public bool IsGenericTypeDefinition;
        public bool IsVirtual;
        public bool IsGenericMethod;
        public bool IsVolatile;
        public bool IsByRef;
        public bool IsPointer;


        public void Store(ObjectInfo obj)
        {
            this.Modifier = obj.Modifier;
            this.Code = obj.Code;
            this.Name = obj.Name;
            this.Type = obj.Type;
            this.Virtual = obj.Virtual;
            this.Comment = obj.Comment;

            this.IsStatic = obj.IsStatic;
            this.IsConst = obj.IsConst;
            this.IsArray = obj.IsArray;
            this.IsAbstract = obj.IsAbstract;
            this.IsClass = obj.IsClass;
            this.IsInterface = obj.IsInterface;
            this.IsGenericType = obj.IsGenericType;
            this.IsValueType = obj.IsValueType;
            this.IsGenericTypeDefinition = obj.IsGenericTypeDefinition;
            this.IsVirtual = obj.IsVirtual;
            this.IsGenericMethod = obj.IsGenericMethod;
            this.IsVolatile = obj.IsVolatile;
            this.IsByRef = obj.IsByRef;
            this.IsPointer = obj.IsPointer;
            this.FullType = obj.FullType;
            this.Implementation.Length = 0;
            this.Implementation.Append(obj.Implementation.ToString());
            this.Default = obj.Default;
        }

        public ObjectInfo Clone()
        {
            ObjectInfo obj = new ObjectInfo();

            obj.Store(this);

            return obj;
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public ObjectInfo()
        {
            this.Implementation = new StringBuilder();
            Clear();
        }

        /// <summary>
        /// 转换为字符串
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return this.Code;
        }

        public void Clear()
        {
            this.Modifier = "public";
            this.Code = this.Name = this.Virtual = this.Comment = this.Type = this.FullType = this.Default = "";
            this.Implementation.Length = 0;
            IsStatic = false;
            IsConst = false;
            IsArray = false;
            IsAbstract = false;
            IsClass = false;
            IsInterface = false;
            IsGenericType = false;
            IsValueType = false;
            IsGenericTypeDefinition = false;
            IsVirtual = false;
            IsGenericMethod = false;
            IsVolatile = false;
            IsByRef = false;
            IsPointer = false;
        }
    }
}
