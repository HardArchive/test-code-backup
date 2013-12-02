/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 3.0.2
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "stdafx.h"
#include "TcpPullServer.h"

const DWORD CTcpPullServer::DEFAULT_FREE_PULL_BUFFER_POOL = DEFAULT_FREE_SOCKETOBJ_POOL;
const DWORD CTcpPullServer::DEFAULT_FREE_PULL_BUFFER_HOLD = DEFAULT_FREE_SOCKETOBJ_HOLD;

ISocketListener::EnHandleResult CTcpPullServer::FireAccept(CONNID dwConnID, SOCKET soClient)
{
	ISocketListener::EnHandleResult result = __super::FireAccept(dwConnID, soClient);

	if(result != ISocketListener::HR_ERROR)
		AddPullBufferMap(dwConnID);

	return result;
}

ISocketListener::EnHandleResult CTcpPullServer::FireReceive(CONNID dwConnID, const BYTE* pData, int iLength)
{
	TPullBuffer* pPullBuffer = FindPullBuffer(dwConnID);

	if(pPullBuffer != nullptr)
	{
		if(pPullBuffer->connID != 0)
		{
			BOOL bOK = FALSE;
			int iTotalLength = 0;

			{
				CCriSecLock locallock(pPullBuffer->cs);

				if(pPullBuffer->connID != 0)
				{
					pPullBuffer->buffers->push_back(new CBufferPtr(pData, iLength));
					pPullBuffer->length += iLength;
					iTotalLength = pPullBuffer->length;

					bOK = TRUE;
				}
			}

			if(bOK) return __super::FireReceive(dwConnID, iTotalLength);
		}
	}

	return ISocketListener::HR_IGNORE;
}

ISocketListener::EnHandleResult CTcpPullServer::FireClose(CONNID dwConnID)
{
	ISocketListener::EnHandleResult result = __super::FireClose(dwConnID);

	AddFreePullBuffer(dwConnID);

	return result;
}

ISocketListener::EnHandleResult CTcpPullServer::FireError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	ISocketListener::EnHandleResult result = __super::FireError(dwConnID, enOperation, iErrorCode);

	AddFreePullBuffer(dwConnID);

	return result;
}

ISocketListener::EnHandleResult CTcpPullServer::FireServerShutdown()
{
	ISocketListener::EnHandleResult result = __super::FireServerShutdown();

	::ClearPtrMap(m_mpPullBuffer);
	::ClearPtrSet(m_lsFreePullBuffer);

	return ISocketListener::HR_OK;
}

IPullSocket::EnFetchResult CTcpPullServer::Fetch(CONNID dwConnID, BYTE* pBuffer, int iLength)
{
	ASSERT(pBuffer != nullptr && iLength > 0);

	IPullSocket::EnFetchResult result	= IPullSocket::FR_DATA_NOT_FOUND;
	TPullBuffer* pPullBuffer			= FindPullBuffer(dwConnID);

	if(pPullBuffer != nullptr)
	{
		if(pPullBuffer->connID != 0)
		{
			CCriSecLock locallock(pPullBuffer->cs);

			if(pPullBuffer->connID != 0)
			{
				if(pPullBuffer->length >= iLength)
				{
					DoFetch(pPullBuffer->buffers, pBuffer, iLength);
					pPullBuffer->length -= iLength;

					result = IPullSocket::FR_OK;
				}
				else
					result = IPullSocket::FR_LENGTH_TOO_LONG;
			}
		}
	}

	return result;
}

void CTcpPullServer::DoFetch(CAutoBufferPtrList& buffers, BYTE* pBuffer, int iLength)
{
	int iIndex	= 0;
	int iRemain	= iLength;

	while(iRemain > 0)
	{
		CBufferPtr* pItem = buffers->front();
		int iItemSize = (int)pItem->Size();
		int iCopySize = min(iItemSize, iRemain);

		memcpy(pBuffer + iIndex, pItem->Ptr(), iCopySize);

		if(iItemSize > iRemain)
		{
			int iLeft = iItemSize - iCopySize;
			memcpy(pItem->Ptr(), pItem->Ptr() + iCopySize, iLeft);
			pItem->Realloc(iLeft);
		}
		else
		{
			buffers->pop_front();
			delete pItem;
		}

		iRemain	-= iCopySize;
		iIndex	+= iCopySize;
	}
}

TPullBuffer* CTcpPullServer::GetFreePullBuffer(CONNID dwConnID)
{
	ASSERT( dwConnID != 0);

	TPullBuffer* pPullBuffer = nullptr;

	if(m_lsFreePullBuffer.size() > 0)
	{
		CCriSecLock locallock(m_csFreePullBuffer);

		if(m_lsFreePullBuffer.size() > 0)
		{
			pPullBuffer = m_lsFreePullBuffer.front();
			m_lsFreePullBuffer.pop_front();
		}
	}

	if(pPullBuffer)	pPullBuffer->connID = dwConnID;
	else			pPullBuffer = new TPullBuffer(dwConnID);

	ASSERT(pPullBuffer);
	return pPullBuffer;
}

void CTcpPullServer::AddFreePullBuffer(CONNID dwConnID)
{
	TPullBuffer* pPullBuffer = FindPullBuffer(dwConnID);

	if(pPullBuffer != nullptr)
	{
		{
			CReentrantWriteLock locallock(m_csPullBufferMap);
			m_mpPullBuffer.erase(dwConnID);
		}

		AddFreePullBuffer(pPullBuffer);
	}
}

void CTcpPullServer::AddFreePullBuffer(TPullBuffer* pPullBuffer)
{
	if(pPullBuffer->connID != 0)
	{
		BOOL bOK = FALSE;

		{
			CCriSecLock locallock(pPullBuffer->cs);

			if(pPullBuffer->connID != 0)
			{
				pPullBuffer->Reset();
				bOK = TRUE;
			}
		}

		if(bOK)
		{
			{
				CCriSecLock locallock(m_csFreePullBuffer);
				m_lsFreePullBuffer.push_back(pPullBuffer);
			}

			if(m_lsFreePullBuffer.size() > m_dwFreePullBufferHold)
				CompressFreePullBuffer(m_dwFreePullBufferPool);
		}
	}
}

void CTcpPullServer::CompressFreePullBuffer(size_t size)
{
	CCriSecLock locallock(m_csFreePullBuffer);

	while(m_lsFreePullBuffer.size() > size)
	{
		TPullBuffer* pPullBuffer = m_lsFreePullBuffer.front();
		m_lsFreePullBuffer.pop_front();

		delete pPullBuffer;
	}
}

void CTcpPullServer::AddPullBufferMap(CONNID dwConnID)
{
	CReentrantWriteLock locallock(m_csPullBufferMap);
	m_mpPullBuffer[dwConnID] = GetFreePullBuffer(dwConnID);
}

TPullBuffer* CTcpPullServer::FindPullBuffer(CONNID dwConnID)
{
	TPullBuffer* pPullBuffer = nullptr;

	CReentrantReadLock locallock(m_csPullBufferMap);

	CAutoPullBufferPtrMapCI it = m_mpPullBuffer.find(dwConnID);
	if(it != m_mpPullBuffer.end())
		pPullBuffer = it->second;

	return pPullBuffer;
}
