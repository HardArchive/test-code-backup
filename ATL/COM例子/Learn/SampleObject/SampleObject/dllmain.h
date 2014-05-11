// dllmain.h : Declaration of module class.

class CSampleObjectModule : public CAtlDllModuleT< CSampleObjectModule >
{
public :
	DECLARE_LIBID(LIBID_SampleObjectLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SAMPLEOBJECT, "{4D7FDA86-C1E8-4912-A23C-9A8B028E2B06}")
};

extern class CSampleObjectModule _AtlModule;
