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
// Render library
////////////////////////////////////////////////////////////

class render
{
public:
	SCRIPT_FUNCTION( Clear )
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

		if ( g_Lua->Get( 1 )->IsTable() )
		{
			r = g_Lua->Get( 1 )->GetMember( "r" )->GetFloat() / 255.0f;
			g = g_Lua->Get( 1 )->GetMember( "g" )->GetFloat() / 255.0f;
			b = g_Lua->Get( 1 )->GetMember( "b" )->GetFloat() / 255.0f;
		}
		
		g_Render->Clear( Passion::Color( r, g, b ) );

		return 0;
	}

	SCRIPT_FUNCTION( Present )
	{
		g_Render->Present();
		return 0;
	}

	static void Bind()
	{
		std::auto_ptr<BaseScriptValue> render = g_Lua->NewTable();

		render->GetMember( "Clear" )->Set( Clear );
		render->GetMember( "Present" )->Set( Present );

		g_Lua->Globals()->GetMember( "render" )->Set( render );
	}
};
