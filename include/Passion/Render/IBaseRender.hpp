#ifndef PASSION_IBASERENDER_HPP
#define PASSION_IBASERENDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Render/Color.hpp>
#include <Passion/Render/Vector.hpp>

#define STENCIL_NEVER			0x0200
#define STENCIL_LESS			0x0201
#define STENCIL_EQUAL			0x0202
#define STENCIL_LESSQUAL		0x0203
#define STENCIL_GREATER			0x0204
#define STENCIL_NOTEQUAL		0x0205
#define STENCIL_GREATEREQUAL	0x0206
#define STENCIL_ALWAYS			0x0207

#define STENCIL_KEEP			0x1E00
#define STENCIL_REPLACE			0x1E01
#define STENCIL_INCREASE		0x1E02
#define STENCIL_DECREASE		0x1E03

#define PIXEL_SHADER			0x8B30
#define VERTEX_SHADER			0x8B31

namespace Passion
{
	typedef void RenderWindow;
	typedef int Texture;
	typedef int Shader;
	typedef int Program;

	////////////////////////////////////////////////////////////
	// Base rendering interface
	////////////////////////////////////////////////////////////

	class IBaseRender
	{
	public:
		virtual RenderWindow* CreateRenderWindow( unsigned int width, unsigned int height, const char* title ) = 0;
		virtual void SetRenderWindow( RenderWindow* window ) = 0;

		virtual void SetWireframeEnabled( bool enabled ) = 0;
		virtual void SetDepthEnabled( bool enabled ) = 0;
		virtual void SetColorEnabled( bool enabled ) = 0;
		virtual void SetStencilEnabled( bool enabled ) = 0;
		virtual void SetCullingEnabled( bool enabled ) = 0;
		virtual void SetScissorEnabled( bool enabled ) = 0;
		virtual void SetAlphaBlendingEnabled( bool enabled ) = 0;
		virtual void SetTexturingEnabled( bool enabled ) = 0;

		virtual Texture LoadTexture( const char* filename ) = 0;

		virtual Shader CreateShader( const char* code, int type ) = 0;
		virtual Program CreateProgram( Shader* shaders, int count ) = 0;

		virtual void SetProgramFloat( const char* name, float value ) = 0;

		virtual void Clear( Color color = Color( 0.0f, 0.0f, 0.0f ) ) = 0;
		virtual void ClearZ( float value = 1.0f ) = 0;
		virtual void ClearStencil() = 0;

		virtual void SetViewport( int x, int y, int w, int h ) = 0;
		virtual void SetScissor( int x, int y, int w, int h ) = 0;

		virtual void SetStencilCompareFunction( unsigned int function, int reference = 0, int mask = 0 ) = 0;
		virtual void SetStencilOperations( unsigned int fail = STENCIL_KEEP, unsigned int zfail = STENCIL_KEEP, unsigned int pass = STENCIL_KEEP ) = 0;

		virtual void Start2D() = 0;
		virtual void End2D() = 0;
		virtual void Start3D( Vector position, Vector lookAt = Vector(), Vector up = Vector( 0.0f, 1.0f, 0.0f ) ) = 0;
		virtual void End3D() = 0;

		virtual void SetDrawColor( Color color ) = 0;
		virtual void SetTexture( Texture texture ) = 0;
		virtual void SetProgram( Program program = 0 ) = 0;

		virtual void DrawPoint( Vector point ) = 0;
		virtual void DrawLine( Vector p1, Vector p2 ) = 0;
		virtual void DrawTriangle( Vector p1, Vector p2, Vector p3 ) = 0;
		virtual void DrawQuad( Vector p1, Vector p2, Vector p3, Vector p4, float repeat = 1.0f ) = 0;

		virtual void DrawBox( Vector min, Vector max ) = 0;

		virtual void Present() = 0;

		// For testing purposes, remove at release
		virtual void Test() = 0;
	};
}

#endif