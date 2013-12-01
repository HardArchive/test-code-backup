#include "StdAfx.h"
#include ".\xmlnode.h"


//////////////////////////////////////////////////////////////////////////
#ifndef _tstof
#ifdef _UNICODE

double __cdecl _tstof(const wchar_t *ptr)
{
	CHAR astring[20];
	WideCharToMultiByte(CP_ACP, 0, ptr, -1, astring, 20, NULL, NULL);
	return atof(astring);
}

#else
#define _tstof atof
#endif
#endif
//////////////////////////////////////////////////////////////////////////



//-------------------------------------------------------------------------
// Function Name    :_GetValue
// Parameter(s)     :CString& strValue IN	ֵ
// Return           :BOOL	�Ƿ�ɹ�
// Memo             :��ȡ��ǰ�ڵ��ֵ
//-------------------------------------------------------------------------
BOOL CXmlNode::_GetValue(CString& strValue OUT)
{
	ASSERT(m_pNode != NULL);

	HRESULT hr = S_OK;
	try
	{
		if(HasChildren())
		{
			// �������ӽ��
			ASSERT(FALSE);
			return FALSE;
		}

		BSTR bstr = NULL;
		hr = m_pNode->get_text(&bstr);
		ASSERT(SUCCEEDED(hr));	
		strValue = (LPCTSTR)_bstr_t(bstr, false);

	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::_GetValue �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
		return FALSE;
	}

	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------------
// Function Name    :_SetValue
// Parameter(s)     :CString& strValue IN
// Return           :BOOL	�Ƿ�ɹ�
// Memo             :��ȡ��ǰ�ڵ��ֵ
//-------------------------------------------------------------------------
BOOL CXmlNode:: _SetValue(LPCTSTR pszValue IN)
{
	ASSERT(m_pNode != NULL);

	HRESULT hr = S_OK;

	try
	{
		if(HasChildren())
		{
			// �������ӽ�� 
			ASSERT(FALSE);
			return FALSE;
		}

		hr = m_pNode->put_text(CComBSTR(pszValue));
		ASSERT(SUCCEEDED(hr));
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::_SetValue �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
		return FALSE;
	}
	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------------
// Function Name    :_GetAttribute
// Parameter(s)     :CString& strName	IN	������
// Return			:CString& strValue OUT	����ֵ
// Memo             :��ȡ����ֵ
//-------------------------------------------------------------------------
BOOL CXmlNode::_GetAttribute(LPCTSTR pszName IN, CString& strValue OUT)
{
	ASSERT(m_pNode != NULL);

	HRESULT hr = S_OK;

	try
	{
		MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMap = NULL;
		hr = m_pNode->get_attributes(&pIXMLDOMNamedNodeMap);
		if(!SUCCEEDED(hr))
		{
			ASSERT(FALSE);
			return FALSE;
		}

		MSXML2::IXMLDOMNodePtr pIXMLDOMNode = NULL;
		pIXMLDOMNode = pIXMLDOMNamedNodeMap->getNamedItem(_bstr_t(pszName));

		if(pIXMLDOMNode == NULL)
		{
			strValue.Empty();
		}
		else
		{
			VARIANT varValue;
			hr = pIXMLDOMNode->get_nodeValue(&varValue);
			if(!SUCCEEDED(hr))
			{
				ASSERT(FALSE);
				return FALSE;
			}

			strValue = (LPCTSTR)(_bstr_t)varValue;
		}

		RELEASE_PTR(pIXMLDOMNode);
		RELEASE_PTR(pIXMLDOMNamedNodeMap);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::_GetAttribute �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}


//-------------------------------------------------------------------------
// Function Name    :_SetAttribute
// Parameter(s)     :CString& strName	IN	����
//					:CString& strValue IN  ����ֵ
// Return           :BOOL
// Memo             :��������ֵ
//-------------------------------------------------------------------------
BOOL CXmlNode::_SetAttribute(LPCTSTR pszName, CString& strValue IN)
{
	ASSERT(m_pNode != NULL);

	HRESULT hr = S_OK;
	try
	{
		MSXML2::IXMLDOMElementPtr pElement = NULL;
		pElement = static_cast<MSXML2::IXMLDOMElementPtr> (m_pNode);
		hr = pElement->setAttribute((_bstr_t)CComBSTR(pszName), _variant_t((LPCTSTR)strValue));
		RELEASE_PTR(pElement);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::_SetAttribute �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
		return FALSE;
	}

	return SUCCEEDED(hr);
}





//-------------------------------------------------------------------------
// Function Name    :CXmlNode
// Parameter(s)     :MSXML2::IXMLDOMNodePtr pNode	[in]
// Memo             :������
//-------------------------------------------------------------------------
CXmlNode::CXmlNode(MSXML2::IXMLDOMNodePtr pNode IN)
{
	m_pNode = pNode;
}


//-------------------------------------------------------------------------
// Function Name    :CXmlNode
// Parameter(s)     :void
// Memo             :������
//-------------------------------------------------------------------------
CXmlNode::CXmlNode(void)
{
	m_pNode = NULL;	
}


//-------------------------------------------------------------------------
// Function Name    :CXmlNode
// Parameter(s)     :CXmlNode& refNode [in]
// Memo             :�������캯��
//-------------------------------------------------------------------------
CXmlNode::CXmlNode(CXmlNode& refNode IN)
{
	m_pNode = refNode.m_pNode;
}


//-------------------------------------------------------------------------
// Function Name    :~CXmlNode
// Parameter(s)     :void
// Memo             :��������
//-------------------------------------------------------------------------
CXmlNode::~CXmlNode(void)
{
	RELEASE_PTR(m_pNode);
}


//-------------------------------------------------------------------------
// Function Name    :Release
// Parameter(s)     :void
// Return           :void
// Memo             :�ͷŽڵ�
//-------------------------------------------------------------------------
void CXmlNode::Release(void)
{
	RELEASE_PTR(m_pNode);
}




//-------------------------------------------------------------------------
// Function Name    :operator =
// Parameter(s)     :CXmlNodePtr pNode	[in]
// Return           :CXmlNodePtr
// Memo             :���� = ������
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNode::operator = (CXmlNodePtr pNode IN)
{
	RELEASE_PTR(m_pNode);

	m_pNode = pNode->m_pNode;
	return pNode;
}


//-------------------------------------------------------------------------
// Function Name    :operator =
// Parameter(s)     :CXmlNode& refNode	[in]
// Return           :CXmlNode&
// Memo             :���� = ������
//-------------------------------------------------------------------------
CXmlNode& CXmlNode::operator = (CXmlNode& refNode IN)
{
	RELEASE_PTR(m_pNode);

	m_pNode = refNode.m_pNode;
	return (*this);
}


//-------------------------------------------------------------------------
// Function Name    :IsNull
// Parameter(s)     :void
// Return           :BOOL
// Memo             :�жϴ˽���Ƿ��ǿսڵ�
//-------------------------------------------------------------------------
BOOL CXmlNode::IsNull(void)
{
	return m_pNode == NULL;
}

//-------------------------------------------------------------------------
// Function Name    :GetChild
// Parameter(s)     :CString strName		�ڵ�����
//					:BOOL bBuildIfNotExist	�������򴴽�?
// Return           :���ӽڵ�ָ��
// Memo             :��ȡ����
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNode::GetChild(LPCTSTR pszName, BOOL bBuildIfNotExist /* = FALSE */)
{
	ASSERT(m_pNode != NULL);

	CXmlNodePtr pChild(new CXmlNode());

	try
	{
		MSXML2::IXMLDOMNodePtr pChildNode = NULL;
		pChildNode = m_pNode->selectSingleNode(_bstr_t(pszName));

		// ������ �� ����
		if(pChildNode == NULL && bBuildIfNotExist)
		{
			MSXML2::IXMLDOMDocumentPtr pDoc = NULL;
			HRESULT hr = m_pNode->get_ownerDocument(&pDoc);
			ASSERT(SUCCEEDED(hr));

			pChildNode = pDoc->createElement(_bstr_t(pszName));
			ASSERT(pChildNode != NULL);

			m_pNode->appendChild(pChildNode);

			RELEASE_PTR(pDoc);
		}

		pChild->m_pNode = pChildNode;	
		RELEASE_PTR(pChildNode);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::GetChild�����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}

	return pChild;
}


//-------------------------------------------------------------------------
// Function Name    :NewChild
// Parameter(s)     :CString strName	�������
// Return           :CXmlNodePtr
// Memo             :����һ���ӽ��
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNode::NewChild(LPCTSTR pszName)
{
	ASSERT(m_pNode != NULL);

	CXmlNodePtr pChild(new CXmlNode());

	try
	{
		MSXML2::IXMLDOMDocumentPtr pDoc = NULL;
		HRESULT hr = m_pNode->get_ownerDocument(&pDoc);
		ASSERT(SUCCEEDED(hr));

		MSXML2::IXMLDOMNodePtr pChildNode = NULL;
		pChildNode = pDoc->createElement(_bstr_t(pszName));
		ASSERT(pChildNode != NULL);
		RELEASE_PTR(pDoc);

		m_pNode->appendChild(pChildNode);
		pChild->m_pNode = pChildNode;
		RELEASE_PTR(pChildNode);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::NewChild �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}
	return pChild;	
}


//-------------------------------------------------------------------------
// Function Name    :AddChild
// Parameter(s)     :CXmlNodePtr& pChildNode
// Return           :void
// Memo             :����ӽ��
//-------------------------------------------------------------------------
void CXmlNode::AddChild(CXmlNodePtr& pChildNode)
{
	ASSERT(m_pNode != NULL);

	try
	{
		m_pNode->appendChild(pChildNode->m_pNode);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::AddChild �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}
}


//-------------------------------------------------------------------------
// Function Name    :AddChild
// Parameter(s)     :CXmlNode& refChildNode
// Return           :void
// Memo             :����ӽ��
//-------------------------------------------------------------------------
void CXmlNode::AddChild(CXmlNode& refChildNode)
{
	ASSERT(m_pNode != NULL);

	try
	{
		m_pNode->appendChild(refChildNode.m_pNode);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::AddChild �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}
}


//-------------------------------------------------------------------------
// Function Name    :GetParent
// Parameter(s)     :void
// Return           :CXmlNodePtr
// Memo             :��ȡ�����
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNode::GetParent(void)
{
	ASSERT(m_pNode != NULL);

	CXmlNodePtr pParent(new CXmlNode());

	try
	{
		pParent->m_pNode = m_pNode->GetparentNode();
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::GetParent �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}
	return pParent;
}




//-------------------------------------------------------------------------
// Function Name    :GetName
// Parameter(s)     :void
// Return           :CString
// Memo             :��ȡ��ǰ�ڵ�����
//-------------------------------------------------------------------------
CString CXmlNode::GetName(void)
{
	ASSERT(m_pNode != NULL);

	CString strRet;

	try
	{
		BSTR bstr = NULL;
		HRESULT hr = m_pNode->get_nodeName(&bstr);
		ASSERT(SUCCEEDED(hr));	
		strRet = (LPCTSTR)_bstr_t(bstr, false);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::GetName �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}

	return strRet;
}


//-------------------------------------------------------------------------
// Function Name    :RemoveAttribute
// Parameter(s)     :CString strName IN ��������
// Return           :BOOL
// Memo             :ɾ������
//-------------------------------------------------------------------------
BOOL CXmlNode::RemoveAttribute(LPCTSTR pszName IN)
{
	ASSERT(m_pNode != NULL);

	HRESULT hr = S_OK;

	try
	{
		MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMap = NULL;
		hr = m_pNode->get_attributes(&pIXMLDOMNamedNodeMap);
		ASSERT(SUCCEEDED(hr));

		if(SUCCEEDED(hr))
			pIXMLDOMNamedNodeMap->removeNamedItem(_bstr_t(pszName));
		ASSERT(SUCCEEDED(hr));

		RELEASE_PTR(pIXMLDOMNamedNodeMap);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::RemoveAttribute �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
		return FALSE;
	}


	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------------
// Function Name    :HasChildren
// Parameter(s)     :void
// Return           :BOOL
// Memo             :�Ƿ����ӽ��
//-------------------------------------------------------------------------
BOOL CXmlNode::HasChildren(void)
{
	/*
	ASSERT(m_pNode != NULL);

	BOOL bHasChildren = FALSE;
	try
	{
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	pNodeList =	m_pNode->selectNodes(_bstr_t("child::*"));
	ASSERT(pNodeList != NULL);

	bHasChildren = static_cast<BOOL> (pNodeList->length != 0);
	RELEASE_PTR(pNodeList);
	}
	catch (_com_error e)
	{
	TRACE(_T("CXmlNode::HasChildren �����쳣��%s\n"), e.ErrorMessage());
	ASSERT(FALSE);
	}
	return bHasChildren;
	*/

	ASSERT(m_pNode != NULL);

	BOOL bHasChildren = FALSE;

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	ASSERT(SUCCEEDED(hr));

	for(int i = 0; i < pNodeList->length; i++)
	{
		pNode = pNodeList->item[i];

		hr = pNode->get_nodeType(&NodeType);
		ASSERT(SUCCEEDED(hr));

		RELEASE_PTR(pNode);

		if(NodeType == MSXML2::NODE_ELEMENT)
		{
			bHasChildren = TRUE;
			break;
		}
	}

	RELEASE_PTR(pNodeList);

	return bHasChildren;
}



//-------------------------------------------------------------------------
// Function Name    :Remove
// Parameter(s)     :
// Return           :
// Memo             :���˽���븸������
//-------------------------------------------------------------------------
CXmlNode& CXmlNode::Remove(void)
{
	ASSERT(m_pNode != NULL);

	try
	{
		HRESULT hr = S_OK;
		MSXML2::IXMLDOMNodePtr pNode = NULL;
		hr = m_pNode->get_parentNode(&pNode);
		ASSERT(SUCCEEDED(hr));

		pNode->removeChild(m_pNode);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::Remove �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}
	return (*this);
}

//-------------------------------------------------------------------------
// Function Name    :RemoveChildren
// Parameter(s)     :
// Return           :BOOL
// Memo             :ɾ���ӽ��
//-------------------------------------------------------------------------
BOOL CXmlNode::RemoveChildren(void)
{
	/*
	ASSERT(m_pNode != NULL);

	try
	{
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	pNodeList =	m_pNode->selectNodes(_bstr_t("child::*"));

	for(int i = 0; i < pNodeList->length; i++)
	{
	m_pNode->removeChild(pNodeList->item[i]);
	}

	RELEASE_PTR(pNodeList);
	}
	catch (_com_error e)
	{
	TRACE(_T("CXmlNode::Remove �����쳣��%s\n"), e.ErrorMessage());
	ASSERT(FALSE);
	return FALSE;
	}

	return TRUE;
	*/

	ASSERT(m_pNode != NULL);

	if(!HasChildren())
		return FALSE;

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	ASSERT(SUCCEEDED(hr));

	for(int i = 0; i < pNodeList->length; i++)
	{
		pNode = pNodeList->item[i];

		hr = pNode->get_nodeType(&NodeType);
		ASSERT(SUCCEEDED(hr));

		if(NodeType == MSXML2::NODE_ELEMENT)
		{
			m_pNode->removeChild(pNode);
		}

		RELEASE_PTR(pNode);
	}

	RELEASE_PTR(pNodeList);
	return TRUE;
}


//-------------------------------------------------------------------------
// Function Name    :GetChildren
// Parameter(s)     :void
// Return           :CXmlNodesPtr
// Memo             :��ȡ�ӽ��
//-------------------------------------------------------------------------
CXmlNodesPtr CXmlNode::GetChildren()
{
	/*
	ASSERT(m_pNode != NULL);

	CXmlNodesPtr pNodes (new CXmlNodes());
	try
	{
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	pNodeList =	m_pNode->selectNodes(_bstr_t("child::*"));

	pNodes->m_pNodeList = pNodeList;
	RELEASE_PTR(pNodeList);
	}
	catch (_com_error e)
	{
	TRACE(_T("CXmlNode::GetChildren �����쳣��%s\n"), e.ErrorMessage());
	ASSERT(FALSE);
	}
	return pNodes;
	*/

	ASSERT(m_pNode != NULL);

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	ASSERT(SUCCEEDED(hr));

	CXmlNodesPtr pNodes(new CXmlNodes(pNodeList));

	return pNodes;
}




//-------------------------------------------------------------------------
// Function Name    :SelectSingleNode
// Parameter(s)     :LPCTSTR lpszPath	·��
// Return           :CXmlNodePtr
// Memo             :ѡ�񵥸����
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNode::SelectSingleNode(LPCTSTR lpszPath)
{
	ASSERT(m_pNode != NULL);

	CXmlNodePtr pNode (new CXmlNode());

	try
	{
		MSXML2::IXMLDOMNodePtr pItem = NULL;
		pItem = m_pNode->selectSingleNode(_bstr_t(lpszPath));

		pNode->m_pNode = pItem;
		RELEASE_PTR(pItem);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::SelectSingleNode �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}

	return pNode;
}



//-------------------------------------------------------------------------
// Function Name    :SelectNodes
// Parameter(s)     :LPCTSTR lpszPath	·��
// Return           :CXmlNodesPtr
// Memo             :ѡ���㼯��
//-------------------------------------------------------------------------
CXmlNodesPtr CXmlNode::SelectNodes(LPCTSTR lpszPath)
{
	ASSERT(m_pNode != NULL);

	CXmlNodesPtr pNodes (new CXmlNodes());
	try
	{
		MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
		pNodeList =	m_pNode->selectNodes(_bstr_t(lpszPath));

		pNodes->m_pNodeList = pNodeList;
		RELEASE_PTR(pNodeList);
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNode::SelectNodes �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}

	return pNodes;
}




//////////////////////////////////////////////////////////////////////////
// ����Ϊȡֵ�����ط���

// ȡֵ���ذ汾
CString CXmlNode::GetValue(LPCTSTR lpszValue /* = NULL */)
{
	CString strValue;
	_GetValue(strValue);

	if(strValue.IsEmpty() &&
		lpszValue != NULL)
	{
		strValue = lpszValue;
		_SetValue(strValue);
	}

	return strValue;
}

// ȡֵ���ذ汾
bool CXmlNode::GetValue(bool bDefault)
{
	CString strValue;
	_GetValue(strValue);

	if(strValue.CompareNoCase(_T("1")) == 0)
	{
		return TRUE;
	}
	else if(strValue.CompareNoCase(_T("0")) == 0)
	{
		return FALSE;
	}
	else
	{
		strValue = bDefault ? "1" : "0";
		_SetValue(strValue);
		return bDefault;
	}	
}

// ȡֵ���ذ汾
int CXmlNode::GetValue(int nDefault)
{
	CString strValue;
	_GetValue(strValue);

	if(strValue.IsEmpty())
	{
		strValue.Format(_T("%d"), nDefault);
		_SetValue(strValue);
	}

	return _ttoi(strValue);
}

// ȡֵ���ذ汾
float CXmlNode::GetValue(float fDefault)
{
	CString strValue;
	_GetValue(strValue);

	if(strValue.IsEmpty())
	{
		strValue.Format(_T("%f"), fDefault);
		_SetValue(strValue);
	}

	return static_cast <float> (_tstof(strValue));
}

// ȡֵ���ذ汾
double CXmlNode::GetValue(double dDefault)
{
	CString strValue;
	_GetValue(strValue);

	if(strValue.IsEmpty())
	{
		strValue.Format(_T("%.12f"), dDefault);
		_SetValue(strValue);
	}

	return _tstof(strValue);
}


//////////////////////////////////////////////////////////////////////////
// ����Ϊ��ֵ�����ط���

// ��ֵ���ذ汾
BOOL CXmlNode::SetValue(LPCTSTR lpszValue)
{
	CString strValue(lpszValue);
	return _SetValue(strValue);
}

// ��ֵ���ذ汾
BOOL CXmlNode::SetValue(bool bValue)
{
	CString strValue;
	strValue = bValue ? _T("1") : _T("0");

	return _SetValue(strValue);
}

// ��ֵ���ذ汾
BOOL CXmlNode::SetValue(int nValue)
{
	CString strValue;
	strValue.Format(_T("%d"), nValue);

	return _SetValue(strValue);
}

// ��ֵ���ذ汾
BOOL CXmlNode::SetValue(float fValue)
{
	CString strValue;
	strValue.Format(_T("%f"), fValue);

	return _SetValue(strValue);
}

// ��ֵ���ذ汾
BOOL CXmlNode::SetValue(double dValue)
{
	CString strValue;
	strValue.Format(_T("%.12f"), dValue);

	return _SetValue(strValue);
}

//////////////////////////////////////////////////////////////////////////
// �����ǻ�ȡ���Եĺ���

// ����ȡֵ���ذ汾
CString CXmlNode::GetAttribute(LPCTSTR pszName, LPCTSTR lpszDefault /* = NULL */)
{
	CString strValue;
	_GetAttribute(pszName, strValue);

	if(strValue.IsEmpty() &&
		lpszDefault != NULL)
	{
		strValue = lpszDefault;
		_SetAttribute(pszName, strValue);
	}
	return strValue;
}

// ����ȡֵ���ذ汾
bool CXmlNode::GetAttribute(LPCTSTR pszName, bool bDefault)
{
	CString strValue;
	_GetAttribute(pszName, strValue);

	if(strValue.CompareNoCase(_T("1")) == 0)
	{
		return true;
	}
	else if(strValue.CompareNoCase(_T("0")) == 0)
	{
		return false;
	}
	else
	{
		strValue = bDefault ? _T("1") : _T("0");
		_SetAttribute(pszName, strValue);
		return bDefault;
	}	
}

// ����ȡֵ���ذ汾
int	CXmlNode::GetAttribute(LPCTSTR pszName, int nDefault)
{
	CString strValue;
	_GetAttribute(pszName, strValue);

	if(strValue.IsEmpty())
	{
		strValue.Format(_T("%d"), nDefault);
		_SetAttribute(pszName, strValue);
	}

	return _ttoi(strValue);
}

// ����ȡֵ���ذ汾
float CXmlNode::GetAttribute(LPCTSTR pszName, float fDefault)
{
	CString strValue;
	_GetAttribute(pszName, strValue);

	if(strValue.IsEmpty())
	{
		strValue.Format(_T("%f"), fDefault);
		_SetAttribute(pszName, strValue);
	}

	return static_cast <float> (_tstof(strValue));
}

// ����ȡֵ���ذ汾
double CXmlNode::GetAttribute(LPCTSTR pszName, double dDefault)
{
	CString strValue;
	_GetAttribute(pszName, strValue);

	if(strValue.IsEmpty())
	{
		strValue.Format(_T("%.12f"), dDefault);
		_SetAttribute(pszName, strValue);
	}

	return _tstof(strValue);
}




//////////////////////////////////////////////////////////////////////////
// �������������Եĺ���

// ���Ը�ֵ���ذ汾
BOOL CXmlNode::SetAttribute(LPCTSTR pszName, LPCTSTR lpszValue)
{
	CString strValue;
	strValue = lpszValue;

	return _SetAttribute(pszName, strValue);	
}

// ���Ը�ֵ���ذ汾
BOOL CXmlNode::SetAttribute(LPCTSTR pszName, bool bValue)
{
	CString strValue;
	strValue = bValue ? _T("1") : _T("0");

	return _SetAttribute(pszName, strValue);	
}

// ���Ը�ֵ���ذ汾
BOOL CXmlNode::SetAttribute(LPCTSTR pszName, int nValue)
{
	CString strValue;
	strValue.Format(_T("%d"), nValue);

	return _SetAttribute(pszName, strValue);	
}

// ���Ը�ֵ���ذ汾
BOOL CXmlNode::SetAttribute(LPCTSTR pszName, float fValue)
{
	CString strValue;
	strValue.Format(_T("%f"), fValue);

	return _SetAttribute(pszName, strValue);	
}

// ���Ը�ֵ���ذ汾]
BOOL CXmlNode::SetAttribute(LPCTSTR pszName, double dValue)
{
	CString strValue;
	strValue.Format(_T("%.12f"), dValue);

	return _SetAttribute(pszName, strValue);	
}