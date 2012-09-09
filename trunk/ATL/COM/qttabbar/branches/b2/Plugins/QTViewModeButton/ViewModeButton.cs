//    This file is part of QTTabBar, a shell extension for Microsoft
//    Windows Explorer.
//    Copyright (C) 2010  Quizo, Paul Accisano
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
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

using QTPlugin;
using QTPlugin.Interop;


namespace QuizoPlugins {
    [Plugin(PluginType.Interactive, Author = "Quizo", Name = "Change Folder View Mode", Version = "1.0.0.0", Description = "Button to change folder viewmode for XP.")]
    public class ViewModeButton : IBarCustomItem {
        private IPluginServer pluginServer;
        private IShellBrowser shellBrowser;

        private ToolStripButton button;
        private FolderViewModeWindow fvmw;

        private string[] ResStrs;


        #region IPluginClient members

        public void Open(IPluginServer pluginServer, IShellBrowser shellBrowser) {
            this.pluginServer = pluginServer;
            this.shellBrowser = shellBrowser;

            if(!this.pluginServer.TryGetLocalizedStrings(this, 7, out this.ResStrs)) {
                // if localized strings not found, falls back to default strings in assembly resource of this dll.

                // this plugin has 2 language resources in assemly, but usually there's no need to do like this.
                // it only has to have a resource of your own language.

                if(System.Globalization.CultureInfo.CurrentCulture.Parent.Name == "ja")
                    this.ResStrs = Resource.viewModes_Ja.Split(new char[] { ';' });
                else
                    this.ResStrs = Resource.viewModes.Split(new char[] { ';' });
            }

            this.pluginServer.NavigationComplete += new PluginEventHandler(pluginServer_NavigationComplete);
        }

        public void Close(EndCode code) {
            this.pluginServer = null;
            this.shellBrowser = null;

            if(this.button != null) {
                this.button.Dispose();
            }

            if(this.fvmw != null) {
                this.fvmw.Dispose();
            }
        }

        public bool QueryShortcutKeys(out string[] actions) {
            actions = null;
            return false;
        }

        public void OnMenuItemClick(MenuType menuType, string menuText, ITab tab) {
        }

        public bool HasOption {
            get {
                return false;
            }
        }

        public void OnOption() {
        }

        public void OnShortcutKeyPressed(int iIndex) {
        }

        #endregion


        #region IBarCustomItem members

        public ToolStripItem CreateItem(bool fLarge, DisplayStyle displayStyle) {
            if(this.button == null) {
                this.button = new ToolStripButton();
                this.button.Text =
                this.button.ToolTipText = this.ResStrs[6];
                this.button.MouseDown += new MouseEventHandler(button_MouseDown);
            }

            switch(displayStyle) {
                case DisplayStyle.NoLabel:
                case DisplayStyle.SelectiveText:
                    this.button.DisplayStyle = ToolStripItemDisplayStyle.Image;
                    break;
                case DisplayStyle.ShowTextLabel:
                    this.button.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText;
                    break;
            }

            this.button.Image = fLarge ? Resource.ViewModeButton_large : Resource.ViewModeButton_small;

            return this.button;
        }

        #endregion



        private void pluginServer_NavigationComplete(object sender, PluginEventArgs e) {
            this.UpdateButtonImage(this.GetCurrentViewMode());
        }

        private void button_MouseDown(object sender, MouseEventArgs e) {
            if(e.Button == MouseButtons.Left) {
                if(this.fvmw == null) {
                    this.fvmw = new FolderViewModeWindow(this.ResStrs);
                    this.fvmw.ViewModeChanged += new EventHandler(this.fvmw_ViewModeChanged);
                }

                this.fvmw.ShowWindow(Control.MousePosition, this.GetCurrentViewMode());
            }
        }

        private void fvmw_ViewModeChanged(object sender, EventArgs e) {
            IShellView shellView = null;
            try {
                if(0 == this.shellBrowser.QueryActiveShellView(out shellView)) {
                    IFolderView folderView = shellView as IFolderView;

                    if(folderView != null) {
                        FOLDERVIEWMODE currentMode = 0;
                        folderView.GetCurrentViewMode(ref currentMode);

                        FOLDERVIEWMODE mode = this.fvmw.ViewMode;

                        if(currentMode != mode) {
                            folderView.SetCurrentViewMode(mode);
                            this.UpdateButtonImage(mode);
                        }
                    }
                }
            }
            catch {
            }
            finally {
                if(shellView != null)
                    Marshal.ReleaseComObject(shellView);
            }
        }


        private FOLDERVIEWMODE GetCurrentViewMode() {
            IShellView shellView = null;
            try {
                if(0 == this.shellBrowser.QueryActiveShellView(out shellView)) {
                    IFolderView folderView = shellView as IFolderView;

                    if(folderView != null) {
                        FOLDERVIEWMODE currentMode = 0;
                        folderView.GetCurrentViewMode(ref currentMode);

                        return currentMode;
                    }
                }
            }
            catch {
            }
            finally {
                if(shellView != null)
                    Marshal.ReleaseComObject(shellView);
            }

            return FOLDERVIEWMODE.FVM_ICON;
        }

        private void UpdateButtonImage(FOLDERVIEWMODE mode) {
            switch(mode) {
                case FOLDERVIEWMODE.FVM_THUMBSTRIP:
                    this.button.Image = Resource.imgFilm;
                    break;
                case FOLDERVIEWMODE.FVM_THUMBNAIL:
                    this.button.Image = Resource.imgThumb;
                    break;
                case FOLDERVIEWMODE.FVM_TILE:
                    this.button.Image = Resource.imgTiles;
                    break;
                case FOLDERVIEWMODE.FVM_ICON:
                    this.button.Image = Resource.imgIcon;
                    break;
                case FOLDERVIEWMODE.FVM_LIST:
                    this.button.Image = Resource.imgList;
                    break;
                case FOLDERVIEWMODE.FVM_DETAILS:
                    this.button.Image = Resource.imgDetails;
                    break;
            }
        }
    }
}
