project "NeuralTest"
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
        "%{wks.location}/Dependencies" 
    }

    links 
    {
        "NeuralNetwork"
    }

    libdirs 
    { 
        "%{wks.location}/bin/" .. outputDir .. "/NeuralNetwork"
    }

    -- postbuildcommands 
    -- {
    --     '{COPY} "bin/Debug/NeuralNetwork/NeuralNetwork.dll" "%{cfg.buildtarget.directory}"'
    -- }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        staticruntime "on"

    filter "configurations:Release"
        runtime "Release"
        symbols "On"
        staticruntime "on"
        
    filter "system:windows"
        systemversion "latest"