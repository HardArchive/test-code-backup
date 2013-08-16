

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Aug 16 16:48:08 2013
 */
/* Compiler settings for .\Simple_ATL.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef __Simple_ATL_i_h__
#define __Simple_ATL_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFirst_ATL_FWD_DEFINED__
#define __IFirst_ATL_FWD_DEFINED__
typedef interface IFirst_ATL IFirst_ATL;
#endif 	/* __IFirst_ATL_FWD_DEFINED__ */


#ifndef __First_ATL_FWD_DEFINED__
#define __First_ATL_FWD_DEFINED__

#ifdef __cplusplus
typedef class First_ATL First_ATL;
#else
typedef struct First_ATL First_ATL;
#endif /* __cplusplus */

#endif 	/* __First_ATL_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IFirst_ATL_INTERFACE_DEFINED__
#define __IFirst_ATL_INTERFACE_DEFINED__

/* interface IFirst_ATL */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IFirst_ATL;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8048195A-F56F-4085-9EB5-3606567C126D")
    IFirst_ATL : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IFirst_ATLVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFirst_ATL * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFirst_ATL * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFirst_ATL * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFirst_ATL * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFirst_ATL * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFirst_ATL * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFirst_ATL * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IFirst_ATLVtbl;

    interface IFirst_ATL
    {
        CONST_VTBL struct IFirst_ATLVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFirst_ATL_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFirst_ATL_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFirst_ATL_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFirst_ATL_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFirst_ATL_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFirst_ATL_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFirst_ATL_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFirst_ATL_INTERFACE_DEFINED__ */



#ifndef __Simple_ATLLib_LIBRARY_DEFINED__
#define __Simple_ATLLib_LIBRARY_DEFINED__

/* library Simple_ATLLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Simple_ATLLib;

EXTERN_C const CLSID CLSID_First_ATL;

#ifdef __cplusplus

class DECLSPEC_UUID("48CB9DA1-D7E3-4527-B3CF-2706A0768A28")
First_ATL;
#endif
#endif /* __Simple_ATLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


