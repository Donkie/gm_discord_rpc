function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

workspace "gmcl_discord_rpc"
    configurations { "Debug", "Release" }
    location ( "projects/" .. os.get() )

project "gmcl_discord_rpc"
    filter {"system:windows", "action:vs*"}
        systemversion(os.winSdkVersion() .. ".0")
    kind         "SharedLib"
    architecture "x86"
    language     "C++"
    includedirs  "include/"
    targetdir    "build"
    symbols      "On"
    libdirs      {"lib"}
    links        {"discord-rpc"}

    if os.is( "windows" ) then targetsuffix "_win32" end
    if os.is( "macosx" )  then targetsuffix "_osx"   end
    if os.is( "linux" )   then targetsuffix "_linux" end

    configuration "Debug"
        optimize "Debug"

    configuration "Release"
        optimize "Speed"
        flags    "StaticRuntime"

    files
    {
        "src/**.*",
        "include/**.*"
    }
