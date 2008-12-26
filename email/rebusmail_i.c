/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Dec 11 16:47:39 2005
 */
/* Compiler settings for C:\save\rebus\src\email\rebusmail.idl:
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

const IID IID_IEMailer = {0xDEAB418B,0xD6F6,0x42AC,{0x93,0x2A,0xCA,0xCC,0xE2,0xD8,0xBD,0x57}};


const IID LIBID_REBUSMAILLib = {0xDAF18973,0x6717,0x472F,{0x9E,0xE8,0xF2,0x8E,0xC8,0x40,0x5C,0x8E}};


const IID DIID__IEMailerEvents = {0x16C89A51,0x2F64,0x4A5F,{0x94,0x7D,0x09,0xF9,0x8A,0x3C,0x2D,0x3C}};


const CLSID CLSID_EMailer = {0xFF7E5E3F,0x3047,0x4FCC,{0xBF,0x4E,0xD9,0x0E,0x9E,0x4F,0xDA,0x07}};


#ifdef __cplusplus
}
#endif

