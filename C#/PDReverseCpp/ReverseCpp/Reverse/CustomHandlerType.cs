namespace LZ.Reverse
{
    using PdOOM;
    using System;

    internal class CustomHandlerType : CustomHandler
    {
        private bool _default;
        private bool _module;

        public CustomHandlerType(Type aType, Classifier aCls) : base(aType.GetCustomAttributes(false), aType.FullName, aCls)
        {
            this._default = false;
            this._module = false;
            base.ProcessAttrs();
        }

        protected override bool Process(object anAttr)
        {
            switch (TypeInfoHelper.GetTypeFullName(anAttr.GetType()))
            {
                case "Microsoft.VisualBasic.CompilerServices.StandardModuleAttribute":
                    this._module = true;
                    return true;

                case "System.Reflection.DefaultMemberAttribute":
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

        public bool Module
        {
            get
            {
                return this._module;
            }
        }
    }
}

