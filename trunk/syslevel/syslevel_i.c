/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Nov 20 17:34:59 2003
 */
/* Compiler settings for E:\My Projects\IPM\syslevel\syslevel.idl:
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

const IID IID_Iprocess = {0xE0306EDB,0xA7FE,0x4D73,{0xBE,0x55,0x98,0x2D,0x10,0x15,0xEF,0x1F}};


const IID IID_Isystem = {0x97BB2E94,0xAF08,0x4AF9,{0x83,0x64,0x64,0x7D,0xEF,0x72,0x97,0xA2}};


const IID LIBID_SYSLEVELLib = {0xDAB0F5D9,0xEC34,0x4440,{0x91,0xE0,0xB5,0xA7,0x1C,0x9B,0x4A,0x09}};


const CLSID CLSID_process = {0x5849BFBD,0x2B01,0x4CD0,{0xAE,0x0E,0x59,0x88,0x81,0xB9,0xD5,0x92}};


const CLSID CLSID_system = {0xE1AB17F5,0x2C65,0x4EAD,{0x88,0x7B,0xB8,0x01,0xB9,0x38,0x57,0xDB}};


#ifdef __cplusplus
}
#endif

