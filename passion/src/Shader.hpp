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
// Shader object
////////////////////////////////////////////////////////////

class Shader
{
public:
	SCRIPT_FUNCTION( Constructor )
	{
		if ( g_Lua->Get( 1 )->IsString() && g_Lua->Get( 2 )->IsString() )
		{
			Passion::Shader shaders[2];
				shaders[0] = g_Render->CreateShader( g_Lua->Get( 1 )->GetString(), VERTEX_SHADER );
				shaders[1] = g_Render->CreateShader( g_Lua->Get( 2 )->GetString(), PIXEL_SHADER );
			Passion::Program program = g_Render->CreateProgram( shaders, 2 );

			g_Lua->Push( &program, sizeof( int ), g_Lua->Registry()->GetMember( "Shader" ) );
		} else {
			if ( g_Lua->Get( 1 )->IsString() ) g_Lua->Error( 2, "string" ); else g_Lua->Error( 1, "string" );
		}

		return 1;
	}

	SCRIPT_FUNCTION( SetFloat )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Shader" ) ) )
			g_Lua->Error( 1, "Shader" );
		if ( !g_Lua->Get( 2 )->IsString() )
			g_Lua->Error( 2, "string" );
		if ( !g_Lua->Get( 3 )->IsNumber() )
			g_Lua->Error( 3, "number" );
		
		g_Render->SetProgram( *reinterpret_cast<Passion::Shader*>( g_Lua->Get( 1 )->GetUserData() ) );

		const char* name = g_Lua->Get( 2 )->GetString();
		int vars = 0;
		for ( int i = 3; g_Lua->Get( i )->IsNumber(); i++ )
			vars++;

		switch ( vars )
		{
			case 1: g_Render->SetProgramFloat( name, g_Lua->Get( 3 )->GetFloat() ); break;
			case 2: g_Render->SetProgramFloat( name, g_Lua->Get( 3 )->GetFloat(), g_Lua->Get( 4 )->GetFloat() ); break;
			case 3: g_Render->SetProgramFloat( name, g_Lua->Get( 3 )->GetFloat(), g_Lua->Get( 4 )->GetFloat(), g_Lua->Get( 5 )->GetFloat() ); break;
			case 4: g_Render->SetProgramFloat( name, g_Lua->Get( 3 )->GetFloat(), g_Lua->Get( 4 )->GetFloat(), g_Lua->Get( 5 )->GetFloat(), g_Lua->Get( 6 )->GetFloat() ); break;
			default: g_Lua->Error( "Too many integers!" );
		}
		
		return 0;
	}

	SCRIPT_FUNCTION( SetInt )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Shader" ) ) )
			g_Lua->Error( 1, "Shader" );
		if ( !g_Lua->Get( 2 )->IsString() )
			g_Lua->Error( 2, "string" );
		if ( !g_Lua->Get( 3 )->IsNumber() )
			g_Lua->Error( 3, "number" );
		
		g_Render->SetProgram( *reinterpret_cast<Passion::Shader*>( g_Lua->Get( 1 )->GetUserData() ) );

		const char* name = g_Lua->Get( 2 )->GetString();
		int vars = 0;
		for ( int i = 3; g_Lua->Get( i )->IsNumber(); i++ )
			vars++;

		switch ( vars )
		{
			case 1: g_Render->SetProgramInt( name, g_Lua->Get( 3 )->GetInteger() ); break;
			case 2: g_Render->SetProgramInt( name, g_Lua->Get( 3 )->GetInteger(), g_Lua->Get( 4 )->GetInteger() ); break;
			case 3: g_Render->SetProgramInt( name, g_Lua->Get( 3 )->GetInteger(), g_Lua->Get( 4 )->GetInteger(), g_Lua->Get( 5 )->GetInteger() ); break;
			case 4: g_Render->SetProgramInt( name, g_Lua->Get( 3 )->GetInteger(), g_Lua->Get( 4 )->GetInteger(), g_Lua->Get( 5 )->GetInteger(), g_Lua->Get( 6 )->GetInteger() ); break;
			default: g_Lua->Error( "Too many integers!" );
		}
		
		return 0;
	}

	static void Bind()
	{
		g_Lua->Globals()->GetMember( "Shader" )->Set( Constructor );
		
		std::auto_ptr<BaseScriptValue> meta = g_Lua->NewTable();
			meta->GetMember( "SetFloat" )->Set( SetFloat );
			meta->GetMember( "SetInt" )->Set( SetInt );
			
			meta->GetMember( "__index" )->Set( meta.get() );
		g_Lua->Registry()->GetMember( "Shader" )->Set( meta );
	}
};