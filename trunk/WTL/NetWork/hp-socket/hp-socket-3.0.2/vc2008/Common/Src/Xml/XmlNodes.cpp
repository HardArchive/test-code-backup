#include "StdAfx.h"
#include ".\xmlnodes.h"


// �����ǹ��캯�� �� ��������
CXmlNodes::CXmlNodes(MSXML2::IXMLDOMNodeListPtr pNodeList)
{
	m_pNodeList = pNodeList;
}

CXmlNodes::CXmlNodes(CXmlNodes& refNodes)
{
	m_pNodeList = refNodes.m_pNodeList;
}

CXmlNodes::CXmlNodes(void)
: m_pNodeList(NULL)
{
}

CXmlNodes::~CXmlNodes(void)
{
	Release();
}

//////////////////////////////////////////////////////////////////////////
void CXmlNodes::Release(void)
{
	RELEASE_PTR(m_pNodeList);
}


//-------------------------------------------------------------------------
// Function Name    :operator =
// Parameter(s)     :CXmlNodesPtr pNodes
// Return           :CXmlNodesPtr
// Memo             :���� = ��
//-------------------------------------------------------------------------
CXmlNodesPtr CXmlNodes::operator = (CXmlNodesPtr pNodes IN)
{
	RELEASE_PTR(m_pNodeList);

	m_pNodeList = pNodes->m_pNodeList;
	return pNodes;
}


//-------------------------------------------------------------------------
// Function Name    :operator =
// Parameter(s)     :CXmlNodes& refNodes
// Return           :CXmlNodes&
// Memo             :���� = ��
//-------------------------------------------------------------------------
CXmlNodes& CXmlNodes::operator = (CXmlNodes& refNodes)
{
	RELEASE_PTR(m_pNodeList);

	m_pNodeList = refNodes.m_pNodeList;
	return (*this);
}



//-------------------------------------------------------------------------
// Function Name    :GetCount
// Parameter(s)     :void
// Return           :LONG
// Memo             :��ȡ����
//-------------------------------------------------------------------------
LONG CXmlNodes::GetCount(void)
{
	ASSERT(m_pNodeList != NULL);

	try
	{
		return m_pNodeList->length;
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNodes::GetCount �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
		return -1;
	}
}



//-------------------------------------------------------------------------
// Function Name    :GetItem
// Parameter(s)     :LONG nIndex	����
// Return           :CXmlNodePtr
// Memo             :��ȡ�����еĽ��
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNodes::GetItem(LONG nIndex)
{
	ASSERT(m_pNodeList != NULL);
	ASSERT(nIndex >= 0 && nIndex < GetCount());

	CXmlNodePtr pNode (new CXmlNode(m_pNodeList->item[nIndex]));

	return pNode;
}


//-------------------------------------------------------------------------
// Function Name    :operator[]
// Parameter(s)     :INT nIndex
// Return           :CXmlNodePtr
// Memo             :����ͬ GetItem(LONG nIndex)
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNodes::operator[] (LONG lIndex)
{
	return GetItem(lIndex);
}


//-------------------------------------------------------------------------
// Function Name    :GetItem
// Parameter(s)     :LPCTSTR lpszName	�������
// Return           :CXmlNodePtr
// Memo             :���������ڽ�㼯�в���
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNodes::GetItem(LPCTSTR lpszName)
{
	ASSERT(m_pNodeList != NULL);
	ASSERT(lpszName != NULL);
	
	CXmlNodePtr pNode (new CXmlNode());

	try
	{
		HRESULT hr = S_OK;
		CString strName;
		BSTR bstr = NULL;
		MSXML2::IXMLDOMNodePtr pItem = NULL;	

		for(int i = 0; i < m_pNodeList->length; i++)
		{
			pItem = m_pNodeList->item[i];

			hr = pItem->get_nodeName(&bstr);		

			strName = (LPCTSTR)_bstr_t(bstr, false);

			// ���ҵ�һ����ȵ�
			if(strName.Compare(lpszName) == 0)
			{
				pNode->m_pNode = pItem;
				RELEASE_PTR(pItem);
				return pNode;
			}

			RELEASE_PTR(pItem);
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("CXmlNodes::GetItem �����쳣��%s\n"), e.ErrorMessage());
		ASSERT(FALSE);
	}

	return pNode;
}


//-------------------------------------------------------------------------
// Function Name    :operator[]
// Parameter(s)     :LPCTSTR lpszName	�������
// Return           :CXmlNodePtr
// Memo             :����ͬ GetItem(LPCTSTR lpszName)
//-------------------------------------------------------------------------
CXmlNodePtr CXmlNodes::operator[] (LPCTSTR lpszName)
{
	return GetItem(lpszName);
}

