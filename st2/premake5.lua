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
		--"winmm",
		--"gdi32",
       -- "flac",


    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "action:vs*"
        buildoptions { "/Zc:__cplusplus" }

   filter "system:windows"
      systemversion "latest"
      defines { "PLATFORM_WINDOWS","_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING" }
      links
      {
          "opengl32",
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
          "FLACd",
          "freetyped",		
          "oggd",
          "vorbisencd",
		  "vorbisfiled",
		  "vorbisd",
          "sfml-graphics-s-d",
          "sfml-window-s-d",
          "sfml-system-s-d",
          "sfml-audio-s-d",
          "sfml-network-s-d"
        }
        libdirs { "../vendor/SFML/SFML-3.0.0/lib/Debug" }

      end   


      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "RELEASE" }
      if WINDOWS then
        links
        {	
          "FLAC",
          "freetype",
          "ogg",
          "vorbisenc",
		  "vorbisfile",
		  "vorbis",
          "sfml-graphics-s",
          "sfml-window-s",
          "sfml-system-s",
          "sfml-audio-s",
          "sfml-network-s",
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
          "FLAC",
          "freetype",
          "ogg",
          "vorbisenc",
		  "vorbisfile",
		  "vorbis",
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