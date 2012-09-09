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
{	return 1;	}	// 做个假的就可以，因为反正这个对象在程序结束前是不会退出的

ULONG __stdcall CPropertyBag::Release(void)
{	return 0;	}	// 做个假的就可以，因为反正这个对象在程序结束前是不会退出的

STDMETHODIMP CPropertyBag::Read(LPCOLESTR pszPropName,VARIANT *pVar,IErrorLog *pErrorLog)
{
	// 本示例中，该函数会被调用2次
	// 一次是取得属性名为 str 的值
	// 一次是取得属性名为 integer 的值

	ASSERT ( m_pEdit );	// 必须已经调用过 SetEditWnd() 函数
	
	CString buf;
	m_pEdit->GetWindowText( buf );	// 取得编辑窗中的文本

	CStringArray saPropertyLine;
	while( TRUE )					// 按回车换行拆分
	{
		int id = buf.Find( _T("\r\n") );
		if( -1 == id )	// 没有回车换行了
		{
			saPropertyLine.Add( buf );
			break;		// 跳出循环
		}
		saPropertyLine.Add( buf.Left( id ) );
		buf = buf.Mid( id+2 );
	}

	USES_CONVERSION;
	CString sPropName(W2CT( pszPropName ) );	// 属性名 转换为 T 类型的字符串
	sPropName += _T("=");				// 转换为 xxx=的形式，下面准备进行比较
	int nPropNameLen = sPropName.GetLength();	// 属性名 的长度

	INT_PTR nCountLine = saPropertyLine.GetSize();	// 总行数
	for( INT_PTR nLine = 0; nLine < nCountLine; nLine ++ )
	{	// 逐行寻找属性名称
		CString sLine = saPropertyLine.GetAt( nLine );	// 取出该行内容
		if( 0 == sLine.Left( nPropNameLen ).CompareNoCase( sPropName ) )
		{	// 找到了该属性行
			sLine = sLine.Mid( nPropNameLen );		// 取得 = 号右边的串
			switch( pVar->vt )
			{
			case VT_BSTR:	// 如果是 BSTR 类型
				pVar->bstrVal = sLine.AllocSysString();
				break;
			case VT_I4:		// 如果是 I4 类型
				pVar->lVal = ::_ttoi( (LPCTSTR)sLine );
				break;
			default:			// 其它类型。因为本示例只有以上2个类型
				ASSERT( FALSE);	// 所以没有再判断处理。真正的程序需要补充完整
				break;
			}
			return S_OK;
		}
	}
	return E_FAIL;	// 没有找到
}

STDMETHODIMP CPropertyBag::Write(LPCOLESTR pszPropName,VARIANT *pVar)
{
	ASSERT ( m_pEdit );	// 必须已经调用过 SetEditWnd() 函数

	USES_CONVERSION;
	m_pEdit->ReplaceSel( W2CT( pszPropName ) );	// 输出属性名称
	m_pEdit->ReplaceSel( _T("=") );				// 输出 =

	CComVariant v( *pVar );						// 为了方便输出
	v.ChangeType( VT_BSTR );					// 转换为字符串类型
	m_pEdit->ReplaceSel( W2CT( v.bstrVal ) );	// 输出属性内容
	m_pEdit->ReplaceSel( _T("\r\n") );			// 输出回车换行

	return S_OK;
}

void CPropertyBag::SetEditWnd(CEdit * pEdit)
{
	m_pEdit = pEdit;	// 保存输出窗口的对象指针，为将来提取或输出内容做准备
}
