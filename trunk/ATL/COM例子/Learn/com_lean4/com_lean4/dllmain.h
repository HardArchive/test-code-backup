// dllmain.h : Declaration of module class.

class Ccom_lean4Module : public CAtlDllModuleT< Ccom_lean4Module >
{
public :
	DECLARE_LIBID(LIBID_com_lean4Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COM_LEAN4, "{FFF79940-1871-4607-8FCA-47E41D4C7DD6}")
};

extern class Ccom_lean4Module _AtlModule;
