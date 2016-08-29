local BASE_DIR="../3rdparty/lua-dev/"

project "lua"
language "C"
--language "C++"
kind "StaticLib"
--kind "SharedLib"
--kind "ConsoleApp"
--kind "WindowedApp"

flags {
    --"WinMain",
}
files {
    BASE_DIR.."lapi.h",
    BASE_DIR.."lauxlib.h",
    BASE_DIR.."lcode.h",
    BASE_DIR.."ldebug.h",
    BASE_DIR.."ldo.h",
    BASE_DIR.."lfunc.h",
    BASE_DIR.."lgc.h",
    BASE_DIR.."llex.h",
    BASE_DIR.."llimits.h",
    BASE_DIR.."lmem.h",
    BASE_DIR.."lobject.h",
    BASE_DIR.."lopcodes.h",
    BASE_DIR.."lparser.h",
    BASE_DIR.."lstate.h",
    BASE_DIR.."lstring.h",
    BASE_DIR.."ltable.h",
    BASE_DIR.."ltm.h",
    BASE_DIR.."lua.h",
    BASE_DIR.."luaconf.h",
    BASE_DIR.."lualib.h",
    BASE_DIR.."lundump.h",
    BASE_DIR.."lvm.h",
    BASE_DIR.."lzio.h",
    BASE_DIR.."lapi.c",
    BASE_DIR.."lauxlib.c",
    BASE_DIR.."lbaselib.c",
    BASE_DIR.."lcode.c",
    BASE_DIR.."ldblib.c",
    BASE_DIR.."ldebug.c",
    BASE_DIR.."ldo.c",
    BASE_DIR.."ldump.c",
    BASE_DIR.."lfunc.c",
    BASE_DIR.."lgc.c",
    BASE_DIR.."linit.c",
    BASE_DIR.."liolib.c",
    BASE_DIR.."llex.c",
    BASE_DIR.."lmathlib.c",
    BASE_DIR.."lmem.c",
    BASE_DIR.."loadlib.c",
    BASE_DIR.."lobject.c",
    BASE_DIR.."lopcodes.c",
    BASE_DIR.."loslib.c",
    BASE_DIR.."lparser.c",
    BASE_DIR.."lstate.c",
    BASE_DIR.."lstring.c",
    BASE_DIR.."lstrlib.c",
    BASE_DIR.."ltable.c",
    BASE_DIR.."ltablib.c",
    BASE_DIR.."ltm.c",
    BASE_DIR.."lundump.c",
    BASE_DIR.."lvm.c",
    BASE_DIR.."lzio.c",
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

