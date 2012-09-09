
#pragma once


// CDispSimple ÃüÁîÄ¿±ê

class CDispSimple : public CCmdTarget
{
	DECLARE_DYNCREATE(CDispSimple)

public:
	CDispSimple();
	virtual ~CDispSimple();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CDispSimple)
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	LONG Add(LONG n1, LONG n2);

	enum 
	{
		dispidUpper = 2L,		dispidAdd = 1L
	};
	BSTR Upper(LPCTSTR str);
};


