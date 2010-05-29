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
#include <string>
#include <fstream>
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

	Passion::RenderWindow* window = render->CreateRenderWindow( 1280, 720, "Deform" );

	Passion::Model tankModel = render->LoadModel( "models/tank.obj" );
	Passion::Texture tankTexture = render->LoadTexture( "textures/models/tank.tga" );

	render->SetTexturingEnabled( true );
	render->SetDepthEnabled( true );
	render->SetTexture( tankTexture );

	input->SetWindow( window );

	////////////////////////////////////////////////////////////
	// Load a simple sine wav shader
	////////////////////////////////////////////////////////////

	Passion::Program shader = 0;

	if ( render->SupportsShaders() )
	{
		Passion::Shader deform[2];
		deform[0] = render->CreateShader( LoadShader( "shaders/wave.vs" ).c_str(), VERTEX_SHADER );
		deform[1] = render->CreateShader( LoadShader( "shaders/null.ps" ).c_str(), PIXEL_SHADER );

		shader = render->CreateProgram( deform, 2 );
		render->SetProgram( shader );
	}

	while ( input->GetEvents() )
	{
		render->Clear( Passion::Color( 0.0f, 0.0f, 0.0f ) );
		render->ClearZ();

		if ( shader != 0 ) render->SetProgramFloat( "time", (float)clock() / (float)CLOCKS_PER_SEC );

		render->Start3D( Passion::Vector( -200.0f, -200.0f, 150.0f ), Passion::Vector( 0.0f, 10.0f, 30.0f ) );
			render->DrawModel( tankModel );
		render->End3D();

		render->Present();
	}

	return 0;
}