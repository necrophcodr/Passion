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
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

namespace Passion
{
	IRender::IRender()
	{
		m_renderWindow = 0;
		m_vertexIndex = 0;
		m_shape = 0;
		m_textTexture = 0;
	}

	IRender::~IRender()
	{
		if ( m_renderWindow ) delete m_renderWindow;
	}

	Window* IRender::CreateRenderWindow( unsigned int width, unsigned int height, const char* title, bool fullscreen )
	{
		if ( m_renderWindow ) return m_renderWindow;

		m_renderWindow = new RenderWindow( title, width, height );

		SetViewport( 0, 0, width, height );

		glGenBuffers( 1, &m_vertexBuffer );
		glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
		glBufferData( GL_ARRAY_BUFFER, 4096 * sizeof( Vertex ), m_vertices, GL_STREAM_DRAW );

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );

		glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), 0 );
		glColorPointer( 4, GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) ) );
		glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) + sizeof( float ) * 4 ) );
		glNormalPointer( GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) + sizeof( float ) * 6 ) );

		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

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

	bool IRender::SupportsShaders()
	{
		return GLEE_ARB_fragment_shader && GLEE_ARB_vertex_shader;
	}

	bool IRender::SupportsRenderTargets()
	{
		return GLEE_ARB_framebuffer_object != 0;
	}

	Texture IRender::LoadTexture( const char* filename, bool mipmaps )
	{
		unsigned int flags = SOIL_FLAG_INVERT_Y;
		if ( mipmaps ) flags |= SOIL_FLAG_MIPMAPS;
		return SOIL_load_OGL_texture( filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags );
	}

	BaseRenderTarget* IRender::CreateRenderTarget( unsigned int width, unsigned int height )
	{
		unsigned int framebuffer, renderbuffer, rendertexture;

		glGenFramebuffersEXT( 1, &framebuffer );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer );

		glGenRenderbuffersEXT( 1, &renderbuffer );
		glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, renderbuffer );
		glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height );

		glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderbuffer );

		glGenTextures( 1, &rendertexture );
		glBindTexture( GL_TEXTURE_2D, rendertexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glGenerateMipmapEXT( GL_TEXTURE_2D );
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rendertexture, 0 );

		SetRenderTarget( 0 );
		SetTexture( 0, 0 );

		return new RenderTarget( framebuffer, renderbuffer, rendertexture );
	}

	Model IRender::LoadModel( const char* filename )
	{
		std::ifstream file;
		file.open( filename, std::ios::in );
		if ( !file.is_open() ) return Model();

		std::vector<Vector> points;
		std::vector<Vector> normals;
		std::vector<Vector> texcoords;
		std::vector<Vertex*> triangles;

		float x, y, z;
		int v, t, n;
		char c;
		std::string keyword;

		while ( !file.eof() )
		{
			file >> keyword;

			if ( keyword == "v" )
			{
				file >> x >> y >> z;
				points.push_back( Vector( x, y, z ) );
			} else if ( keyword == "vt" )
			{
				file >> x >> y >> z;
				texcoords.push_back( Vector( x, y, z ) );
			} else if ( keyword == "vn" )
			{
				file >> x >> y >> z;
				normals.push_back( Vector( x, y, z ) );
			} else if ( keyword == "f" )
			{
				Vector point;
				Vector uv = Vector();
				Vector normal = Vector();
				Vertex* tr = new Vertex[3];

				for ( int face = 0; face < 3; face++ ) {
					file >> v;
					point = points[v-1];
					if ( texcoords.size() > 0 ) {
						file >> c >> t;
						uv = texcoords[t-1];
					}
					if ( normals.size() > 0 ) {
						if ( texcoords.size() > 0 )
							file >> c >> n;
						else
							file >> c >> c >> n;

						normal = normals[n-1];
					}

					tr[face] = Vertex( point.x, point.y, point.z, Color(), uv.x, uv.y, normal.x, normal.y, normal.z );
				}

				triangles.push_back( tr );
			}
		}

		file.close();

		unsigned int vertexcount = triangles.size() * 3;

		Vertex* vertices = new Vertex[vertexcount];

		for ( unsigned int i = 0; i < triangles.size(); i++ )
		{
			memcpy( &vertices[i*3], triangles[i], sizeof( Vertex ) * 3 );
			delete [] triangles[i];
		}

		Model model = CreateModel( vertices, vertexcount );

		delete [] vertices;

		return model;
	}

	Model IRender::CreateModel( Passion::Vertex* points, unsigned int count )
	{
		if ( count % 3 != 0 ) return Model();

		Model model;
		model.vertices = count;

		glGenBuffers( 1, &model.id );
		glBindBuffer( GL_ARRAY_BUFFER, model.id );
		glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * count, points, GL_STATIC_DRAW );

		return model;
	}

	void IRender::DrawModel( Model model )
	{
		glBindBuffer( GL_ARRAY_BUFFER, model.id );

		glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), 0 );
		glColorPointer( 4, GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) ) );
		glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) + sizeof( float ) * 4 ) );
		glNormalPointer( GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) + sizeof( float ) * 6 ) );

		glDrawArrays( GL_TRIANGLES, 0, model.vertices );
	}

	void IRender::Flush()
	{
		if ( m_vertexIndex > 0 )
		{
			glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
			glBufferSubData( GL_ARRAY_BUFFER, 0, m_vertexIndex * sizeof( Vertex ), m_vertices );

			glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), 0 );
			glColorPointer( 4, GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) ) );
			glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) + sizeof( float ) * 4 ) );
			glNormalPointer( GL_FLOAT, sizeof( Vertex ), reinterpret_cast<void*>( sizeof( Vector ) + sizeof( float ) * 6 ) );

			glDrawArrays( m_shape, 0, m_vertexIndex );

			m_vertexIndex = 0;
		}
	}

	Shader IRender::CreateShader( const char* code, int type )
	{
		int shader = glCreateShader( type );
		glShaderSource( shader, 1, &code, 0 );
		glCompileShader( shader );

		char buffer[1024];
		int length;
		glGetShaderInfoLog( shader, 1024, &length, buffer );

		#ifdef _DEBUG_
			std::cout << buffer;
		#endif

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

	void IRender::SetProgramFloat( const char* name, float value1, float value2 )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform2f( loc, value1, value2 );
	}

	void IRender::SetProgramFloat( const char* name, float value1, float value2, float value3 )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform3f( loc, value1, value2, value3 );
	}

	void IRender::SetProgramFloat( const char* name, float value1, float value2, float value3, float value4 )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform4f( loc, value1, value2, value3, value4 );
	}

	void IRender::SetProgramInt( const char* name, int value )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform1i( loc, value );
	}

	void IRender::SetProgramInt( const char* name, int value1, int value2 )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform2i( loc, value1, value2 );
	}

	void IRender::SetProgramInt( const char* name, int value1, int value2, int value3 )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform3i( loc, value1, value2, value3 );
	}

	void IRender::SetProgramInt( const char* name, int value1, int value2, int value3, int value4 )
	{
		int loc = glGetUniformLocation( m_activeProgram, name );
		glUniform4i( loc, value1, value2, value3, value4 );
	}

	Window* IRender::GetRenderWindow()
	{
		return m_renderWindow;
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
		glScissor( x, (int)m_viewH - y - h, w, h );
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
		Matrix mat;
		mat.Orthogonal( 0.0f, m_viewW, m_viewH, 0.0f, -1.0f, 1.0f );
		SetTransform( mat, MATRIX_PROJECTION );
	}

	void IRender::End2D()
	{
		Flush();
	}

	void IRender::Start3D( Vector position, Vector lookAt, float fov, float znear, float zfar, Vector up )
	{
		Matrix mat;
		mat.Perspective( fov, m_viewW / m_viewH, znear, zfar );
		mat.LookAt( position, lookAt, up );
		SetTransform( mat, MATRIX_PROJECTION );
		m_camPos = position;
	}

	void IRender::End3D()
	{
		Flush();
	}

	void IRender::SetTransform( Matrix matrix, MatrixMode mode )
	{
		Flush();
		
		glMatrixMode( 0x1700 + mode );
		glLoadMatrixf( matrix );
	}

	Matrix IRender::GetTransform( MatrixMode mode )
	{
		Matrix m;
		glGetFloatv( 0x0BA6 + mode, (float*)&m );
		return m;
	}

	void IRender::SetDrawColor( Color color )
	{
		m_drawColor = color;
	}

	void IRender::SetTexture( Texture texture, unsigned int target )
	{
		Flush();

		glActiveTexture( GL_TEXTURE0 + target );
		glEnable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, texture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	}

	void IRender::SetRenderTarget( BaseRenderTarget* rendertarget )
	{
		Flush();
		if ( rendertarget != 0 )
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, ((RenderTarget*)rendertarget)->GetFrameBuffer() );
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

	void IRender::DrawPoint( Vertex point )
	{
		if ( m_shape != GL_POINTS || m_vertexIndex > 4095 ) {
			Flush();
			m_shape = GL_POINTS;
		}

		m_vertices[m_vertexIndex++] = point;
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

	void IRender::DrawLine( Vertex p1, Vertex p2 )
	{
		if ( m_shape != GL_LINES || m_vertexIndex > 4094 ) {
			Flush();
			m_shape = GL_LINES;
		}

		m_vertices[m_vertexIndex++] = p1;
		m_vertices[m_vertexIndex++] = p2;
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

	void IRender::DrawTriangle( Vertex p1, Vertex p2, Vertex p3 )
	{
		if ( m_shape != GL_TRIANGLES || m_vertexIndex > 4093 ) {
			Flush();
			m_shape = GL_TRIANGLES;
		}

		m_vertices[m_vertexIndex++] = p1;
		m_vertices[m_vertexIndex++] = p2;
		m_vertices[m_vertexIndex++] = p3;
	}

	void IRender::DrawQuad( Vector p1, Vector p2, Vector p3, Vector p4, float repeat )
	{
		if ( m_shape != GL_TRIANGLES || m_vertexIndex > 4090 ) {
			Flush();
			m_shape = GL_TRIANGLES;
		}

		m_vertices[m_vertexIndex++] = Vertex( p1.x, p1.y, p1.z, m_drawColor, 0.0f, repeat );
		m_vertices[m_vertexIndex++] = Vertex( p2.x, p2.y, p2.z, m_drawColor, repeat, repeat );
		m_vertices[m_vertexIndex++] = Vertex( p3.x, p3.y, p3.z, m_drawColor, repeat, 0.0f );

		m_vertices[m_vertexIndex++] = Vertex( p3.x, p3.y, p3.z, m_drawColor, repeat, 0.0f );
		m_vertices[m_vertexIndex++] = Vertex( p4.x, p4.y, p4.z, m_drawColor, 0.0f, 0.0f );
		m_vertices[m_vertexIndex++] = Vertex( p1.x, p1.y, p1.z, m_drawColor, 0.0f, repeat );
	}

	void IRender::DrawQuad( Vertex p1, Vertex p2, Vertex p3, Vertex p4 )
	{
		if ( m_shape != GL_TRIANGLES || m_vertexIndex > 4090 ) {
			Flush();
			m_shape = GL_TRIANGLES;
		}

		m_vertices[m_vertexIndex++] = p1;
		m_vertices[m_vertexIndex++] = p2;
		m_vertices[m_vertexIndex++] = p3;

		m_vertices[m_vertexIndex++] = p3;
		m_vertices[m_vertexIndex++] = p4;
		m_vertices[m_vertexIndex++] = p1;
	}

	void IRender::DrawRect( float x, float y, float w, float h, float repeat )
	{
		DrawQuad( Vector( x, y ), Vector( x + w, y ), Vector( x + w, y + h ), Vector( x, y + h ), repeat );
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

	void IRender::DrawText( int x, int y, const char* str )
	{
		if ( m_textTexture == 0 ) m_textTexture = LoadTexture( "textures/font.png", false );
		SetTexture( m_textTexture, 0 );

		int ox = x;
		int len = strlen( str );
		float u, v;

		for ( int i = 0; i < len; i++ )
		{
			if ( str[i] == '\n' ) {
				x = ox;
				y = y + 9;
				continue;
			}

			u = (float)( str[i] % 16 ) * 0.0625f;
			v = floor( (float)str[i] / 16.0f ) * 0.0625f;

			DrawQuad(
				Vertex( x, y, 0.0f, m_drawColor, u, 1.0f - v ),
				Vertex( x + 6, y, 0.0f, m_drawColor, u + 0.0625f, 1.0f - v ),
				Vertex( x + 6, y + 8, 0.0f, m_drawColor, u + 0.0625f, 1.0f - v - 0.0625f ),
				Vertex( x, y + 8, 0.0f, m_drawColor, u, 1.0f - v - 0.0625f )
			);

			x = x + 6;
		}
	}

	Vector IRender::WorldToScreen( Vector pos )
	{
		int view[4]; glGetIntegerv( GL_VIEWPORT, view );

		Matrix m = GetTransform( MATRIX_PROJECTION ) * GetTransform( MATRIX_MODEL );
		Vector v = m * pos;

		float w = m.m[0][3] * pos.x + m.m[1][3] * pos.y + m.m[2][3] * pos.z + m.m[3][3];

		v.x = ( v.x / w * 0.5f + 0.5f ) * view[2] + view[0];
		v.y = view[3] - ( v.y / w * 0.5f + 0.5f ) * view[3] + view[1];
		v.z = v.z / w * 0.5f + 0.5f;
		
		return v;
	}

	Vector IRender::ScreenToWorld( float x, float y )
	{
		double proj[16], model[16];
		int view[4];

		glGetDoublev( GL_PROJECTION_MATRIX, proj );
		glGetDoublev( GL_MODELVIEW_MATRIX, model );
		glGetIntegerv( GL_VIEWPORT, view );

		double x2, y2, z2;
		gluUnProject( (double)x, (double)( view[3] - y ), 1.0, model, proj, view, &x2, &y2, &z2 );

		Vector aim( (float)x2, (float)y2, (float)z2 );
		aim = aim - m_camPos;
		aim.Normalize();

		return aim;
	}

	void IRender::GetPixel( int x, int y, Color* color, float* depth, int* stencil )
	{
		if ( color != 0 )
			glReadPixels( x, m_viewH - y, 1, 1, GL_RGBA, GL_FLOAT, color );
		if ( depth != 0 )
			glReadPixels( x, m_viewH - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, depth );
		if ( stencil != 0 )
			glReadPixels( x, m_viewH - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, stencil );
	}

	void IRender::Present( bool immediate )
	{
		m_renderWindow->Present();

		#ifndef _DEBUG_
            if ( !immediate )
            #ifdef WIN32
                Sleep( 1 );
            #else
                usleep( 1000 );
            #endif
		#endif
	}
}
