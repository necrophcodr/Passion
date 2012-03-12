solution "PASSION"
	configurations { "Debug", "Release" }

	includedirs { "../include", "../extlibs/headers" }
	libdirs { "../extlibs/lib", "../lib" }
	if( _ACTION == nil ) then
		location "projects/"
	else
		location( "projects/" .. _ACTION )
	end

	project "input"
		kind "SharedLib"
		language "C++"
		files { "../src/Passion/Input/**.cpp" }
	project "render"
		kind "SharedLib"
		language "C++"
		files { "../src/Passion/Render/**.cpp" }
		links { "GL", "SOIL" }
	project "network"
		kind "SharedLib"
		language "C++"
		files { "../src/Passion/Network/**.cpp" }
	project "scripting"
		kind "SharedLib"
		language "C++"
		files { "../src/Passion/Scripting/**.cpp" }
		links { "dl", "lua" }
	project "Passion"
		kind "WindowedApp"
		language "C++"
		files { "../passion/src/**.cpp", "../src/Passion/Render/GLee/GLee.c" }
		links { "input", "GL", "GLU", "SOIL", "render", "network", "dl", "scripting" }
