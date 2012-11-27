namespace LZ.Reverse
{
    using PdOOM;
    using System;
    using System.Reflection;

    internal class CustomHandlerParam : CustomHandler
    {
        private bool _params;

        public CustomHandlerParam(ParameterInfo aParam, Parameter aPrm) : base(aParam.GetCustomAttributes(false), string.Concat(new object[] { aParam.Member.DeclaringType, ".", aParam.Member.Name, ".", aParam.Name }), aPrm)
        {
            base.ProcessAttrs();
        }

        public CustomHandlerParam(ObjectInfo aParam, Parameter aPrm)
            : base(null, aParam.Name, aPrm)
        {
            base.ProcessAttrs();
        }

        protected override bool Process(object anAttr)
        {
            if (TypeInfoHelper.GetTypeFullName(anAttr.GetType()) == "System.ParamArrayAttribute")
            {
                this._params = true;
                return true;
            }
            return false;
        }

        public bool Params
        {
            get
            {
                return this._params;
            }
        }
    }
}

