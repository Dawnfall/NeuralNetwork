workspace "NeuralNetwork"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    startproject "NumberRecognition"

    outputDir = "%{cfg.buildcfg}"

    group "Dependecies"
    include "Dependencies/imgui"
    group ""

    include "NeuralNetwork"
    include "NeuralTest"
    include "NumberRecognition"