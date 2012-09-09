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

namespace QTTabBarLib {
    using QTPlugin;
    using System;
    using System.Drawing;

    internal sealed class PluginInformation : IDisposable {
        public string Author;
        public string Description;
        public bool Enabled;
        public Image ImageLarge;
        public Image ImageSmall;
        public int Index;
        public string Name;
        public string Path;
        public string PluginID;
        public QTPlugin.PluginType PluginType;
        public string[] ShortcutKeyActions;
        public string TypeFullName;
        public string Version;

        public PluginInformation(PluginAttribute pluginAtt, string path, string pluginID, string typeFullName) {
            this.Author = pluginAtt.Author;
            this.Name = pluginAtt.Name;
            this.Version = pluginAtt.Version;
            this.Description = pluginAtt.Description;
            this.PluginType = pluginAtt.PluginType;
            this.Path = path;
            this.PluginID = pluginID;
            this.TypeFullName = typeFullName;
        }

        public PluginInformation Clone(int index) {
            PluginInformation information = (PluginInformation)base.MemberwiseClone();
            information.Index = index;
            return information;
        }

        public void Dispose() {
            if(this.ImageLarge != null) {
                this.ImageLarge.Dispose();
                this.ImageLarge = null;
            }
            if(this.ImageSmall != null) {
                this.ImageSmall.Dispose();
                this.ImageSmall = null;
            }
        }
    }
}
