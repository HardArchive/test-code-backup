

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Jun 24 12:25:32 2011
 */
/* Compiler settings for .\FileTime.idl:
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

#ifndef __FileTime_h__
#define __FileTime_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFileTimeShlExt_FWD_DEFINED__
#define __IFileTimeShlExt_FWD_DEFINED__
typedef interface IFileTimeShlExt IFileTimeShlExt;
#endif 	/* __IFileTimeShlExt_FWD_DEFINED__ */


#ifndef __FileTimeShlExt_FWD_DEFINED__
#define __FileTimeShlExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class FileTimeShlExt FileTimeShlExt;
#else
typedef struct FileTimeShlExt FileTimeShlExt;
#endif /* __cplusplus */

#endif 	/* __FileTimeShlExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IFileTimeShlExt_INTERFACE_DEFINED__
#define __IFileTimeShlExt_INTERFACE_DEFINED__

/* interface IFileTimeShlExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IFileTimeShlExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3FCEF00F-09A4-11D4-8D3B-D12F9D3D8B02")
    IFileTimeShlExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IFileTimeShlExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileTimeShlExt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileTimeShlExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileTimeShlExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFileTimeShlExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFileTimeShlExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFileTimeShlExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFileTimeShlExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IFileTimeShlExtVtbl;

    interface IFileTimeShlExt
    {
        CONST_VTBL struct IFileTimeShlExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileTimeShlExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileTimeShlExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileTimeShlExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileTimeShlExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFileTimeShlExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFileTimeShlExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFileTimeShlExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFileTimeShlExt_INTERFACE_DEFINED__ */



#ifndef __FILETIMELib_LIBRARY_DEFINED__
#define __FILETIMELib_LIBRARY_DEFINED__

/* library FILETIMELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_FILETIMELib;

EXTERN_C const CLSID CLSID_FileTimeShlExt;

#ifdef __cplusplus

class DECLSPEC_UUID("3FCEF010-09A4-11D4-8D3B-D12F9D3D8B02")
FileTimeShlExt;
#endif
#endif /* __FILETIMELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


