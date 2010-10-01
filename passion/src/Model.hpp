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
// Model object
////////////////////////////////////////////////////////////

class Model
{
public:
	SCRIPT_FUNCTION( Constructor )
	{
		if ( g_Lua->Get( 1 )->IsTable() )
		{
			std::auto_ptr<BaseScriptValue> vertexTable = g_Lua->Get( 1 );
			std::vector<Passion::Vertex> vertices;

			for ( int i = 1; vertexTable->GetMember( i )->IsTable(); i++ )
			{
				vertexTable->GetMember( i )->GetMember( "pos" )->Push();
				Passion::Vector pos = GetVector( -1 );

				vertexTable->GetMember( i )->GetMember( "color" )->Push();
				Passion::Color color = GetColor( -1 );

				vertexTable->GetMember( i )->GetMember( "normal" )->Push();
				Passion::Vector normal = GetVector( -1 );

				vertices.push_back( Passion::Vertex( pos.x, pos.y, pos.z, color, vertexTable->GetMember( i )->GetMember( "u" )->GetFloat(), vertexTable->GetMember( i )->GetMember( "v" )->GetFloat(), normal.x, normal.y, normal.z ) );
			}

			Passion::Vertex* vertexArray = new Passion::Vertex[vertices.size()];

			for ( unsigned int i = 0; i < vertices.size(); i++ )
			{
				vertexArray[i] = vertices[i];
			}

			Passion::Model model = g_Render->CreateModel( vertexArray, vertices.size() );
			g_Lua->Push( &model, sizeof( Passion::Model ), g_Lua->Registry()->GetMember( "Model" ) );

			delete [] vertexArray;
		} else if ( g_Lua->Get( 1 )->IsString() ) {
			Passion::Model model = g_Render->LoadModel( g_Lua->Get( 1 )->GetString() );
			g_Lua->Push( &model, sizeof( Passion::Model ), g_Lua->Registry()->GetMember( "Model" ) );
		} else {
			g_Lua->Error( 1, "string" );
		}

		return 1;
	}
	
	SCRIPT_FUNCTION( Draw )
	{
		if ( g_Lua->Get( 1 )->IsUserData() && g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Model" ) ) )
		{
			Passion::Model* model = reinterpret_cast<Passion::Model*>( g_Lua->Get( 1 )->GetUserData() );
			g_Render->DrawModel( *model );
		} else {
			g_Lua->Error( 1, "Model" );
		}

		return 0;
	}

	static void Bind()
	{
		g_Lua->Globals()->GetMember( "Model" )->Set( Constructor );
		
		std::auto_ptr<BaseScriptValue> meta = g_Lua->NewTable();
			meta->GetMember( "Draw" )->Set( Draw );

			meta->GetMember( "__index" )->Set( meta.get() );
		g_Lua->Registry()->GetMember( "Model" )->Set( meta );
	}
};