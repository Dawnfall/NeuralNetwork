workspace "NeuralNetwork"
    configurations { "Debug", "Release" }
    architecture "x86_64"

    outputDir = "%{cfg.buildcfg}"

    group "Dependecies"
    include "Dependencies/imgui"
    group ""

    include "NeuralNetwork"
    include "NeuralTest"
    include "NumberRecognition"