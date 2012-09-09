#pragma once

template<class T>
class CProxy_IMDIChildFrameEvents :
	public IConnectionPointImpl<T, &__uuidof(_IMDIChildFrameEvents)>
{
public:
	HRESULT Fire_NotifyCommand(UINT  codeNotify, UINT  cmdID, VARIANT_BOOL *  bHandle)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = codeNotify;
				avarParams[1] = cmdID;
				avarParams[0] = bHandle;	avarParams[0].vt = VT_BOOL|VT_BYREF;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};

