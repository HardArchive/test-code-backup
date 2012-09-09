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

using System;
using System.Runtime.InteropServices;
using QTPlugin.Interop;

namespace QTPlugin {
    [StructLayout(LayoutKind.Sequential)]
    public struct Address {
        public byte[] ITEMIDLIST;
        public string Path;
        public Address(IntPtr pidl, string path) {
            ITEMIDLIST = PInvoke.GetIDListData(pidl);
            Path = path;
        }

        public Address(IntPtr pidl) {
            ITEMIDLIST = PInvoke.GetIDListData(pidl);
            Path = null;
        }

        public Address(string path) {
            ITEMIDLIST = null;
            Path = path;
        }

        public Address(byte[] idl, string path) {
            ITEMIDLIST = idl;
            Path = path;
        }
    }
}
