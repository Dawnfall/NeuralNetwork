workspace "NeuralNetwork"
    configurations { "Debug", "Release" }
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}"

    include "NeuralNetwork"
    include "NeuralTest"
    include "NumberRecognition"