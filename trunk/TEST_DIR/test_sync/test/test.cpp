// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include "Mutex.h"
//#include "Event.h"
//#include "CriticalSection.h"

#include <map>
#include <queue>
using namespace std;
#include "SyncUtil.h"
using namespace RG;
int _tmain(int argc, _TCHAR* argv[])
{
	map<string, DWORD> mapBarcodeID;	
	mapLockBarcodeID m_mapLockBarcodeID(mapBarcodeID);
	m_mapLockBarcodeID.Lock();
	m_mapLockBarcodeID.m_pObject.size();

	m_mapLockBarcodeID.UnLock();
	return 0;
}

