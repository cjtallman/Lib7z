
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

public:
    typedef std::string              string;
    typedef unsigned char            byte;
    typedef std::vector<string>      stringlist;
    typedef std::vector<byte>        bytelist;
    typedef std::shared_ptr<Archive> ArchivePtr;
    typedef unsigned long long       ulonglong;

    Lib7z();
    ~Lib7z();

    bool libraryValid() const;

    ArchivePtr getArchive(const char* archive_path, const char* password = 0) const;
    int getFileNames(stringlist& out_names, const ArchivePtr& archive) const;
    static int getFileData(bytelist& data, const ArchivePtr& archive, const int id);
    static Lib7z::ulonglong getUncompressedSize(const ArchivePtr& archive, const int id);
    static Lib7z::ulonglong getCompressedSize(const ArchivePtr& archive, const int id);

private:
    class impl;
    impl* _pimpl;
};

#endif // Lib7z_h__
