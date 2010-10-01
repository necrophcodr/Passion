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

#include "Interfaces.hpp"

////////////////////////////////////////////////////////////
// Texture object
////////////////////////////////////////////////////////////

class Texture
{
public:
	SCRIPT_FUNCTION( Constructor )
	{
		if ( g_Lua->Get( 1 )->IsString() )
		{
			const char* path = g_Lua->Get( 1 )->GetString();
				int tex = g_Render->LoadTexture( path, g_Lua->Get( 2 )->GetBoolean() );
			g_Lua->Push( &tex, sizeof( int ), g_Lua->Registry()->GetMember( "Texture" ) );
		} else {
			g_Lua->Error( 1, "string" );
		}

		return 1;
	}

	static void Bind()
	{
		g_Lua->Globals()->GetMember( "Texture" )->Set( Constructor );
		
		std::auto_ptr<BaseScriptValue> meta = g_Lua->NewTable();
			meta->GetMember( "__index" )->Set( meta.get() );
		g_Lua->Registry()->GetMember( "Texture" )->Set( meta );
	}
};