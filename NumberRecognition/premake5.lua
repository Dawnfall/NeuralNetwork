project "NumberRecognition"
    kind "ConsoleApp"
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
        "%{wks.location}/NeuralNetwork/src",
        "%{wks.location}/Dependencies",
        "%{wks.location}/Dependencies/SDL/include"
    }

    links 
    {
        "NeuralNetwork",
        "SDL2",
        "SDL2main",
    }

    libdirs 
    { 
        "%{wks.location}/bin/" .. outputDir .. "/NeuralNetwork",
        "%{wks.location}/Dependencies/SDL/lib/x64"
    }

    postbuildcommands 
    {
        --'{COPY} "bin/Debug/NeuralNetwork/NeuralNetwork.dll" "%{cfg.buildtarget.directory}"'
        '{COPY} "%{wks.location}/Dependencies/SDL/lib/x64/SDL2.dll" "%{cfg.buildtarget.directory}"'

    }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        symbols "On"

    filter "system:windows"
        systemversion "latest"
    