#include "stdafx.h"
#include "helper.h"
//#include "../../Common/Src/FuncHelper.h"


info_msg* info_msg::Construct(DWORD dwConnectionID, LPCTSTR lpszEvent, int iContentLength, LPCTSTR lpszContent)
{
	return new info_msg(dwConnectionID, lpszEvent, iContentLength, lpszContent);
}

void info_msg::Destruct(info_msg* pMsg)
{
	delete pMsg;
}

info_msg::info_msg(DWORD dwConnectionID, LPCTSTR lpszEvent, int iContentLength, LPCTSTR lpszContent)
	: connID(dwConnectionID), evt(lpszEvent), contentLength(iContentLength), content(lpszContent)
{

}

info_msg::~info_msg()
{
	if(contentLength > 0)
		delete[] content;
}
