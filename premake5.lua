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
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files 
    { 
        "NeuralNetwork/src/**.cpp",
        "NeuralNetwork/src/**.h" 
    } 

    includedirs 
    { 
        "Dependencies" 
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
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files 
    { 
        "NeuralTest/src/**.cpp", 
        "NeuralTest/src/**.h" 
    } 
    includedirs 
    { 
        "NeuralNetwork/src",
        "Dependencies" 
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

project "NeuralBridge"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files 
    { 
        "NeuralBridge/src/**.cpp",
        "NeuralBridge/src/**.h"
    } 

    includedirs 
    { 
        "Dependencies" 
    }
      
    defines "NEURAL_IMPORT"

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"