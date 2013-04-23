// FindCaptureRuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "FindCaptureRuleDlg.h"

//CFindCaptureRuleDlg 对话框
CFindCaptureRuleDlg *CFindCaptureRuleDlg::m_pThis = NULL;
IMPLEMENT_DYNAMIC(CFindCaptureRuleDlg, CDialog)

CFindCaptureRuleDlg::CFindCaptureRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindCaptureRuleDlg::IDD, pParent)
{
	m_pThis = this;
	m_packServiceType = CAPTURE_PACK_RULE;
}

CFindCaptureRuleDlg::~CFindCaptureRuleDlg()
{
	m_pThis = NULL;
}

void CFindCaptureRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FINDDLG_CAPTURE, m_ListFindPack);
	DDX_Control(pDX, IDC_COMBO_SET_ADAPTER, m_ComboBox);
}


BEGIN_MESSAGE_MAP(CFindCaptureRuleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FINDDLG_STARTCAPTURE, &CFindCaptureRuleDlg::OnBnClickedButtonFinddlgStartcapture)
	ON_BN_CLICKED(IDC_BUTTON_FINDDLG_FILTER, &CFindCaptureRuleDlg::OnBnClickedButtonFinddlgFilter)
	ON_BN_CLICKED(IDC_BUTTON_FINDDLG_STOP, &CFindCaptureRuleDlg::OnBnClickedButtonFinddlgStop)
	ON_BN_CLICKED(IDC_BUTTON_FINDDLG_QUITE, &CFindCaptureRuleDlg::OnBnClickedButtonFinddlgQuite)
	ON_BN_CLICKED(IDC_BUTTON_FINDDLG_SAVE, &CFindCaptureRuleDlg::OnBnClickedButtonFinddlgSave)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CFindCaptureRuleDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CFindCaptureRuleDlg 消息处理程序

BOOL CFindCaptureRuleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_BUTTON_FINDDLG_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINDDLG_FILTER)->EnableWindow(FALSE);

	vector <string>::iterator pVectAdaterName = CGlobalData::m_vectAdapterName.begin();
	for(;pVectAdaterName != CGlobalData::m_vectAdapterName.end(); ++pVectAdaterName)
	{
		CString strAdapterName(pVectAdaterName->c_str());
		m_ComboBox.AddString(strAdapterName);
	}

	if(!AddListItem())
	{
		m_strError.Format(_T("加载列表错误!"));
		return FALSE;
	}
	
	m_ComboBox.SetCurSel(0);
	GetDlgItem(IDC_EDIT_FILTER_NUM)->SetWindowText(_T("2"));
	GetDlgItem(IDC_EDIT_FILTER_HEAD_LEN)->SetWindowText(_T("10"));
	((CButton *)GetDlgItem(IDC_EDIT_FIND_PACKRULELEN))->SetWindowText(_T("10"));
	((CButton *)GetDlgItem(IDC_EDIT_SUBCONTRACTING_NUM))->SetWindowText(_T("8"));
	((CButton *)GetDlgItem(IDC_RADIO_FIND_ONLINE_PACK))->SetCheck(true);
	GetDlgItem(IDC_EDIT_FILTER_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINDDLG_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINDDLG_FILTER)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool CFindCaptureRuleDlg::AddListItem()
{
	m_ListFindPack.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES|LVS_EX_TRACKSELECT|\
				  		LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVN_ODFINDITEM);
	//m_ListFindPack.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_CHECKBOXES|LVN_ODFINDITEM);
	if(-1 == m_ListFindPack.InsertColumn(0,_T(""),LVCFMT_LEFT,20))                               return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(1,_T("PackHead"),LVCFMT_LEFT,130))			             return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(2,_T("Host"),LVCFMT_CENTER,105))		                 return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(3,_T("Refere"),LVCFMT_CENTER,105))	       	             return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(4,_T("UserNeameBegin"),LVCFMT_CENTER,105))	             return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(5,_T("UserNeameEND"),LVCFMT_CENTER,105))	             return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(6,_T("PassWordBegin"),LVCFMT_CENTER,105))	             return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(7,_T("PassWordEND"),LVCFMT_CENTER,105))	             return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(8,_T("SendContentBegin"),LVCFMT_CENTER,105))	         return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(9,_T("SendContentEND"),LVCFMT_CENTER,105))	             return FALSE;
	if(-1 == m_ListFindPack.InsertColumn(10,_T("CodeType"),LVCFMT_CENTER,100))                   return FALSE;
	return TRUE;
}

void CFindCaptureRuleDlg::AddListInfo(int nRow,PACKRULEFILE *pPackRuleInfo)
{
	CString strItemName;
	m_ListFindPack.InsertItem(nRow,_T(""));
 	strItemName = pPackRuleInfo->PackHead;
 	m_ListFindPack.SetItemText(nRow,1,strItemName);
	strItemName = pPackRuleInfo->Host;
	m_ListFindPack.SetItemText(nRow,2,strItemName);
	strItemName = pPackRuleInfo->Referer;
	m_ListFindPack.SetItemText(nRow,3,strItemName);
	strItemName = pPackRuleInfo->UserNameKey;
	m_ListFindPack.SetItemText(nRow,4,strItemName);
	strItemName = pPackRuleInfo->UserEndNameKey;
	m_ListFindPack.SetItemText(nRow,5,strItemName);
	strItemName = pPackRuleInfo->PassWordKey;
	m_ListFindPack.SetItemText(nRow,6,strItemName);
	strItemName = pPackRuleInfo->EndPassWordKey;
	m_ListFindPack.SetItemText(nRow,7,strItemName);
	strItemName = pPackRuleInfo->SendContentKey;
	m_ListFindPack.SetItemText(nRow,8,strItemName);
	strItemName = pPackRuleInfo->EndSengContentKey;
	m_ListFindPack.SetItemText(nRow,9,strItemName);
	strItemName = pPackRuleInfo->CodeType;
	m_ListFindPack.SetItemText(nRow,10,strItemName);
}

void CFindCaptureRuleDlg::OnBnClickedButtonFinddlgStartcapture()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_ListFindPack.DeleteAllItems();
	CGlobalData::m_vtOfflinePackPath.clear();                                             //Xt: 清空离线包的路径
	m_setStrPackHead.clear();

	FindInfo fi;
	CString  strUserName,
		     strPassword,
			 strSubcontractingNum,
		     strSendContent;

	GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->GetWindowText(strUserName);
	GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->GetWindowText(strPassword);
	GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->GetWindowText(strSendContent);
	GetDlgItem(IDC_EDIT_SUBCONTRACTING_NUM)->GetWindowText(strSubcontractingNum);
	CGlobalData::m_nSeparatePackNum = _wtoi(strSubcontractingNum.GetBuffer());


	if(strUserName.IsEmpty() && strSendContent.IsEmpty())
	{
		m_strError.Format(_T("搜索内容不能为空!"));
		MessageBox(m_strError);
		return;
	}

	if(!strUserName.IsEmpty())	    CChangeCode::UnicodeToGB2312Code(fi.userName,strUserName.GetBuffer());
	if(!strPassword.IsEmpty())		CChangeCode::UnicodeToGB2312Code(fi.Password,strPassword.GetBuffer());
	if(!strSendContent.IsEmpty())   CChangeCode::UnicodeToGB2312Code(fi.SendContent,strSendContent.GetBuffer());

	if(!CGlobalData::SaveFindInf(fi))                                                         //Xt:添加多字码保存搜索内容
	{  
		m_strError.Format(_T("加载搜索内容错误!"));
		MessageBox(m_strError);
		return;
	}

	CString strAdapater;
	char    AdapaterName[MAX_PATH] = {0};
	int     n = m_ComboBox.GetCurSel();

	if(CB_ERR != n)
	{
		m_ComboBox.GetLBText(n,strAdapater);
		CChangeCode::UnicodeToGB2312Code(AdapaterName,strAdapater.GetBuffer());
		if(!m_packThread.SetAdapterInfo(AdapaterName))
		{
			MessageBox(_T("设置网卡信息错误!"));
			return;
		}
	}
	else
	{
		MessageBox(_T("没有选择网卡信息!"));
		return;
	}
	
	CString strRuleLen;
	((CButton*)GetDlgItem(IDC_EDIT_FIND_PACKRULELEN))->GetWindowText(strRuleLen);             //Xt:获取截取关键字的长度
	if(strRuleLen.IsEmpty())
	{
		MessageBox(_T("设置获取规则长度不能为空"));
		return;
	}
	else
	{
		int nLen = _wtoi(strRuleLen.GetBuffer());
		if(nLen<=1 || nLen>20)
		{
			MessageBox(_T("设置获取规则长度应在\"1~30\"之间"));
			return;
		}
		else
		{
			CGlobalData::m_nPackRuleLen = nLen;
		}
	}

	CString strHeadRuleLen;
	((CButton*)GetDlgItem(IDC_EDIT_FILTER_HEAD_LEN))->GetWindowText(strHeadRuleLen);          //Xt:获取截取关键字的长度
	if(strHeadRuleLen.IsEmpty())
	{
		MessageBox(_T("设置获取规则长度不能为空"));
		return;
	}
	else
	{
		int nHeadLen = _wtoi(strHeadRuleLen.GetBuffer());
		if(nHeadLen<=4 || nHeadLen>100)
		{
			MessageBox(_T("设置获取规则长度应在\"4~100\"之间"));
			return;
		}
		else
		{
			CGlobalData::m_nPackHeadRuleLen = nHeadLen;
		}
	}

	m_packServiceType = CAPTURE_PACK_RULE;                               
	if(((CButton *)GetDlgItem(IDC_RADIO_FIND_ONLINE_PACK))->GetCheck())                       //Xt:获取离线或在线的设置
	{
		m_packServiceType |= CAPTURE_INLINE_PACK;
	}
	else
	{
		char strPath[MAX_PATH] = {0};
		CString strError;
		CFileDialog  fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
			_T("pcap FILE(*.PCAP)|*.PCAP||"),NULL);
		if(fileDlg.DoModal() == IDOK)
		{
			CString str = fileDlg.GetPathName();
			char strPath[MAX_PATH] = {0};
			CChangeCode::UnicodeToGB2312Code(strPath,fileDlg.GetPathName().GetBuffer());
			if(strlen(strPath)<3)
			{
				MessageBox(_T("获取离线包错误!"));
				return;
			}
			CGlobalData::m_vtOfflinePackPath.push_back(strPath);
		}
		m_packServiceType |= CAPTURE_OFFLINE_PACK;
	}

	if(!m_packThread.OpenThread(&m_packServiceType))
	{
		m_strError.Format(_T("%s"),CGlobalData::m_strError);
		MessageBox(m_strError);
		return;
	}
	GetDlgItem(IDC_BUTTON_FINDDLG_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FINDDLG_FILTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINDDLG_STARTCAPTURE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINDDLG_SAVE)->EnableWindow(TRUE);
}

void CFindCaptureRuleDlg::OnBnClickedButtonFinddlgFilter()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString  strFilterNum;
	GetDlgItem(IDC_EDIT_FILTER_NUM)->GetWindowText(strFilterNum);
    
	int		 nKeyNum = 0,
			 nKeySum = _wtoi(strFilterNum.GetBuffer());
	char     strFindItemKey[1024] = {0}; 
 	for(int i = 0; i<m_ListFindPack.GetItemCount();i++)
	{
		CChangeCode::UnicodeToGB2312Code(strFindItemKey,(m_ListFindPack.GetItemText(i,1)).GetBuffer());
		nKeyNum = m_setStrPackHead.count(strFindItemKey);
		if( !(nKeyNum>0 && nKeyNum <= nKeySum) )
		{
			m_ListFindPack.DeleteItem(i);
			m_ListFindPack.Update(TRUE);
			--i;
		}
		memset(strFindItemKey,0,100);
	}
}

void CFindCaptureRuleDlg::OnBnClickedButtonFinddlgStop()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_FINDDLG_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINDDLG_FILTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FINDDLG_STARTCAPTURE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_FILTER_NUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FINDDLG_SAVE)->EnableWindow(TRUE);


	GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->SetWindowText(_T(""));

	if(m_packThread.m_bRun)                                               
	{
		m_packThread.Close();
	}
	CGlobalData::m_vtFindInfo.clear();                                          //Xt:清空捕获规则
	m_packThread.GetAdapterInfo();
}

void CFindCaptureRuleDlg::OnBnClickedButtonFinddlgQuite()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonFinddlgStop();	
	remove(CGlobalData::m_strRulePackInfoPath);
	remove(CGlobalData::m_strRulePackPath);
	OnCancel();
}

void CFindCaptureRuleDlg::_CallAddPackRuleInfo(PackRuleFile *pPackRuleFile)
{
	int nRow = m_pThis->m_ListFindPack.GetItemCount();
	m_pThis->AddListInfo(nRow,pPackRuleFile);
	m_pThis->m_setStrPackHead.insert(pPackRuleFile->PackHead);
}

void CFindCaptureRuleDlg::_CallStopCapture()
{
	if(m_pThis->m_packThread.m_bRun)                                               
	{
		m_pThis->m_packThread.Close();
	}
	CGlobalData::m_vtFindInfo.clear();                                          //Xt:清空捕获规则
	m_pThis->m_packThread.GetAdapterInfo();

	m_pThis->GetDlgItem(IDC_BUTTON_FINDDLG_STOP)->EnableWindow(FALSE);  
	m_pThis->GetDlgItem(IDC_BUTTON_FINDDLG_FILTER)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(IDC_BUTTON_FINDDLG_STARTCAPTURE)->EnableWindow(TRUE);

	m_pThis->GetDlgItem(IDC_EDIT_FILTER_NUM)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->EnableWindow(TRUE);

	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->SetWindowText(_T(""));
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->SetWindowText(_T(""));
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->SetWindowText(_T(""));
}

void CFindCaptureRuleDlg::LoadRulePackInfoToList()
{
// 	char strFileContentInfo[1024] = {0};
// 	PACKRULEFILE packRuleInfo;
// 	memset(&packRuleInfo,0,sizeof(packRuleInfo));
// 
// 	FILE *pFile = fopen(CGlobalData::m_strRulePackInfoPath,"rb");
// 	if(NULL != pFile)
// 	{
// 		int nRow = 0;
// 		while(fgets(strFileContentInfo,1024,pFile))
// 		{
// 			if(*strFileContentInfo != EOF || *strFileContentInfo != '\0')
// 			{
// 				if(CGlobalData::FindRuleFileInfo(strFileContentInfo,&packRuleInfo))
// 				{
// 					AddListInfo(nRow,&packRuleInfo);
// 					nRow++;
// 				}
// 			}
// 		}	
// 	}
// 	fclose(pFile);
}
void CFindCaptureRuleDlg::OnBnClickedButtonFinddlgSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSaveRulePath("");
	CString strSavepackPath("");

	CFileDialog filedlg(FALSE,_T(".txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("TXT FILE(*.TXT)|*.TXT||"),NULL);
	if(IDOK == filedlg.DoModal())
	{
		strSaveRulePath = filedlg.GetPathName();
	}

	CString strSource(CGlobalData::m_strRulePackInfoPath);
	if(!strSource.IsEmpty())
	{
		FILE *file1 = fopen(CGlobalData::m_strRulePackInfoPath,"r");
		if(file1)                                                                             //Xt:判断目录是否存在	
		{
			fclose(file1);
			file1 = NULL;
			if(!MoveFile(strSource,strSaveRulePath))                                          //Xt:移动文件
			{
				m_strError.Format(_T("保存文件错误!\r\n%d"),GetLastError());
				MessageBox(m_strError);
				return;
			}
			else
			{
				remove(CGlobalData::m_strRulePackInfoPath);
			}
		}
		strSource.Empty();
	}

	strSaveRulePath.Insert(strSaveRulePath.Find(_T(".TXT")),_T("CompletePack"));
	strSource = (CGlobalData::m_strRulePackPath);
	if(!strSource.IsEmpty())
	{
		FILE *file2 = fopen(CGlobalData::m_strRulePackPath,"r");
		if(file2)                                                                             //Xt:判断目录是否存在	
		{
			fclose(file2);
			file2 = NULL;
			if(!MoveFile(strSource,strSaveRulePath))                                          //Xt：移动文件
			{
				m_strError.Format(_T("保存文件错误!\r\n%d"),GetLastError());
				MessageBox(m_strError);
				return;
			}
			else
			{
				remove(CGlobalData::m_strRulePackPath);
			}
		}
	}
	GetDlgItem(IDC_BUTTON_FINDDLG_SAVE)->EnableWindow(FALSE);
}

void CFindCaptureRuleDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	if(0 != m_ListFindPack.GetItemCount())
		m_ListFindPack.DeleteAllItems();
	if(!m_setStrPackHead.empty())
		m_setStrPackHead.clear();

	m_pThis->GetDlgItem(IDC_BUTTON_FINDDLG_STOP)->EnableWindow(FALSE);  
	m_pThis->GetDlgItem(IDC_BUTTON_FINDDLG_FILTER)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(IDC_BUTTON_FINDDLG_STARTCAPTURE)->EnableWindow(TRUE);

	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->EnableWindow(TRUE);

	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_USERNAME)->SetWindowText(_T(""));
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_PASSWORD)->SetWindowText(_T(""));
	m_pThis->GetDlgItem(IDC_EDIT_FINDDLG_SENDCONTENT)->SetWindowText(_T(""));
}
