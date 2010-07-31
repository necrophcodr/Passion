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

// Headers
#include <Passion/Render.hpp>
#include <Passion/Input.hpp>
#include <Passion/Scripting.hpp>
#include <Passion/Network.hpp>
#include <ctime>
#include "CDefaultScene.hpp"

int main()
{
	// Load the interfaces
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

	// Create scripting state
	Passion::BaseScriptState* script = scripting->CreateState();

	// GAME:Initialize (Creates window too!)

	Passion::RenderWindow* window = render->CreateRenderWindow( 1280, 720, "Passion", false );
	input->SetWindow( window );

	render->SetAlphaBlendingEnabled( true );
	render->SetTexturingEnabled( true );
	
	//  GAME:Load()

	// NO GAME SCREEN, SHOULD BE LUA BASED AND REMOVED
	CDefaultScene scene( render );
	// END OF NO GAME SCREEN

	// Game loop
	while ( input->GetEvents() )
	{
		// GAME:KeyPress() or GAME:MousePress etc.
		// GAME:Update()

		// GAME:Draw()
		
		// NO GAME SCREEN, SHOULD BE LUA BASED AND REMOVED
			render->Clear( Passion::Color( 0.0f, 0.0f, 0.0f ) );

			// Draw default scene
			scene.Update( render->FrameTime() );
			scene.Draw();

			render->Present();
		// END OF NO GAME SCREEN
	}

	// GAME:Unload()

	// Destroy script state
	//scripting->DestroyState( script );

	// Clean up
	Passion::DestroyInterface<Passion::IBaseRender>( render );
	Passion::DestroyInterface<Passion::IBaseInput>( input );
	Passion::DestroyInterface<Passion::IBaseScripting>( scripting );
	Passion::DestroyInterface<Passion::IBaseNetwork>( network );

	return 0;
}