# Lib7z

[![AppVeyor branch](https://img.shields.io/appveyor/ci/cjtallman/Lib7z/master.svg)](https://ci.appveyor.com/project/cjtallman/Lib7z/branch/master)

[![Travis branch](https://img.shields.io/travis/cjtallman/Lib7z/master.svg)](https://travis-ci.org/cjtallman/Lib7z)

A lightweight wrapper for 7Zip written in C++.

The library contains functions for opening and extracting data to memory.

## Example Usage

```cpp
// Initialize the libary.
Lib7z lib;

// Open the archive.
Lib7z::ArchivePtr foo_archive = lib.getArchive("foo.7z", "password");

// Get file with index 0.
Lib7z::bytelist foo_data;
const int foo_size = lib.getFileData(foo_data, foo_archive, 0);
```
