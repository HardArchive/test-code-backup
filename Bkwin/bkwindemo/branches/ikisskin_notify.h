///////////////////////////////////////////////////////////////////
//
//	filename: 	E:\duba\kisengine\branches\kis_2011_sp4_fb\publish\kisgui\ikisskin_notify.h
//	creator:	刘飞
//	date:		23-6-2011   9:59:44
//	created:	2011/06/23
//	comment:	写几个类，方便换肤支持。贝壳的窗口可以直接从KSkinObserverHelper进行继承实现换肤.
//
///////////////////////////////////////////////////////////////////
#ifndef _IKISSK_NOTIFY_H_
#define _IKISSK_NOTIFY_H_
#include <vector>
#include <framework/KLocker.h>
class ISkinObserver
{
public:
	virtual void NotifyChange() = 0;
};

class KSkinNotify
{
public:
	KSkinNotify()
	{

	}

	virtual ~KSkinNotify()
	{
		m_Observers.clear();
	}

	void AddObserver(ISkinObserver *pObserver)
	{
		KLocker lock(m_cs);
		m_Observers.push_back(pObserver);
	}

	void RemoveObserver(ISkinObserver *pObserver)
	{
		KLocker lock(m_cs);
		KObserverList::iterator iter = m_Observers.begin();
		while (iter != m_Observers.end())
		{
			if ((*iter) == pObserver)
			{
				m_Observers.erase(iter);
				return;
			}
			iter++;
		}
	}

	void NotifyChange()
	{
		KLocker lock(m_cs);
		KObserverList::iterator iter = m_Observers.begin();
		while (iter != m_Observers.end())
		{
			(*iter)->NotifyChange();
			iter++;
		}
	}

	static KSkinNotify& CreateInstance()
	{
		static KSkinNotify _obj;
		return _obj;
	}
private:
	typedef std::vector<ISkinObserver*> KObserverList;
	KObserverList m_Observers;
	KCriticalSesion m_cs;
};

template <typename T>
class KSkinObserverHelper : public ISkinObserver
{
public:
	KSkinObserverHelper()
	{
		KSkinNotify::CreateInstance().AddObserver(this);
	}
	virtual ~KSkinObserverHelper()
	{
		KSkinNotify::CreateInstance().RemoveObserver(this);
	}

	virtual void NotifyChange()
	{
		T *pThis = static_cast<T*>(this);
		if (pThis->IsWindow())
		{
			pThis->RedrawView();
		}
	}

};
#endif