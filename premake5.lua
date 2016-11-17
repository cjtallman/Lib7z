
newoption{trigger = "build_dir", description = "Project build directory."}
newoption{trigger = "output_dir", description = "Project output directory."}

_OPTIONS.build_dir = _OPTIONS.build_dir or "_build"
_OPTIONS.output_dir = _OPTIONS.output_dir or "_output"

solution("Lib7z")
    location(_OPTIONS.build_dir)
    configurations{"Release","Debug"}
    filter{"configurations:Release"}
        defines{"NDEBUG"}
        flags{"Optimize"}
    filter{"configurations:Debug"}
        defines{"DEBUG"}
        if type(symbols) == "function" then
            symbols("On")    -- premake 5 alpha 10
        else
            flags{"Symbols"} -- premake 5 alpha 9
        end
    filter{}
        flags{"C++11"}
        architecture("x86_64")

    project("Lib7z")
        kind("StaticLib")
        targetdir(_OPTIONS.output_dir)
        language("C++")
        filter{"system:windows"}
            includedirs
            {
                "lib/7zip/CPP/7zip",
                "lib/7zip/CPP/Common",
            }
            files
            {
                "src/Lib7z.*",
                "lib/7zip/C/Alloc.c",
                "lib/7zip/CPP/Common/IntToString.cpp",
                "lib/7zip/CPP/Common/NewHandler.cpp",
                "lib/7zip/CPP/Common/MyString.cpp",
                "lib/7zip/CPP/Common/StringConvert.cpp",
                "lib/7zip/CPP/Common/StringToInt.cpp",
                "lib/7zip/CPP/Common/MyVector.cpp",
                "lib/7zip/CPP/Common/Wildcard.cpp",
                "lib/7zip/CPP/Windows/DLL.cpp",
                "lib/7zip/CPP/Windows/FileDir.cpp",
                "lib/7zip/CPP/Windows/FileFind.cpp",
                "lib/7zip/CPP/Windows/FileIO.cpp",
                "lib/7zip/CPP/Windows/FileName.cpp",
                "lib/7zip/CPP/Windows/PropVariant.cpp",
                "lib/7zip/CPP/Windows/PropVariantConv.cpp",
                "lib/7zip/CPP/7zip/Common/FileStreams.cpp",
                "lib/7zip/CPP/7zip/Common/StreamObjects.cpp",
            }
        filter{"system:linux"}
            defines
            {
                "_FILE_OFFSET_BITS=64",
                "_LARGEFILE_SOURCE",
                "_REENTRANT",
                "ENV_UNIX",
                "UNICODE",
                "_UNICODE",
                "UNIX_USE_WIN_FILE",
            }
            includedirs
            {
                "lib/p7zip/C",
                "lib/p7zip/CPP",
                "lib/p7zip/CPP/7zip",
                "lib/p7zip/CPP/Common",
                "lib/p7zip/CPP/myWindows",
                "lib/p7zip/CPP/include_windows",
            }
            files
            {
                "src/Lib7z.*",
                "lib/p7zip/C/Alloc.*",
                "lib/p7zip/C/Threads.*",
                "lib/p7zip/CPP/Common/IntToString.*",
                "lib/p7zip/CPP/Common/NewHandler.*",
                "lib/p7zip/CPP/Common/MyString.*",
                "lib/p7zip/CPP/Common/MyVector.*",
                "lib/p7zip/CPP/Common/MyWindows.*",
                "lib/p7zip/CPP/Common/Wildcard.*",
                "lib/p7zip/CPP/Common/StringConvert.*",
                "lib/p7zip/CPP/Common/StringToInt.*",
                "lib/p7zip/CPP/Windows/DLL.*",
                "lib/p7zip/CPP/Windows/FileDir.*",
                "lib/p7zip/CPP/Windows/FileFind.*",
                "lib/p7zip/CPP/Windows/FileIO.*",
                "lib/p7zip/CPP/Windows/FileName.*",
                "lib/p7zip/CPP/Windows/PropVariant.*",
                "lib/p7zip/CPP/Windows/PropVariantConv.*",
                "lib/p7zip/CPP/myWindows/wine_date_and_time.*",
                "lib/p7zip/CPP/7zip/Common/FileStreams.*",
                "lib/p7zip/CPP/7zip/Common/StreamObjects.*",
            }
        filter{}

for _,v in ipairs(os.matchdirs("./test/test_*")) do
    local name = path.getbasename(v)
    project(name)
    kind("ConsoleApp")
    location(path.join(_OPTIONS.build_dir, "test"))
    targetdir(path.join(_OPTIONS.output_dir, "test"))
    libdirs{_OPTIONS.output_dir}
    links{"Lib7z"}
    filter{"system:linux"}
        links{"dl", "pthread"}
    filter{}
    debugdir(path.join(path.join(_OPTIONS.output_dir, "test")))
    includedirs
    {
        path.join("lib", "googletest"),
        path.join("lib", "googletest", "include"),
        "src",
    }
    files
    {
        path.join("lib", "googletest", "src", "gtest-all.cc"),
        path.join(v, "main.cpp"),
    }
    postbuildcommands
    {
        [[{COPY} ../../test/*.7z %{cfg.buildtarget.directory}]]
    }
end
