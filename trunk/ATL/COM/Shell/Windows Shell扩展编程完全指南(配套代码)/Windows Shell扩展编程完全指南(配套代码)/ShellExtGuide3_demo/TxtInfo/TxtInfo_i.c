/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Nov 12 16:11:45 2010
 */
/* Compiler settings for F:\Windows+Shell扩展编程完全指南(配套代码)\Windows Shell扩展编程完全指南(配套代码)\ShellExtGuide3_demo\TxtInfo\TxtInfo.idl:
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

const IID IID_ITxtInfoShlExt = {0xF4D78AE0,0x05AB,0x11D4,{0x8D,0x3B,0x44,0x45,0x53,0x54,0x00,0x00}};


const IID LIBID_TXTINFOLib = {0x29C6936B,0x0596,0x11D4,{0x8D,0x3B,0xB4,0xBA,0xC1,0xC0,0x8C,0x1E}};


const CLSID CLSID_TxtInfoShlExt = {0xF4D78AE1,0x05AB,0x11D4,{0x8D,0x3B,0x44,0x45,0x53,0x54,0x00,0x00}};


#ifdef __cplusplus
}
#endif

