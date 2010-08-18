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
// Input library
////////////////////////////////////////////////////////////

class input
{
public:
	SCRIPT_FUNCTION( MouseX )
	{
		g_Lua->Push( g_Input->GetMouseX() );
		return 1;
	}

	SCRIPT_FUNCTION( MouseY )
	{
		g_Lua->Push( g_Input->GetMouseY() );
		return 1;
	}

	SCRIPT_FUNCTION( MouseWheel )
	{
		g_Lua->Push( g_Input->GetMouseWheel() );
		return 1;
	}

	SCRIPT_FUNCTION( IsMouseDown )
	{
		g_Lua->Push( g_Input->IsMouseDown( (Passion::MouseButton)g_Lua->Get( 1 )->GetInteger() ) );
		return 1;
	}

	SCRIPT_FUNCTION( IsKeyDown )
	{
		g_Lua->Push( g_Input->IsKeyDown( (Passion::Key)g_Lua->Get( 1 )->GetInteger() ) );
		return 1;
	}

	static void Bind()
	{
		std::auto_ptr<BaseScriptValue> input = g_Lua->NewTable();

		input->GetMember( "MouseX" )->Set( MouseX );
		input->GetMember( "MouseY" )->Set( MouseY );
		input->GetMember( "MouseWheel" )->Set( MouseWheel );

		input->GetMember( "IsMouseDown" )->Set( IsMouseDown );
		input->GetMember( "IsKeyDown" )->Set( IsKeyDown );

		g_Lua->Globals()->GetMember( "input" )->Set( input );
	}
};