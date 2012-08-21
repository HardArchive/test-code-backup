// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__1A0BACBD_B6DB_4CBC_B1DC_4B71F126003B__INCLUDED_)
#define AFX_MAINDLG_H__1A0BACBD_B6DB_4CBC_B1DC_4B71F126003B__INCLUDED_

#pragma once

class CMainDlg : 
   public CDialogImpl<CMainDlg>,
   public CDialogSkin<CMainDlg, IDB_BACKGROUND>
{
public:
   enum { IDD = IDD_MAINDLG };

   CDib24 m_dibLogo;
   CRoundDibButtonCtrl m_ctlNew;
   CRoundDibButtonCtrl m_ctlOpen;
   CRoundDibButtonCtrl m_ctlSave;
   CRoundDibButtonCtrl m_ctlSaveAs;
   CRoundDibButtonCtrl m_ctlRollOver;
   CRoundDibButtonCtrl m_ctlWordUp;
   CRoundDibButtonCtrl m_ctlExit;
   CRoundDibButtonCtrl m_ctlHelp;
   CRoundDibButtonCtrl m_ctlEdit;
   CRoundDibButtonCtrl m_ctlHouse;
   CRoundDibButtonCtrl m_ctlUp;
   CRoundDibButtonCtrl m_ctlDown;
   CRoundDibButtonCtrl m_ctlSmallDown;
   CRoundDibButtonCtrl m_ctlSmallPlus;
   CRoundDibButtonCtrl m_ctlExport;
   CRoundDibButtonCtrl m_ctlEye;

   CRoundDibFrameCtrl m_ctlFrame1;
   CRoundDibFrameCtrl m_ctlFrame2;
   CRoundDibFrameCtrl m_ctlFrame3;
   CRoundDibFrameCtrl m_ctlFrame4;
   CRoundDibFrameCtrl m_ctlFrame5;
   CRoundDibFrameCtrl m_ctlFrame6;
   CRoundDibFrameCtrl m_ctlFrame7;
   CRoundDibFrameCtrl m_ctlFrame8;
   CRoundDibFrameCtrl m_ctlFrame9;
   CRoundDibFrameCtrl m_ctlFrame10;
   CRoundDibFrameCtrl m_ctlFrame11;
   CRoundDibFrameCtrl m_ctlFrame12;
   CRoundDibFrameCtrl m_ctlFrame13;
   CRoundDibFrameCtrl m_ctlFrame14;
   CRoundDibFrameCtrl m_ctlFrame15;

   CShadedTextCtrl m_ctlText1;
   CShadedTextCtrl m_ctlText2;
   CShadedTextCtrl m_ctlText3;
   CShadedTextCtrl m_ctlText4;
   CShadedTextCtrl m_ctlText5;
   CShadedTextCtrl m_ctlText6;
   CShadedTextCtrl m_ctlText7;
   CShadedTextCtrl m_ctlText8;
   CShadedTextCtrl m_ctlText9;

   CRoundColorFrameCtrl m_ctlSample1;
   CRoundColorFrameCtrl m_ctlSample2;
   CRoundColorFrameCtrl m_ctlSample3;
   CRoundColorFrameCtrl m_ctlSample4;
   CRoundColorFrameCtrl m_ctlSample5;
   CRoundColorFrameCtrl m_ctlSample6;

   CListViewCtrl m_list1;
   CComboBox m_combo1;
   CImageList m_images;
   CTreeViewCtrl m_tree1;

   CBrush m_brPurple;

   typedef CDialogSkin<CMainDlg, IDB_BACKGROUND> skinClass;

   BEGIN_MSG_MAP(CMainDlg)
      MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
      CHAIN_MSG_MAP(skinClass)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
      COMMAND_ID_HANDLER(IDOK, OnOK)
      COMMAND_ID_HANDLER(IDCANCEL, OnOK)
   END_MSG_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Set icons
      HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
         IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
      SetIcon(hIcon, TRUE);
      HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
         IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
      SetIcon(hIconSmall, FALSE);

      m_dibLogo.LoadBitmap(IDB_LOGO);

      m_ctlNew.SubclassWindow(GetDlgItem(IDC_NEW));
      m_ctlNew.CreateButtons(IDB_NEW, IDB_BUTTON1, IDB_BACKGROUND);
      m_ctlOpen.SubclassWindow(GetDlgItem(IDC_OPEN));
      m_ctlOpen.CreateButtons(IDB_OPEN, IDB_BUTTON1, IDB_BACKGROUND);
      m_ctlSave.SubclassWindow(GetDlgItem(IDC_SAVE));
      m_ctlSave.CreateButtons(IDB_SAVE, IDB_BUTTON1, IDB_BACKGROUND);
      m_ctlSaveAs.SubclassWindow(GetDlgItem(IDC_SAVEAS));
      m_ctlSaveAs.CreateButtons(IDB_SAVEAS, IDB_BUTTON1, IDB_BACKGROUND);
      m_ctlRollOver.SubclassWindow(GetDlgItem(IDC_ROLLOVER));
      m_ctlRollOver.CreateButtons(IDB_ROLLOVER, IDB_BUTTON1, IDB_BACKGROUND);
      m_ctlWordUp.SubclassWindow(GetDlgItem(IDC_WORDUP));
      m_ctlWordUp.CreateButtons(IDB_WORDUP, IDB_BUTTON1, IDB_BACKGROUND);
      m_ctlHelp.SubclassWindow(GetDlgItem(IDC_HELPFILE));
      m_ctlHelp.CreateButtons(IDB_HELP, IDB_BUTTON2, IDB_GREY);
      m_ctlEdit.SubclassWindow(GetDlgItem(IDC_EDIT));
      m_ctlEdit.CreateButtons(IDB_EDIT, IDB_BUTTON3, IDB_WHITE);
      m_ctlHouse.SubclassWindow(GetDlgItem(IDC_HOUSE));
      m_ctlHouse.CreateButtons(IDB_HOUSE, IDB_BUTTON3, IDB_WHITE);
      m_ctlUp.SubclassWindow(GetDlgItem(IDC_UP));
      m_ctlUp.CreateButtons(IDB_UP, IDB_BUTTON3, IDB_WHITE);
      m_ctlDown.SubclassWindow(GetDlgItem(IDC_DOWN));
      m_ctlDown.CreateButtons(IDB_DOWN, IDB_BUTTON3, IDB_WHITE);
      m_ctlSmallPlus.SubclassWindow(GetDlgItem(IDC_SMALLPLUS));
      m_ctlSmallPlus.CreateButtons(IDB_SMALLPLUS, IDB_BUTTON4, IDB_BACKGROUND);
      m_ctlSmallDown.SubclassWindow(GetDlgItem(IDC_SMALLDOWN));
      m_ctlSmallDown.CreateButtons(IDB_SMALLDOWN, IDB_BUTTON4, IDB_LUMGREY);
      m_ctlEye.SubclassWindow(GetDlgItem(IDC_EYE));
      m_ctlEye.CreateButtons(IDB_EYE, IDB_BUTTON4, IDB_LUMGREY);
      m_ctlExport.SubclassWindow(GetDlgItem(IDC_EXPORT));
      m_ctlExport.CreateButtons(IDB_EXPORT, IDB_BUTTON4, IDB_LUMGREY);

#define COLOR_BLACK RGB(0,0,0)
#define COLOR_WHITE RGB(255,255,255)
#define COLOR_STD_FRAME_LIGHT RGB(245,245,245)
#define COLOR_STD_FRAME_DARK RGB(126,125,110)
#define COLOR_STD_FRAME_BACK RGB(226,223,223)
#define COLOR_PURPLE_FRAME_BACK RGB(223,224,239)
#define COLOR_TOOLTIP_FRAME_BACK RGB(224,223,208)

      m_brPurple.CreateSolidBrush(COLOR_PURPLE_FRAME_BACK);

      m_ctlFrame1.SubclassWindow(GetDlgItem(IDC_FRAME1));
      m_ctlFrame1.CreateFrame(8,4, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, RGB(160,167,208));
      m_ctlFrame2.SubclassWindow(GetDlgItem(IDC_FRAME2));
      m_ctlFrame2.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_STD_FRAME_DARK, COLOR_TOOLTIP_FRAME_BACK);
      m_ctlFrame3.SubclassWindow(GetDlgItem(IDC_FRAME3));
      m_ctlFrame3.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_STD_FRAME_DARK, COLOR_STD_FRAME_BACK);
      m_ctlFrame4.SubclassWindow(GetDlgItem(IDC_FRAME4));
      m_ctlFrame4.CreateFrame(8,4, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_WHITE);
      m_ctlFrame5.SubclassWindow(GetDlgItem(IDC_FRAME5));
      m_ctlFrame5.CreateFrame(8,5, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_WHITE);
      m_ctlFrame6.SubclassWindow(GetDlgItem(IDC_FRAME6));
      m_ctlFrame6.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_STD_FRAME_DARK, COLOR_STD_FRAME_BACK);
      m_ctlFrame7.SubclassWindow(GetDlgItem(IDC_FRAME7));
      m_ctlFrame7.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_WHITE, IDB_LUMGREY);
      m_ctlFrame9.SubclassWindow(GetDlgItem(IDC_FRAME9));
      m_ctlFrame9.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_STD_FRAME_DARK, COLOR_STD_FRAME_BACK);
      m_ctlFrame8.SubclassWindow(GetDlgItem(IDC_FRAME8));
      m_ctlFrame8.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_WHITE, IDB_LUMGREY);
      m_ctlFrame10.SubclassWindow(GetDlgItem(IDC_FRAME10));
      m_ctlFrame10.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_STD_FRAME_DARK, COLOR_STD_FRAME_BACK);
      m_ctlFrame11.SubclassWindow(GetDlgItem(IDC_FRAME11));
      m_ctlFrame11.CreateFrame(8,0, COLOR_STD_FRAME_LIGHT, COLOR_STD_FRAME_DARK, COLOR_STD_FRAME_BACK);
      m_ctlFrame12.SubclassWindow(GetDlgItem(IDC_FRAME12));
      m_ctlFrame12.CreateFrame(8,4, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_PURPLE_FRAME_BACK);
      m_ctlFrame13.SubclassWindow(GetDlgItem(IDC_FRAME13));
      m_ctlFrame13.CreateFrame(8,4, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_PURPLE_FRAME_BACK);
      m_ctlFrame14.SubclassWindow(GetDlgItem(IDC_FRAME14));
      m_ctlFrame14.CreateFrame(8,4, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_PURPLE_FRAME_BACK);
      m_ctlFrame15.SubclassWindow(GetDlgItem(IDC_FRAME15));
      m_ctlFrame15.CreateFrame(8,4, COLOR_STD_FRAME_LIGHT, COLOR_BLACK, COLOR_WHITE);

      m_ctlText1.SubclassWindow(GetDlgItem(IDC_TEXT1));
      m_ctlText1.CreateText(_T("Arial"), 16, FW_NORMAL, true, true, IDB_LUMGREY, COLOR_TOOLTIP_FRAME_BACK);
      m_ctlText2.SubclassWindow(GetDlgItem(IDC_TEXT2));
      m_ctlText2.CreateText(_T("Arial"), 16, FW_EXTRABOLD, true, true, IDB_LUMGREY, COLOR_STD_FRAME_BACK);
      m_ctlText3.SubclassWindow(GetDlgItem(IDC_TEXT3));
      m_ctlText3.CreateText(_T("Arial"), 14, FW_BOLD, false, true, IDB_LUMGREY, COLOR_STD_FRAME_BACK);
      m_ctlText4.SubclassWindow(GetDlgItem(IDC_TEXT4));
      m_ctlText4.CreateText(_T("Arial"), 14, FW_BOLD, false, true, IDB_BACKGROUND, RGB(224,223,208));
      m_ctlText5.SubclassWindow(GetDlgItem(IDC_TEXT5));
      m_ctlText5.CreateText(_T("Arial"), 15, FW_BOLD, true, true, IDB_LUMGREY, COLOR_STD_FRAME_BACK);
      m_ctlText6.SubclassWindow(GetDlgItem(IDC_TEXT6));
      m_ctlText6.CreateText(_T("Arial"), 15, FW_BOLD, true, true, IDB_LUMGREY, COLOR_STD_FRAME_BACK);
      m_ctlText7.SubclassWindow(GetDlgItem(IDC_TEXT7));
      m_ctlText7.CreateText(_T("Arial"), 15, FW_BOLD, true, true, IDB_WHITE, COLOR_WHITE);
      m_ctlText8.SubclassWindow(GetDlgItem(IDC_TEXT8));
      m_ctlText8.CreateText(_T("Arial"), 16, FW_BOLD, true, true, IDB_LUMGREY, COLOR_PURPLE_FRAME_BACK, RGB(117,117,117));
      m_ctlText9.SubclassWindow(GetDlgItem(IDC_TEXT9));
      m_ctlText9.CreateText(_T("Arial"), 16, FW_BOLD, true, true, IDB_LUMGREY, COLOR_PURPLE_FRAME_BACK, RGB(117,117,117));

      m_ctlSample1.SubclassWindow(GetDlgItem(IDC_COLOR1));
      m_ctlSample1.CreateFrame(IDB_PURPLE, IDB_WHITE, 8);
      m_ctlSample2.SubclassWindow(GetDlgItem(IDC_COLOR2));
      m_ctlSample2.CreateFrame(IDB_PURPLE, IDB_SAMPLE1, 8);
      m_ctlSample3.SubclassWindow(GetDlgItem(IDC_COLOR3));
      m_ctlSample3.CreateFrame(IDB_PURPLE, IDB_SAMPLE2, 8);
      m_ctlSample4.SubclassWindow(GetDlgItem(IDC_COLOR4));
      m_ctlSample4.CreateFrame(IDB_PURPLE, IDB_SAMPLE3, 8);
      m_ctlSample5.SubclassWindow(GetDlgItem(IDC_COLOR5));
      m_ctlSample5.CreateFrame(IDB_PURPLE, IDB_SAMPLE4, 8);
      m_ctlSample6.SubclassWindow(GetDlgItem(IDC_COLOR6));
      m_ctlSample6.CreateFrame(IDB_PURPLE, IDB_PURPLE, 7);

      m_list1.Attach(GetDlgItem(IDC_LIST1));
      m_list1.SetBkColor(COLOR_WHITE);
      m_list1.SetTextColor(COLOR_BLACK);
      m_list1.SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_FRAMECHANGED);

      m_combo1.Attach(GetDlgItem(IDC_COMBO1));
      m_combo1.InsertString(-1, _T("1:1"));
      m_combo1.InsertString(-1, _T("2:1"));
      m_combo1.InsertString(-1, _T("4:1"));
      m_combo1.SetCurSel(0);

      ::CheckDlgButton(m_hWnd, IDC_RADIO2, BST_CHECKED);

      m_images.Create(IDR_TOOLBAR1, 16, 1, COLOR_WHITE);
      m_tree1.Attach(GetDlgItem(IDC_TREE1));
      m_tree1.SetImageList(m_images, LVSIL_NORMAL);
      m_tree1.SetBkColor(COLOR_WHITE);
      m_tree1.SetTextColor(COLOR_BLACK);
      m_tree1.InsertItem(_T("Background"), 0,0, TVI_ROOT, TVI_LAST);
      m_tree1.InsertItem(_T("Shapes"), 1,1, TVI_ROOT, TVI_LAST);
      m_tree1.InsertItem(_T("Surfaces"), 2,2, TVI_ROOT, TVI_LAST);
      m_tree1.InsertItem(_T("Outlines"), 3,3, TVI_ROOT, TVI_LAST);
      HTREEITEM hItem;
      hItem = m_tree1.InsertItem(_T("Effects"), 4,4, TVI_ROOT, TVI_LAST);
        m_tree1.InsertItem(_T("Circle"), 7,7, hItem, TVI_LAST);
        m_tree1.InsertItem(_T("Box"), 8,8, hItem, TVI_LAST);
      hItem = m_tree1.InsertItem(_T("Examples"), 5,5, TVI_ROOT, TVI_LAST);
        m_tree1.InsertItem(_T("Circle"), 7,7, hItem, TVI_LAST);
        m_tree1.InsertItem(_T("Box"), 8,8, hItem, TVI_LAST);
      m_tree1.InsertItem(_T("Publications"), 6,6, TVI_ROOT, TVI_LAST);
      m_tree1.SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_FRAMECHANGED);

      // Always keep dialog the same size (e.g. ignore Large Fonts setting)
      MoveWindow(0,0,736,558);
      CenterWindow();
      return TRUE;
   }

   LRESULT OnCtlColorListBox(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      ::SetBkMode((HDC)wParam, TRANSPARENT);
      ::SetTextColor((HDC)wParam, COLOR_BLACK);
      ::SetBkColor((HDC)wParam, COLOR_WHITE);
      return (LRESULT)::GetStockObject(WHITE_BRUSH); 
   }

   LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      // STATIC (label) controls need this to shine through...
      ::SetBkMode((HDC)wParam, TRANSPARENT);
      ::SetTextColor((HDC)wParam, COLOR_BLACK);
      TCHAR szClass[64];
      ::GetClassName((HWND)lParam, szClass, sizeof(szClass)/sizeof(TCHAR));
      if( ::lstrcmpi(szClass, CButton::GetWndClassName())==0 ) {
         DWORD dwStyle = (DWORD)::GetWindowLong((HWND)lParam, GWL_STYLE);
         switch( dwStyle & 0x0F ) {
         case BS_CHECKBOX:
         case BS_AUTOCHECKBOX:
         case BS_RADIOBUTTON:
         case BS_AUTORADIOBUTTON:
            return (LRESULT)m_brPurple.m_hBrush;
         }
      }
      return (LRESULT)::GetStockObject(NULL_BRUSH); 
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if(wParam != NULL) {
         DoPaint((HDC)wParam);
      }
      else {
         CPaintDC dc(m_hWnd);
         DoPaint(dc.m_hDC);
      }
      return 0;
   }

   void DoPaint(CDCHandle dc)
   {
      RECT rc;
      GetClientRect(&rc);
      // Paint the reversed window frame
      dc.FillSolidRect(rc.left, rc.top, (rc.right-m_dibLogo.GetWidth()-rc.left), 1, RGB(255,255,255));
      dc.FillSolidRect(rc.left, rc.top, 1, (rc.bottom-rc.top) - 1, RGB(0,0,0));
      dc.FillSolidRect(rc.right, rc.top, -1, (rc.bottom-rc.top), RGB(255,255,255));
      dc.FillSolidRect(rc.left, rc.bottom, (rc.right-rc.left), -1, RGB(0,0,0));
      // Paint the logo in the corner
      m_dibLogo.Draw(dc, rc.right - m_dibLogo.GetWidth(), 0);
   }

   LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      // TODO: Add validation code 
      EndDialog(wID);
      return 0;
   }

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__1A0BACBD_B6DB_4CBC_B1DC_4B71F126003B__INCLUDED_)
