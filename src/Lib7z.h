
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2016 Chris Tallman
///
/// @file   Lib7z.h
/// @date   06/17/16
/// @author ctallman
///
/// @brief
///
////////////////////////////////////////////////////////////////////////////////

#ifndef Lib7z_h__
#define Lib7z_h__

#include <vector>
#include <string>
#include <memory>

class Lib7z
{
    struct Archive;
    class impl;

public:
    enum ARType;

    typedef std::string              string;
    typedef unsigned char            byte;
    typedef std::vector<string>      stringlist;
    typedef std::vector<byte>        bytelist;
    typedef std::shared_ptr<Archive> ArchivePtr;
    typedef unsigned long long       ulonglong;

    Lib7z();
    ~Lib7z();

    bool libraryValid() const;

    ArchivePtr getArchive(const char* archive_path, const ARType& artype,
                          const char* password = 0) const;
    int getFileNames(stringlist& out_names, const ArchivePtr& archive) const;
    static int getFileData(bytelist& data, const ArchivePtr& archive, const int id);
    static Lib7z::ulonglong getUncompressedSize(const ArchivePtr& archive, const int id);
    static Lib7z::ulonglong getCompressedSize(const ArchivePtr& archive, const int id);

private:
    impl* _pimpl;
};

enum Lib7z::ARType
{
    TZip      = 0x01,
    TBZip2    = 0x02,
    TRar      = 0x03,
    TArj      = 0x04,
    TZ        = 0x05,
    TLzh      = 0x06,
    T7z       = 0x07,
    TCab      = 0x08,
    TNsis     = 0x09,
    Tlzma     = 0x0A,
    Tlzma86   = 0x0B,
    Txz       = 0x0C,
    Tppmd     = 0x0D,
    TExt      = 0xC7,
    TVMDK     = 0xC8,
    TVDI      = 0xC9,
    TQcow     = 0xCA,
    TGPT      = 0xCB,
    TRar5     = 0xCC,
    TIHex     = 0xCD,
    THxs      = 0xCE,
    TTE       = 0xCF,
    TUEFIc    = 0xD0,
    TUEFIs    = 0xD1,
    TSquashFS = 0xD2,
    TCramFS   = 0xD3,
    TAPM      = 0xD4,
    TMslz     = 0xD5,
    TFlv      = 0xD6,
    TSwf      = 0xD7,
    TSwfc     = 0xD8,
    TNtfs     = 0xD9,
    TFat      = 0xDA,
    TMbr      = 0xDB,
    TVhd      = 0xDC,
    TPe       = 0xDD,
    TElf      = 0xDE,
    TMach_O   = 0xDF,
    TUdf      = 0xE0,
    TXar      = 0xE1,
    TMub      = 0xE2,
    THfs      = 0xE3,
    TDmg      = 0xE4,
    TCompound = 0xE5,
    TWim      = 0xE6,
    TIso      = 0xE7,
    TChm      = 0xE9,
    TSplit    = 0xEA,
    TRpm      = 0xEB,
    TDeb      = 0xEC,
    TCpio     = 0xED,
    TTar      = 0xEE,
    TGZip     = 0xEF,
};

#endif // Lib7z_h__
