#include "StdAfx.h"
#include ".\propertybag.h"
#include <atlbase.h>
#include <atlconv.h>

CPropertyBag::CPropertyBag(void)
: m_pEdit(NULL)
{
}

CPropertyBag::~CPropertyBag(void)
{
}

STDMETHODIMP CPropertyBag::QueryInterface(const struct _GUID &iid,void ** ppv)
{
	*ppv=this;
	return S_OK;
}

ULONG __stdcall CPropertyBag::AddRef(void)
{	return 1;	}	// �����ٵľͿ��ԣ���Ϊ������������ڳ������ǰ�ǲ����˳���

ULONG __stdcall CPropertyBag::Release(void)
{	return 0;	}	// �����ٵľͿ��ԣ���Ϊ������������ڳ������ǰ�ǲ����˳���

STDMETHODIMP CPropertyBag::Read(LPCOLESTR pszPropName,VARIANT *pVar,IErrorLog *pErrorLog)
{
	// ��ʾ���У��ú����ᱻ����2��
	// һ����ȡ��������Ϊ str ��ֵ
	// һ����ȡ��������Ϊ integer ��ֵ

	ASSERT ( m_pEdit );	// �����Ѿ����ù� SetEditWnd() ����
	
	CString buf;
	m_pEdit->GetWindowText( buf );	// ȡ�ñ༭���е��ı�

	CStringArray saPropertyLine;
	while( TRUE )					// ���س����в��
	{
		int id = buf.Find( _T("\r\n") );
		if( -1 == id )	// û�лس�������
		{
			saPropertyLine.Add( buf );
			break;		// ����ѭ��
		}
		saPropertyLine.Add( buf.Left( id ) );
		buf = buf.Mid( id+2 );
	}

	USES_CONVERSION;
	CString sPropName(W2CT( pszPropName ) );	// ������ ת��Ϊ T ���͵��ַ���
	sPropName += _T("=");				// ת��Ϊ xxx=����ʽ������׼�����бȽ�
	int nPropNameLen = sPropName.GetLength();	// ������ �ĳ���

	INT_PTR nCountLine = saPropertyLine.GetSize();	// ������
	for( INT_PTR nLine = 0; nLine < nCountLine; nLine ++ )
	{	// ����Ѱ����������
		CString sLine = saPropertyLine.GetAt( nLine );	// ȡ����������
		if( 0 == sLine.Left( nPropNameLen ).CompareNoCase( sPropName ) )
		{	// �ҵ��˸�������
			sLine = sLine.Mid( nPropNameLen );		// ȡ�� = ���ұߵĴ�
			switch( pVar->vt )
			{
			case VT_BSTR:	// ����� BSTR ����
				pVar->bstrVal = sLine.AllocSysString();
				break;
			case VT_I4:		// ����� I4 ����
				pVar->lVal = ::_ttoi( (LPCTSTR)sLine );
				break;
			default:			// �������͡���Ϊ��ʾ��ֻ������2������
				ASSERT( FALSE);	// ����û�����жϴ��������ĳ�����Ҫ��������
				break;
			}
			return S_OK;
		}
	}
	return E_FAIL;	// û���ҵ�
}

STDMETHODIMP CPropertyBag::Write(LPCOLESTR pszPropName,VARIANT *pVar)
{
	ASSERT ( m_pEdit );	// �����Ѿ����ù� SetEditWnd() ����

	USES_CONVERSION;
	m_pEdit->ReplaceSel( W2CT( pszPropName ) );	// �����������
	m_pEdit->ReplaceSel( _T("=") );				// ��� =

	CComVariant v( *pVar );						// Ϊ�˷������
	v.ChangeType( VT_BSTR );					// ת��Ϊ�ַ�������
	m_pEdit->ReplaceSel( W2CT( v.bstrVal ) );	// �����������
	m_pEdit->ReplaceSel( _T("\r\n") );			// ����س�����

	return S_OK;
}

void CPropertyBag::SetEditWnd(CEdit * pEdit)
{
	m_pEdit = pEdit;	// ����������ڵĶ���ָ�룬Ϊ������ȡ�����������׼��
}
