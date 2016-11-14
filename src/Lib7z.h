
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

class Lib7z
{
public:
    typedef std::string string;
    typedef unsigned char byte;
    typedef std::vector<string> stringlist;
    typedef std::vector<byte> bytelist;

    Lib7z();
    ~Lib7z();

    int getFileNames(stringlist & filenames, const char * archive, const char * password = 0) const;
    int getFileData(bytelist & data, const char * archive, const string & filename, const char * password = 0) const;

private:
    struct impl;
    impl * _pimpl;
};

#endif // Lib7z_h__
