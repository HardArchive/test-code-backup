#pragma once
/***************************************************************************************************
* 1、 File       ： ConfigXML.h
* 2、 Version    ： *.*
* 3、 Description： XML配置文件辅助类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-7-18 11:10:11
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
//#include "pugixml.hpp"
//#include "pugiconfig.hpp"
#include "../third_party/pugixml/pugixml.hpp"
#include "../third_party/pugixml/pugiconfig.hpp"

#ifndef _INC_TCHAR
#include <tchar.h>
#endif
typedef TCHAR* PTCHAR;
#include <string>
using namespace std;
#include "protocol.h"
class CConfigXML
{
public:
	CConfigXML(const PTCHAR ptInPath = NULL)
	{
		m_bIsOpenXML = false;
		memset(m_tszPathXML, 0, MAX_PATH*sizeof(TCHAR));
		if (!ptInPath)
		{
			GetXmlPath(m_tszPathXML);
		}

		CheckOpenXML();
	}
	~CConfigXML(void){}

public:

	//取得解析XML root结点下的所有结点的内容 
	int GetServicesFindInfo(PSERVICESFINDINFO pstuOutServicesFindInfo)
	{
		if (!CheckOpenXML()) return -1;
		if (!pstuOutServicesFindInfo) return -2;

		return GetXMLServicesFindInfo(m_clsxml_doc.child("root"), pstuOutServicesFindInfo);
	}

private:
	//取得解析XML ServicesFind结点下的所有结点的内容 
	int GetXMLServicesFindInfo(pugi::xml_node node, PSERVICESFINDINFO pstuOutServicesFindInfo)
	{
		if (!pstuOutServicesFindInfo || !node)	return -1;

		bool bIsFirst = true;                                              //是否为第一个结点 true为是 false为否
		PSERVICESFINDINFO pstuServicesFindInfo = pstuOutServicesFindInfo;   //用来保存链表当前操作节点指针

		for (pugi::xml_node child = node.first_child(); child; )
		{
			if ((child.type() == pugi::node_element) && (0 == strcmp(child.name(), "ServicesFind")))
			{
				//第二个节点需要申请新的空间
				if (!bIsFirst)
				{
					pstuServicesFindInfo->AddServicesFindInfo();
					pstuServicesFindInfo = pstuServicesFindInfo->pstuServicesFindInfoNext;
				}	
				else
				{
					bIsFirst =  false;
				}
				//取得服务类型
				pstuServicesFindInfo->iServicesType = child.child("ServicesType").first_child().text().as_int();
				//取得FindInfo结点信息
				pstuServicesFindInfo->pstuFindInfo = pstuServicesFindInfo->pstuFindInfo->NewFindInfo();
				GetXMLFindInfo(child, pstuServicesFindInfo->pstuFindInfo);			
			}
			child = child.next_sibling();
		}
		return 1;
	}

	//取得解析XML FindInfo结点下的所有结点的内容 
	//获取查找标记信息链表  node为除XML根结点外的xml_node指针  pstuFindInfo检查结构链表指针 
	int GetXMLFindInfo(pugi::xml_node node, PFINDINFO pstuOutFindInfo)
	{		
		if (!pstuOutFindInfo || !node)	return -1;

		bool bIsFirst = true; 
		PFINDINFO pstuFindInfo = pstuOutFindInfo;   //用来保存链表当前操作节点指针

		for (pugi::xml_node child = node.first_child(); child; )
		{
			if ((child.type() == pugi::node_element) && (0 == strcmp(child.name(), "FindInfo")))
			{
				if (!bIsFirst)
				{
					pstuFindInfo->AddFindInfo();
					pstuFindInfo = pstuFindInfo->pstuFindInfoNext;
				}	
				else
				{
					bIsFirst =  false;
				}

				pstuFindInfo->bIsExtendFind = child.child("IsExtendFind").first_child().text().as_bool();
				pstuFindInfo->iHeadTagType = child.child("HeadTagType").first_child().text().as_int();
				strcpy_s(pstuFindInfo->szHost, 128, child.child("Host").first_child().value());
				strcpy_s(pstuFindInfo->szPacketHeadTag, 64, child.child("PacketHeadTag").first_child().value());

				//非http类型的数据 host为空则 包头标记为 非字符串类型
				if (2 == pstuFindInfo->iHeadTagType)
				{
					BYTE szbyTem[64] = {0};
					pstuFindInfo->ibyBufLen = str2hex(pstuOutFindInfo->szPacketHeadTag, szbyTem, 64);
					memset(pstuFindInfo->szPacketHeadTag, 0, 64);
					memcpy(pstuFindInfo->szPacketHeadTag, szbyTem, 64);
				}

				pstuFindInfo->pstuMarkFind = pstuFindInfo->pstuMarkFind->NewMarkFind();
				GetXMLMarkFind(child.child("MarkFind"), pstuFindInfo->pstuMarkFind);					
			}

			child = child.next_sibling();
		}

		return 1;
	}

	int GetXMLMarkFind(pugi::xml_node node, PMARKFIND pstuOutMarkFind)
	{
		if (!pstuOutMarkFind || !node)	return -1;

		bool bIsFirst = true; 
		PMARKFIND pstuMarkFind = pstuOutMarkFind;   

		for (pugi::xml_node child = node.first_child(); child; )
		{
			if ((child.type() == pugi::node_element) && (0 == strcmp(child.name(), "MARK")))
			{
				if (!bIsFirst)
				{
					pstuMarkFind->AddMarkFind();
					pstuMarkFind = pstuMarkFind->pstuMarkFindNext;					
				}
				else
				{
					bIsFirst =  false;
				}
				pstuMarkFind->iEncodingType = child.attribute("encodingtype").as_int();
				pstuMarkFind->iPacketNum = child.attribute("packetnum").as_int();
				pstuMarkFind->iMarkType = child.attribute("marktype").as_int();		
				strcpy_s(pstuMarkFind->szMarkStart, 64, child.attribute("markstart").value());
				strcpy_s(pstuMarkFind->szMarkEnd, 16, child.attribute("markend").value());
				pstuMarkFind->enumSaveType = (SAVE_TYPE_TAG)child.attribute("savetype").as_int();
				CheckMarkType(pstuMarkFind);
			}
			child = child.next_sibling();
		}

		return true;
	}

	//检查marktype标记 并跟据标记转换 字符串
	void CheckMarkType(PMARKFIND pstuInOutMarkFind)
	{
		BYTE szbyTem[64] = {0};
		//非http类型的数据 host为空则 包头标记为 非字符串类型
		//开始标记为 16进制字符串需转成二进制数据
		if ((MT_START_TAG&pstuInOutMarkFind->iMarkType)||(MT_ALL_TAG&pstuInOutMarkFind->iMarkType))
		{
			pstuInOutMarkFind->ibyStartBufLen = str2hex(pstuInOutMarkFind->szMarkStart, szbyTem, 64);
			memset(pstuInOutMarkFind->szMarkStart, 0, 64);
			memcpy(pstuInOutMarkFind->szMarkStart, szbyTem, 64);
		}

		if ((MT_END_TAG&pstuInOutMarkFind->iMarkType)||(MT_ALL_TAG&pstuInOutMarkFind->iMarkType))
		{					
			memset(szbyTem, 0, 64);
			pstuInOutMarkFind->ibyEndBufLen = str2hex(pstuInOutMarkFind->szMarkEnd, szbyTem, 16);
			memset(pstuInOutMarkFind->szMarkEnd, 0, 16);
			memcpy(pstuInOutMarkFind->szMarkEnd, szbyTem, 16);
		}
	}

	bool CheckOpenXML()
	{
		if (!m_bIsOpenXML)
		{
			m_bIsOpenXML = m_clsxml_doc.load_file(m_tszPathXML);;
			return m_bIsOpenXML;
		}
		return true;
	}
	//获取XML路径 ptInPath当前路径 iType为XML类型
	PTCHAR GetXmlPath(PTCHAR ptInPath, PTCHAR ptInNameXML = NULL)
	{
		if (GetExePath(ptInPath))
		{
			_tcscat_s(ptInPath, MAX_PATH-_tcslen(ptInPath), _T("\\"));
			if (ptInNameXML)
			{
				_tcscat_s(ptInPath, MAX_PATH-_tcslen(ptInPath), ptInNameXML);
			}
			else
				_tcscat_s(ptInPath, MAX_PATH-_tcslen(ptInPath), _T("protocol.xml"));
		}
		return ptInPath;
	}

	//检查当前目录需要的文件夹是否存在,如果不存在返回false，存在返回true
	bool ChickDirExist(const PTCHAR ptInPath)
	{
		if (!ptInPath || _tcsclen(ptInPath)<2)	return false;

		//检验路径是否存在	如果不存在则创建
		if (GetFileAttributes(ptInPath) != FILE_ATTRIBUTE_DIRECTORY)	return false;
		return true;
	}

	//获取当前程序所在目录 成功返回true，失败返回false
	bool GetExePath(TCHAR* ptInPath)
	{
		TCHAR* ptTem = NULL;
		TCHAR tszTemp[MAX_PATH] = {0};
		//获取当前目录  //这里是获取当前进程文件的完整路径 
		if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
			return false; 

		ptTem = _tcsrchr(tszTemp, _T('\\'));
		memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
		return true;
	}

	//将字符所代表的数字转化为数值
	int char2int(char ch)
	{
		if(ch>='0' && ch<='9') return (char)(ch-'0');
		if(ch>='a' && ch<='f') return (char)(ch-'a'+10);
		if(ch>='A' && ch<='F') return (char)(ch-'A'+10);
		return -1;
	}

	//将16进制字符串转换成二进制数据(hex)
	//pbyInHex 二进制数据传入指针 iHexLen数据长度 pOutStr转换的16进制字符串传出指针 iMaxStrLen存放字符串数据的最大长度 超过最大长度将会被截断
	//返回转换后数据的实际长度 如果超过最大长度将会被截断 注意如果转出的数据为字符串需注意补零
	int str2hex(char* pInstr, BYTE* pbyOuthex, int iInMaxHexLen)
	{
		int i=0;
		int iTem = strlen(pInstr);
		for(int j = 0; j < iTem; )
		{
			if (i+1 >= iInMaxHexLen) break;
			unsigned int a =  char2int(pInstr[j++]);
			unsigned int b =  char2int(pInstr[j++]);
			pbyOuthex[i++] = (a << 4) | b;
		}
		return i;
	}

private:
	bool m_bIsOpenXML;                                                     //XML是否打开
	TCHAR m_tszPathXML[MAX_PATH];                                          //XML路径
	pugi::xml_document m_clsxml_doc;
};
