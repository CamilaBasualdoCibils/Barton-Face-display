-- premake5.lua
workspace "Wisereply"
   configurations { "Debug", "Release" }


project "Barton-face-display"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   links {"glfw","GLESv2" }
   
   files { "src/**.h", "src/**.c","src/**.cpp"}
   pchheader "pch.hpp"
   includedirs {"src","lib"}
   libdirs {}
   --
   filter "configurations:Debug"
      defines {"_DEBUG"}
      symbols "On"
   
   filter "configurations:Release"
      defines { "_NDEBUG","_RELEASE" }
      optimize "On"

function customClean()
    -- Specify the directories or files to be cleaned
    local dirsToRemove = {
        "bin",
        "obj",
        "Intermediate"
    }

    local filesToRemove = {
        "Makefile",
        "myproject.sln",
        "Demo.make",
        "ImGui.make",
        "ImPlot.make",
        "Landmark.make",
        "imgui.ini",
        "log.txt",
        "Demo.vcxproj",
        "ImGui.vcxproj",
        "ImPlot.vcxproj",
        "Landmark.vcxproj",
        "Landmark.vcxproj.filters",
        "Landmark.sln"
    }

    -- Remove specified directories
    for _, dir in ipairs(dirsToRemove) do
        if os.isdir(dir) then
            os.rmdir(dir)
            print("Removed directory: " .. dir)
        end
    end

    -- Remove specified files
    for _, file in ipairs(filesToRemove) do
        if os.isfile(file) then
            os.remove(file)
            print("Removed file: " .. file)
        end
    end
end

-- Add the custom clean function to the clean action
newaction {
    trigger = "clean",
    description = "Custom clean action",
    execute = function()
        customClean()
    end
}