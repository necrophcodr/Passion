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
#include <time.h>
#include <string>
#include <fstream>

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
		Passion::IBaseRender* render = (Passion::IBaseRender*)Passion::CreateInterface( "../../lib/passion-render-d" );
		Passion::IBaseInput* input = (Passion::IBaseInput*)Passion::CreateInterface( "../../lib/passion-input-d" );
	#else
		Passion::IBaseRender* render = (Passion::IBaseRender*)Passion::CreateInterface( "../../lib/passion-render" );
		Passion::IBaseInput* input = (Passion::IBaseInput*)Passion::CreateInterface( "../../lib/passion-input" );
	#endif

	Passion::RenderWindow* window = render->CreateRenderWindow( 1280, 720, "Post-processing" );

	Passion::Texture pattern = render->LoadTexture( "textures/box.png" );

	input->SetWindow( window );

	render->SetTexturingEnabled( true );
	render->SetDepthEnabled( true );
	render->SetAlphaBlendingEnabled( true );

	////////////////////////////////////////////////////////////
	// Load a simple PP shader
	////////////////////////////////////////////////////////////

	Passion::Shader boxblur[2];
	boxblur[0] = render->CreateShader( LoadShader( "shaders/null.vs" ).c_str(), VERTEX_SHADER );
	boxblur[1] = render->CreateShader( LoadShader( "shaders/boxblur.ps" ).c_str(), PIXEL_SHADER );

	Passion::Program postfx = render->CreateProgram( boxblur, 2 );

	////////////////////////////////////////////////////////////
	// Create a render target
	////////////////////////////////////////////////////////////

	Passion::BaseRenderTarget* rt = render->CreateRenderTarget( 1280, 720 );

	while ( input->GetEvents() )
	{
		render->SetRenderTarget( rt );
		render->SetDepthEnabled( true );

		render->Clear( Passion::Color( 0.1f, 0.1f, 0.1f ) );
		render->ClearZ();
		
		render->Start3D( Passion::Vector( cos( (float)clock() / (float)CLOCKS_PER_SEC ) * 250.0f, sin( (float)clock() / (float)CLOCKS_PER_SEC ) * 250.0f, 200.0f ), Passion::Vector() );
			render->SetTexture( pattern );
			render->SetProgram();

			render->SetDrawColor( Passion::Color( 1.0f, 1.0f, 0.0f ) );
			render->DrawBox( Passion::Vector( -30.0f, -30.0f, -30.0f ), Passion::Vector( 30.0f, 30.0f, 30.0f ) );

			render->SetDrawColor( Passion::Color( 0.0f, 1.0f, 0.0f ) );
			render->DrawBox( Passion::Vector( 40.0f, -30.0f, -30.0f ), Passion::Vector( 100.0f, 30.0f, 30.0f ) );

			render->SetDrawColor( Passion::Color( 1.0f, 0.0f, 0.0f ) );
			render->DrawBox( Passion::Vector( -40.0f, -30.0f, -30.0f ), Passion::Vector( -100.0f, 30.0f, 30.0f ) );
		render->End3D();

		render->SetRenderTarget();
		render->SetDepthEnabled( false );

		render->Clear( Passion::Color( 0.0f, 0.0f, 0.0f ) );

		render->Start2D();
			render->SetDrawColor( Passion::Color( 1.0f, 1.0f, 1.0f, 1.0f ) );
			render->SetTexture( rt->GetTexture() );
			render->SetProgram( postfx );
			
			render->DrawQuad( Passion::Vector( 0.0f, 0.0f ), Passion::Vector( 1280.0f, 0.0f ), Passion::Vector( 1280.0f, 720.0f ), Passion::Vector( 0.0f, 720.0f ) );
		render->End2D();

		render->Present();
	}

	return 0;
}