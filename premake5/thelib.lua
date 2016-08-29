local BASE_DIR="../sources/"

project "thelib"
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
    BASE_DIR.."thelib/src/**.cpp",
}
excludes {
}
defines {
    "LITTLE_ENDIAN_BYTE_ALIGNED",
    "NET_SELECT",
    "HAS_LUA",
    "HAS_PROTOCOL_HTTP",
    "HAS_PROTOCOL_RTMP",
    "HAS_PROTOCOL_RTP",
    "HAS_PROTOCOL_LIVEFLV",
    "HAS_PROTOCOL_TS",
    "HAS_MEDIA_TS",
    "HAS_PROTOCOL_VAR",
    "HAS_MEDIA_MP3",
    "HAS_MEDIA_MP4",
    "HAS_MEDIA_FLV",
    "VM_LUA",
    "WIN32",
}
includedirs {
    BASE_DIR.."common/include",
    BASE_DIR.."thelib/include",
    BASE_DIR.."../3rdparty/openssl64/include",
    BASE_DIR.."../3rdparty/tinyxml",
    BASE_DIR.."../3rdparty/lua-dev",
}
forceincludes {
    "common.h",
}
buildoptions {
    "/wd4005",
    "/wd4251",
    "/wd4503",
    "/wd4800",
    "/wd4819",
}
libdirs {
}
links {
}

