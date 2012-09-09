//    This file is part of QTTabBar, a shell extension for Microsoft
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

namespace QTTabBarLib.Interop {
    using BandObjectLib;
    using QTTabBarLib;
    using System;
    using System.Runtime.CompilerServices;
    using System.Runtime.InteropServices;
    using System.Runtime.InteropServices.ComTypes;
    using System.Windows.Forms;

    internal class DropTargetWrapper : _IDropTarget, IDisposable {
        private const short CF_HDROP = 15;
        private IntPtr hwnd;
        private int iLastKeyState;
        private const int MK_ALT = 0x20;
        private const int MK_CONTROL = 8;
        private const int MK_LBUTTON = 1;
        private const int MK_RBUTTON = 2;
        private const int MK_SHIFT = 4;

        public event EventHandler DragDropEnd;

        public event DragFileDropEventHandler DragFileDrop;

        public event DragFileEnterEventHandler DragFileEnter;

        public event EventHandler DragFileLeave;

        public event DragEventHandler DragFileOver;

        public DropTargetWrapper(Control ctrl) {
            this.hwnd = ctrl.Handle;
            PInvoke.RegisterDragDrop(this.hwnd, this);
        }

        public void Dispose() {
            if(this.hwnd != IntPtr.Zero) {
                PInvoke.RevokeDragDrop(this.hwnd);
                this.hwnd = IntPtr.Zero;
            }
        }

        public int DragDrop(System.Runtime.InteropServices.ComTypes.IDataObject pDataObj, int grfKeyState, BandObjectLib.POINT pt, ref DragDropEffects pdwEffect) {
            try {
                if(this.DragFileOver != null) {
                    DragEventArgs e = new DragEventArgs(null, grfKeyState, pt.x, pt.y, DragDropEffects.Move | DragDropEffects.Copy | DragDropEffects.Scroll, pdwEffect);
                    this.DragFileOver(null, e);
                    pdwEffect = e.Effect;
                }
                else {
                    pdwEffect = DragDropEffects.Copy;
                }
                if(pdwEffect != DragDropEffects.None) {
                    if(this.DragFileDrop != null) {
                        IntPtr ptr;
                        byte[] buffer;
                        switch(this.DragFileDrop(out ptr, out buffer)) {
                            case -1:
                                return 0;

                            case 0: {
                                    IShellFolder ppv = null;
                                    object obj2 = null;
                                    Guid riid = ExplorerGUIDs.IID_IShellFolder;
                                    Guid guid2 = ExplorerGUIDs.IID_IDropTarget;
                                    using(IDLWrapper wrapper = new IDLWrapper(buffer)) {
                                        if(wrapper.Available && wrapper.IsDropTarget) {
                                            try {
                                                IntPtr ptr2;
                                                if(PInvoke.SHBindToParent(wrapper.PIDL, riid, out ppv, out ptr2) == 0) {
                                                    uint rgfReserved = 0;
                                                    IntPtr[] apidl = new IntPtr[] { ptr2 };
                                                    if(ppv.GetUIObjectOf(ptr, 1, apidl, ref guid2, ref rgfReserved, out obj2) == 0) {
                                                        _IDropTarget target = obj2 as _IDropTarget;
                                                        if(target != null) {
                                                            DragDropEffects effects = pdwEffect;
                                                            if(target.DragEnter(pDataObj, this.iLastKeyState, pt, ref effects) == 0) {
                                                                effects = pdwEffect;
                                                                if(target.DragOver(this.iLastKeyState, pt, ref effects) == 0) {
                                                                    if((this.iLastKeyState & 2) != 0) {
                                                                        pdwEffect = DragDropEffects.Link | DragDropEffects.Move | DragDropEffects.Copy;
                                                                    }
                                                                    return target.DragDrop(pDataObj, this.iLastKeyState, pt, ref pdwEffect);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            catch {
                                            }
                                            finally {
                                                if(ppv != null) {
                                                    Marshal.ReleaseComObject(ppv);
                                                }
                                                if(obj2 != null) {
                                                    Marshal.ReleaseComObject(obj2);
                                                }
                                                if(this.DragDropEnd != null) {
                                                    this.DragDropEnd(this, EventArgs.Empty);
                                                }
                                            }
                                        }
                                    }
                                    return 0;
                                }
                        }
                    }
                    FORMATETC format = new FORMATETC();
                    format.cfFormat = 15;
                    format.ptd = IntPtr.Zero;
                    format.dwAspect = DVASPECT.DVASPECT_CONTENT;
                    format.lindex = -1;
                    format.tymed = TYMED.TYMED_HGLOBAL;
                    STGMEDIUM medium = new STGMEDIUM();
                    try {
                        pDataObj.GetData(ref format, out medium);
                        PInvoke.SendMessage(this.hwnd, 0x233, medium.unionmember, IntPtr.Zero);
                    }
                    catch {
                    }
                    finally {
                        PInvoke.ReleaseStgMedium(ref medium);
                    }
                }
            }
            finally {
                if(pDataObj != null) {
                    Marshal.FinalReleaseComObject(pDataObj);
                }
            }
            return 0;
        }

        public int DragEnter(System.Runtime.InteropServices.ComTypes.IDataObject pDataObj, int grfKeyState, BandObjectLib.POINT pt, ref DragDropEffects pdwEffect) {
            try {
                if(this.DragFileEnter != null) {
                    FORMATETC format = new FORMATETC();
                    format.cfFormat = 15;
                    format.ptd = IntPtr.Zero;
                    format.dwAspect = DVASPECT.DVASPECT_CONTENT;
                    format.lindex = -1;
                    format.tymed = TYMED.TYMED_HGLOBAL;
                    if(pDataObj.QueryGetData(ref format) >= 0) {
                        STGMEDIUM medium = new STGMEDIUM();
                        try {
                            try {
                                pDataObj.GetData(ref format, out medium);
                                pdwEffect = this.DragFileEnter(medium.unionmember, pt, grfKeyState);
                            }
                            catch {
                                pdwEffect = DragDropEffects.None;
                            }
                            goto Label_00A0;
                        }
                        finally {
                            PInvoke.ReleaseStgMedium(ref medium);
                        }
                    }
                    pdwEffect = DragDropEffects.None;
                }
                else {
                    pdwEffect = DragDropEffects.Copy;
                }
            }
            finally {
                if(pDataObj != null) {
                    Marshal.FinalReleaseComObject(pDataObj);
                }
            }
        Label_00A0:
            return 0;
        }

        public int DragLeave() {
            if(this.DragFileLeave != null) {
                this.DragFileLeave(null, EventArgs.Empty);
            }
            return 0;
        }

        public int DragOver(int grfKeyState, BandObjectLib.POINT pt, ref DragDropEffects pdwEffect) {
            this.iLastKeyState = grfKeyState;
            if(this.DragFileOver != null) {
                DragEventArgs e = new DragEventArgs(null, grfKeyState, pt.x, pt.y, DragDropEffects.Move | DragDropEffects.Copy | DragDropEffects.Scroll, DragDropEffects.None);
                this.DragFileOver(null, e);
                pdwEffect = e.Effect;
            }
            else {
                pdwEffect = DragDropEffects.Copy;
            }
            return 0;
        }

        internal static DragDropEffects MakeEffect(int grfKeyState, int iSourceState) {
            switch((grfKeyState & 0x2c)) {
                case 12:
                case 0x20:
                    return DragDropEffects.Link;

                case 4:
                    if(iSourceState == 3) {
                        return DragDropEffects.Copy;
                    }
                    return DragDropEffects.Move;

                case 8:
                    return DragDropEffects.Copy;
            }
            if((iSourceState != 1) && (iSourceState != 3)) {
                return DragDropEffects.Move;
            }
            return DragDropEffects.Copy;
        }

        internal delegate int DragFileDropEventHandler(out IntPtr hwnd, out byte[] idlReal);

        internal delegate DragDropEffects DragFileEnterEventHandler(IntPtr hDrop, BandObjectLib.POINT pnt, int grfKeyState);
    }
}
