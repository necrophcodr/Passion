////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Render.hpp>
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
		Passion::IBaseRender* render = (Passion::IBaseRender*)Passion::CreateInterface( "../../lib/passion-render-d" );
	#else
		Passion::IBaseRender* render = (Passion::IBaseRender*)Passion::CreateInterface( "../../lib/passion-render" );
	#endif

	Passion::RenderWindow* window = render->CreateRenderWindow( 1280, 720, "Game of Life" );

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

	while ( true )
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

		render->Start3D( Passion::Vector( cos( angle ) * 1500.0f, sin( angle ) * 1500.0f, 700.0f ), Passion::Vector( 0.0f, 0.0f, -200.0f ) );

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

	return 0;
}
