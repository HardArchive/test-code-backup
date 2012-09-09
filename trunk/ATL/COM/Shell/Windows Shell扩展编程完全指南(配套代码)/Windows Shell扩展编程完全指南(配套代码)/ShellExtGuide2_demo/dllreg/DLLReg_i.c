/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Nov 11 13:52:44 2010
 */
/* Compiler settings for F:\Windows+Shell扩展编程完全指南(配套代码)\Windows Shell扩展编程完全指南(配套代码)\ShellExtGuide2_demo\dllreg\DLLReg.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


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

const IID IID_IDLLRegShlExt = {0x8AB81E71,0xCB2F,0x11D3,{0x8D,0x3B,0xAC,0x2F,0x34,0xF1,0xFA,0x3C}};


const IID LIBID_DLLREGLib = {0x8AB81E65,0xCB2F,0x11D3,{0x8D,0x3B,0xAC,0x2F,0x34,0xF1,0xFA,0x3C}};


const CLSID CLSID_DLLRegShlExt = {0x8AB81E72,0xCB2F,0x11D3,{0x8D,0x3B,0xAC,0x2F,0x34,0xF1,0xFA,0x3C}};


#ifdef __cplusplus
}
#endif

