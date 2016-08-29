local BASE_DIR="../3rdparty/tinyxml/"

project "tinyxml"
--language "C"
language "C++"
kind "StaticLib"
--kind "SharedLib"
--kind "ConsoleApp"
--kind "WindowedApp"

flags {
    --"WinMain",
}
files {
    BASE_DIR.."*.cpp",
    BASE_DIR.."*.h",
}
excludes {
}
defines {
}
includedirs {
}
forceincludes {
}
buildoptions {
}
libdirs {
}
links {
}

