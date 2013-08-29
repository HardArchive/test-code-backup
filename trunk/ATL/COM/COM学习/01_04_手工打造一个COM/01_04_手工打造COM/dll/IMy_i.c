

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Aug 21 14:05:04 2013
 */
/* Compiler settings for .\IMy.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

MIDL_DEFINE_GUID(IID, IID_IMy,0xD77FDBC5,0xE7D5,0x48aa,0x98,0x6F,0xE5,0xD5,0x8F,0x57,0xF5,0x9F);


MIDL_DEFINE_GUID(IID, IID_IMy2,0x0F30D079,0xC236,0x4baf,0x88,0x69,0x28,0xAF,0x39,0xDA,0xC3,0x65);


MIDL_DEFINE_GUID(IID, LIBID_SSZLib,0x5BC9017F,0xF3FB,0x4f7b,0xBE,0x3B,0xEA,0x44,0xF1,0xD1,0x01,0x80);


MIDL_DEFINE_GUID(CLSID, CLSID_SSZ,0xA738E0FE,0xEF11,0x482a,0xAF,0xE2,0x11,0x64,0xE4,0x74,0x9F,0x25);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



