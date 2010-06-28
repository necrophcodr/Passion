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
#include <fstream>
#include <iostream>
#include <cstring>
#include <time.h>

////////////////////////////////////////////////////////////
// Helper function for loading the shaders
////////////////////////////////////////////////////////////

std::string LoadShader( const char* filename )
{
	char buffer[4096];
	memset( buffer, 0, 4096 );

	std::fstream shaderFile( filename, std::fstream::in );
		shaderFile.read( buffer, 4096 );
	shaderFile.close();

	return buffer;
}

////////////////////////////////////////////////////////////
// Entry point of the application
////////////////////////////////////////////////////////////

int main()
{
	////////////////////////////////////////////////////////////
	// Set up the renderer
	////////////////////////////////////////////////////////////

	#ifdef _DEBUG_
		Passion::IBaseRender* render = (Passion::IBaseRender*)Passion::CreateInterface( "../../lib/render-d" );
		Passion::IBaseInput* input = (Passion::IBaseInput*)Passion::CreateInterface( "../../lib/input-d" );
	#else
		Passion::IBaseRender* render = (Passion::IBaseRender*)Passion::CreateInterface( "../../lib/render" );
		Passion::IBaseInput* input = (Passion::IBaseInput*)Passion::CreateInterface( "../../lib/input" );
	#endif

	Passion::RenderWindow* window = render->CreateRenderWindow( 1280, 720, "Julia Set" );

	if ( !render->SupportsShaders() )
	{
		std::cout << "Shaders and/or render targets aren't supported by your graphics card!" << std::endl;
		return 1;
	}

	render->SetAlphaBlendingEnabled( true );

	input->SetWindow( window );

	////////////////////////////////////////////////////////////
	// Load the julia set shader
	////////////////////////////////////////////////////////////

	Passion::Shader julia[2];
	julia[0] = render->CreateShader( LoadShader( "shaders/julia.ps" ).c_str(), PIXEL_SHADER );
	julia[1] = render->CreateShader( LoadShader( "shaders/julia.vs" ).c_str(), VERTEX_SHADER );

	Passion::Program juliaShader = render->CreateProgram( julia, 2 );
	render->SetProgram( juliaShader );

	////////////////////////////////////////////////////////////
	// Set up the julia
	////////////////////////////////////////////////////////////

	bool dragging = false;
	int lastX, lastY;
	float centerRe, centerIm, scaleRe, scaleIm;

	centerRe = -0.0f;
	centerIm = 0.0f;
	scaleRe = 2.2f;
	scaleIm = 1.32f;

	render->SetProgramFloat( "centerRe", centerRe );
	render->SetProgramFloat( "centerIm", centerIm );
	render->SetProgramFloat( "scaleRe", scaleRe );
	render->SetProgramFloat( "scaleIm", scaleIm );

	float time = 0.0f;
	clock_t lastTime = clock();

	while ( input->GetEvents() )
	{
		if ( clock() - lastTime > 0 ) {
			time += (float)( clock() - lastTime ) / 1000.0f / ( 4.0f / scaleRe ) / 2.0f;
			lastTime = clock();
		}

		render->SetProgramFloat( "time", time );

		////////////////////////////////////////////////////////////
		// Calculate the center and scale based on input
		////////////////////////////////////////////////////////////

		if ( input->IsMouseDown( Passion::MOUSE_LEFT ) ) {
			if ( dragging ) {
				centerRe += (float)( lastX - input->GetMouseX() ) / 1280.0f * scaleRe * 2.0f;
				centerIm += (float)( lastY - input->GetMouseY() ) / 800.0f * scaleIm * 2.0f;
			} else {
				dragging = true;
			}

			lastX = input->GetMouseX();
			lastY = input->GetMouseY();

			render->SetProgramFloat( "centerRe", centerRe );
			render->SetProgramFloat( "centerIm", centerIm );
		} else {
			dragging = false;
		}

		if ( input->GetMouseWheel() > 0 ) {
			scaleRe /= 1.2f;
			scaleIm /= 1.2f;

			render->SetProgramFloat( "scaleRe", scaleRe );
			render->SetProgramFloat( "scaleIm", scaleIm );
		} else if ( input->GetMouseWheel() < 0 ) {
			scaleRe *= 1.2f;
			scaleIm *= 1.2f;

			render->SetProgramFloat( "scaleRe", scaleRe );
			render->SetProgramFloat( "scaleIm", scaleIm );
		}

		////////////////////////////////////////////////////////////
		// Draw the julia quad filling the whole screen
		////////////////////////////////////////////////////////////

		render->Clear( Passion::Color( 0.05f, 0.05f, 0.05f ) );

		render->Start2D();

			render->DrawQuad( Passion::Vector( 0.0f, 0.0f ), Passion::Vector( 1280.0f, 0.0f ), Passion::Vector( 1280.0f, 800.0f ), Passion::Vector( 0.0f, 800.0f ) );

		render->End2D();

		render->Present();
	}

	return 0;
}
