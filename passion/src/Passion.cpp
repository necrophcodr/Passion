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

#include <Passion/Render.hpp>
#include <Passion/Input.hpp>
#include <Passion/Scripting.hpp>
#include <Passion/Network.hpp>
#include <fstream>

// Convenience
using Passion::BaseScriptValue;

////////////////////////////////////////////////////////////
// Error handling
////////////////////////////////////////////////////////////

int handleError( const char* error )
{
	std::cout << error << std::endl;

	getchar();
	return 1;
}

int main( int argc, const char* argv[] )
{
	////////////////////////////////////////////////////////////
	// Load interfaces
	////////////////////////////////////////////////////////////

	#ifndef _DEBUG_
		Passion::IBaseRender*		render		= Passion::CreateInterface<Passion::IBaseRender>( "bin/render" );
		Passion::IBaseInput*		input		= Passion::CreateInterface<Passion::IBaseInput>( "bin/input" );
		Passion::IBaseScripting*	scripting	= Passion::CreateInterface<Passion::IBaseScripting>( "bin/scripting" );
		Passion::IBaseNetwork*		network		= Passion::CreateInterface<Passion::IBaseNetwork>( "bin/network" );
	#else
		Passion::IBaseRender*		render		= Passion::CreateInterface<Passion::IBaseRender>( "bin/render-d" );
		Passion::IBaseInput*		input		= Passion::CreateInterface<Passion::IBaseInput>( "bin/input-d" );
		Passion::IBaseScripting*	scripting	= Passion::CreateInterface<Passion::IBaseScripting>( "bin/scripting-d" );
		Passion::IBaseNetwork*		network		= Passion::CreateInterface<Passion::IBaseNetwork>( "bin/network-d" );
	#endif

	////////////////////////////////////////////////////////////
	// Create script state and initialize the GAME table
	////////////////////////////////////////////////////////////

	Passion::BaseScriptState* script = scripting->CreateState();
	std::auto_ptr<BaseScriptValue> GAME = script->Globals()->GetMember( "GAME" );
	GAME->Set( script->NewTable() );

	////////////////////////////////////////////////////////////
	// Load specified code or default code
	////////////////////////////////////////////////////////////
	
	if ( argc > 1 ) {
		std::ifstream i ( argv[1], std::ios::in );
		if ( i.is_open() ) {
			i.close();

			script->DoFile( argv[1] );
		} else {
			i.close();
			
			std::cout << argv[1] << ": No such file or directory!" << std::endl;

			return 1;
		}
	} else {
		script->DoString( "print( \"No default screen code.\" )" );
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
		std::auto_ptr<BaseScriptValue> config = script->NewTable();

		config->GetMember( "Title" )->Set( title );
		config->GetMember( "Fullscreen" )->Set( fullscreen );
		config->GetMember( "Width" )->Set( w );
		config->GetMember( "Height" )->Set( h );
		
		script->Push( GAME->GetMember( "Config" ).get() );
		script->Push( GAME.get() );
		script->Push( config );

		if ( script->Call( 2, 1 ) ) {
			config = script->Get( -1 );

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
			return handleError( script->Error() );
	}

	window = render->CreateRenderWindow( w, h, title, fullscreen );
	input->SetWindow( window );
	
	////////////////////////////////////////////////////////////
	// Initialize game and load resources
	////////////////////////////////////////////////////////////

	if ( GAME->IsTable() && GAME->GetMember( "Initialize" )->IsFunction() ) {
		script->Push( GAME->GetMember( "Initialize" ).get() );
		script->Push( GAME.get() );

		if ( !script->Call( 1, 0 ) )
			return handleError( script->Error() );
	}
	
	while ( input->GetEvents() )
	{
		////////////////////////////////////////////////////////////
		// Update
		////////////////////////////////////////////////////////////

		if ( GAME->IsTable() && GAME->GetMember( "Update" )->IsFunction() ) {
			script->Push( GAME->GetMember( "Update" ).get() );
			script->Push( GAME.get() );
			script->Push( render->FrameTime() );

			if ( !script->Call( 2, 0 ) )
				return handleError( script->Error() );
		}

		////////////////////////////////////////////////////////////
		// Draw
		////////////////////////////////////////////////////////////

		if ( GAME->IsTable() && GAME->GetMember( "Draw" )->IsFunction() ) {
			script->Push( GAME->GetMember( "Draw" ).get() );
			script->Push( GAME.get() );

			if ( !script->Call( 1, 0 ) )
				return handleError( script->Error() );
		}
	}

	////////////////////////////////////////////////////////////
	// Unload
	////////////////////////////////////////////////////////////

	if ( GAME->IsTable() && GAME->GetMember( "Unload" )->IsFunction() ) {
		script->Push( GAME->GetMember( "Unload" ).get() );
		script->Push( GAME.get() );

		if ( !script->Call( 1, 0 ) )
			return handleError( script->Error() );
	}

	// Destroy script state
	scripting->DestroyState( script );

	// Clean up
	Passion::DestroyInterface<Passion::IBaseRender>( render );
	Passion::DestroyInterface<Passion::IBaseInput>( input );
	Passion::DestroyInterface<Passion::IBaseScripting>( scripting );
	Passion::DestroyInterface<Passion::IBaseNetwork>( network );
	
	return 0;
}