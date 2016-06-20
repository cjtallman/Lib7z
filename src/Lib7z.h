
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

    int getFileNames(stringlist & filenames, const string & archive) const;
    int getFileData(bytelist & data, const string & archive, const string & filename) const;
    int setPassword(const string & password);

private:
    struct impl;
    impl * _pimpl;
    string _password;
};

#endif // Lib7z_h__
