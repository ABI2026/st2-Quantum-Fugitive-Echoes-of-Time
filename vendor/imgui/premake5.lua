project "ImGui"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",
		"imgui-SFML.cpp",
		"imgui-SFML.h",
		"imconfig-SFML.h",
		"imgui-SFML_export.h"
	}
	defines {"SFML_STATIC"}
 
	libdirs { "../SFML/SFML-3.0.0/lib" }

	 links
	 {
		"freetype",
		"winmm",
		"gdi32",
        --"flac",
		"vorbisenc",
		"vorbisfile",
		"vorbis",
		"ogg",
		"ws2_32",
        "legacy_stdio_definitions" --falls was schiefgeht einfach wieder reinmachen
	 }
	includedirs
	{
	 "../glm",
	 "../SFML/SFML-3.0.0/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
        links
        {
        		"opengl32",
        }

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
        links
        {
            "GL"
        }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		links
		{	
			"sfml-graphics-s-d",
			"sfml-window-s-d",
			"sfml-system-s-d",
            "sfml-audio-s-d",
			"sfml-network-s-d"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		links
		{	
			"sfml-graphics-s",
			"sfml-window-s",
			"sfml-system-s",
			"sfml-audio-s",
			"sfml-network-s"
		}

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
		links
		{	
			"sfml-graphics-s",
			"sfml-window-s",
			"sfml-system-s",
            "sfml-audio-s",
			"sfml-network-s"
		}
