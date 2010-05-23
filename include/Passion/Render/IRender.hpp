#ifndef PASSION_IRENDER_HPP
#define PASSION_IRENDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Render/IBaseRender.hpp>
#include <Passion/Render/GLee/GLee.h>
#include <SFML/Window.hpp>

namespace Passion
{
	struct Vertex
	{
		Vertex() {}
		Vertex( float X, float Y, float Z, Color color, float U = 0.0f, float V = 0.0f ) { x = X; y = Y; z = Z; r = color.R; g = color.G; b = color.B; a = color.A; u = U; v = V; }
		float x, y, z;
		float r, g, b, a;
		float u, v;
	};
	
	////////////////////////////////////////////////////////////
	// OpenGL implementation of IBaseRender
	////////////////////////////////////////////////////////////

	class IRender : public IBaseRender
	{
	public:
		IRender();
		~IRender();

		RenderWindow* CreateRenderWindow( unsigned int width, unsigned int height, const char* title );
		void SetRenderWindow( RenderWindow* window );

		void SetWireframeEnabled( bool enabled );
		void SetDepthEnabled( bool enabled );
		void SetColorEnabled( bool enabled );
		void SetStencilEnabled( bool enabled );
		void SetCullingEnabled( bool enabled );
		void SetScissorEnabled( bool enabled );
		void SetAlphaBlendingEnabled( bool enabled );
		void SetTexturingEnabled( bool enabled );

		Texture LoadTexture( const char* filename );

		Shader CreateShader( const char* code, int type );
		Program CreateProgram( Shader* shaders, int count );

		void SetProgramFloat( const char* name, float value );

		void Clear( Color color );
		void ClearZ( float value = 0.0f );
		void ClearStencil();

		void SetViewport( int x, int y, int w, int h );
		void SetScissor( int x, int y, int w, int h );
		
		void SetStencilCompareFunction( unsigned int function, int reference, int mask );
		void SetStencilOperations( unsigned int fail, unsigned int zfail, unsigned int pass );

		void Start2D();
		void End2D();
		void Start3D( Vector position, Vector lookAt, Vector up );
		void End3D();

		void SetDrawColor( Color color );
		void SetTexture( Texture texture );
		void SetProgram( Program program = 0 );

		void DrawPoint( Vector point );
		void DrawLine( Vector p1, Vector p2 );
		void DrawTriangle( Vector p1, Vector p2, Vector p3 );
		void DrawQuad( Vector p1, Vector p2, Vector p3, Vector p4, float repeat = 1.0f );

		void DrawBox( Vector min, Vector max );

		void Flush();

		void Present();

		// For testing purposes, remove at release
		void Test();
		
	private:
		sf::Window* m_renderWindow;
		float m_viewW, m_viewH;

		Vertex* m_vertices;
		int m_vertexIndex;

		char m_shape;
		Color m_drawColor;

		Program m_activeProgram;
	};
}

#endif