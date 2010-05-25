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

#include <Passion/Render/IRender.hpp>
#include <SOIL.h>
#include <iostream>

namespace Passion
{
	IRender::IRender()
	{
		m_renderWindow = 0;
		m_vertices = new Vertex[4096];
		m_vertexIndex = 0;
		m_shape = 0;
	}

	IRender::~IRender()
	{
		if ( m_renderWindow ) delete m_renderWindow;
	}

	RenderWindow* IRender::CreateRenderWindow( unsigned int width, unsigned int height, const char* title )
	{
		m_renderWindow = new sf::Window( sf::VideoMode( width, height, 32 ), title, sf::Style::Close );
		SetViewport( 0, 0, width, height );

		glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), m_vertices );
		glColorPointer( 4, GL_FLOAT, sizeof( Vertex ), (char*)m_vertices + sizeof( float ) * 3 );
		glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), (char*)m_vertices + sizeof( float ) * 7 );

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		return m_renderWindow;
	}

	void IRender::SetWireframeEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}

	void IRender::SetDepthEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glEnable( GL_DEPTH_TEST );
		else
			glDisable( GL_DEPTH_TEST );
	}

	void IRender::SetColorEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
		else
			glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	}

	void IRender::SetStencilEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glEnable( GL_STENCIL_TEST );
		else
			glDisable( GL_STENCIL_TEST );
	}

	void IRender::SetCullingEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glEnable( GL_CULL_FACE );
		else
			glDisable( GL_CULL_FACE );
	}

	void IRender::SetScissorEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glEnable( GL_SCISSOR_TEST );
		else
			glDisable( GL_SCISSOR_TEST );
	}

	void IRender::SetAlphaBlendingEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glEnable( GL_BLEND );
		else
			glDisable( GL_BLEND );
	}

	void IRender::SetTexturingEnabled( bool enabled )
	{
		Flush();
		if ( enabled )
			glEnable( GL_TEXTURE_2D );
		else
			glDisable( GL_TEXTURE_2D );
	}

	Texture IRender::LoadTexture( const char* filename )
	{
		return SOIL_load_OGL_texture( filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT );
	}

	BaseRenderTarget* IRender::CreateRenderTarget( unsigned int width, unsigned int height )
	{
		RenderTarget* rt = new RenderTarget();

		glGenFramebuffersEXT( 1, &rt->m_framebuffer );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, rt->m_framebuffer );

		glGenRenderbuffersEXT( 1, &rt->m_renderbuffer );
		glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rt->m_renderbuffer );
		glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 1280, 720 );

		glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rt->m_renderbuffer );

		glGenTextures( 1, &rt->m_rendertexture );
		glBindTexture( GL_TEXTURE_2D, rt->m_rendertexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glGenerateMipmapEXT( GL_TEXTURE_2D );
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rt->m_rendertexture, 0 );

		SetRenderTarget( 0 );
		SetTexture( 0 );

		return rt;
	}

	Shader IRender::CreateShader( const char* code, int type )
	{
		int shader = glCreateShader( type );
		glShaderSource( shader, 1, &code, 0 );
		glCompileShader( shader );
		
		char buffer[1024];
		int length;
		glGetShaderInfoLog( shader, 1024, &length, buffer );
		std::cout << buffer;

		return shader;
	}

	Program IRender::CreateProgram( Shader* shaders, int count )
	{
		int program = glCreateProgram();
		for ( int i = 0; i < count; i++ )
			glAttachShader( program, shaders[i] );
		glLinkProgram( program );
		return program;
	}

	void IRender::SetProgramFloat( const char* name, float value )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform1f( loc, value );
	}

	void IRender::SetRenderWindow( RenderWindow* window )
	{
		((sf::Window*)window)->SetActive();
		m_renderWindow = (sf::Window*)window;
	}

	void IRender::Clear( Color color )
	{
		glClearColor( color.R, color.G, color.B, color.A );
		glClear( GL_COLOR_BUFFER_BIT );
	}

	void IRender::ClearZ( float value )
	{
		glClearDepth( value );
		glClear( GL_DEPTH_BUFFER_BIT );
	}

	void IRender::ClearStencil()
	{
		glClear( GL_STENCIL_BUFFER_BIT );
	}

	void IRender::SetViewport( int x, int y, int w, int h )
	{
		glViewport( x, y, w, h );
		m_viewW = (float)w;
		m_viewH = (float)h;
	}

	void IRender::SetScissor( int x, int y, int w, int h )
	{
		Flush();
		glScissor( x, (int)m_viewH / 2 - y, w, h );
	}

	void IRender::SetStencilCompareFunction( unsigned int function, int reference, int mask )
	{
		Flush();
		glStencilFunc( function, reference, mask );
	}

	void IRender::SetStencilOperations( unsigned int fail, unsigned int zfail, unsigned int pass )
	{
		Flush();
		glStencilOp( fail, zfail, pass );
	}

	void IRender::Start2D()
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D( 0.0, m_viewW, m_viewH, 0.0 );
	}

	void IRender::End2D()
	{
		Flush();
	}

	void IRender::Start3D( Vector position, Vector lookAt, Vector up )
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 45.0f, m_viewW / m_viewH, 1.0f, 100000.0f );
		gluLookAt( position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.z, up.y );
	}

	void IRender::End3D()
	{
		Flush();
	}

	void IRender::SetDrawColor( Color color )
	{
		m_drawColor = color;
	}

	void IRender::SetTexture( Texture texture )
	{
		glBindTexture( GL_TEXTURE_2D, texture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	}

	void IRender::SetRenderTarget( BaseRenderTarget* rendertarget )
	{
		Flush();
		if ( rendertarget != 0 )
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, ((RenderTarget*)rendertarget)->m_framebuffer );
		else
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	}

	void IRender::SetProgram( Program program )
	{
		Flush();

		glUseProgram( program );
		m_activeProgram = program;
	}

	void IRender::DrawPoint( Vector point )
	{
		if ( m_shape != GL_POINTS || m_vertexIndex > 4095 ) {
			Flush();
			m_shape = GL_POINTS;
		}

		m_vertices[m_vertexIndex++] = Vertex( point.x, point.y, point.z, m_drawColor );
	}

	void IRender::DrawLine( Vector p1, Vector p2 )
	{
		if ( m_shape != GL_LINES || m_vertexIndex > 4094 ) {
			Flush();
			m_shape = GL_LINES;
		}

		m_vertices[m_vertexIndex++] = Vertex( p1.x, p1.y, p1.z, m_drawColor );
		m_vertices[m_vertexIndex++] = Vertex( p2.x, p2.y, p2.z, m_drawColor );
	}

	void IRender::DrawTriangle( Vector p1, Vector p2, Vector p3 )
	{
		if ( m_shape != GL_TRIANGLES || m_vertexIndex > 4093 ) {
			Flush();
			m_shape = GL_TRIANGLES;
		}

		m_vertices[m_vertexIndex++] = Vertex( p1.x, p1.y, p1.z, m_drawColor, 0.0f, 1.0f );
		m_vertices[m_vertexIndex++] = Vertex( p2.x, p2.y, p2.z, m_drawColor, 1.0f, 1.0f );
		m_vertices[m_vertexIndex++] = Vertex( p3.x, p3.y, p3.z, m_drawColor, 1.0f, 0.0f );
	}

	void IRender::DrawQuad( Vector p1, Vector p2, Vector p3, Vector p4, float repeat )
	{
		if ( m_shape != GL_QUADS || m_vertexIndex > 4092 ) {
			Flush();
			m_shape = GL_QUADS;
		}

		m_vertices[m_vertexIndex++] = Vertex( p1.x, p1.y, p1.z, m_drawColor, 0.0f, repeat );
		m_vertices[m_vertexIndex++] = Vertex( p2.x, p2.y, p2.z, m_drawColor, repeat, repeat );
		m_vertices[m_vertexIndex++] = Vertex( p3.x, p3.y, p3.z, m_drawColor, repeat, 0.0f );
		m_vertices[m_vertexIndex++] = Vertex( p4.x, p4.y, p4.z, m_drawColor, 0.0f, 0.0f );
	}

	void IRender::DrawBox( Vector min, Vector max )
	{
		DrawQuad( min, Vector( max.x, min.y, min.z ), Vector( max.x, max.y, min.z ), Vector( min.x, max.y, min.z ) );
		DrawQuad( Vector( min.x, min.y, max.z ), Vector( max.x, min.y, max.z ), Vector( max.x, max.y, max.z ), Vector( min.x, max.y, max.z ) );
		DrawQuad( Vector( max.x, min.y, min.z ), Vector( max.x, min.y, max.z ), Vector( min.x, min.y, max.z ), min );
		DrawQuad( Vector( min.x, max.y, min.z ), Vector( min.x, max.y, max.z ), max, Vector( max.x, max.y, min.z ) );
		DrawQuad( min, Vector( min.x, min.y, max.z ), Vector( min.x, max.y, max.z ), Vector( min.x, max.y, min.z ) );
		DrawQuad( Vector( max.x, max.y, min.z ), max, Vector( max.x, min.y, max.z ), Vector( max.x, min.y, min.z ) );
	}

	void IRender::Flush()
	{
		if ( m_vertexIndex > 0 )
		{
			glDrawArrays( m_shape, 0, m_vertexIndex );

			m_vertexIndex = 0;
		}
	}

	void IRender::Present()
	{
		m_renderWindow->Display();
	}

	// For testing purposes, remove at release
	bool testInitialized = false;
	RenderTarget* rt;

	void IRender::Test()
	{
		if ( !testInitialized )
		{
			rt = (RenderTarget*)CreateRenderTarget( 1280, 720 );
			testInitialized = true;
		}

		// Draw the scene of the render target
		SetRenderTarget( rt );
		SetTexture( 0 );

		Clear( Color( 0.2f, 0.2f, 0.2f ) );
		ClearZ();

		Start3D( Vector( cos( (float)clock() / (float)CLOCKS_PER_SEC ) * 50.0f, sin( (float)clock() / (float)CLOCKS_PER_SEC ) * 50.0f, 40.0f ), Vector(), Vector( 0.0f, 1.0f, 0.0f ) );
			SetDrawColor( Color( 0.0f, 1.0f, 0.0f ) );

			DrawBox( Vector( -10.0f, -10.0f, -10.0f ), Vector( 10.0f, 10.0f, 10.0f ) );
		End3D();

		// Draw the actual scene
		SetRenderTarget( 0 );
		SetTexture( rt->GetTexture() );

		Clear( Color( 0.0f, 0.0f, 0.0f ) );
		ClearZ();
		
		Start3D( Vector( cos( (float)clock() / (float)CLOCKS_PER_SEC / 4.0f ) * 50.0f, sin( (float)clock() / (float)CLOCKS_PER_SEC / 4.0f ) * 50.0f, 40.0f ), Vector(), Vector( 0.0f, 1.0f, 0.0f ) );
			SetDrawColor( Color( 1.0f, 1.0f, 1.0f ) );
			DrawBox( Vector( -10.0f, -10.0f, -10.0f ), Vector( 10.0f, 10.0f, 10.0f ) );
		End3D();

		Present();
	}
}
