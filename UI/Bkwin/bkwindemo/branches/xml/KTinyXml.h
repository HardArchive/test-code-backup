#ifndef __KTinyXml_h__
#define __KTinyXml_h__

#include "..\3rdparty\tinyxml.h"
#include "KPathParser.h"

class KTinyXml
{
public:
	KTinyXml();

	BOOL LoadFile(IN LPCSTR szXmlFile);
	BOOL SaveFile(IN LPCSTR szXmlFile);
	BOOL Parse(IN LPCSTR szXml);
	void Clear(void);

	BOOL Open(IN LPCSTR szPath, BOOL bCreate = FALSE);
	BOOL OpenSubPath(IN LPCSTR szSubPath, BOOL bCreate = FALSE);

	BOOL Write(IN LPCSTR szKey, IN int nValue);
	BOOL Write(IN LPCSTR szKey, IN LPCSTR szValue);
	BOOL Write(IN LPCSTR szKey, IN LPCWSTR szValue);
	BOOL WriteText(IN LPCSTR szText, BOOL bCDATA = FALSE);
	BOOL WriteText(IN LPCWSTR szText, BOOL bCDATA = FALSE);

	BOOL Read(IN LPCSTR szKey, OUT int& nValue);
	void Read(IN LPCSTR szKey, OUT CStringA& strValue);
	void Read(IN LPCSTR szKey, OUT CStringW& strValue);
	void ReadText(OUT CStringA& strText);
	void ReadText(OUT CStringW& strText);

	CStringA ToXml(void);

	void RootElement(void);
	BOOL ParentElement(void);
	BOOL FirstChild(LPCSTR szName = NULL);
	BOOL NextSibling(LPCSTR szName = NULL);
	
	BOOL AddSibling(LPCSTR szName, BOOL bOpen = FALSE);
	BOOL AddChild(LPCSTR szName, BOOL bOpen = FALSE);

	BOOL DeleteCurrentElement(void);

protected:
	TiXmlElement* _Open(TiXmlElement* pParentElement, LPCSTR szPath, BOOL bCreate);
	TiXmlElement* _Create(TiXmlNode* pParentNode, KPathParserA& path, int nPathIndex);

private:
	TiXmlDocument m_xmlDoc;
	TiXmlElement* m_pOpenElement;
};

#endif