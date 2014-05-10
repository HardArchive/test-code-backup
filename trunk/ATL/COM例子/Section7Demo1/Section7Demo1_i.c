

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Dec 11 12:53:54 2012
 */
/* Compiler settings for .\Section7Demo1.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
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

MIDL_DEFINE_GUID(IID, IID_IHelloSOE,0x0155C7BF,0x721F,0x4107,0xA5,0x26,0x40,0x6A,0x7F,0x1D,0x32,0xBC);


MIDL_DEFINE_GUID(IID, LIBID_Section7Demo1Lib,0xA41764AE,0x5B0B,0x4098,0xB5,0xF1,0xCC,0x1C,0x7A,0x8B,0xEA,0x49);


MIDL_DEFINE_GUID(IID, DIID__IHelloSOEEvents,0xA3F2D07E,0x4F99,0x4B17,0xB6,0x8F,0x76,0x1F,0x2C,0x20,0x3A,0xF0);


MIDL_DEFINE_GUID(CLSID, CLSID_HelloSOE,0xEC5897D9,0x4F28,0x414F,0x9A,0x5A,0xCF,0xFD,0x49,0xF6,0x80,0xE4);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



