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

using Passion::Vector;

int main()
{
	// Load the interfaces
	Passion::IBaseRender*		render		= Passion::CreateInterface<Passion::IBaseRender>( "bin/render" );
	Passion::IBaseInput*		input		= Passion::CreateInterface<Passion::IBaseInput>( "bin/input" );
	Passion::IBaseScripting*	scripting	= Passion::CreateInterface<Passion::IBaseScripting>( "bin/scripting" );
	Passion::IBaseNetwork*		network		= Passion::CreateInterface<Passion::IBaseNetwork>( "bin/network" );

	Passion::RenderWindow* window = render->CreateRenderWindow( 1280, 720, "Passion", false );
	input->SetWindow( window );

	render->SetAlphaBlendingEnabled( true );
	render->SetTexturingEnabled( true );

	CDefaultScene scene( render );
	clock_t ltime = clock();

	while ( input->GetEvents() )
	{
		render->Clear( Passion::Color( 0.0f, 0.0f, 0.0f ) );

		// Draw default scene
		scene.Update( (float)( clock() - ltime ) / (float)CLOCKS_PER_SEC );
		ltime = clock();
		scene.Draw();

		render->Present();
	}

	// Clean up
	delete network;
	delete scripting;
	delete input;
	delete render;

	return 0;
}