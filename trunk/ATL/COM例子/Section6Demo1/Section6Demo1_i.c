

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Dec 10 18:17:16 2012
 */
/* Compiler settings for .\Section6Demo1.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IHelloCompositeControl,0x185B107D,0xE152,0x4ABC,0xA4,0x5F,0xBD,0xF0,0xDA,0x9E,0x2C,0x2F);


MIDL_DEFINE_GUID(IID, LIBID_Section6Demo1Lib,0xA19D5833,0xDD0A,0x4C82,0xB3,0x3E,0x18,0xDB,0x8B,0x82,0x18,0x9E);


MIDL_DEFINE_GUID(IID, DIID__IHelloCompositeControlEvents,0x2CD72493,0xCC54,0x451E,0xBB,0xE0,0x5E,0x5D,0x25,0x74,0x66,0x44);


MIDL_DEFINE_GUID(CLSID, CLSID_HelloCompositeControl,0x56B23F6D,0xF0D3,0x401D,0xA1,0xF8,0xBF,0x88,0x0B,0xE8,0xC2,0x22);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



