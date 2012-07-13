// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

LRESULT CMainDlg::OnMaxWindow()
{
	if (WS_MAXIMIZE == (GetStyle() & WS_MAXIMIZE))
	{		
		SendMessage(WM_SYSCOMMAND, SC_RESTORE | HTCAPTION, 0);
	}
	else
	{		
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE | HTCAPTION, 0);
	}
	return 0;
}

LRESULT CMainDlg::OnMinWindow()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return 0;
}

//LRESULT CMainDlg::OnTest()
//{
//	::MessageBox(NULL, L"测试按钮成功", L"提示", MB_OK);
//	return 0;
//}

void CMainDlg::OnSysCommand(UINT nID, CPoint point)
{
	SetMsgHandled(FALSE);

	switch (nID & 0xFFF0)
	{
	case SC_CLOSE:
		SetMsgHandled(TRUE);
		//OnClose();
		EndDialog(IDOK);
		return;
		break;
	case SC_RESTORE:
		{
			DWORD dwStyle = GetStyle();
			if (WS_MINIMIZE == (dwStyle & WS_MINIMIZE))
				break;

			if (WS_MAXIMIZE == (dwStyle & WS_MAXIMIZE))
			{
				SetItemAttribute(IDC_BTN_MAX, "skin", "dlg_btn_max");
				break;
			}
		}
	case SC_MAXIMIZE:
		SetItemAttribute(IDC_BTN_MAX, "skin", "dlg_btn_normal");
		break;
	default:
		break;
	}
}

LRESULT CMainDlg::OnInitDialog(HWND, LPARAM)
{
	// center the dialog on the screen
	CenterWindow();
	LOGFONT logFont = { 0 };
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = 90;
	lstrcpy( logFont.lfFaceName, _T( "New Times Roman" ) );
	logFont.lfWeight = FW_BOLD;
	logFont.lfItalic = (BYTE)TRUE;

	m_fntCustomFont1.CreatePointFontIndirect( &logFont );

	logFont.lfHeight = 100;
	lstrcpy( logFont.lfFaceName, _T( "Arial" ) );
	logFont.lfUnderline = (BYTE)TRUE;
	m_fntCustomFont2.CreatePointFontIndirect( &logFont );
	
	m_edit.Create(GetViewHWND(), NULL, _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, NULL, IDC_LIST_VIEW_CTRL);

	//SetBkColor();
	//m_edit.SetBkColor(RGB(0xfb,0xfc,0xfd));
	

	
	m_wndListCtrl.SubclassWindow(m_edit.m_hWnd);

	m_wndListCtrl.SetFocusSubItem( FALSE );

	m_wndListCtrl.AddColumn( _T( "Column 1" ), 150 );
	m_wndListCtrl.AddColumn( _T( "" ), 0, ITEM_IMAGE_3STATE, TRUE, ITEM_FORMAT_CHECKBOX_3STATE );
	m_wndListCtrl.AddColumn( _T( "Column 2" ), 130 );
	m_wndListCtrl.AddColumn( _T( "Column 3" ), 80, -1, FALSE, ITEM_FORMAT_NONE, ITEM_FLAGS_RIGHT );
	m_wndListCtrl.AddColumn( _T( "Column 4" ), 120 );
	m_wndListCtrl.AddColumn( _T( "Column 5" ), 170, ITEM_IMAGE_ATTACHMENT, FALSE, ITEM_FORMAT_PROGRESS );

	srand( 12345 );

	for ( int nItem = 1; nItem <= 1000; nItem++ )
	{
#ifdef _UNICODE
		wostringstream osItemText;
#else
		ostringstream osItemText;
#endif

		osItemText << _T( "Item " ) << nItem;

		int nNewItem = m_wndListCtrl.AddItem( osItemText.str().c_str(), rand() % 6 );
		m_wndListCtrl.SetItemCheck( nNewItem, 1, ( rand() % 3 ) - 1 );

		osItemText.str( _T( "" ) );
		TCHAR nSortChar = _T( 'A' ) + ( rand() % 26 );

		osItemText << nSortChar << _T( " Random Text" );
		m_wndListCtrl.SetItemText( nNewItem, 2, osItemText.str().c_str() );
		m_wndListCtrl.SetItemImage( nNewItem, 2, rand() % 6 );

		osItemText.str( _T( "" ) );
		osItemText << nItem;
		m_wndListCtrl.SetItemText( nNewItem, 3, osItemText.str().c_str() );

		osItemText.str( _T( "" ) );
		osItemText << ( rand() % 101 );
		m_wndListCtrl.SetItemText( nNewItem, 5, osItemText.str().c_str() );
	}

	CListArray < CString > aComboList;
	aComboList.Add( _T( "Item 1" ) );
	aComboList.Add( _T( "Item 2" ) );
	aComboList.Add( _T( "Item 3" ) );
	aComboList.Add( _T( "Item 4" ) );
	aComboList.Add( _T( "Item 5" ) );

	m_wndListCtrl.SetItemText( 0, 4, _T( "Combo (Edit):" ) );
	m_wndListCtrl.SetItemFormat( 0, 5, ITEM_FORMAT_COMBO, ITEM_FLAGS_COMBO_EDIT, aComboList );
	m_wndListCtrl.SetItemComboIndex( 0, 5, 0 ); // "Item 1"		

	m_wndListCtrl.SetItemText( 1, 4, _T( "Combo (List):" ) );
	m_wndListCtrl.SetItemFormat( 1, 5, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, aComboList );
	m_wndListCtrl.SetItemText( 1, 5, _T( "Item 1" ) );		

	m_wndListCtrl.SetItemText( 2, 4, _T( "Edit:" ) );
	m_wndListCtrl.SetItemFormat( 2, 5, ITEM_FORMAT_EDIT );
	m_wndListCtrl.SetItemText( 2, 5, _T( "<alpha-numeric>" ) );

	m_wndListCtrl.SetItemText( 3, 4, _T( "Edit (Numeric):" ) );
	m_wndListCtrl.SetItemFormat( 3, 5, ITEM_FORMAT_EDIT, ITEM_FLAGS_EDIT_NUMBER );
	m_wndListCtrl.SetItemText( 3, 5, _T( "123" ) );

	CTime tmDateTime = CTime::GetCurrentTime();
	SYSTEMTIME stDateTime;
	tmDateTime.GetAsSystemTime( stDateTime );

	m_wndListCtrl.SetItemText( 4, 4, _T( "Date (inc Time):" ) );
	m_wndListCtrl.SetItemFormat( 4, 5, ITEM_FORMAT_DATETIME, ITEM_FLAGS_DATETIME_NONE );
	m_wndListCtrl.SetItemDate( 4, 5, stDateTime );

	m_wndListCtrl.SetItemText( 5, 4, _T( "Date (Date only):" ) );
	m_wndListCtrl.SetItemFormat( 5, 5, ITEM_FORMAT_DATETIME, ITEM_FLAGS_DATE_ONLY );
	m_wndListCtrl.SetItemDate( 5, 5, stDateTime );

	m_wndListCtrl.SetItemText( 6, 4, _T( "Solid progress:" ) );
	m_wndListCtrl.SetItemFormat( 6, 5, ITEM_FORMAT_PROGRESS, ITEM_FLAGS_PROGRESS_SOLID );

	m_wndListCtrl.SetItemText( 7, 4, _T( "Normal progress:" ) );

	m_wndListCtrl.SetItemText( 8, 4, _T( "Checkbox:" ) );
	m_wndListCtrl.SetItemFormat( 8, 5, ITEM_FORMAT_CHECKBOX );

	m_wndListCtrl.SetItemText( 9, 4, _T( "Checkbox (3 State):" ) );
	m_wndListCtrl.SetItemFormat( 9, 5, ITEM_FORMAT_CHECKBOX_3STATE );

	m_wndListCtrl.SetItemText( 10, 4, _T( "Checkbox (read-only):" ) );
	m_wndListCtrl.SetItemFormat( 10, 5, ITEM_FORMAT_CHECKBOX, ITEM_FLAGS_READ_ONLY );

	m_wndListCtrl.SetItemText( 11, 4, _T( "Hyperlink" ) );
	m_wndListCtrl.SetItemFormat( 11, 4, ITEM_FORMAT_HYPERLINK );

	m_wndListCtrl.SetItemText( 12, 4, _T( "Some very long text that should trigger titletip" ) );

	m_wndListCtrl.SetItemText( 13, 4, _T( "Built-in tooltip support" ) );
	m_wndListCtrl.SetItemToolTip( 13, _T( "This is a\nmultiline tooltip" ) );

	m_wndListCtrl.SetItemText( 14, 4, _T( "Font Support" ) );
	m_wndListCtrl.SetItemFont( 14, 4, m_fntCustomFont1 );

	m_wndListCtrl.SetItemText( 15, 4, _T( "Font Support" ) );
	m_wndListCtrl.SetItemFont( 15, 4, m_fntCustomFont2 );

	m_wndListCtrl.SetItemText( 16, 4, _T( "Colour Support" ) );
	m_wndListCtrl.SetItemColours( 16, 4, RGB( 128, 128, 64 ), RGB( 0, 255, 0 ) );

	m_wndListCtrl.SetItemText( 17, 4, _T( "Colour Support" ) );
	m_wndListCtrl.SetItemColours( 17, 4, RGB( 128, 0, 128 ), RGB( 255, 255, 128 ) );


	return TRUE;
}


