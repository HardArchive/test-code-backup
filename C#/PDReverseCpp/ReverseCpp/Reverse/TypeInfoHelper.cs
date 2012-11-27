namespace LZ.Reverse
{
    using System;

    internal class TypeInfoHelper
    {
        public static string GetTypeFullName(Type aType)
        {
            if (aType.IsGenericType)
            {
                if (aType.FullName != null)
                {
                    return aType.FullName;
                }
                return (aType.Namespace + "." + GetTypeName(aType));
            }
            if (aType.IsGenericParameter)
            {
                return aType.Name;
            }
            return aType.FullName;
        }

        public static string GetTypeName(Type aType)
        {
            if (aType.IsGenericType)
            {
                if (aType.Name != null)
                {
                    if (!aType.IsNested || !aType.IsValueType)
                    {
                        int index = aType.Name.IndexOf('`');
                        if (index != -1)
                        {
                            return aType.Name.Remove(index);
                        }
                    }
                    return aType.Name;
                }
                return null;
            }
            return aType.Name;
        }

        public static string GetTypeNamespace(Type aType)
        {
            if (aType.Namespace == null)
            {
                return "";
            }
            return (aType.Namespace + "." + GetTypeName(aType));
        }

        public static string GetTypeNamespace(TypeInfo aType)
        {
            if (aType.Namespace == null)
            {
                return "";
            }

            return "";
            ////return (aType.Namespace + "." + aType.Code);
        }
    }
}

