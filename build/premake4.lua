solution "PASSION"
	configurations { "Debug", "Release" }

	configuration "Debug"
		defines { "_DEBUG_" }
		flags { "Symbols" }
	configuration "Release"
		flags { "Optimize" }

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
		includedirs { "../include" }
		files { "../src/Passion/Input/**.cpp" }
	project "render"
		kind "SharedLib"
		language "C++"
		includedirs { "../include", "../extlibs/headers" }
		libdirs { "../extlibs/lib" }
		files { "../src/Passion/Render/**.cpp" }
		links { "GL", "SOIL" }
	project "network"
		kind "SharedLib"
		language "C++"
		includedirs { "../include" }
		files { "../src/Passion/Network/**.cpp" }
	project "scripting"
		kind "SharedLib"
		language "C++"
		includedirs { "../include", "../extlibs/headers" }
		files { "../src/Passion/Scripting/**.cpp" }
		links { "dl", "lua" }
	project "passion"
		kind "WindowedApp"
		language "C++"
		targetdir( "../passion/bin" )
		libdirs { "../extlibs/lib", "../passion/bin/bin" }
		includedirs { "../include", "../extlibs/headers" }
		files { "../passion/src/**.cpp", "../src/Passion/Render/GLee/GLee.c" }
		links { "GL", "GLU", "SOIL", "dl" }
