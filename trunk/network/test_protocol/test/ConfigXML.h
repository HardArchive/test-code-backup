#pragma once
/***************************************************************************************************
* 1�� File       �� ConfigXML.h
* 2�� Version    �� *.*
* 3�� Description�� XML�����ļ�������
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-7-18 11:10:11
* 6�� History    �� 
* 7�� Remark     �� 
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

	//ȡ�ý���XML root����µ����н������� 
	int GetServicesFindInfo(PSERVICESFINDINFO pstuOutServicesFindInfo)
	{
		if (!CheckOpenXML()) return -1;
		if (!pstuOutServicesFindInfo) return -2;

		return GetXMLServicesFindInfo(m_clsxml_doc.child("root"), pstuOutServicesFindInfo);
	}

private:
	//ȡ�ý���XML ServicesFind����µ����н������� 
	int GetXMLServicesFindInfo(pugi::xml_node node, PSERVICESFINDINFO pstuOutServicesFindInfo)
	{
		if (!pstuOutServicesFindInfo || !node)	return -1;

		bool bIsFirst = true;                                              //�Ƿ�Ϊ��һ����� trueΪ�� falseΪ��
		PSERVICESFINDINFO pstuServicesFindInfo = pstuOutServicesFindInfo;   //������������ǰ�����ڵ�ָ��

		for (pugi::xml_node child = node.first_child(); child; )
		{
			if ((child.type() == pugi::node_element) && (0 == strcmp(child.name(), "ServicesFind")))
			{
				//�ڶ����ڵ���Ҫ�����µĿռ�
				if (!bIsFirst)
				{
					pstuServicesFindInfo->AddServicesFindInfo();
					pstuServicesFindInfo = pstuServicesFindInfo->pstuServicesFindInfoNext;
				}	
				else
				{
					bIsFirst =  false;
				}
				//ȡ�÷�������
				pstuServicesFindInfo->iServicesType = child.child("ServicesType").first_child().text().as_int();
				//ȡ��FindInfo�����Ϣ
				pstuServicesFindInfo->pstuFindInfo = pstuServicesFindInfo->pstuFindInfo->NewFindInfo();
				GetXMLFindInfo(child, pstuServicesFindInfo->pstuFindInfo);			
			}
			child = child.next_sibling();
		}
		return 1;
	}

	//ȡ�ý���XML FindInfo����µ����н������� 
	//��ȡ���ұ����Ϣ����  nodeΪ��XML��������xml_nodeָ��  pstuFindInfo���ṹ����ָ�� 
	int GetXMLFindInfo(pugi::xml_node node, PFINDINFO pstuOutFindInfo)
	{		
		if (!pstuOutFindInfo || !node)	return -1;

		bool bIsFirst = true; 
		PFINDINFO pstuFindInfo = pstuOutFindInfo;   //������������ǰ�����ڵ�ָ��

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

				//��http���͵����� hostΪ���� ��ͷ���Ϊ ���ַ�������
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

	//���marktype��� �����ݱ��ת�� �ַ���
	void CheckMarkType(PMARKFIND pstuInOutMarkFind)
	{
		BYTE szbyTem[64] = {0};
		//��http���͵����� hostΪ���� ��ͷ���Ϊ ���ַ�������
		//��ʼ���Ϊ 16�����ַ�����ת�ɶ���������
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
	//��ȡXML·�� ptInPath��ǰ·�� iTypeΪXML����
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

	//��鵱ǰĿ¼��Ҫ���ļ����Ƿ����,��������ڷ���false�����ڷ���true
	bool ChickDirExist(const PTCHAR ptInPath)
	{
		if (!ptInPath || _tcsclen(ptInPath)<2)	return false;

		//����·���Ƿ����	����������򴴽�
		if (GetFileAttributes(ptInPath) != FILE_ATTRIBUTE_DIRECTORY)	return false;
		return true;
	}

	//��ȡ��ǰ��������Ŀ¼ �ɹ�����true��ʧ�ܷ���false
	bool GetExePath(TCHAR* ptInPath)
	{
		TCHAR* ptTem = NULL;
		TCHAR tszTemp[MAX_PATH] = {0};
		//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
		if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
			return false; 

		ptTem = _tcsrchr(tszTemp, _T('\\'));
		memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
		return true;
	}

	//���ַ������������ת��Ϊ��ֵ
	int char2int(char ch)
	{
		if(ch>='0' && ch<='9') return (char)(ch-'0');
		if(ch>='a' && ch<='f') return (char)(ch-'a'+10);
		if(ch>='A' && ch<='F') return (char)(ch-'A'+10);
		return -1;
	}

	//��16�����ַ���ת���ɶ���������(hex)
	//pbyInHex ���������ݴ���ָ�� iHexLen���ݳ��� pOutStrת����16�����ַ�������ָ�� iMaxStrLen����ַ������ݵ���󳤶� ������󳤶Ƚ��ᱻ�ض�
	//����ת�������ݵ�ʵ�ʳ��� ���������󳤶Ƚ��ᱻ�ض� ע�����ת��������Ϊ�ַ�����ע�ⲹ��
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
	bool m_bIsOpenXML;                                                     //XML�Ƿ��
	TCHAR m_tszPathXML[MAX_PATH];                                          //XML·��
	pugi::xml_document m_clsxml_doc;
};
