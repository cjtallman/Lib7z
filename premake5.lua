
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

    project("Lib7z")
        kind("StaticLib")
        targetdir(_OPTIONS.output_dir)
        architecture("x86_64")
        includedirs{"lib/7zip/CPP/7zip"}
        files
        {
            "src/Lib7z.*",
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
        }

for _,v in ipairs(os.matchdirs("./test/test_*")) do
    local name = path.getbasename(v)
    project(name)
    kind("ConsoleApp")
    location(path.join(_OPTIONS.build_dir, "test"))
    targetdir(path.join(_OPTIONS.output_dir, "test"))
    architecture("x86_64")
    libdirs{_OPTIONS.output_dir}
    links("Lib7z")
    flags "NoIncrementalLink"
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
