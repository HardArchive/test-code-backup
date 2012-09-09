

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jun 23 13:57:44 2011
 */
/* Compiler settings for .\OpenedFiles.idl:
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

#ifndef __OpenedFiles_h__
#define __OpenedFiles_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IInterFaceCls_FWD_DEFINED__
#define __IInterFaceCls_FWD_DEFINED__
typedef interface IInterFaceCls IInterFaceCls;
#endif 	/* __IInterFaceCls_FWD_DEFINED__ */


#ifndef __InterFaceCls_FWD_DEFINED__
#define __InterFaceCls_FWD_DEFINED__

#ifdef __cplusplus
typedef class InterFaceCls InterFaceCls;
#else
typedef struct InterFaceCls InterFaceCls;
#endif /* __cplusplus */

#endif 	/* __InterFaceCls_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IInterFaceCls_INTERFACE_DEFINED__
#define __IInterFaceCls_INTERFACE_DEFINED__

/* interface IInterFaceCls */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IInterFaceCls;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("47F5FA97-212E-4507-909E-08C1F0106C02")
    IInterFaceCls : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IInterFaceClsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInterFaceCls * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInterFaceCls * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInterFaceCls * This);
        
        END_INTERFACE
    } IInterFaceClsVtbl;

    interface IInterFaceCls
    {
        CONST_VTBL struct IInterFaceClsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInterFaceCls_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInterFaceCls_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInterFaceCls_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInterFaceCls_INTERFACE_DEFINED__ */



#ifndef __CHKOPENFILELib_LIBRARY_DEFINED__
#define __CHKOPENFILELib_LIBRARY_DEFINED__

/* library CHKOPENFILELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CHKOPENFILELib;

EXTERN_C const CLSID CLSID_InterFaceCls;

#ifdef __cplusplus

class DECLSPEC_UUID("B3490870-0081-45C4-AE05-C4B7E788BC29")
InterFaceCls;
#endif
#endif /* __CHKOPENFILELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


