﻿//    This file is part of QTTabBar, a shell extension for Microsoft
//    Windows Explorer.
//    Copyright (C) 2007-2010  Quizo, Paul Accisano
//
//    QTTabBar is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    QTTabBar is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with QTTabBar.  If not, see <http://www.gnu.org/licenses/>.

using QTTabBarLib.Interop;
using System;
using System.Runtime.InteropServices;
using System.Drawing;

namespace QTTabBarLib.Automation {
    // This class is for use in AutomationManager.  Never instantiate it
    // elsewhere.

    class AutomationElement : IDisposable {
        private static readonly Guid IID_IUIAutomationRegistrar = new Guid("{8609C4EC-4A1A-4D88-A357-5A66E060E1CF}");
        private static readonly Guid CLSID_CUIAutomationRegistrar = new Guid("{6E29FABF-9977-42D1-8D0E-CA7E61AD87E6}");
        private static readonly Guid ItemCount_Property_GUID = new Guid("{ABBF5C45-5CCC-47b7-BB4E-87CB87BBD162}");
        private static readonly Guid SelectedItemCount_Property_GUID = new Guid("{8FE316D2-0E52-460a-9C1E-48F273D470A3}");
        private static readonly Guid ItemIndex_Property_GUID = new Guid("{92A053DA-2969-4021-BF27-514CFC2E4A69}");

        private const int UIAutomationType_Int = 1;
        private const int UIA_SelectionItemPatternId = 10010;
        private static int UIA_ItemCountPropertyId;
        private static int UIA_SelectedCountPropertyId;
        private static int UIA_ItemIndexPropertyId;
        private const int UIA_BoundingRectanglePropertyId = 30001;
        private const int UIA_NamePropertyId = 30005;
        private const int UIA_AutomationIdPropertyId = 30011;
        private const int UIA_ClassNamePropertyId = 30012;

        AutomationElementFactory factory;
        IUIAutomationElement pElement;

        static AutomationElement() {
            // Register the ItemsView custom properties, as documented here:
            // http://msdn.microsoft.com/en-us/library/ff486373%28VS.85%29.aspx

            Guid rclsid = CLSID_CUIAutomationRegistrar;
            Guid riid = IID_IUIAutomationRegistrar;
            object obj;
            PInvoke.CoCreateInstance(ref rclsid, IntPtr.Zero, 1, ref riid, out obj);
            IUIAutomationRegistrar pRegistrar = obj as IUIAutomationRegistrar;
            try {
                UIAutomationPropertyInfo propinfo;
                propinfo = new UIAutomationPropertyInfo {
                    guid = ItemCount_Property_GUID,
                    pProgrammaticName = "ItemCount",
                    type = UIAutomationType_Int
                };
                pRegistrar.RegisterProperty(ref propinfo, out UIA_ItemCountPropertyId);
                propinfo = new UIAutomationPropertyInfo {
                    guid = SelectedItemCount_Property_GUID,
                    pProgrammaticName = "SelectedItemCount",
                    type = UIAutomationType_Int
                };
                pRegistrar.RegisterProperty(ref propinfo, out UIA_SelectedCountPropertyId);
                propinfo = new UIAutomationPropertyInfo {
                    guid = ItemIndex_Property_GUID,
                    pProgrammaticName = "ItemIndex",
                    type = UIAutomationType_Int
                };
                pRegistrar.RegisterProperty(ref propinfo, out UIA_ItemIndexPropertyId);
            }
            finally {
                if(pRegistrar != null) Marshal.ReleaseComObject(pRegistrar);
            }
        }

        internal AutomationElement(IUIAutomationElement pElement, AutomationElementFactory factory) {
            this.factory = factory;
            this.pElement = pElement;
            factory.AddToDisposeList(this);
        }

        ~AutomationElement() {
            Dispose();
        }

        public void Dispose() {
            if(pElement != null) {
                Marshal.ReleaseComObject(pElement);
                pElement = null;
            }
            GC.SuppressFinalize(this);
        }


        public AutomationElement FindMatchingChild(Predicate<AutomationElement> pred) {
            IUIAutomationTreeWalker walker = factory.CreateTreeWalker();
            try {
                IUIAutomationElement elem;
                walker.GetFirstChildElement(pElement, out elem);
                while(elem != null) {
                    AutomationElement ae = new AutomationElement(elem, factory);
                    if(pred(ae)) {
                        return ae;
                    }
                    IUIAutomationElement next;
                    walker.GetNextSiblingElement(elem, out next);
                    elem = next;
                }
                return null;
            }
            finally {
                if(walker != null) Marshal.ReleaseComObject(walker);
            }
        }

        public bool IsSelected() {
            object obj;
            pElement.GetCurrentPattern(UIA_SelectionItemPatternId, out obj);
            try {
                if(obj == null) {
                    return false;
                }
                IUIAutomationSelectionItemPattern selprov = obj as IUIAutomationSelectionItemPattern;
                bool ret;
                selprov.get_CurrentIsSelected(out ret);
                return ret;
            }
            finally {
                if(obj != null) Marshal.ReleaseComObject(obj);
            }
        }

        public string GetAutomationId() {
            object obj;
            pElement.GetCurrentPropertyValue(UIA_AutomationIdPropertyId, out obj);
            return obj.ToString();
        }

        public Rectangle GetBoundingRect() {
            object obj;
            pElement.GetCurrentPropertyValue(UIA_BoundingRectanglePropertyId, out obj);
            double[] rect = obj as double[];
            return new Rectangle((int)rect[0], (int)rect[1], (int)rect[2], (int)rect[3]);
        }

        public string GetClassName() {
            object obj;
            pElement.GetCurrentPropertyValue(UIA_ClassNamePropertyId, out obj);
            return obj.ToString();
        }

        public AutomationElement GetFirstChild() {
            IUIAutomationTreeWalker walker = factory.CreateTreeWalker();
            try {
                IUIAutomationElement elem;
                walker.GetFirstChildElement(pElement, out elem);
                return (elem == null) ? null : new AutomationElement(elem, factory);
            }
            finally {
                if(walker != null) Marshal.ReleaseComObject(walker);
            }
        }

        // Only valid for ItemsView element.
        public int GetItemCount() {
            object obj;
            pElement.GetCurrentPropertyValue(UIA_ItemCountPropertyId, out obj);
            return (int)obj;
        }
        
        // Only valid for ListItem element.
        public int GetItemIndex() {
            object obj;
            pElement.GetCurrentPropertyValue(UIA_ItemIndexPropertyId, out obj);
            return (int)obj - 1;
        }

        public string GetItemName() {
            object obj;
            pElement.GetCurrentPropertyValue(UIA_NamePropertyId, out obj);
            return obj.ToString();
        }

        public AutomationElement GetNextSibling() {
            IUIAutomationTreeWalker walker = factory.CreateTreeWalker();
            try {
                IUIAutomationElement elem;
                walker.GetNextSiblingElement(pElement, out elem);
                return (elem == null) ? null : new AutomationElement(elem, factory);
            }
            finally {
                if(walker != null) Marshal.ReleaseComObject(walker);
            }

        }

        public AutomationElement GetParent() {
            IUIAutomationTreeWalker walker = factory.CreateTreeWalker();
            try {
                IUIAutomationElement elem;
                walker.GetParentElement(pElement, out elem);
                return (elem == null) ? null : new AutomationElement(elem, factory);
            }
            finally {
                if(walker != null) Marshal.ReleaseComObject(walker);
            }
        }

        // Only valid for ItemsView element.
        public int GetSelectedCount() {
            object obj;
            pElement.GetCurrentPropertyValue(UIA_SelectedCountPropertyId, out obj);
            return (int)obj;
        }
    }
}
