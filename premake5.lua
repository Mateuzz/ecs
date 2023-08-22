workspace "Ecs"

    configurations  {"Debug", "Release"}
    platforms {"x64", "x32"}

    language "C++"
    buildoptions {"-Wall", "-W", "-Wextra"}
    cppdialect "C++20"
    targetdir "bin/"

    filter "configurations:Release"
        targetdir "release/"
        defines {"RELEASE"}
        optimize "On"

    filter "configurations:Debug"
        targetdir "debug/"
        defines {"DEBUG"}
        symbols "On"

    filter {"platforms:x64"}
        targetsuffix "-64"
        system "linux"
        architecture "x86_64"

    filter {"platforms:x32"}
        targetsuffix "-32"
        system "linux"
        architecture "x86"

    filter {}

    project "main"
        kind "ConsoleApp"
        files {"*.cpp", "*.h"}
        targetname "main"
