#include "StdAfx.h"
#include "IpLookup.h"
//#include   <atlbase.h>
//#include   <atlcom.h> 
#include <boost/regex.hpp>

using namespace boost;
#include <iostream>
#include <fstream>
//#include <sstream>
#include <string>
#include <iterator>
CIpLookup::CIpLookup(void)
{
}

CIpLookup::~CIpLookup(void)
{
}

bool CIpLookup::GetCurrentPath(LPTSTR pstrPath)
{
	LPTSTR ptTem = NULL;
	TCHAR tszTemp[MAX_PATH] = {0};
	//获取当前目录  //这里是获取当前进程文件的完整路径 
	if (!GetModuleFileName(NULL, tszTemp, MAX_PATH))
	{
		return false;
	} 

	ptTem = _tcsrchr(tszTemp, '\\');
	//GetCurrentDirectory(MAX_PATH, g_szMinitorFolderPah);
	memcpy(pstrPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
	return true;
}

//设置第三解析服务器网址和与其相关的正则表达
void CIpLookup::SetUrlRegex(LPCTSTR pstrUrl, LPCTSTR pstrRegex)
{
	ZeroMemory(m_tszUrl, sizeof(m_tszUrl));
	ZeroMemory(m_tszRegex, sizeof(m_tszRegex));
	_tcscpy_s(m_tszUrl, pstrUrl);

	_tcscpy_s(m_tszRegex, pstrRegex);
}

/******************************************************************************* 
1、 函数名称： StartIpLookup
2、 功能描述： 开始反查
3、 输入参数： LPCTSTR pstrIP 反查IP地址
4、 返 回 值： 成功返回ture， 失败返回false
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2012-7-9 13:03:17
8、 备    注： 
********************************************************************************/ 
bool CIpLookup::StartIpLookup(LPCTSTR pstrIP)
{
	//从第三方服务器下载网页
	//解析parse网页 取得查询结果
	// 
	//_tcscpy_s(m_tszIP, pstrIP);

	TCHAR tszUrl[MAX_PATH] = {0} ;
	TCHAR tszPath[MAX_PATH] = {0};
	_stprintf(tszUrl, _T("%s%s"), m_tszUrl, pstrIP);
	_stprintf(tszPath, _T("C:\\Lookup\\%s.html"), pstrIP);
	CDownLoad CObjDL;

	if (1 != CObjDL.DownLoadFile(tszUrl, tszPath))
	{
		OutputDebugString(_T("CIpLookup::StartIpLookup DownLoadFile error!"));
		return false;
	}
	
	//GetHtmlBuffer();
	ParseBuffer(tszPath);



	return true;
}

////从第三方服务器下载页面
//bool CIpLookup::Download()
//{
//	TCHAR tszUrl[MAX_PATH] = {0} ;
//	TCHAR tszPath[MAX_PATH] =  _T("C:\\Lookup\\1.html");
//	_stprintf(tszUrl, _T("%s%s"), m_tszUrl, m_tszIP);
//	CDownLoad CObjDL;
//	CObjDL.DownLoadFile(tszUrl, tszPath);
//
//	return true;
//}

//bool CIpLookup::GetHtmlBuffer()
//{
//	ZeroMemory(m_tszLoadBuf, sizeof(m_tszLoadBuf));
//	TCHAR szFilename[MAX_PATH] = {_T("C:\\Lookup\\1.html")};
//	CFile f;
//	if( f.Open(szFilename)==TRUE ) 
//	{
//		DWORD dwSize = f.GetSize();
//		if (dwSize>512*1024)
//		{
//			OutputDebugString(_T("CIpLookup::GetHtmlBuffer buffer small!"));
//			return false;
//		}
//		f.Read(m_tszLoadBuf, dwSize);
//		m_tszLoadBuf[dwSize] = '\0';
//		f.Close();
//	}
//	return true;
//}

void CIpLookup::LoadFile(std::wstring& wstrOut, std::wistream& wistreamIn)
{
	wchar_t wch = 0;
	wstrOut.erase();    //删除字符串
	//bad() 如果出现错误则返回true 
	if(wistreamIn.bad()) 
		return;
	wstrOut.reserve(wistreamIn.rdbuf()->in_avail());   //reserve 预留空间 

	//get() 读取字符 
	while(wistreamIn.get(wch))
	{	//capacity 在不重新分配内存的情况下，字符串可能的大小  size 得到字符串的大小 
		if(wstrOut.capacity() == wstrOut.size())
			wstrOut.reserve(wstrOut.capacity() * 3);
		wstrOut.append(1, wch);  //append 追加字符
	}
}

bool CIpLookup::ParseBuffer(LPCTSTR pstrPath)
{
	std::wstring inw;
	//std::wifstream fs("C:\\Lookup\\1.html");
	std::wifstream fs(pstrPath);
	LoadFile(inw, fs);
	fs.close();

	wsmatch mat2;
	wregex expression2(m_tszRegex);
	std::wstring::const_iterator start = inw.begin();
	std::wstring::const_iterator end = inw.end();
	while( boost::regex_search(start, end, mat2, expression2) )
	{
		std::wcout<< "域名:" <<mat2[1].str().c_str() <<std::endl;
		m_strVecDomain.push_back(mat2[1].str());
		start = mat2[0].second;
	}


	return true;
}