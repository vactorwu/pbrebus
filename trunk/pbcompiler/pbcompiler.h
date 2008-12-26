/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 28 23:08:20 2005
 */
/* Compiler settings for C:\save\rebus\src\pbcompiler\pbcompiler.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __pbcompiler_h__
#define __pbcompiler_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IPBCompiler_FWD_DEFINED__
#define __IPBCompiler_FWD_DEFINED__
typedef interface IPBCompiler IPBCompiler;
#endif 	/* __IPBCompiler_FWD_DEFINED__ */


#ifndef ___IcompilerEvents_FWD_DEFINED__
#define ___IcompilerEvents_FWD_DEFINED__
typedef interface _IcompilerEvents _IcompilerEvents;
#endif 	/* ___IcompilerEvents_FWD_DEFINED__ */


#ifndef __compiler_FWD_DEFINED__
#define __compiler_FWD_DEFINED__

#ifdef __cplusplus
typedef class compiler compiler;
#else
typedef struct compiler compiler;
#endif /* __cplusplus */

#endif 	/* __compiler_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IPBCompiler_INTERFACE_DEFINED__
#define __IPBCompiler_INTERFACE_DEFINED__

/* interface IPBCompiler */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPBCompiler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("63DD36AF-75F1-49DE-8277-3617C42CD26A")
    IPBCompiler : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ BSTR szPbVersion,
            /* [in] */ BSTR szBinPath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RegenerateLibrary( 
            BSTR libname) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Regenerate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE BuildLibrary( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Build( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLibraryList( 
            /* [in] */ BSTR szXMLLibList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Migrate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FullRebuild( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLinkedLibrary( 
            /* [in] */ BSTR szLibList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetApplicationLibrary( 
            /* [in] */ BSTR szAppLibrary) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetExeName( 
            /* [in] */ BSTR szExeName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetIconFile( 
            /* [in] */ BSTR szIconFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetApplicationObject( 
            /* [in] */ BSTR szApplicationObject) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetApplicationResourceFile( 
            /* [in] */ BSTR szApplicationResourceFile) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPBCompilerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPBCompiler __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPBCompiler __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPBCompiler __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szPbVersion,
            /* [in] */ BSTR szBinPath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegenerateLibrary )( 
            IPBCompiler __RPC_FAR * This,
            BSTR libname);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Regenerate )( 
            IPBCompiler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BuildLibrary )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Build )( 
            IPBCompiler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLibraryList )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szXMLLibList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Migrate )( 
            IPBCompiler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FullRebuild )( 
            IPBCompiler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLinkedLibrary )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szLibList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetApplicationLibrary )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szAppLibrary);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetExeName )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szExeName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetIconFile )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szIconFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetApplicationObject )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szApplicationObject);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetApplicationResourceFile )( 
            IPBCompiler __RPC_FAR * This,
            /* [in] */ BSTR szApplicationResourceFile);
        
        END_INTERFACE
    } IPBCompilerVtbl;

    interface IPBCompiler
    {
        CONST_VTBL struct IPBCompilerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPBCompiler_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPBCompiler_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPBCompiler_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPBCompiler_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPBCompiler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPBCompiler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPBCompiler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPBCompiler_Init(This,szPbVersion,szBinPath)	\
    (This)->lpVtbl -> Init(This,szPbVersion,szBinPath)

#define IPBCompiler_RegenerateLibrary(This,libname)	\
    (This)->lpVtbl -> RegenerateLibrary(This,libname)

#define IPBCompiler_Regenerate(This)	\
    (This)->lpVtbl -> Regenerate(This)

#define IPBCompiler_BuildLibrary(This,name)	\
    (This)->lpVtbl -> BuildLibrary(This,name)

#define IPBCompiler_Build(This)	\
    (This)->lpVtbl -> Build(This)

#define IPBCompiler_SetLibraryList(This,szXMLLibList)	\
    (This)->lpVtbl -> SetLibraryList(This,szXMLLibList)

#define IPBCompiler_Migrate(This)	\
    (This)->lpVtbl -> Migrate(This)

#define IPBCompiler_FullRebuild(This)	\
    (This)->lpVtbl -> FullRebuild(This)

#define IPBCompiler_SetLinkedLibrary(This,szLibList)	\
    (This)->lpVtbl -> SetLinkedLibrary(This,szLibList)

#define IPBCompiler_SetApplicationLibrary(This,szAppLibrary)	\
    (This)->lpVtbl -> SetApplicationLibrary(This,szAppLibrary)

#define IPBCompiler_SetExeName(This,szExeName)	\
    (This)->lpVtbl -> SetExeName(This,szExeName)

#define IPBCompiler_SetIconFile(This,szIconFile)	\
    (This)->lpVtbl -> SetIconFile(This,szIconFile)

#define IPBCompiler_SetApplicationObject(This,szApplicationObject)	\
    (This)->lpVtbl -> SetApplicationObject(This,szApplicationObject)

#define IPBCompiler_SetApplicationResourceFile(This,szApplicationResourceFile)	\
    (This)->lpVtbl -> SetApplicationResourceFile(This,szApplicationResourceFile)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_Init_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szPbVersion,
    /* [in] */ BSTR szBinPath);


void __RPC_STUB IPBCompiler_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_RegenerateLibrary_Proxy( 
    IPBCompiler __RPC_FAR * This,
    BSTR libname);


void __RPC_STUB IPBCompiler_RegenerateLibrary_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_Regenerate_Proxy( 
    IPBCompiler __RPC_FAR * This);


void __RPC_STUB IPBCompiler_Regenerate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_BuildLibrary_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR name);


void __RPC_STUB IPBCompiler_BuildLibrary_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_Build_Proxy( 
    IPBCompiler __RPC_FAR * This);


void __RPC_STUB IPBCompiler_Build_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_SetLibraryList_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szXMLLibList);


void __RPC_STUB IPBCompiler_SetLibraryList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_Migrate_Proxy( 
    IPBCompiler __RPC_FAR * This);


void __RPC_STUB IPBCompiler_Migrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_FullRebuild_Proxy( 
    IPBCompiler __RPC_FAR * This);


void __RPC_STUB IPBCompiler_FullRebuild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_SetLinkedLibrary_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szLibList);


void __RPC_STUB IPBCompiler_SetLinkedLibrary_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_SetApplicationLibrary_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szAppLibrary);


void __RPC_STUB IPBCompiler_SetApplicationLibrary_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_SetExeName_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szExeName);


void __RPC_STUB IPBCompiler_SetExeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_SetIconFile_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szIconFile);


void __RPC_STUB IPBCompiler_SetIconFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_SetApplicationObject_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szApplicationObject);


void __RPC_STUB IPBCompiler_SetApplicationObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPBCompiler_SetApplicationResourceFile_Proxy( 
    IPBCompiler __RPC_FAR * This,
    /* [in] */ BSTR szApplicationResourceFile);


void __RPC_STUB IPBCompiler_SetApplicationResourceFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPBCompiler_INTERFACE_DEFINED__ */



#ifndef __PBCOMPILERLib_LIBRARY_DEFINED__
#define __PBCOMPILERLib_LIBRARY_DEFINED__

/* library PBCOMPILERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PBCOMPILERLib;

#ifndef ___IcompilerEvents_DISPINTERFACE_DEFINED__
#define ___IcompilerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IcompilerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IcompilerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CF0E19AB-F60D-43E0-BF60-2D484DE34201")
    _IcompilerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IcompilerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IcompilerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IcompilerEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IcompilerEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IcompilerEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IcompilerEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IcompilerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IcompilerEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IcompilerEventsVtbl;

    interface _IcompilerEvents
    {
        CONST_VTBL struct _IcompilerEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IcompilerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IcompilerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IcompilerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IcompilerEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IcompilerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IcompilerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IcompilerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IcompilerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_compiler;

#ifdef __cplusplus

class DECLSPEC_UUID("77765AE8-6757-4ad0-8741-FCC99DE0E0A4")
compiler;
#endif
#endif /* __PBCOMPILERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
