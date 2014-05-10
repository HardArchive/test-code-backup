// Section1Demo2.cpp : �������̨Ӧ�ó������ڵ㡣
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

		AddRef();		//����ڶ����ὲ

		return S_OK;	//����S_OK
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		//��ʱ��ʵ�֣���ʱ����0
		return 0;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		//��ʱ��ʵ�֣���ʱ����0
		return 0;
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
public:
	long m_lAA;
	long m_lAB;
	long m_lAC;

};


int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;

	//�������
	CA *pA = new CA();

	//�������ѯIUnknown�ӿ�
	IUnknown *pIUnknown = NULL;
	hr = pA->QueryInterface(IID_IUnknown, (void**)&pIUnknown);
	if (SUCCEEDED(hr))		//��HRESULT����ֵ���жϣ�һ�����SUCCEEDED
	{
		//��IUnknown��ѯIX�ӿ�
		IX *pIX = NULL;
		hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
		if (SUCCEEDED(hr))
		{
			//����IX�ӿڵķ���
			pIX->Fx1();
			pIX->Fx2();
		}

		//��IUnknown��ѯIY�ӿ�
		IY *pIY = NULL;
		hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);
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

		if ((void*)pIUnknown == (void*)pIX)
		{
			cout << "pIUnknown == pIX" <<endl;
		}

		//��IX��ѯIY
		IY *pIY2 = NULL;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY2);
		if (SUCCEEDED(hr))
		{
			pIY2->Fy1();
			pIY2->Fy2();
		}

		//��IXҲ���Բ�ѯ��IUnknown
		//��IYҲ���Բ�ѯ��IX
		//��IYҲ���Բ�ѯ��IUnknown
		//��CAҲ���Բ�ѯ��IX
		//��CAҲ���Բ�ѯ��IY
		//�ܽ᣺
			//ֻҪ��CA���̳еĽӿڣ���CA������ﶼ���Բ�ѯ����
			//ֻҪ��CA���̳еĽӿڣ���CA���̳е������ӿ��ﶼ���Բ�ѯ����
	}

	//�ͷ����
	delete pA;

	return 0;
}

