namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Collections;

    internal class ProperRef
    {
        private static PdOOM.Model _mdl;
        private BaseObject _obj;
        private static Hashtable _tabRefs = new Hashtable();

        private ProperRef(string aKey, BaseObject anObj)
        {
            this._obj = anObj;
            _tabRefs.Add(aKey, this);
        }

        public static BaseObject Link(NamespaceMapping aPckg, TypeMapping aType)
        {
            if (!aType.HasClassifier())
            {
                return null;
            }
            string aKey = aType.FullName;   //aPckg.FullName + "::" + aType.FullName;
            ProperRef ref2 = (ProperRef) _tabRefs[aKey];
            if (ref2 != null)
            {
                return ref2._obj;
            }
            Classifier anObj = aType.Classifier;
            BasePackage newPackage = aPckg.Package;
            BasePackage package = (BasePackage) anObj.Package;
            if (newPackage.ObjectID == package.ObjectID)
            {
                ref2 = new ProperRef(aKey, anObj);
                return anObj;
            }
            BaseObject obj2 = anObj.CreateShortcut(newPackage, "");
            ref2 = new ProperRef(aKey, obj2);
            return obj2;
        }

        public static PdOOM.Model Model
        {
            set
            {
                _tabRefs.Clear();
                _mdl = value;
            }
        }
    }
}

