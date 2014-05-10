// Section2Demo1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <Unknwn.h>

using namespace std;

// {A348FBDD-E765-4b41-8477-6D8B7038FCC6}
static const IID IID_IX = 
{ 0xa348fbdd, 0xe765, 0x4b41, { 0x84, 0x77, 0x6d, 0x8b, 0x70, 0x38, 0xfc, 0xc6 } };

// {10A90ED2-FCDE-4067-92DA-ABA38F5C1B12}
static const IID IID_IY = 
{ 0x10a90ed2, 0xfcde, 0x4067, { 0x92, 0xda, 0xab, 0xa3, 0x8f, 0x5c, 0x1b, 0x12 } };


//�ӿ�IX
interface IX : public IUnknown
{	
	virtual void Fx1() = 0;
	virtual void Fx2() = 0;
};

//�ӿ�IY
interface IY : public IUnknown
{	
	virtual void Fy1() = 0;
	virtual void Fy2() = 0;
};

//���CA
class CA: public IX, public IY
{
//����������
public:
	CA()
	{
		m_lCount = 0;

		//����ʱ����Ҫ�������ü���
		AddRef();
	}
	virtual ~CA()		//��������һ������麯��
	{
		cout << "�ұ��ͷ�����" << endl;
	}

//ʵ��
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppv)
	{
		//�鿴PPT��CA���ڴ�ṹ�������µ�ת������

		if (iid == IID_IUnknown)
		{
			//��ʹCA�̳�������IUnknown�ӿڣ�����һ��������IX����һ��������IY������һ�㷵�ص�һ�����̳е�IX�ӿڡ�
			*ppv = static_cast<IX*>(this);		
		}
		else if (iid == IID_IX)
		{
			//����IX�ӿ�
			*ppv = static_cast<IX*>(this);		
		}
		else if (iid == IID_IY)
		{
			//����IY�ӿ�
			*ppv = static_cast<IY*>(this);
		}
		else
		{
			//��ѯ����IID��*ppv����NULL��
			*ppv = NULL;
			return E_NOINTERFACE;	//��������ֵ����E_NOINTERFACE����ʾ�����֧��iid�Ľӿڡ�
		}

		//��ѯ�ɹ�ʱ����Ҫ�������ü���
		AddRef();		

		return S_OK;	//����S_OK
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		//��ʵ�ַ���
		return ++m_lCount;

		//���̱߳�̲������·��������ַ���ȷ��ͬһ��ʱ��ֻ����һ���߳������ʳ�Ա����
		//return InterlockedIncrement(&m_lCount);
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		//��ʵ�ַ���
		if (--m_lCount == 0)
		{
			delete this;	//�����Լ�
			return 0;
		}
		return m_lCount;

		////���̱߳�̲������·��������ַ���ȷ��ͬһ��ʱ��ֻ����һ���߳������ʳ�Ա����
		//if (InterlockedDecrement(&m_lCount) == 0)
		//{
		//	delete this;		//�����Լ�
		//	return 0;
		//}
		//return m_lCount;
	}

	virtual void Fx1()
	{
		cout << "Fx1" << endl;
	}

	virtual void Fx2()
	{
		cout << "Fx2" << endl;
	}

	virtual void Fy1()
	{
		cout << "Fy1" << endl;
	}

	virtual void Fy2()
	{
		cout << "Fy2" << endl;
	}

//����
private:
	long m_lCount;		//���ü������ü���ֻ�����������粻�ɷ��ʣ�����Ȩ������Ϊprivate

};


int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;

	CA *pA = new CA();		//���ü���1

	//�������ѯIUnknown�ӿ�
	IUnknown *pIUnknown = NULL;
	hr = pA->QueryInterface(IID_IUnknown, (void**)&pIUnknown);		//���ü���2
	if (SUCCEEDED(hr))		//��HRESULT����ֵ���жϣ�һ�����SUCCEEDED
	{
		pA->Release();	//pA����ʹ�ã����ü���1
		pA = NULL;		//��ֹ�ٲ�С��ʹ��m_pA

		//��IUnknown��ѯIX�ӿ�
		IX *pIX = NULL;
		hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);	//���ü���2
		if (SUCCEEDED(hr))
		{
			//����IX�ӿڵķ���
			pIX->Fx1();
			pIX->Fx2();
		}

		//��IUnknown��ѯIY�ӿ�
		IY *pIY = NULL;
		hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);	//���ü���3
		if (SUCCEEDED(hr))
		{
			//����IY�ӿڵķ���
			pIY->Fy1();
			pIY->Fy1();
		}

		if ((void*)pIX != (void*)pIY)
		{
			cout << "pIX != pIY" <<endl;
		}

		if ((void*)pIUnknown != (void*)pIY)
		{
			cout << "pIUnknown != pIY" <<endl;
		}

		pIY->Release();		//pIY����ʹ�ã����ü���2
		pIY = NULL;

		if ((void*)pIUnknown == (void*)pIX)
		{
			cout << "pIUnknown == pIX" <<endl;
		}

		//��IX��ѯIY
		IY *pIY2 = NULL;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY2);		//���ü��������ü���3

		pIX->Release();		//pIX����ʹ�ã����ü���2
		pIX = NULL;

		if (SUCCEEDED(hr))
		{
			pIY2->Fy1();
			pIY2->Fy2();
		}

		pIY2->Release();	//pIY����ʹ�ã����ü���1
		pIY2 = NULL;

	}

	//Ŀǰ���ü���Ϊ1����ΪpIUnknown����ʹ�á�

	IX *pIX2 = NULL;
	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX2);		//���ü���Ϊ2
	if (SUCCEEDED(hr))
	{
		IX *pIX3 = NULL;
		pIX3 = pIX2;		//ִ���˸�ֵ
		pIX3->AddRef();		//�����Ͼ�ִ���˸�ֵ���������ü�����Ҫ���������ü���Ϊ3

		pIX3->Fx1();
		pIX3->Fx2();

		pIX3->Release();		//pIX3����ʹ�ã����ü���Ϊ2
		pIX3 = NULL;
	}
	pIX2->Release();		//pIX2����ʹ�ã����ü���Ϊ1
	pIX2 = NULL;

	pIUnknown->Release();		//pIUnknown����ʹ�ã����ü���Ϊ0��Release������ִ����delete this������������ڴ���Դ
	pIUnknown = NULL;

	//�ͷ����? no!
	//delete pA;		//������Ҫдdelete����


	return 0;
}

