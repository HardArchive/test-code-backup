namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Reflection;

    internal class CustomHandlerProperty : CustomHandler
    {
        private bool _default;

        public CustomHandlerProperty(PropertyInfo aProperty, PdOOM.Attribute anAttr) : base(aProperty.GetCustomAttributes(false), aProperty.DeclaringType + "." + aProperty.Name, anAttr)
        {
            base.ProcessAttrs();
        }

        protected override bool Process(object anAttr)
        {
            if (TypeInfoHelper.GetTypeFullName(anAttr.GetType()) == "System.Reflection.DefaultMemberAttribute")
            {
                this._default = true;
                return true;
            }
            return false;
        }

        public bool Default
        {
            get
            {
                return this._default;
            }
        }
    }
}

