project "NeuralNetwork"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files 
    { 
        "src/**.cpp",
        "src/**.h" 
    } 

    includedirs 
    { 
        "%{wks.location}/Dependencies" 
    }  

    -- postbuildcommands 
    -- {
    --     '{COPY} "bin/Debug/NeuralNetwork/NeuralNetwork.dll" "%{cfg.buildtarget.directory}"'
    -- }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"