/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Aug 29 16:02:58 2003
 */
/* Compiler settings for E:\My Projects\IPM\mvinfo\mvinfo.idl:
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

const IID IID_IVersionInfo = {0x6869846A,0x588B,0x40BB,{0x97,0xCB,0x00,0x48,0xDB,0x57,0xBE,0xD7}};


const IID LIBID_MVINFOLib = {0xD4C9703F,0x8881,0x47C0,{0xA1,0x0D,0x35,0x07,0xA4,0xED,0xC9,0x41}};


const CLSID CLSID_VersionInfo = {0x327DEAC6,0x34E3,0x41E6,{0x89,0x86,0xBF,0xE8,0x82,0xC8,0xAE,0xDB}};


#ifdef __cplusplus
}
#endif

