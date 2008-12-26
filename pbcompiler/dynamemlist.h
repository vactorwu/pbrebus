// dynamemlist.h

#pragma once
#ifndef __DYNAMEMLIST_H__
#define __DYNAMEMLIST_H__

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CDynamicMemberList

class CDynamicMemberList
{
public:
    CDynamicMemberList();

    // Should be called only once before any members are added
    HRESULT ExcludeDispIDs(ITypeInfo* pInfo);

    size_t  Count();
    HRESULT AddMember(BSTR bstrName, DWORD grfdex, const VARIANT& varValue, DISPID* pid);
    HRESULT GetDispID(BSTR bstrName, DWORD grfdex, DISPID* pid);
    HRESULT GetProperty(DISPID id, VARIANT* pvarRes);
    HRESULT PutProperty(DISPID id, const VARIANT& var);
    HRESULT InvokeEx(DISPID id, LCID lcid, WORD wFlags, DISPPARAMS* pdp, VARIANT* pvarRes,	EXCEPINFO* pei, IServiceProvider* pspCaller);
    HRESULT DeleteMember(BSTR bstrName, DWORD grfdex);
    HRESULT DeleteMember(DISPID id);
    HRESULT GetMemberProperties(DISPID id, BSTR* pbstrName, VARTYPE* pvt, DWORD* pgrfdex);
    HRESULT GetNextDispID(DWORD grfdex, DISPID id, DISPID* pid);
    HRESULT GetNewEnum(IUnknown** punkEnum);

private:
    class CMember;
    typedef std::vector<CMember> CMemberVector;
    typedef CMemberVector::iterator CMemberIterator;

    class FindMemberByName
    {
    public:
        FindMemberByName(const BSTR bstrName, DWORD grfdex);
        bool operator()(const CMember& member);

    private:
        const BSTR  m_bstrName;
        DWORD       m_grfdex;
    };

    class FindMemberByID
    {
    public:
        FindMemberByID(DISPID id);
        bool operator()(const CMember& member);

    private:
        DISPID  m_id;
    };

    CMemberIterator FindMember(BSTR bstrName, DWORD grfdex, bool bFindDeleted = false);
    CMemberIterator FindMember(DISPID id, bool bFindDeleted = false);

    DISPID GetNextDispID();

    class CMember
    {
    public:
        CMember(DISPID id, BSTR bstrName, DWORD grfdex, const VARIANT& varValue);

        DISPID          ID() const;
        BSTR            Name() const;
        DWORD           Flags() const;
        const VARIANT&  Value() const;
        bool            Deleted() const;

        void SetID(DISPID id);
        void SetName(BSTR bstrName);
        void SetFlags(DWORD grfdex);
        void SetValue(const VARIANT& var);
        void SetDeleted(bool bDeleted);

    private:
        DISPID          m_id;
        CComBSTR        m_bstrName;
        DWORD           m_grfdex;
        CComVariant     m_varValue;
        bool            m_bDeleted;
    };

private:
    CMemberVector   m_vecMembers;
    DISPID          m_idNext;
};

#endif  // __DYNAMEMLIST_H__
