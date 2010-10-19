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
#include <cstdlib>
#include <cstring>

////////////////////////////////////////////////////////////
// Settings
////////////////////////////////////////////////////////////

const int generations = 5;
const float generationsPerSecond = 1.0f;
const int gridSize = 64;

////////////////////////////////////////////////////////////
// Entry point of the application
////////////////////////////////////////////////////////////

int main()
{
	////////////////////////////////////////////////////////////
	// Set up the renderer and load resources
	////////////////////////////////////////////////////////////

	#ifdef _DEBUG_
		Passion::IBaseRender* render = Passion::CreateInterface<Passion::IBaseRender>( "../../lib/render-d" );
		Passion::IBaseInput* input = Passion::CreateInterface<Passion::IBaseInput>( "../../lib/input-d" );
	#else
		Passion::IBaseRender* render = Passion::CreateInterface<Passion::IBaseRender>( "../../lib/render" );
		Passion::IBaseInput* input = Passion::CreateInterface<Passion::IBaseInput>( "../../lib/input" );
	#endif

	Passion::Window* window = render->CreateRenderWindow( 1280, 720, "Game of Life" );
	input->SetWindow( window );

	Passion::Texture texBox = render->LoadTexture( "textures/box.png" );
	render->SetTexture( texBox );

	render->SetDepthEnabled( true );
	render->SetAlphaBlendingEnabled( true );
	render->SetTexturingEnabled( true );

	////////////////////////////////////////////////////////////
	// Set up the grid and starting pattern
	////////////////////////////////////////////////////////////

	bool grid[gridSize][gridSize][generations];
	memset( grid, 0, gridSize * gridSize * generations );

	srand( clock() );

	for ( int x = 0; x < gridSize; x++ )
		for ( int y = 0; y < gridSize; y++ )
			grid[x][y][0] = rand() > RAND_MAX / 2 ? true : false;

	long nextUpdate = clock() + (long)( (float)CLOCKS_PER_SEC / generationsPerSecond );
	int grayOffset = 0;

	while ( input->GetEvents() )
	{
		////////////////////////////////////////////////////////////
		// Update the grid
		////////////////////////////////////////////////////////////

		if ( clock() > nextUpdate )
		{
			// First move everything one generation down
			for ( int gen = generations - 1; gen > 0; gen-- )
				for ( int x = 0; x < gridSize; x++ )
					for ( int y = 0; y < gridSize; y++ )
						grid[x][y][gen] = grid[x][y][gen-1];

			// Now generate the new generation
			for ( int x = 0; x < gridSize; x++ )
			{
				for ( int y = 0; y < gridSize; y++ )
				{
					grid[x][y][0] = grid[x][y][1];

					int aliveNeighbours = 0;
					if ( x > 0 && grid[x-1][y][1] ) aliveNeighbours++;
					if ( x < 31 && grid[x+1][y][1] ) aliveNeighbours++;
					if ( y > 0 && grid[x][y-1][1] ) aliveNeighbours++;
					if ( y < 31 && grid[x][y+1][1] ) aliveNeighbours++;
					if ( x > 0 && y > 0 && grid[x-1][y-1][1] ) aliveNeighbours++;
					if ( x < 31 && y < 31 && grid[x+1][y+1][1] ) aliveNeighbours++;
					if ( x < 31 && y > 0 && grid[x+1][y-1][1] ) aliveNeighbours++;
					if ( x > 0 && y < gridSize && grid[x-1][y+1][1] ) aliveNeighbours++;

					if ( grid[x][y][1] ) {
						if ( aliveNeighbours < 2 || aliveNeighbours > 3 )
							grid[x][y][0] = false;
					} else {
						if ( aliveNeighbours == 3 )
							grid[x][y][0] = true;
					}
				}
			}

			grayOffset++;

			nextUpdate = clock() + (long)( (float)CLOCKS_PER_SEC / generationsPerSecond );
		}

		////////////////////////////////////////////////////////////
		// Draw the grid
		////////////////////////////////////////////////////////////

		float angle = (float)clock() / (float)CLOCKS_PER_SEC / 5.0f;

		render->Clear( Passion::Color( 0.0f, 0.4f, 0.4f ) );
		render->ClearZ();

		render->SetTexture( texBox );

		render->Start3D( Passion::Vector( cos( angle ) * 1000.0f, sin( angle ) * 1000.0f, 350.0f ), Passion::Vector( 0.0f, 0.0f, -200.0f ) );

			float generationProgress = 1.0f - (float)( nextUpdate - clock() ) / ( (float)CLOCKS_PER_SEC / generationsPerSecond );

			for ( int gen = generations - 1; gen >= 0; gen-- )
			{
				for ( int x = 0; x < gridSize; x++ )
				{
					for ( int y = 0; y < gridSize; y++ )
					{
						if ( grid[x][y][gen] )
						{
							float gray = ( gen + grayOffset ) % 2 == 0 ? 0.5f : 0.7f;

							if ( gen == 0 )
								render->SetDrawColor( Passion::Color( (float)y / gridSize, 1.0f, 1.0f - (float)x / gridSize, generationProgress ) );
							else if ( gen == generations - 1 )
								render->SetDrawColor( Passion::Color( gray, gray, gray, 1.0f - generationProgress ) );
							else if ( gen == 1 )
								render->SetDrawColor( Passion::Color( gray * generationProgress + (float)y / gridSize * ( 1.0f - generationProgress ), gray * generationProgress + 1.0f - generationProgress, gray * generationProgress + ( 1.0f - (float)x / gridSize ) * ( 1.0f - generationProgress ), 1.0f ) );
							else {
								render->SetDrawColor( Passion::Color( gray, gray, gray, 1.0f ) );
							}
							
							render->DrawBox( Passion::Vector( (float)x * 25.0f - gridSize * 12.5f, (float)y * 25.0f - gridSize * 12.5f, 0.0f - gen * 25.0f - generationProgress * 25.0f ), Passion::Vector( (float)x * 25.0f + 25.0f - gridSize * 12.5f, (float)y * 25.0f + 25.0f - gridSize * 12.5f, 25.0f - gen * 25.0f - generationProgress * 25.0f ) );
						}
					}
				}
			}

		render->End3D();

		render->Present();
	}

	////////////////////////////////////////////////////////////
	// Clean upŘ
	////////////////////////////////////////////////////////////

	Passion::DestroyInterface<Passion::IBaseRender>( render );
	//Passion::DestroyInterface<Passion::IBaseInput>( input );

	return 0;
}
