// dllmain.h : 模块类的声明。

class CSimpleExtModule : public CAtlDllModuleT< CSimpleExtModule >
{
public :
	DECLARE_LIBID(LIBID_SimpleExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIMPLEEXT, "{85420C45-6C10-4924-905B-4BB62C1A6EE8}")
};

extern class CSimpleExtModule _AtlModule;
