/******************************************************************************* 
* 1�� Class      �� CPugiXmlLog
* 2�� Version    �� 1.0
* 3�� Description�� C++�е�XML��־��-����PugiXml��
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-7-12 11:26:12
* 6�� History    �� 
* 7�� Remark     �� ֻ��Ҫ����Ӧ��XML�ļ�Log.XML�Ϳ��Կ���һ����־����ˣ�
*					Ϊ����ר��д��һ��XSL��ʽ�ļ���XML��ʽXSL�ļ�������XML�ļ�ͬһĿ¼�£���
*					�Ҳο���Emilio Guijarro Cameros��CXMLProfileдXML�����ļ���˼�룡����
ע����Ӹ�ʽ  ʱ�����  �������  �������
//Level 1���������̲�Ӧ�÷����Ĺ��ϣ���new����ʧ�ܣ��ļ�IO����ʧ�ܣ��쳣�˳��ȵȣ�
//Level 2��������Ч���жϣ��Լ�ָ��NULL�ж�Ϊ�����͹���
//Level 3: ������;�쳣���أ�
//Level 4: ������Ϣ�Ĵ�ӡ�����
//Level 5: ������������Ҫ�������ݵĵ��������
//Ĭ����־������3�����������־����־��¼��ʱ���;���û������FileByDay|FileByNameѡ�Ĭ��ΪDisplayScreen���������Ļ
//���������־����ÿ������һ��������Ϊ�ļ�������־��������ӿ����Լ�������ܣ�egɾ��30����ǰ����־��
********************************************************************************/ 
#ifndef  _PUGIXML_LOG_H_
#define  _PUGIXML_LOG_H_

#include "stdafx.h"
#include <windows.h>
//#include <pugiconfig.hpp>

#include <pugixml.hpp>
#pragma comment(lib, "../bin/pugixmld.lib")
namespace QNA
{
	class CPugiXmlLog
	{
	private:
		//��ȡ��ǰ��������Ŀ¼
		bool GetExePath(LPTSTR pstrPath)
		{
			LPTSTR ptTem = NULL;
			TCHAR tszTemp[MAX_PATH] = {0};
			//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
			if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && pstrPath)
				return false; 

			ptTem = _tcsrchr(tszTemp, '\\');
			memcpy(pstrPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
			return true;
		}

	public:
		CPugiXmlLog(LPCTSTR lpszFileName = NULL)
		{
			TCHAR tszTem[MAX_PATH] = {0};
			TCHAR tszFileName[MAX_PATH] = {0};
			if (NULL == lpszFileName)
			{
				GetExePath(tszTem);
				_stprintf_s(tszFileName, _T("%s\\Log.xml"), tszTem);
			}
			else
			{
				_tcscpy_s(tszFileName, lpszFileName);
			}

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(tszFileName);
			
			if (!result)
			{ 
				doc.load("<foo bar='baz'>hey</foo>");
				//doc.load("<?xml-stylesheet href='Log.xsl' type='text/xsl'?>");
				doc.save_file(tszFileName);
				result = doc.load_file(tszFileName);
			}
			doc.reset();

		}
		~CPugiXmlLog(void){}

		bool ReadXml(LPCTSTR lpszFileName = NULL)
		{
			std::string strFile = "D:\\WinPath\\desktop\\test.xml";
			pugi::xml_document doc;
			if (!doc.load_file(strFile.c_str()))
			{
				//return 0;
			}
			pugi::xml_node form = doc.child("root").child("form");
			std::string ip = form.attribute("ip").value();
			std::string port = form.attribute("port").value();

			char cBuf[2083] = {0};
			sprintf(cBuf, "http://%s:%s", ip.c_str(), port.c_str());
			std::string strTemp(cBuf);
			std::string m_strURLBase = strTemp;

			for (pugi::xml_node input = form.first_child(); input;
				input = input.next_sibling())
			{
				std::string strValue = input.attribute("value").value();
				if (!strValue.empty())
				{
					std::string strName = input.attribute("name").value();
					sprintf(cBuf, "%s=%s&", strName.c_str(), strValue.c_str());
					std::string strTemp(cBuf);
					m_strURLBase += strTemp;
				}


			}

			//��ȡxpath
			pugi::xml_node xpath = doc.child("root").child("xpath");
			std::string m_strPOIRoot = xpath.attribute("poiroot").value();
			std::string m_strPOIID = xpath.attribute("idfield").value();

			//��ȡ����Ȩ��
			pugi::xml_node weight = doc.child("root").child("weight");
			float m_fThred = atof(weight.child_value("threshold"));
			float m_fStep = atof(weight.child_value("step"));

			return true;
		}

		void Log(LPCTSTR lpszFilName, LPCTSTR s,...);  //Log�����һ����־
		bool ClearAll();                      //ClearAll�����������־
	};
}


#endif // (_PUGIXML_LOG_H_)