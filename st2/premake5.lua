WINDOWS = os.host() == "windows"


project "st2"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.hpp" , "src/**.cpp" }
   
   includedirs
   {
    "../%{IncludeDir.glm}",
    "../%{IncludeDir.spd_log}",
    "../%{IncludeDir.SFML}",
    "../%{IncludeDir.ImGui}",
    "src"
   }
   defines {"SFML_STATIC"}
 
   libdirs { "../vendor/SFML/SFML-3.0.0/lib" }

    links
	{
        "imgui",
		"freetype",
		--"winmm",
		--"gdi32",
       -- "flac",
		"vorbisenc",
		"vorbisfile",
		"vorbis",
		"ogg",

    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "action:vs*"
        buildoptions { "/Zc:__cplusplus" }

   filter "system:windows"
      systemversion "latest"
      defines { "PLATFORM_WINDOWS" }
      links
      {
          "opengl32",
          "flac",
      }
   filter "system:linux"
        links
        {
          "stdc++fs",
          "GL",
          "sfml-graphics",
          "sfml-window",
          "sfml-system",
          "sfml-audio",
          "sfml-network"
        }

   filter "configurations:Debug"
      defines { "DEBUG" }
      if WINDOWS then
        links
        {	
          "sfml-graphics-s-d",
          "sfml-window-s-d",
          "sfml-system-s-d",
          "sfml-audio-s-d",
          "sfml-network-s-d"
        }
      end   


      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "RELEASE" }
      if WINDOWS then
        links
        {	
          "sfml-graphics-s",
          "sfml-window-s",
          "sfml-system-s",
          "sfml-audio-s",
          "sfml-network-s"
        }
      end  
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "DIST" }
      if WINDOWS then
        links
        {	
          "sfml-graphics-s",
          "sfml-window-s",
          "sfml-system-s",
          "sfml-audio-s",
          "sfml-network-s"
        }
      end
      runtime "Release"
      optimize "On"
      symbols "Off"