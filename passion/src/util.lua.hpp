////////////////////////////////////////////////////////////
//
//  Copyright (C) 2010 Alexander Overvoorde (overv161@gmail.com)
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// util.lua - Contains utility functions
////////////////////////////////////////////////////////////

const char* util_lua =
"function math.Clamp( val, min, max ) \
	return math.max( min, math.min( val, max ) ) \
end \
\
function Color( r, g, b, a ) \
	return { r = math.Clamp( r, 0, 255 ), g = math.Clamp( g, 0, 255 ), b = math.Clamp( b, 0, 255 ), a = math.Clamp( a or 255, 0, 255 ) } \
end \
\
function Vector( x, y, z ) \
	return { x = x, y = y, z = z or 0 } \
end \
\
function Vertex( pos, color, u, v ) \
	return { pos = pos, color = color or Color( 255, 255, 255, 255 ), u = u or 0, v = v or 0 } \
end \
";