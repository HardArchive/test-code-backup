

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 22 17:51:31 2011
 */
/* Compiler settings for .\TestMenu3.idl:
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

#ifndef __TestMenu3_h__
#define __TestMenu3_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICCopyPathEx_FWD_DEFINED__
#define __ICCopyPathEx_FWD_DEFINED__
typedef interface ICCopyPathEx ICCopyPathEx;
#endif 	/* __ICCopyPathEx_FWD_DEFINED__ */


#ifndef __CCopyPathEx_FWD_DEFINED__
#define __CCopyPathEx_FWD_DEFINED__

#ifdef __cplusplus
typedef class CCopyPathEx CCopyPathEx;
#else
typedef struct CCopyPathEx CCopyPathEx;
#endif /* __cplusplus */

#endif 	/* __CCopyPathEx_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ICCopyPathEx_INTERFACE_DEFINED__
#define __ICCopyPathEx_INTERFACE_DEFINED__

/* interface ICCopyPathEx */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICCopyPathEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E587C45D-3B9E-4DCE-BF19-897F54C0C826")
    ICCopyPathEx : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICCopyPathExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICCopyPathEx * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICCopyPathEx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICCopyPathEx * This);
        
        END_INTERFACE
    } ICCopyPathExVtbl;

    interface ICCopyPathEx
    {
        CONST_VTBL struct ICCopyPathExVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICCopyPathEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICCopyPathEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICCopyPathEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICCopyPathEx_INTERFACE_DEFINED__ */



#ifndef __TestMenu3Lib_LIBRARY_DEFINED__
#define __TestMenu3Lib_LIBRARY_DEFINED__

/* library TestMenu3Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TestMenu3Lib;

EXTERN_C const CLSID CLSID_CCopyPathEx;

#ifdef __cplusplus

class DECLSPEC_UUID("7CCC83BC-42DD-440D-8F85-B94964844F78")
CCopyPathEx;
#endif
#endif /* __TestMenu3Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


