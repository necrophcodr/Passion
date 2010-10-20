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

class Matrix
{
public:
	SCRIPT_FUNCTION( Constructor )
	{
		Passion::Matrix matrix;
		g_Lua->Push( &matrix, sizeof( matrix ), g_Lua->Registry()->GetMember( "Matrix" ) );
		
		return 1;
	}

	SCRIPT_FUNCTION( Translate )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
			g_Lua->Error( 1, "Matrix" );
		
		Passion::Matrix* matrix = (Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
		matrix->Translate( GetVector( 2 ) );

		return 0;
	}
	
	SCRIPT_FUNCTION( Scale )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
			g_Lua->Error( 1, "Matrix" );
		
		Passion::Matrix* matrix = (Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
		matrix->Scale( GetVector( 2 ) );

		return 0;
	}

	SCRIPT_FUNCTION( RotateX )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
			g_Lua->Error( 1, "Matrix" );
		if ( !g_Lua->Get( 2 )->IsNumber() ) g_Lua->Error( 2, "number" );

		Passion::Matrix* matrix = (Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
		matrix->RotateX( g_Lua->Get( 2 )->GetFloat() );

		return 0;
	}

	SCRIPT_FUNCTION( RotateY )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
			g_Lua->Error( 1, "Matrix" );
		if ( !g_Lua->Get( 2 )->IsNumber() ) g_Lua->Error( 2, "number" );

		Passion::Matrix* matrix = (Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
		matrix->RotateY( g_Lua->Get( 2 )->GetFloat() );
		return 0;
	}

	SCRIPT_FUNCTION( RotateZ )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
			g_Lua->Error( 1, "Matrix" );
		if ( !g_Lua->Get( 2 )->IsNumber() ) g_Lua->Error( 2, "number" );

		Passion::Matrix* matrix = (Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
		matrix->RotateZ( g_Lua->Get( 2 )->GetFloat() );

		return 0;
	}

	SCRIPT_FUNCTION( Invert )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
			g_Lua->Error( 1, "Matrix" );
		
		Passion::Matrix* matrix = (Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
		bool result = matrix->Invert();

		g_Lua->Push( result );

		return 1;
	}

	SCRIPT_FUNCTION( Multiply )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
			g_Lua->Error( 1, "Matrix" );

		if ( g_Lua->Get( 2 )->IsTable() && g_Lua->Get( 2 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Vector" ) ) )
		{
			Passion::Matrix mat = *(Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
			Passion::Vector vec = GetVector( 2 );

			Passion::Vector result = mat * vec;

			std::auto_ptr<BaseScriptValue> vector = g_Lua->NewTable();
				vector->GetMember( "x" )->Set( result.x );
				vector->GetMember( "y" )->Set( result.y );
				vector->GetMember( "z" )->Set( result.z );
			vector->SetMetaTable( g_Lua->Registry()->GetMember( "Vector" ) );

			g_Lua->Push( vector );
		} else {
			if ( !g_Lua->Get( 2 )->IsUserData() || !g_Lua->Get( 2 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
				g_Lua->Error( 2, "Matrix" );

			Passion::Matrix mat = *(Passion::Matrix*)g_Lua->Get( 1 )->GetUserData();
			Passion::Matrix mat2 = *(Passion::Matrix*)g_Lua->Get( 2 )->GetUserData();

			Passion::Matrix result = mat * mat2;

			g_Lua->Push( &result, sizeof( result ), g_Lua->Registry()->GetMember( "Matrix" ) );
		}

		return 1;
	}

	static void Bind()
	{
		g_Lua->Globals()->GetMember( "Matrix" )->Set( Constructor );
		
		std::auto_ptr<BaseScriptValue> meta = g_Lua->NewTable();
			meta->GetMember( "Translate" )->Set( Translate );
			meta->GetMember( "Scale" )->Set( Scale );

			meta->GetMember( "RotateX" )->Set( RotateX );
			meta->GetMember( "RotateY" )->Set( RotateY );
			meta->GetMember( "RotateZ" )->Set( RotateZ );

			meta->GetMember( "Invert" )->Set( Invert );

			meta->GetMember( "__mul" )->Set( Multiply );

			meta->GetMember( "__index" )->Set( meta.get() );
		g_Lua->Registry()->GetMember( "Matrix" )->Set( meta );
	}
};