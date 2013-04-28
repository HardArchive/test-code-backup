#pragma once

class CShareRestrictedSD
{
public:
	CShareRestrictedSD();
	virtual ~CShareRestrictedSD();
	SECURITY_ATTRIBUTES* GetSA();
protected:
	PVOID  ptr;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

};
