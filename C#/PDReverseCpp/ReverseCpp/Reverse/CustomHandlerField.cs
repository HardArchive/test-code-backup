namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Reflection;

    internal class CustomHandlerField : CustomHandler
    {
        private bool _through;

        public CustomHandlerField(FieldInfo aField, PdOOM.Attribute anAttr) : base(aField.GetCustomAttributes(false), aField.DeclaringType + "." + aField.Name, anAttr)
        {
            this._through = false;
            base.ProcessAttrs();
        }

        public CustomHandlerField(AttributeInfo aField, PdOOM.Attribute anAttr)
            : base(null, aField.Name, anAttr)
        {
            this._through = false;
            base.ProcessAttrs();
        }

       protected override bool Process(object anAttr)
        {
            if (TypeInfoHelper.GetTypeFullName(anAttr.GetType()) == "System.Runtime.CompilerServices.AccessedThroughPropertyAttribute")
            {
                this._through = true;
                return true;
            }
            return false;
        }

        public bool ThroughProperty
        {
            get
            {
                return this._through;
            }
        }
    }
}

