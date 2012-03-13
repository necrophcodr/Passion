include( "../../build" )

solution "samples"
	configurations { "Debug", "Release" }

	configuration "Debug"
		flags { "Symbols" }

	libdirs { "../../passion/bin/bin" }
	links { "PASSION/input", "dl", "GL", "GLU", "SOIL", "PASSION/render", "PASSION/network", "lua", "PASSION/scripting" }
	includedirs { "../../include" }
	targetdir( "../bin" )
	if( _ACTION == nil ) then
		location( "projects/" )
	else
		location( "projects/" .. _ACTION )
	end

	project "blur"
		kind "WindowedApp"
		language "C++"
		files { "../blur/*.cpp" }
	project "deform"
		kind "WindowedApp"
		language "C++"
		files { "../deform/*.cpp" }
	project "gameoflife"
		kind "WindowedApp"
		language "C++"
		files { "../gameoflife/*.cpp" }
	project "interpreter"
		kind "ConsoleApp"
		language "C++"
		files { "../interpreter/*.cpp" }
	project "ip"
		kind "WindowedApp"
		language "C++"
		files { "../ip/*.cpp" }
	project "julia"
		kind "WindowedApp"
		language "C++"
		files { "../julia/*.cpp" }
