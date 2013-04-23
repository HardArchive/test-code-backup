// AddWebRuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "AddWebRuleDlg.h"

// CAddWebRuleDlg 对话框

IMPLEMENT_DYNAMIC(CAddWebRuleDlg, CDialog)

CAddWebRuleDlg::CAddWebRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddWebRuleDlg::IDD, pParent)
{

}

CAddWebRuleDlg::~CAddWebRuleDlg()
{
}

void CAddWebRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WEB, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_WEB_OPERATEITEM, m_ComboBox);
}


BEGIN_MESSAGE_MAP(CAddWebRuleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_WEB_DELETE, &CAddWebRuleDlg::OnBnClickedButtonWebDelete)
	ON_BN_CLICKED(IDC_BUTTON_WEB_MODIFY, &CAddWebRuleDlg::OnBnClickedButtonWebModify)
	ON_BN_CLICKED(IDC_BUTTON_WEB_QUIT, &CAddWebRuleDlg::OnBnClickedButtonWebQuit)
	ON_BN_CLICKED(IDC_BUTTON_WEB_ADD, &CAddWebRuleDlg::OnBnClickedButtonWebAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WEB, &CAddWebRuleDlg::OnNMDblclkListWeb)
END_MESSAGE_MAP()

BOOL CAddWebRuleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(!AddListItem())	return FALSE;
	int nRow = 0;
	map <string,LoginFileContentInfo>::iterator itorMapLoginFileInfo = CGlobalData::m_mapHttpFileInfo.begin();
	for(;itorMapLoginFileInfo != CGlobalData::m_mapHttpFileInfo.end(); ++itorMapLoginFileInfo)
	{
		AddListInfo(&itorMapLoginFileInfo->second,nRow);
		nRow++;
	}
	m_ComboBox.AddString(_T("1 代表登录"));
	m_ComboBox.AddString(_T("2 代表发帖回帖"));
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL  CAddWebRuleDlg::AddListItem()
{
	m_ListCtrl.ShowScrollBar(SB_HORZ,FALSE);
	m_ListCtrl.ShowScrollBar(SB_VERT,TRUE);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES|LVS_EX_TRACKSELECT|\
		LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVN_ODFINDITEM);

	if(-1 == m_ListCtrl.InsertColumn(0,_T(""),LVCFMT_LEFT,20))		         	 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(1,_T("ItemName"),LVCFMT_LEFT,100))			 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(2,_T("UserName"),LVCFMT_CENTER,150))		 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(3,_T("Password"),LVCFMT_CENTER,150))		 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(4,_T("UserNameKey"),LVCFMT_CENTER,150))	 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(5,_T("PassWordKey"),LVCFMT_CENTER,150))     return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(6,_T("OKButtonName"),LVCFMT_CENTER,100))    return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(7,_T("URL"),LVCFMT_CENTER,300))			 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(8,_T("OperateItem"),LVCFMT_CENTER,100))     return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(9,_T("SendContent"),LVCFMT_CENTER,350))     return FALSE; 
	if(-1 == m_ListCtrl.InsertColumn(10,_T("SendKey"),LVCFMT_CENTER,100))		 return FALSE; 
	return TRUE;
}

void CAddWebRuleDlg::AddListInfo(LoginFileContentInfo *pLoginFileInfo,int nRow)
{	
	CString str;
	m_ListCtrl.InsertItem(nRow,_T(""));
	str = pLoginFileInfo->Item;
	m_ListCtrl.SetItemText(nRow,1,str);
	str = pLoginFileInfo->UserName;
	m_ListCtrl.SetItemText(nRow,2,str);
	str = pLoginFileInfo->Password;
	m_ListCtrl.SetItemText(nRow,3,str);
	str = pLoginFileInfo->UserNameMark;
	m_ListCtrl.SetItemText(nRow,4,str);
	str = pLoginFileInfo->PassWordMark;
	m_ListCtrl.SetItemText(nRow,5,str);
	str = pLoginFileInfo->ButtonName;
	m_ListCtrl.SetItemText(nRow,6,str);
	str = pLoginFileInfo->URL;
	m_ListCtrl.SetItemText(nRow,7,str);
	str.Format(_T("%d"),pLoginFileInfo->OperateItem);
	m_ListCtrl.SetItemText(nRow,8,str);
	str = pLoginFileInfo->SendContent;
	m_ListCtrl.SetItemText(nRow,9,str);
	str = pLoginFileInfo->SendKey;
	m_ListCtrl.SetItemText(nRow,10,str);
}

void CAddWebRuleDlg::ClearControlInfo()
{
	m_ComboBox.SetCurSel(0);
	CString str;
	GetDlgItem(IDC_EDIT_WEB_ITEMNAME)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_URL)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_USERNAME)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_PASSWORD)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_USERNAME_KEY)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_PASSWORD_KEY)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_OK_BUTTON_NAME)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_SEND_CONTENT)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_WEB_REMARK)->SetWindowText(str);
	UpdateData(FALSE);
}

BOOL CAddWebRuleDlg::CheckEnterInfo(LoginFileContentInfo *pLoginFileInfo)
{
	int nNum = 0;
	CString strError;
	CString strInfo;
	nNum = m_ComboBox.GetCurSel();
	if(CB_ERR == nNum)
	{
		MessageBox(_T("操作类型不能为空!"));
		return FALSE;
	}
	else 
	{
		pLoginFileInfo->OperateItem = nNum+1;
	}

	GetDlgItem(IDC_EDIT_WEB_ITEMNAME)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->Item,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("Item 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_URL)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->URL,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("URL 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_USERNAME)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->UserName,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else if( 1 == pLoginFileInfo->OperateItem)
	{
		strError.Format(_T("UserName 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_PASSWORD)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->Password,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else if( 1 == pLoginFileInfo->OperateItem)
	{
		strError.Format(_T("Password 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_USERNAME_KEY)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->UserNameMark,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else if( 1 == pLoginFileInfo->OperateItem)
	{
		strError.Format(_T("UserNameMark 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_PASSWORD_KEY)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->PassWordMark,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else if( 1 == pLoginFileInfo->OperateItem)
	{
		strError.Format(_T("PassWordMark 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_OK_BUTTON_NAME)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->ButtonName,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("ButtonName 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_SEND_CONTENT)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->SendContent,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else if( 2 == pLoginFileInfo->OperateItem)
	{
		strError.Format(_T("SendContent 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_WEB_SENDKEY)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pLoginFileInfo->SendKey,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else if( 2 == pLoginFileInfo->OperateItem)
	{
		strError.Format(_T("SendKey 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	return TRUE;
}

void CAddWebRuleDlg::OnBnClickedButtonWebAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nNum = 0;
	LoginFileContentInfo loginFileInfo;
	if(CheckEnterInfo(&loginFileInfo))
	{
		CGlobalData::m_mapHttpFileInfo.insert(make_pair(loginFileInfo.Item,loginFileInfo));
		FILE *file = fopen(CGlobalData::m_strHttpFilePath,"a");
		CGlobalData::WriteHttpRuleFile(&loginFileInfo,file);
		ClearControlInfo();

		nNum = m_ListCtrl.GetItemCount();
		AddListInfo(&loginFileInfo,nNum);

		fclose(file);	
		m_ListCtrl.Update(TRUE);
	}
}

void CAddWebRuleDlg::OnBnClickedButtonWebDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strItem;
	GetDlgItem(IDC_EDIT_WEB_ITEMNAME)->GetWindowText(strItem);
	if(strItem.IsEmpty())
	{
		MessageBox(_T("没有选择删除项!"));
		return;
	}
 	if( IDOK == MessageBox(_T("是否确认删除!")))
	{	
		char strDeleteItemName[20] = {0};
 		//Xt: 删除列表中的要删除的值
		CChangeCode::UnicodeToGB2312Code(strDeleteItemName,strItem.GetBuffer());
 		if(1 == CGlobalData::m_mapHttpFileInfo.erase(strDeleteItemName))
 		{	
 			//Xt: 创建文件副本以防丢失
 			char strDuplicateFile[MAX_PATH] = {0};
			memcpy(strDuplicateFile,CGlobalData::m_strHttpFilePath,strlen(CGlobalData::m_strHttpFilePath)-4);
 			strcat(strDuplicateFile,"Dup.txt");
 			MoveFileA(CGlobalData::m_strHttpFilePath,strDuplicateFile);

			//Xt: 清空输入列表和控件内容
			ClearControlInfo();
			m_ListCtrl.DeleteAllItems();
 
 			//Xt: 从新创建包规则文件并存入信息
 			FILE *file = fopen(CGlobalData::m_strHttpFilePath,"a");
			int nRow = 0;
			map <string,LoginFileContentInfo>::iterator pWebInfo = CGlobalData::m_mapHttpFileInfo.begin();
			for(;pWebInfo != CGlobalData::m_mapHttpFileInfo.end(); ++pWebInfo)
 			{
 				CGlobalData::WriteHttpRuleFile(&(pWebInfo->second),file);
 				AddListInfo(&(pWebInfo->second),nRow);
 				nRow++;
 			}
 
 			//Xt: 移除副本文件，更新列表控件
			remove(strDuplicateFile);
			fclose(file);
			m_ListCtrl.Update(FALSE);
 		}
 	}
}

void CAddWebRuleDlg::OnBnClickedButtonWebModify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strItem;
	GetDlgItem(IDC_EDIT_WEB_ITEMNAME)->GetWindowText(strItem);
	if(strItem.IsEmpty())
	{
		MessageBox(_T("没有选择修改项!"));
		return;
	}
	if( IDOK == MessageBox(_T("是否确认修改!")))
	{
		char strDeleteItemName[20] = {0};
		//Xt: 删除列表中的要删除的值
		LoginFileContentInfo loginFileInfo;
		if(CheckEnterInfo(&loginFileInfo))
		{
			CChangeCode::UnicodeToGB2312Code(strDeleteItemName,strItem.GetBuffer());
			if(1 == CGlobalData::m_mapHttpFileInfo.erase(strDeleteItemName))
			{	
				//Xt: 创建文件副本以防丢失
				char strDuplicateFile[MAX_PATH] = {0};
				memcpy(strDuplicateFile,CGlobalData::m_strHttpFilePath,strlen(CGlobalData::m_strHttpFilePath)-4);
				strcat(strDuplicateFile,"Dup.txt");
				MoveFileA(CGlobalData::m_strHttpFilePath,strDuplicateFile);

				//Xt: 清空输入列表和控件内容
				ClearControlInfo();
				m_ListCtrl.DeleteAllItems();
				//Xt: 添加修改后信息
				CGlobalData::m_mapHttpFileInfo.insert(make_pair(loginFileInfo.Item,loginFileInfo));

				//Xt: 从新创建包规则文件并存入信息
				FILE *file = fopen(CGlobalData::m_strHttpFilePath,"a");
				int nRow = 0;
				map <string,LoginFileContentInfo>::iterator pWebInfo = CGlobalData::m_mapHttpFileInfo.begin();
				for(;pWebInfo != CGlobalData::m_mapHttpFileInfo.end(); ++pWebInfo)
				{
					CGlobalData::WriteHttpRuleFile(&(pWebInfo->second),file);
					AddListInfo(&(pWebInfo->second),nRow);
					nRow++;
				}
				//Xt: 移除副本文件，更新列表控件
				remove(strDuplicateFile);
				fclose(file);
				m_ListCtrl.Update(FALSE);
			}
		}
		else
		{
			MessageBox(_T("无此项信息,不能修改\"ITEM\"项!"));
		}
	}
}

void CAddWebRuleDlg::OnBnClickedButtonWebQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CAddWebRuleDlg::OnNMDblclkListWeb(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
	int nItem=pNMListView->iItem;


	//Xt: 判断双击位置是否在有数据的列表项上面
	if(nItem>=0 && nItem<m_ListCtrl.GetItemCount())                          
	{
		ClearControlInfo();
		GetDlgItem(IDC_EDIT_WEB_ITEMNAME)->SetWindowText(m_ListCtrl.GetItemText(nItem,1));	
		GetDlgItem(IDC_EDIT_WEB_USERNAME)->SetWindowText(m_ListCtrl.GetItemText(nItem,2));
		GetDlgItem(IDC_EDIT_WEB_PASSWORD)->SetWindowText(m_ListCtrl.GetItemText(nItem,3));
		GetDlgItem(IDC_EDIT_WEB_USERNAME_KEY)->SetWindowText(m_ListCtrl.GetItemText(nItem,4));
		GetDlgItem(IDC_EDIT_WEB_PASSWORD_KEY)->SetWindowText(m_ListCtrl.GetItemText(nItem,5));
		GetDlgItem(IDC_EDIT_WEB_OK_BUTTON_NAME)->SetWindowText(m_ListCtrl.GetItemText(nItem,6));
		GetDlgItem(IDC_EDIT_WEB_URL)->SetWindowText(m_ListCtrl.GetItemText(nItem,7));
		GetDlgItem(IDC_EDIT_WEB_SEND_CONTENT)->SetWindowText(m_ListCtrl.GetItemText(nItem,9));
		GetDlgItem(IDC_EDIT_WEB_REMARK)->SetWindowText(m_ListCtrl.GetItemText(nItem,10));
	}
	*pResult = 0;
}
