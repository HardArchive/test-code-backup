// dllmain.h : 模块类的声明。

class CPiSvrModule : public CAtlDllModuleT< CPiSvrModule >
{
public :
	DECLARE_LIBID(LIBID_PiSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PISVR, "{1E61681E-4C70-4750-94DE-7815CBB2F7C5}")
};

extern class CPiSvrModule _AtlModule;
