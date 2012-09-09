

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Jun 21 17:06:23 2011
 */
/* Compiler settings for .\OverlayIcon.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __OverlayIcon_h__
#define __OverlayIcon_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMyOverlayIcon_FWD_DEFINED__
#define __IMyOverlayIcon_FWD_DEFINED__
typedef interface IMyOverlayIcon IMyOverlayIcon;
#endif 	/* __IMyOverlayIcon_FWD_DEFINED__ */


#ifndef __MyOverlayIcon_FWD_DEFINED__
#define __MyOverlayIcon_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyOverlayIcon MyOverlayIcon;
#else
typedef struct MyOverlayIcon MyOverlayIcon;
#endif /* __cplusplus */

#endif 	/* __MyOverlayIcon_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IMyOverlayIcon_INTERFACE_DEFINED__
#define __IMyOverlayIcon_INTERFACE_DEFINED__

/* interface IMyOverlayIcon */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMyOverlayIcon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7BCA6879-A9F8-47DE-AE05-F5CE7EA3A474")
    IMyOverlayIcon : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IMyOverlayIconVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMyOverlayIcon * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMyOverlayIcon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMyOverlayIcon * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMyOverlayIcon * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMyOverlayIcon * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMyOverlayIcon * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMyOverlayIcon * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IMyOverlayIconVtbl;

    interface IMyOverlayIcon
    {
        CONST_VTBL struct IMyOverlayIconVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyOverlayIcon_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMyOverlayIcon_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMyOverlayIcon_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMyOverlayIcon_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMyOverlayIcon_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMyOverlayIcon_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMyOverlayIcon_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMyOverlayIcon_INTERFACE_DEFINED__ */



#ifndef __OverlayIconLib_LIBRARY_DEFINED__
#define __OverlayIconLib_LIBRARY_DEFINED__

/* library OverlayIconLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_OverlayIconLib;

EXTERN_C const CLSID CLSID_MyOverlayIcon;

#ifdef __cplusplus

class DECLSPEC_UUID("81539FE6-33C7-4CE7-90C7-1C7B8F2F2D40")
MyOverlayIcon;
#endif
#endif /* __OverlayIconLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


