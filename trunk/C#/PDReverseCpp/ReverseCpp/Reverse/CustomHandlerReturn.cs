namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Reflection;

    internal class CustomHandlerReturn : CustomHandler
    {
        public CustomHandlerReturn(MethodInfo aMethod, Operation anOper) : base(null, aMethod.DeclaringType + "." + aMethod.Name, anOper)
        {
            ICustomAttributeProvider returnTypeCustomAttributes = aMethod.ReturnTypeCustomAttributes;
            if (returnTypeCustomAttributes != null)
            {
                base._atts = returnTypeCustomAttributes.GetCustomAttributes(false);
                base.ProcessAttrs();
            }
        }

        public override void Convert()
        {
            base.Convert();
        }
    }
}

