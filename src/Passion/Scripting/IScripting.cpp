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
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Scripting/IScripting.hpp>
#include <string.h>

namespace Passion
{
	IScripting::IScripting()
	{
		lua = lua_open();
		luaL_openlibs( lua );
	}

	IScripting::~IScripting()
	{
		lua_close( lua );
	}

	bool IScripting::DoString( const char* code )
	{
		int err = luaL_loadstring( lua, code ) || lua_pcall( lua, 0, 0, 0 );

		if ( err )
		{
			strcpy( m_error, lua_tostring( lua, -1 ) );
			lua_pop( lua, -1 );
		}

		return err == 0;
	}

	const char* IScripting::Error()
	{
		return m_error;
	}
}
