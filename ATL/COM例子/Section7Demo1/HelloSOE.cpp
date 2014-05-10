// HelloSOE.cpp : CHelloSOE µÄÊµÏÖ

#include "stdafx.h"
#include "HelloSOE.h"


// CHelloSOE


STDMETHODIMP CHelloSOE::Add(LONG lA, LONG lB)
{
	LONG lResult = lA + lB;

	Fire_OnAddResult(lResult);

	return S_OK;
}
