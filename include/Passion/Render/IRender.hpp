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

#include <Passion/Render/RenderWindow.hpp>

#ifdef WIN32
	#undef DrawText
#endif

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

		Window* CreateRenderWindow( unsigned int width, unsigned int height, const char* title, bool fullscreen );
		Window* GetRenderWindow();

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
		void SetProgramFloat( const char* name, float value1, float value2 );
		void SetProgramFloat( const char* name, float value1, float value2, float value3 );
		void SetProgramFloat( const char* name, float value1, float value2, float value3, float value4 );

		void SetProgramInt( const char* name, int value );
		void SetProgramInt( const char* name, int value1, int value2 );
		void SetProgramInt( const char* name, int value1, int value2, int value3 );
		void SetProgramInt( const char* name, int value1, int value2, int value3, int value4 );

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

		void SetTransform( Matrix matrix, MatrixMode mode );
		Matrix GetTransform( MatrixMode mode );

		void SetDrawColor( Color color );
		void SetTexture( Texture texture, unsigned int target );
		void SetRenderTarget( BaseRenderTarget* rendertarget );
		void SetProgram( Program program = 0 );

		void DrawPoint( Vector point );
		void DrawPoint( Vertex point );
		void DrawLine( Vector p1, Vector p2 );
		void DrawLine( Vertex p1, Vertex p2 );
		void DrawTriangle( Vector p1, Vector p2, Vector p3 );
		void DrawTriangle( Vertex p1, Vertex p2, Vertex p3 );
		void DrawQuad( Vector p1, Vector p2, Vector p3, Vector p4, float repeat = 1.0f );
		void DrawQuad( Vertex p1, Vertex p2, Vertex p3, Vertex p4 );
		void DrawRect( float x, float y, float w, float h, float repeat = 1.0f );

		void DrawBox( Vector min, Vector max );
		void DrawSphere( Vector center, float radius, int slices, int stacks );

		void DrawText( int x, int y, const char* str );

		void Present( bool immediate );

		Vector WorldToScreen( Vector pos );
		Vector ScreenToWorld( float x, float y );

		void GetPixel( int x, int y, Color* color, float* depth, int* stencil );

	private:
		RenderWindow* m_renderWindow;
		float m_viewW, m_viewH;

		Vertex m_vertices[4096];
		unsigned int m_vertexBuffer;
		int m_vertexIndex;

		char m_shape;
		Color m_drawColor;
		Vector m_camPos;

		Program m_activeProgram;

		Texture m_textTexture;

		void Flush();
	};
}

#endif
