namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Reflection;

    internal class CustomHandlerMethod : CustomHandler
    {
        public CustomHandlerMethod(MethodBase aMethod, Operation anOper) : base(aMethod.GetCustomAttributes(false), aMethod.DeclaringType + "." + aMethod.Name, anOper)
        {
            base.ProcessAttrs();
        }

        public CustomHandlerMethod(MethordInfo aMethod, Operation anOper)
            : base(null, aMethod.Name, anOper)
        {
            base.ProcessAttrs();
        }
    }
}

