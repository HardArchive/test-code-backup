// AddPackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "AddPackRuleDlg.h"
#include "AddWebRuleDlg.h"

// CAddPackDlg 对话框

IMPLEMENT_DYNAMIC(CAddPackRuleDlg, CDialog)

CAddPackRuleDlg::CAddPackRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddPackRuleDlg::IDD, pParent)
{

}

CAddPackRuleDlg::~CAddPackRuleDlg()
{
}

void CAddPackRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CODETYPE, m_ComboxBox);
	DDX_Control(pDX, IDC_LIST_PACK_RULE, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CAddPackRuleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAddPackRuleDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CAddPackRuleDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CAddPackRuleDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CAddPackRuleDlg::OnBnClickedButtonModify)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PACK_RULE, &CAddPackRuleDlg::OnNMDblclkListPackRule)
END_MESSAGE_MAP()


// CAddPackDlg 消息处理程序
BOOL CAddPackRuleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if(!AddListItem())	return FALSE;
	int nRow = 0;
	map<string,PACKRULEFILE>::iterator pPackInfo = CGlobalData::m_mapPackRuleFileInfo.begin();
	for(;pPackInfo != CGlobalData::m_mapPackRuleFileInfo.end(); ++pPackInfo)
	{
		AddListInfo(&(pPackInfo->second),nRow);
		nRow++;
	}
// 	m_ComboxBox.AddString(_T("UTF8"));
// 	m_ComboxBox.AddString(_T("URL GB2312"));
// 	m_ComboxBox.AddString(_T("URL UTF8"));
// 	m_ComboxBox.AddString(_T("%25 UTF8"));
	m_ComboxBox.AddString(_T("U-"));
	m_ComboxBox.AddString(_T("U-G"));
	m_ComboxBox.AddString(_T("U-U"));
	m_ComboxBox.AddString(_T("U-G-U"));
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CAddPackRuleDlg::AddListItem()
{
	m_ListCtrl.ShowScrollBar(SB_HORZ,FALSE);
	m_ListCtrl.ShowScrollBar(SB_VERT,TRUE);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES|LVS_EX_TRACKSELECT|\
		LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVN_ODFINDITEM);

	if(-1 == m_ListCtrl.InsertColumn(0,_T(""),LVCFMT_LEFT,20))			                     return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(1,_T("ItemName"),LVCFMT_LEFT,150))			             return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(2,_T("PackHead"),LVCFMT_LEFT,300))			             return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(3,_T("HostName"),LVCFMT_LEFT,150))	                     return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(4,_T("UserNameKeyName"),LVCFMT_LEFT,150))		         return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(5,_T("UserEndNameKeyName"),LVCFMT_CENTER,100))	       	 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(6,_T("PassWordKeyName"),LVCFMT_LEFT,150))               return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(7,_T("EndPassWordKey"),LVCFMT_CENTER,100))              return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(8,_T("SendContentKeyName"),LVCFMT_LEFT,150))            return FALSE; 
	if(-1 == m_ListCtrl.InsertColumn(9,_T("EndSendContentKey"),LVCFMT_CENTER,100))			 return FALSE; 
	if(-1 == m_ListCtrl.InsertColumn(10,_T("CodeType"),LVCFMT_CENTER,80))                    return FALSE;
	UpdateData(FALSE);
	return TRUE;
}

void CAddPackRuleDlg::AddListInfo(PACKRULEFILE *pPackInfo,int nRow)
{
	CString str;
	m_ListCtrl.InsertItem(nRow,_T(""));
	str = pPackInfo->Item;
	m_ListCtrl.SetItemText(nRow,1,str);
	str = pPackInfo->PackHead;
	m_ListCtrl.SetItemText(nRow,2,str);
	str = pPackInfo->Host;
	m_ListCtrl.SetItemText(nRow,3,str);
	str = pPackInfo->UserNameKey;
	m_ListCtrl.SetItemText(nRow,4,str);
	str = pPackInfo->UserEndNameKey;
	m_ListCtrl.SetItemText(nRow,5,str);
	str = pPackInfo->PassWordKey;
	m_ListCtrl.SetItemText(nRow,6,str);
	str = pPackInfo->EndPassWordKey;
	m_ListCtrl.SetItemText(nRow,7,str);
	str = pPackInfo->SendContentKey;
	m_ListCtrl.SetItemText(nRow,8,str);
	str = pPackInfo->EndSengContentKey;
	m_ListCtrl.SetItemText(nRow,9,str);
	str = pPackInfo->CodeType;
	m_ListCtrl.SetItemText(nRow,10,str);

}


void CAddPackRuleDlg::OnNMDblclkListPackRule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
	int nItem=pNMListView->iItem;

	//Xt: 判断双击位置是否在有数据的列表项上面
	if(nItem>=0 && nItem<m_ListCtrl.GetItemCount())                          
	{
		ClearControlInfo();
		GetDlgItem(IDC_EDIT_PACK_ITEMNAME)->SetWindowText(m_ListCtrl.GetItemText(nItem,1));
		GetDlgItem(IDC_EDIT_PACK_PACKHEAD)->SetWindowText(m_ListCtrl.GetItemText(nItem,2));
		GetDlgItem(IDC_EDIT_PACK_HOSTNAME)->SetWindowText(m_ListCtrl.GetItemText(nItem,3));
		GetDlgItem(IDC_EDIT_PACK_USERNAME)->SetWindowText(m_ListCtrl.GetItemText(nItem,4));
		GetDlgItem(IDC_EDIT_PACK_ENDUSERNAME)->SetWindowText(m_ListCtrl.GetItemText(nItem,5));	
		GetDlgItem(IDC_EDIT_PACK_PASSWORD_KEY)->SetWindowText(m_ListCtrl.GetItemText(nItem,6));
		GetDlgItem(IDC_EDIT_PACK_ENDPASSWORD)->SetWindowText(m_ListCtrl.GetItemText(nItem,7));
		GetDlgItem(IDC_EDIT_PACK_SENDKEY)->SetWindowText(m_ListCtrl.GetItemText(nItem,8));
		GetDlgItem(IDC_EDIT_PACK_ENDSEND)->SetWindowText(m_ListCtrl.GetItemText(nItem,9));
		m_ComboxBox.SetCurSel(m_ComboxBox.FindString(-1,(m_ListCtrl.GetItemText(nItem,10)).GetBuffer()));
	}
	*pResult = 0;
}

void CAddPackRuleDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nNum = 0;
	PACKRULEFILE PackRuleInfo;
	if(CheckEnterInfo(&PackRuleInfo))
	{
		CGlobalData::m_mapPackRuleFileInfo.insert(make_pair(PackRuleInfo.Item,PackRuleInfo));
		FILE *file = fopen(CGlobalData::m_strPackRulePath,"a");
		CGlobalData::WritePackRuleFile(&PackRuleInfo,file);
		ClearControlInfo();
		nNum = m_ListCtrl.GetItemCount();
		AddListInfo(&PackRuleInfo,nNum);
		fclose(file);	
		UpdateData(false);
	}
}

BOOL CAddPackRuleDlg::CheckEnterInfo(PACKRULEFILE *pPackRuleInfo)
{
	CString strError;
	CString strInfo;
	GetDlgItem(IDC_EDIT_PACK_ITEMNAME)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->Item,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("Item 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_PACK_USERNAME)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->UserNameKey,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("UserNameKey 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_PACK_ENDUSERNAME)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->UserEndNameKey,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("EndUserNameKey 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_PACK_HOSTNAME)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->Host,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("Host 不能为空!"));
		AfxMessageBox(strError);
		return FALSE;
	}

	GetDlgItem(IDC_EDIT_PACK_PACKHEAD)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->PackHead,strInfo.GetBuffer());
		strInfo.Empty();
	}
	else
	{
		strError.Format(_T("PackHead 不能为空!"));
		AfxMessageBox(strError);
		return FALSE; 
	}

	GetDlgItem(IDC_EDIT_PACK_PASSWORD_KEY)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->PassWordKey,strInfo.GetBuffer());
		strInfo.Empty();
	}

	GetDlgItem(IDC_EDIT_PACK_ENDPASSWORD)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->EndPassWordKey,strInfo.GetBuffer());
		strInfo.Empty();
	}

	GetDlgItem(IDC_EDIT_PACK_SENDKEY)->GetWindowText(strInfo);
	if(!strInfo.IsEmpty())
	{
		CChangeCode::UnicodeToGB2312Code(pPackRuleInfo->SendContentKey,strInfo.GetBuffer());
		strInfo.Empty();
	}

	switch(m_ComboxBox.GetCurSel())
	{
	case CB_ERR: 
		memcpy(pPackRuleInfo->CodeType,"U-U",sizeof("U-U"));
		break;
	case 0:
		memcpy(pPackRuleInfo->CodeType,"U-",sizeof("U-"));
		break;
	case 1:
		memcpy(pPackRuleInfo->CodeType,"U-G",sizeof("U-G"));
		break;
	case 2:
		memcpy(pPackRuleInfo->CodeType,"U-U",sizeof("U-U"));
		break;
	case 3:
		memcpy(pPackRuleInfo->CodeType,"U-G-U",sizeof("U-G-U"));
		break;
	}
	return TRUE;
}


void CAddPackRuleDlg::ClearControlInfo()
{
	GetDlgItem(IDC_EDIT_PACK_ITEMNAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_USERNAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_ENDUSERNAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_HOSTNAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_PACKHEAD)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_PASSWORD_KEY)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_ENDPASSWORD)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_SENDKEY)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PACK_ENDSEND)->SetWindowText(_T(""));
	m_ComboxBox.SetCurSel(2);
}

void CAddPackRuleDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strItem;
	GetDlgItem(IDC_EDIT_PACK_ITEMNAME)->GetWindowText(strItem);
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
		if(1 == CGlobalData::m_mapPackRuleFileInfo.erase(strDeleteItemName))
		{	
			//Xt: 创建文件副本以防丢失
			char strDuplicateFile[MAX_PATH] = {0};
			memcpy(strDuplicateFile,CGlobalData::m_strPackRulePath,strlen(CGlobalData::m_strPackRulePath)-4);
			strcat(strDuplicateFile,"Dup.txt");
			MoveFileA(CGlobalData::m_strPackRulePath,strDuplicateFile);

			//Xt: 清空输入列表和控件内容
			ClearControlInfo();
			m_ListCtrl.DeleteAllItems();

			//Xt: 从新创建包规则文件并存入信息
			FILE *file = fopen(CGlobalData::m_strPackRulePath,"a");
			int nRow = 0;
			map<string,PACKRULEFILE>::iterator pPackInfo = CGlobalData::m_mapPackRuleFileInfo.begin();
			for(;pPackInfo != CGlobalData::m_mapPackRuleFileInfo.end(); ++pPackInfo)
			{
				CGlobalData::WritePackRuleFile(&(pPackInfo->second),file);
				AddListInfo(&(pPackInfo->second),nRow);
				nRow++;
			}

			//Xt: 移除副本文件，更新列表控件
			remove(strDuplicateFile);
			fclose(file);
			m_ListCtrl.Update(false);
		}
	}
}

void CAddPackRuleDlg::OnBnClickedButtonModify()
{
	CString strItem;
	GetDlgItem(IDC_EDIT_PACK_ITEMNAME)->GetWindowText(strItem);
	if(strItem.IsEmpty() )
	{
		MessageBox(_T("没有选择修改项!"));
		return;
	}
	if( IDOK == MessageBox(_T("是否确认修改!")))
	{
		//Xt：检测修改信息是否有误,取得输入信息
		PACKRULEFILE PackRuleInfo;
		if(CheckEnterInfo(&PackRuleInfo))
		{
			char strDeleteItemName[20] = {0};
			//Xt：删除修改信息
			CChangeCode::UnicodeToGB2312Code(strDeleteItemName,strItem.GetBuffer());
			if(1 == CGlobalData::m_mapPackRuleFileInfo.erase(strDeleteItemName))
			{	
				//Xt: 创建文件副本以防丢失
				char strDuplicateFile[MAX_PATH] = {0};
				memcpy(strDuplicateFile,CGlobalData::m_strPackRulePath,strlen(CGlobalData::m_strPackRulePath)-4);
				strcat(strDuplicateFile,"Dup.txt");
				MoveFileA(CGlobalData::m_strPackRulePath,strDuplicateFile);

				//Xt: 清空输入列表和控件内容
				ClearControlInfo();
				m_ListCtrl.DeleteAllItems();
				
				//Xt: 添加修改后信息
				CGlobalData::m_mapPackRuleFileInfo.insert(make_pair(PackRuleInfo.Item,PackRuleInfo));
				
				//Xt: 从新创建包规则文件并存入信息
				FILE *file = fopen(CGlobalData::m_strPackRulePath,"a");
				int nRow = 0;
				map<string,PACKRULEFILE>::iterator pPackInfo = CGlobalData::m_mapPackRuleFileInfo.begin();
				for(;pPackInfo != CGlobalData::m_mapPackRuleFileInfo.end(); ++pPackInfo)
				{
					CGlobalData::WritePackRuleFile(&(pPackInfo->second),file);
					AddListInfo(&(pPackInfo->second),nRow);
					nRow++;
				}

				//Xt: 移除副本文件，更新列表控件
				remove(strDuplicateFile);
				fclose(file);
				m_ListCtrl.Update(false);
			}
			else
			{
				MessageBox(_T("无此项信息,不能修改\"ITEM\"项!"));
			}
		}
	}
}

void CAddPackRuleDlg::OnBnClickedButtonQuit()
{
	OnCancel();
}

