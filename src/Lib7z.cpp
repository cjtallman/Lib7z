
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2016 Chris Tallman
///
/// @file   Lib7z.cpp
/// @date   06/17/16
/// @author ctallman
///
/// @brief
///
////////////////////////////////////////////////////////////////////////////////

#include "Lib7z.h"

#include "../Common/MyWindows.h"

#include "../Common/Defs.h"
#include "../Common/MyInitGuid.h"

#include "../Common/IntToString.h"
#include "../Common/StringConvert.h"

#include "../Windows/DLL.h"
#include "../Windows/FileDir.h"
#include "../Windows/FileFind.h"
#include "../Windows/FileName.h"
#include "../Windows/NtCheck.h"
#include "../Windows/PropVariant.h"
#include "../Windows/PropVariantConv.h"

#include "Common/FileStreams.h"

#include "Archive/IArchive.h"

#include "../../C/7zVersion.h"
#include "IPassword.h"

HINSTANCE g_hInstance = 0;

DEFINE_GUID(CLSID_CFormat7z, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00,
            0x00);
#define CLSID_Format CLSID_CFormat7z

static FString CmdStringToFString(const char* s)
{
    return us2fs(GetUnicodeString(s));
}

class CArchiveOpenCallback : public IArchiveOpenCallback,
                             public ICryptoGetTextPassword,
                             public CMyUnknownImp
{
public:
    MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

    STDMETHOD(SetTotal)(const UInt64* files, const UInt64* bytes);
    STDMETHOD(SetCompleted)(const UInt64* files, const UInt64* bytes);
    STDMETHOD(CryptoGetTextPassword)(BSTR* password);

    bool    PasswordIsDefined;
    UString Password;

    CArchiveOpenCallback() : PasswordIsDefined(false) {}
};

STDMETHODIMP CArchiveOpenCallback::SetTotal(const UInt64*, const UInt64*)
{
    return S_OK;
}

STDMETHODIMP CArchiveOpenCallback::SetCompleted(const UInt64*, const UInt64*)
{
    return S_OK;
}

STDMETHODIMP CArchiveOpenCallback::CryptoGetTextPassword(BSTR* password)
{
    if (!PasswordIsDefined)
    {
        return E_ABORT;
    }

    return StringToBstr(Password, password);
}

struct Lib7z::Archive
{
private:
    CMyComPtr<IInArchive> _archive;

public:
    // constructor
    Archive(CMyComPtr<IInArchive>& ar) : _archive(ar) {}

    // valid operator
    operator bool() const { return _archive != NULL; }

    // conversion operator
    operator CMyComPtr<IInArchive>() const { return _archive; }

    // dot operator
    const CMyComPtr<IInArchive>& operator*() const { return _archive; }

    // arrow operator
    CMyComPtr<IInArchive> operator->() const { return _archive; }
};

struct Lib7z::impl
{
    impl() : _createObjectFunc(NULL)
    {
        _lib.Load(NWindows::NDLL::GetModuleDirPrefix() + FTEXT("7z.dll"));
        if (_lib.IsLoaded())
        {
            _createObjectFunc = (Func_CreateObject)_lib.GetProc("CreateObject");
        }
        else
        {
            _lib.Load(FTEXT("7z.dll"));
            if (_lib.IsLoaded())
            {
                _createObjectFunc = (Func_CreateObject)_lib.GetProc("CreateObject");
            }
        }
    }

    CMyComPtr<IInArchive> getArchive(const char* in_archive, const char* password)
    {
        CMyComPtr<IInArchive> archive;

        if (!_createObjectFunc)
        {
            return NULL;
        }

        if (_createObjectFunc(&CLSID_Format, &IID_IInArchive, (void**)&archive) != S_OK)
        {
            return NULL;
        }

        CInFileStream*       fileSpec = new CInFileStream;
        CMyComPtr<IInStream> file     = fileSpec;

        if (!fileSpec->Open(CmdStringToFString(in_archive)))
        {
            return NULL;
        }

        CArchiveOpenCallback*           openCallbackSpec = new CArchiveOpenCallback;
        CMyComPtr<IArchiveOpenCallback> openCallback(openCallbackSpec);
        if (password)
        {
            openCallbackSpec->PasswordIsDefined = true;
            openCallbackSpec->Password          = CmdStringToFString(password);
        }
        else
        {
            openCallbackSpec->PasswordIsDefined = false;
            openCallbackSpec->Password          = L"";
        }

        const UInt64 scanSize = 1 << 23;
        if (archive->Open(file, &scanSize, openCallback) != S_OK)
        {
            return NULL;
        }

        return archive;
    }

    NWindows::NDLL::CLibrary _lib;
    Func_CreateObject        _createObjectFunc;
};

Lib7z::Lib7z() : _pimpl(new impl)
{
}

Lib7z::~Lib7z()
{
    delete _pimpl;
}

bool Lib7z::libraryValid() const
{
    return (_pimpl != NULL) && (_pimpl->_createObjectFunc != NULL);
}

int Lib7z::getFileNames(stringlist& out_names, const ArchivePtr& archive) const
{
    if (archive)
    {
        // List command
        UInt32 numItems = 0;
        (*archive)->GetNumberOfItems(&numItems);
        for (UInt32 i = 0; i < numItems; i++)
        {
            NWindows::NCOM::CPropVariant propIsDir;
            (*archive)->GetProperty(i, kpidIsDir, &propIsDir);
            if (propIsDir.vt == VT_BOOL)
            {
                if (!propIsDir.boolVal)
                {
                    // Get name of file
                    NWindows::NCOM::CPropVariant prop;
                    (*archive)->GetProperty(i, kpidPath, &prop);

                    if (prop.vt == VT_BSTR && prop.bstrVal)
                    {
                        AString s = UnicodeStringToMultiByte(prop.bstrVal, CP_OEMCP);
                        s.Replace((char)0xA, ' ');
                        s.Replace((char)0xD, ' ');
                        out_names.push_back(s.Ptr());
                    }
                }
            }
        }
        return (int)out_names.size();
    }
    else
        return 0;
}

Lib7z::ArchivePtr Lib7z::getArchive(const char* in_archive, const char* password) const
{
    CMyComPtr<IInArchive> archive = _pimpl->getArchive(in_archive, password);

    if (archive)
    {
        return ArchivePtr(new Archive(archive));
    }
    else
    {
        return NULL;
    }
}

Lib7z::ulonglong Lib7z::getUncompressedSize(const ArchivePtr& archive, const int id) const
{
    if (archive)
    {
        NWindows::NCOM::CPropVariant prop;
        (*archive)->GetProperty(id, kpidSize, &prop);

        switch (prop.vt)
        {
        default:
        case VT_EMPTY:
            return 0;
        case VT_UI1:
            return prop.bVal;
        case VT_UI2:
            return prop.uiVal;
        case VT_UI4:
            return prop.ulVal;
        case VT_UI8:
            return (UInt64)prop.uhVal.QuadPart;
        }
    }
    else
    {
        return 0;
    }
}

Lib7z::ulonglong Lib7z::getCompressedSize(const ArchivePtr& archive, const int id) const
{
    if (archive)
    {
        NWindows::NCOM::CPropVariant prop;
        (*archive)->GetProperty(id, kpidPackSize, &prop);

        switch (prop.vt)
        {
        default:
        case VT_EMPTY:
            return 0;
        case VT_UI1:
            return prop.bVal;
        case VT_UI2:
            return prop.uiVal;
        case VT_UI4:
            return prop.ulVal;
        case VT_UI8:
            return (UInt64)prop.uhVal.QuadPart;
        }
    }
    else
    {
        return 0;
    }
}
