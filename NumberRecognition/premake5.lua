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
        "%{wks.location}/Dependencies/SDL/include",
        "%{wks.location}/Dependencies/SDL_image/include",
        "%{wks.location}/Dependencies/imgui",
    }

    links 
    {
        "NeuralNetwork",
        "SDL2",
        "SDL2main",
        "SDL2_image",
        "imgui"
    }

    libdirs 
    { 
        "%{wks.location}/bin/" .. outputDir .. "/NeuralNetwork",
        "%{wks.location}/bin/" .. outputDir .. "/NeuralNetwork",
        "%{wks.location}/Dependencies/SDL/lib/x64",
        "%{wks.location}/Dependencies/SDL_image/lib/x64"
    }

    postbuildcommands 
    {
        --'{COPY} "bin/Debug/NeuralNetwork/NeuralNetwork.dll" "%{cfg.buildtarget.directory}"'
        '{COPY} "%{wks.location}/Dependencies/SDL/lib/x64/SDL2.dll" "%{cfg.buildtarget.directory}"',
        '{COPY} "%{wks.location}/Dependencies/SDL_image/lib/x64/SDL2_image.dll" "%{cfg.buildtarget.directory}"'
    }
    
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
    