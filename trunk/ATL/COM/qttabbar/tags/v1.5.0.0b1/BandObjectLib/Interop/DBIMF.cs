//    This file is part of QTTabBar, a shell extension for Microsoft
//    Windows Explorer.
//    Copyright (C) 2002-2010  Pavel Zolnikov, Quizo, Paul Accisano
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

namespace BandObjectLib {
    [Flags]
    public enum DBIMF : uint {
        ADDTOFRONT = 0x200,
        BKCOLOR = 0x40,
        BREAK = 0x100,
        DEBOSSED = 0x20,
        FIXED = 1,
        FIXEDBMP = 4,
        NORMAL = 0,
        TOPALIGN = 0x400,
        UNDELETEABLE = 0x10,
        USECHEVRON = 0x80,
        VARIABLEHEIGHT = 8
    }
}
