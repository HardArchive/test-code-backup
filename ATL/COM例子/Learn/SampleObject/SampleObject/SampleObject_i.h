

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sun May 11 18:33:28 2014
 */
/* Compiler settings for .\SampleObject.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
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

#ifndef __SampleObject_i_h__
#define __SampleObject_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHelloSampleObject_FWD_DEFINED__
#define __IHelloSampleObject_FWD_DEFINED__
typedef interface IHelloSampleObject IHelloSampleObject;
#endif 	/* __IHelloSampleObject_FWD_DEFINED__ */


#ifndef __HelloSampleObject_FWD_DEFINED__
#define __HelloSampleObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class HelloSampleObject HelloSampleObject;
#else
typedef struct HelloSampleObject HelloSampleObject;
#endif /* __cplusplus */

#endif 	/* __HelloSampleObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IHelloSampleObject_INTERFACE_DEFINED__
#define __IHelloSampleObject_INTERFACE_DEFINED__

/* interface IHelloSampleObject */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHelloSampleObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("58F909AE-2942-428C-954E-BF6B7B6BE936")
    IHelloSampleObject : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Sumlong( 
            /* [in] */ LONG lA,
            /* [in] */ LONG lB,
            /* [retval][out] */ LONG *plSum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHelloSampleObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHelloSampleObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHelloSampleObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHelloSampleObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHelloSampleObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHelloSampleObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHelloSampleObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHelloSampleObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Sumlong )( 
            IHelloSampleObject * This,
            /* [in] */ LONG lA,
            /* [in] */ LONG lB,
            /* [retval][out] */ LONG *plSum);
        
        END_INTERFACE
    } IHelloSampleObjectVtbl;

    interface IHelloSampleObject
    {
        CONST_VTBL struct IHelloSampleObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHelloSampleObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHelloSampleObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHelloSampleObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHelloSampleObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHelloSampleObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHelloSampleObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHelloSampleObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHelloSampleObject_Sumlong(This,lA,lB,plSum)	\
    ( (This)->lpVtbl -> Sumlong(This,lA,lB,plSum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHelloSampleObject_INTERFACE_DEFINED__ */



#ifndef __SampleObjectLib_LIBRARY_DEFINED__
#define __SampleObjectLib_LIBRARY_DEFINED__

/* library SampleObjectLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SampleObjectLib;

EXTERN_C const CLSID CLSID_HelloSampleObject;

#ifdef __cplusplus

class DECLSPEC_UUID("77025312-9202-4EF6-9FA4-2233E3A57D25")
HelloSampleObject;
#endif
#endif /* __SampleObjectLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


