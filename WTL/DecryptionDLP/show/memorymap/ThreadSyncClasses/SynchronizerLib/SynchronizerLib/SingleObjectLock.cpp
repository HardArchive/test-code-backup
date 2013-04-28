#include "stdafx.h"
#include "StdAfx.h"
#include "Synchronizer/SingleObjectLock.h"
#include "Synchronizer/SynchronizerBase.h"

SingleObjectLock::SingleObjectLock(SynchronizerBase* pSyncBase,
                                   DWORD dwTimeout):
                                   m_pSyncBase(pSyncBase)
{
    if( m_pSyncBase )
    {
        m_pSyncBase->Lock(dwTimeout);
    }
}


SingleObjectLock::~SingleObjectLock()
{
    if( m_pSyncBase )
    {
        m_pSyncBase->UnLock();
    }
}
