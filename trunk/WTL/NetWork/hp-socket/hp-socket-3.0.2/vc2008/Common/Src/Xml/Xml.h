#pragma once

#include "XmlNode.h"


//-------------------------------------------------------------------------
// Classes Name	    : CXml CXmlNode CXmlNodes
// Author			: Kingfisher
// CreateTime       : 2005-11-03
// Memo             : Xml������
// Version          : v1.0.5
//-------------------------------------------------------------------------



class CXML_API CXml
{
public:
	enum EnXmlSrcType
	{
		XST_FILE,
		XST_STREAM,
	};

public:
	CXml(LPCTSTR pszXmlSrc = NULL, EnXmlSrcType xst = XST_FILE);
	~CXml(void);

public:
	BOOL Base64Decode(LPCTSTR pszIn, LPBYTE& pBuf, LONG& lSize);
	CString Base64Encode(LPBYTE pBuf, ULONG ulSize);

	// ����Xml
	BOOL Open(LPCTSTR pszXmlSrc, EnXmlSrcType xst = XST_FILE);

	// ����Xml�ļ�
	BOOL Save(LPCTSTR lpszFilePath = NULL);

	// �ر�Xml�ļ�
	void Close(void);

	// ���ΪXml���� (BSTR)
	BSTR ToBSTR() const;

	// ���ΪXml���� (CString)
	BOOL ToString(CString& strOut) const;

	// ��ȡ����Ĵ�����Ϣ
	CString GetLastError(void) const;

	// ��ȡXml���ĸ����
	CXmlNodePtr GetRoot(void) const;		

	// ѡ�񵥸����
	CXmlNodePtr SelectSingleNode(LPCTSTR lpszPath) const;

	// ѡ���㼯��
	CXmlNodesPtr SelectNodes(LPCTSTR lpszPath) const;

	// �Ƿ����
	BOOL IsValid()  const {return m_pDoc != NULL;}

	// �������
	CXmlNodePtr CreateNode(LPCTSTR pszName);

	// ����Xml�ĵ�
	static CXml CreateDocument(LPCTSTR pszRootName, LPCTSTR pszCharSet = _T("GBK"), BOOL bCreateHeader = TRUE);

private:
	// �ж��ļ��Ƿ����
	//static BOOL IsFileExist(LPCTSTR pszFilePath);

protected:
	MSXML2::IXMLDOMDocument2Ptr m_pDoc; 
	CString						m_strFilePath;
};
