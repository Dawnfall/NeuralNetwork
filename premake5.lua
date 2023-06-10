workspace "NeuralNetwork"
    configurations { "Debug", "Release" }
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}"

project "NeuralNetwork"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("NeuralNetwork/bin-int/" .. outputDir .. "/%{prj.name}")

    files 
    { 
        "NeuralNetwork/src/**.cpp",
        "NeuralNetwork/src/**.h" 
    } 

    includedirs 
    { 
        "3rd" 
    }  
    defines "NEURAL_EXPORT"

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    

project "NeuralTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("NeuralNetwork/bin-int/" .. outputDir .. "/%{prj.name}")

    files 
    { 
        "NeuralTest/src/**.cpp", 
        "NeuralTest/src/**.h" 
    } 
    includedirs 
    { 
        "NeuralNetwork/src",
        "3rd" 
    }

    links 
    {
        "NeuralNetwork"
    }

    libdirs 
    { 
        "%{wks.location}/bin/" .. outputDir .. "/NeuralNetwork"
    }

    postbuildcommands 
    {
        '{COPY} "bin/Debug/NeuralNetwork/NeuralNetwork.dll" "%{cfg.buildtarget.directory}"'
    }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        symbols "On"

    filter "system:windows"
        systemversion "latest"
