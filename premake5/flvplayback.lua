local BASE_DIR="../sources/applications/flvplayback/"

project "flvplayback"
--language "C"
language "C++"
--kind "StaticLib"
kind "SharedLib"
--kind "ConsoleApp"
--kind "WindowedApp"

flags {
    --"WinMain",
}
files {
    BASE_DIR.."src/*.cpp",
    BASE_DIR.."include/*.h",
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
    BASE_DIR.."include",
    BASE_DIR.."../../thelib/include",
    BASE_DIR.."../../common/include",
    BASE_DIR.."../../../3rdparty/openssl64/include",
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
    BASE_DIR.."../../../3rdparty/openssl64/lib",
}
links {
    "thelib",
    "common",

    "tinyxml",
    "lua",
    "libeay32",
    "ssleay32",

    "ws2_32",
    "Shlwapi",
}

postbuildcommands {
    "mkdir $(TargetDir)",
    "copy $(TargetPath) ..\\Applications\\flvplayback\\$(TargetName)$(TargetExt)",
}
filter {"platforms:Win64", "configurations:Debug" }
do
    postbuildcommands {
        "copy $(TargetDir)$(TargetName).pdb ..\\Applications\\flvplayback\\$(TargetName).pdb",
    }
end

