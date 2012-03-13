solution "PASSION"
	configurations { "Debug", "Release" }

	configuration "Debug"
		defines { "_DEBUG_" }
		flags { "Symbols" }
	configuration "Release"
		flags { "Optimize" }

	includedirs { "../include", "../extlibs/headers" }
	libdirs { "../extlibs/lib", "../lib", "../passion/bin/bin" }
	targetdir( "../passion/bin/bin" )
	if( _ACTION == nil ) then
		location "projects/"
	else
		location( "projects/" .. _ACTION )
	end
	targetprefix ""

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
	project "passion"
		kind "WindowedApp"
		language "C++"
		targetdir( "../passion/bin" )
		files { "../passion/src/**.cpp", "../src/Passion/Render/GLee/GLee.c" }
		--links { "input", "GL", "GLU", "SOIL", "render", "network", "dl", "scripting" }
		links { "GL", "GLU", "SOIL", "dl" }
