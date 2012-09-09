#ifndef __BaseObject___H
#define __BaseObject___H

extern IDispatch *g_retMDIFrame;


#define __COMMDIFrame \
	STDMETHOD(get_COMMDIFrame)(IDispatch** pVal)	\
	{	\
		ATLASSERT(NULL != g_retMDIFrame);\
		if (NULL != g_retMDIFrame)	\
		{	\
			*pVal = (IDispatch*)g_retMDIFrame;	\
			return S_OK;	\
		}	\
				\
		return S_FALSE;	\
	}



template<class _TInterface>
class CCopyVariantLong
{
public:
	static void init(VARIANT *p)
	{
		VariantInit(p);
	}
	static HRESULT copy(VARIANT *pTo, _TInterface **pFrom)
	{
		pTo->vt			= VT_DISPATCH;
		pTo->ppdispVal	= (IDispatch**)pFrom;
		return S_OK;
	}
	static void destroy(VARIANT *p)
	{
		VariantClear(p);
	}
};




#endif	//__BaseObject___H