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

#include <fstream>

// Base header
#include "Interfaces.hpp"

// Libraries
#include "render.hpp"
#include "input.hpp"

// Lua files
#include "util.lua.hpp"
#include "file.lua.hpp"
#include "enums.lua.hpp"

// Error handling
#include "ErrorHandling.hpp"

////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////

Passion::IBaseRender*		g_Render	= NULL;
Passion::IBaseInput*		g_Input		= NULL;

Passion::IBaseScripting*	g_Scripting	= NULL;
Passion::BaseScriptState*	g_Lua		= NULL;

//Passion::IBaseNetwork*		g_Network	= NULL;

////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////

int main( int argc, const char* argv[] )
{
	////////////////////////////////////////////////////////////
	// Load interfaces
	////////////////////////////////////////////////////////////

	#ifndef _DEBUG_
		g_Render	= Passion::CreateInterface<Passion::IBaseRender>( "bin/render" );
		g_Input		= Passion::CreateInterface<Passion::IBaseInput>( "bin/input" );
		g_Scripting	= Passion::CreateInterface<Passion::IBaseScripting>( "bin/scripting" );
		//g_Network	= Passion::CreateInterface<Passion::IBaseNetwork>( "bin/network" );
	#else
		g_Render	= Passion::CreateInterface<Passion::IBaseRender>( "bin/render-d" );
		g_Input		= Passion::CreateInterface<Passion::IBaseInput>( "bin/input-d" );
		g_Scripting	= Passion::CreateInterface<Passion::IBaseScripting>( "bin/scripting-d" );
		//g_Network	= Passion::CreateInterface<Passion::IBaseNetwork>( "bin/network-d" );
	#endif

	////////////////////////////////////////////////////////////
	// Create script state and initialize the GAME table
	////////////////////////////////////////////////////////////

	g_Lua = g_Scripting->CreateState();

	std::auto_ptr<BaseScriptValue> GAME = g_Lua->Globals()->GetMember( "GAME" );
	GAME->Set( g_Lua->NewTable() );

	////////////////////////////////////////////////////////////
	// Load libraries
	////////////////////////////////////////////////////////////

	render::Bind();
	input::Bind();

	////////////////////////////////////////////////////////////
	// Load Lua files
	////////////////////////////////////////////////////////////

	g_Lua->DoString( util_lua );
	g_Lua->DoString( file_lua );
	g_Lua->DoString( enums_lua );

	////////////////////////////////////////////////////////////
	// Load specified code or default code
	////////////////////////////////////////////////////////////
	
	if ( argc > 1 ) {
		std::ifstream i ( argv[1], std::ios::in );
		if ( i.is_open() ) {
			i.close();

			if ( !g_Lua->DoFile( argv[1] ) )
				return Error( g_Lua->Error() );			
		} else {
			i.close();
			
			std::cout << argv[1] << ": No such file or directory!" << std::endl;

			return 1;
		}
	} else {
		g_Lua->DoString( "print( \"No default screen code.\" )" );
	}

	////////////////////////////////////////////////////////////
	// Configure the window
	////////////////////////////////////////////////////////////

	Passion::RenderWindow* window;

	int w = 1280;
	int h = 720;
	char title[128] = "Passion 0.1a";
	bool fullscreen = false;

	if ( GAME->IsTable() && GAME->GetMember( "Config" )->IsFunction() ) {
		std::auto_ptr<BaseScriptValue> config = g_Lua->NewTable();

		config->GetMember( "Title" )->Set( title );
		config->GetMember( "Fullscreen" )->Set( fullscreen );
		config->GetMember( "Width" )->Set( w );
		config->GetMember( "Height" )->Set( h );
		
		g_Lua->Push( GAME->GetMember( "Config" ).get() );
		g_Lua->Push( GAME.get() );
		g_Lua->Push( config );

		if ( g_Lua->Call( 2, 1 ) ) {
			config = g_Lua->Get( -1 );

			if ( config->IsTable() && config->GetMember( "Title" )->IsString() && config->GetMember( "Fullscreen" )->IsBoolean() &&
				config->GetMember( "Width" )->IsNumber() && config->GetMember( "Height" )->IsNumber() )
			{
				w = config->GetMember( "Width" )->GetInteger();
				h = config->GetMember( "Height" )->GetInteger();
				strcpy( title, config->GetMember( "Title" )->GetString() );
				fullscreen = config->GetMember( "Fullscreen" )->GetBoolean();
			} else {
				std::cout << "Warning: Unexpected return value from GAME:Config, ignored." << std::endl;
			}
		} else
			return Error( g_Lua->Error() );
	}

	window = g_Render->CreateRenderWindow( w, h, title, fullscreen );
	g_Input->SetWindow( window );
	
	////////////////////////////////////////////////////////////
	// Initialize game and load resources
	////////////////////////////////////////////////////////////

	if ( GAME->IsTable() && GAME->GetMember( "Initialize" )->IsFunction() ) {
		g_Lua->Push( GAME->GetMember( "Initialize" ).get() );
		g_Lua->Push( GAME.get() );

		if ( !g_Lua->Call( 1, 0 ) )
			return Error( g_Lua->Error() );
	}
	
	while ( g_Input->GetEvents() )
	{
		////////////////////////////////////////////////////////////
		// Update
		////////////////////////////////////////////////////////////

		if ( GAME->IsTable() && GAME->GetMember( "Update" )->IsFunction() ) {
			g_Lua->Push( GAME->GetMember( "Update" ).get() );
			g_Lua->Push( GAME.get() );
			g_Lua->Push( g_Render->FrameTime() );

			if ( !g_Lua->Call( 2, 0 ) )
				return Error( g_Lua->Error() );
		}

		////////////////////////////////////////////////////////////
		// Draw
		////////////////////////////////////////////////////////////

		if ( GAME->IsTable() && GAME->GetMember( "Draw" )->IsFunction() ) {
			g_Lua->Push( GAME->GetMember( "Draw" ).get() );
			g_Lua->Push( GAME.get() );

			if ( !g_Lua->Call( 1, 0 ) )
				return Error( g_Lua->Error() );
		}
	}

	////////////////////////////////////////////////////////////
	// Unload
	////////////////////////////////////////////////////////////

	if ( GAME->IsTable() && GAME->GetMember( "Unload" )->IsFunction() ) {
		g_Lua->Push( GAME->GetMember( "Unload" ).get() );
		g_Lua->Push( GAME.get() );

		if ( !g_Lua->Call( 1, 0 ) )
			return Error( g_Lua->Error() );
	}

	// Destroy g_Lua state
	g_Scripting->DestroyState( g_Lua );

	// Clean up
	Passion::DestroyInterface<Passion::IBaseRender>( g_Render );
	Passion::DestroyInterface<Passion::IBaseInput>( g_Input );
	Passion::DestroyInterface<Passion::IBaseScripting>( g_Scripting );
	//Passion::DestroyInterface<Passion::IBaseNetwork>( g_Network );
	
	return 0;
}