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
// Vector object
////////////////////////////////////////////////////////////

class Vector
{
public:
	static void pushVector( Passion::Vector v )
	{
		std::auto_ptr<BaseScriptValue> vector = g_Lua->NewTable();
			vector->GetMember( "x" )->Set( v.x );
			vector->GetMember( "y" )->Set( v.y );
			vector->GetMember( "z" )->Set( v.z );
			
			vector->SetMetaTable( g_Lua->Registry()->GetMember( "Vector" ) );
		g_Lua->Push( vector );
	}

	SCRIPT_FUNCTION( Constructor )
	{
		pushVector( Passion::Vector( g_Lua->Get( 1 )->GetFloat(), g_Lua->Get( 2 )->GetFloat(), g_Lua->Get( 3 )->GetFloat() ) );
		
		return 1;
	}

	SCRIPT_FUNCTION( __tostring )
	{
		Passion::Vector vec = GetVector( 1 );

		char str[64];
		sprintf( str, "( %f, %f, %f )", vec.x, vec.y, vec.z );

		g_Lua->Push( str );

		return 1;
	}

	SCRIPT_FUNCTION( __add )
	{
		pushVector( GetVector( 1 ) + GetVector( 2 ) );
		return 1;
	}

	SCRIPT_FUNCTION( __sub )
	{
		pushVector( GetVector( 1 ) - GetVector( 2 ) );
		return 1;
	}

	SCRIPT_FUNCTION( __mul )
	{
		pushVector( GetVector( 1 ) * g_Lua->Get( 2 )->GetFloat() );
		return 1;
	}

	SCRIPT_FUNCTION( __div )
	{
		pushVector( GetVector( 1 ) / g_Lua->Get( 2 )->GetFloat() );
		return 1;
	}

	SCRIPT_FUNCTION( __unm )
	{
		pushVector( GetVector( 1 ) * -1.0f );
		return 1;
	}

	SCRIPT_FUNCTION( Distance )
	{
		g_Lua->Push( GetVector( 1 ).Distance( GetVector( 2 ) ) );
		return 1;
	}

	SCRIPT_FUNCTION( Length )
	{
		g_Lua->Push( GetVector( 1 ).Length() );
		return 1;
	}

	SCRIPT_FUNCTION( LengthSqr )
	{
		g_Lua->Push( GetVector( 1 ).LengthSqr() );
		return 1;
	}

	SCRIPT_FUNCTION( Dot )
	{
		g_Lua->Push( GetVector( 1 ).Dot( GetVector( 2 ) ) );
		return 1;
	}

	SCRIPT_FUNCTION( Cross )
	{
		pushVector( GetVector( 1 ).Cross( GetVector( 2 ) ) );
		return 1;
	}

	SCRIPT_FUNCTION( Normal )
	{
		pushVector( GetVector( 1 ).Normal() );
		return 1;
	}

	static void Bind()
	{
		g_Lua->Globals()->GetMember( "Vector" )->Set( Constructor );
		
		std::auto_ptr<BaseScriptValue> meta = g_Lua->NewTable();
			meta->GetMember( "__tostring" )->Set( __tostring );

			meta->GetMember( "__add" )->Set( __add );
			meta->GetMember( "__sub" )->Set( __sub );
			meta->GetMember( "__mul" )->Set( __mul );
			meta->GetMember( "__div" )->Set( __div );
			meta->GetMember( "__unm" )->Set( __unm );
			
			meta->GetMember( "Distance" )->Set( Distance );
			meta->GetMember( "Length" )->Set( Length );
			meta->GetMember( "LengthSqr" )->Set( LengthSqr );

			meta->GetMember( "Dot" )->Set( Dot );
			meta->GetMember( "Cross" )->Set( Cross );

			meta->GetMember( "Normal" )->Set( Normal );

			meta->GetMember( "__index" )->Set( meta.get() );
		g_Lua->Registry()->GetMember( "Vector" )->Set( meta );
	}
};