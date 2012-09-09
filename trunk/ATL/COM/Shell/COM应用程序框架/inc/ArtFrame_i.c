

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Jun 13 16:59:16 2011
 */
/* Compiler settings for .\ArtFrame.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


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

MIDL_DEFINE_GUID(IID, IID_IDataView,0xE2E319A4,0xD764,0x4dc1,0x95,0xCD,0xC9,0x2D,0x37,0x54,0xD9,0x5B);


MIDL_DEFINE_GUID(IID, IID__COMFrameDispObj,0x3B8C7933,0x4F37,0x4d83,0x91,0x07,0xCF,0x68,0x58,0x61,0x55,0xE2);


MIDL_DEFINE_GUID(IID, IID_IMDIFrame,0xD3137FF6,0x206B,0x4767,0xA9,0x05,0x5D,0x25,0x2B,0x3D,0x93,0x25);


MIDL_DEFINE_GUID(IID, IID_IMDIChildFrame,0xA61BB8F9,0x6141,0x427C,0xAA,0x31,0xCC,0x48,0x88,0x33,0x5E,0x51);


MIDL_DEFINE_GUID(IID, IID_IMDIChildFrames,0xDBDC4265,0x324B,0x4E01,0x9B,0xBE,0x8A,0xB7,0x6F,0xBB,0x8C,0xF6);


MIDL_DEFINE_GUID(IID, LIBID_ArtFrameLib,0x28462839,0xFA53,0x4A4C,0xA7,0x76,0x67,0x6C,0xD6,0x0D,0x42,0x38);


MIDL_DEFINE_GUID(IID, DIID__CommandButtonEvents,0x62F13529,0xA28A,0x4ECB,0xAC,0xD6,0x14,0x19,0x2A,0xF1,0x20,0x5D);


MIDL_DEFINE_GUID(IID, DIID__IMDIFrameEvents,0x866BD9AA,0x8EC6,0x45D1,0xB1,0xD2,0x55,0xB1,0xE8,0x96,0x7E,0x37);


MIDL_DEFINE_GUID(CLSID, CLSID_MDIFrame,0xB092D48B,0xB0B7,0x4AFE,0x90,0x10,0x98,0x62,0x74,0xA7,0x86,0x73);


MIDL_DEFINE_GUID(IID, DIID__IMDIChildFrameEvents,0x5BE13374,0x8203,0x4A09,0x8A,0x34,0xCA,0xE9,0x2D,0x27,0x47,0xA0);


MIDL_DEFINE_GUID(CLSID, CLSID_MDIChildFrame,0x47B924E4,0x4E16,0x4DB9,0xAE,0x43,0x3F,0x01,0x44,0xBE,0x0D,0xD2);


MIDL_DEFINE_GUID(CLSID, CLSID_MDIChildFrames,0x9462FE98,0x891A,0x463D,0x9B,0x6D,0xCD,0xEA,0x80,0x46,0x2B,0xB2);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

