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

#ifndef PASSION_IRENDER_HPP
#define PASSION_IRENDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Render/IBaseRender.hpp>
#include <Passion/Render/RenderTarget.hpp>
#include <Passion/Render/GLee/GLee.h>
#include <SFML/Window.hpp>

namespace Passion
{	
	////////////////////////////////////////////////////////////
	// OpenGL implementation of IBaseRender
	////////////////////////////////////////////////////////////

	class IRender : public IBaseRender
	{
	public:
		IRender();
		~IRender();

		RenderWindow* CreateRenderWindow( unsigned int width, unsigned int height, const char* title, bool fullscreen );
		void SetRenderWindow( RenderWindow* window );

		bool SupportsShaders();
		bool SupportsRenderTargets();

		void SetWireframeEnabled( bool enabled );
		void SetDepthEnabled( bool enabled );
		void SetColorEnabled( bool enabled );
		void SetStencilEnabled( bool enabled );
		void SetCullingEnabled( bool enabled );
		void SetScissorEnabled( bool enabled );
		void SetAlphaBlendingEnabled( bool enabled );
		void SetTexturingEnabled( bool enabled );

		Texture LoadTexture( const char* filename, bool mipmaps );
		BaseRenderTarget* CreateRenderTarget( unsigned int width, unsigned int height );

		Model LoadModel( const char* filename );
		Model CreateModel( Vertex* points, unsigned int count );
		void DrawModel( Model model );

		Shader CreateShader( const char* code, int type );
		Program CreateProgram( Shader* shaders, int count );

		void SetProgramFloat( const char* name, float value );

		void Clear( Color color );
		void ClearZ( float value = 1.0f );
		void ClearStencil();

		void SetViewport( int x, int y, int w, int h );
		void SetScissor( int x, int y, int w, int h );
		
		void SetStencilCompareFunction( unsigned int function, int reference, int mask );
		void SetStencilOperations( unsigned int fail, unsigned int zfail, unsigned int pass );

		void Start2D();
		void End2D();
		void Start3D( Vector position, Vector lookAt, float fov, float znear, float zfar, Vector up );
		void End3D();

		void SetDrawColor( Color color );
		void SetTexture( Texture texture );
		void SetRenderTarget( BaseRenderTarget* rendertarget );
		void SetProgram( Program program = 0 );

		void DrawPoint( Vector point );
		void DrawLine( Vector p1, Vector p2 );
		void DrawTriangle( Vector p1, Vector p2, Vector p3 );
		void DrawQuad( Vector p1, Vector p2, Vector p3, Vector p4, float repeat = 1.0f );

		void DrawBox( Vector min, Vector max );

		Vector WorldToScreen( Vector pos );
		Vector ScreenToWorld( float x, float y );

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

		void Flush();
	};
}

#endif