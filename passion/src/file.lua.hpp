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
// file.lua - Contains file functions
////////////////////////////////////////////////////////////

const char* file_lua =
"file = {} \
\
function file.Exists( filename ) \
	local f = io.open( filename ) \
	local e = f ~= nil \
	f:close() \
	return e \
end \
\
function file.Read( filename ) \
	local f = io.open( filename, \"r\" ) \
	if ( not f ) then return nil end \
	local c = f:read( \"*all\" ) \
	f:close() \
	return c \
end \
\
function file.Write( filename, ... ) \
	local f = io.open( filename, \"w\" ) \
	if ( not f ) then return nil end \
	f:write( ... ) \
	f:close() \
end \
";