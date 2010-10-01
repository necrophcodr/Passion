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

#include "Interfaces.hpp"
#include <vector>

////////////////////////////////////////////////////////////
// Render library
////////////////////////////////////////////////////////////

class render
{
public:
	SCRIPT_FUNCTION( SupportsShaders )
	{
		g_Lua->Push( g_Render->SupportsShaders() );
		return 1;
	}

	SCRIPT_FUNCTION( SupportsRenderTargets )
	{
		g_Lua->Push( g_Render->SupportsRenderTargets() );
		return 1;
	}

	SCRIPT_FUNCTION( SetWireframeEnabled )
	{
		g_Render->SetWireframeEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( SetDepthEnabled )
	{
		g_Render->SetDepthEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( SetColorEnabled )
	{
		g_Render->SetColorEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( SetStencilEnabled )
	{
		g_Render->SetStencilEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( SetCullingEnabled )
	{
		g_Render->SetCullingEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( SetScissorEnabled )
	{
		g_Render->SetScissorEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( SetAlphaBlendingEnabled )
	{
		g_Render->SetAlphaBlendingEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( SetTexturingEnabled )
	{
		g_Render->SetTexturingEnabled( g_Lua->Get( 1 )->GetBoolean() );
		return 0;
	}

	SCRIPT_FUNCTION( Clear )
	{
		g_Render->Clear( GetColor( 1 ) );
		return 0;
	}

	SCRIPT_FUNCTION( ClearZ )
	{
		float val = 1.0f; if ( g_Lua->Get( 1 )->IsNumber() ) val = g_Lua->Get( 1 )->GetFloat();
		g_Render->ClearZ( val );
		return 0;
	}

	SCRIPT_FUNCTION( ClearStencil )
	{
		g_Render->ClearStencil();
		return 0;
	}

	SCRIPT_FUNCTION( SetViewport )
	{
		g_Render->SetViewport( g_Lua->Get( 1 )->GetInteger(), g_Lua->Get( 2 )->GetInteger(), g_Lua->Get( 3 )->GetInteger(), g_Lua->Get( 4 )->GetInteger() );
		return 0;
	}

	SCRIPT_FUNCTION( SetScissor )
	{
		g_Render->SetScissor( g_Lua->Get( 1 )->GetInteger(), g_Lua->Get( 2 )->GetInteger(), g_Lua->Get( 3 )->GetInteger(), g_Lua->Get( 4 )->GetInteger() );
		return 0;
	}

	SCRIPT_FUNCTION( SetStencilCompareFunction )
	{
		g_Render->SetStencilCompareFunction( g_Lua->Get( 1 )->GetInteger(), g_Lua->Get( 2 )->GetInteger(), g_Lua->Get( 3 )->GetInteger() );
		return 0;
	}

	SCRIPT_FUNCTION( SetStencilOperations )
	{
		g_Render->SetStencilOperations( g_Lua->Get( 1 )->GetInteger(), g_Lua->Get( 2 )->GetInteger(), g_Lua->Get( 3 )->GetInteger() );
		return 0;
	}

	SCRIPT_FUNCTION( Start2D )
	{
		g_Render->Start2D();
		return 0;
	}

	SCRIPT_FUNCTION( End2D )
	{
		g_Render->End2D();
		return 0;
	}

	SCRIPT_FUNCTION( Start3D )
	{
		float fov = 45.0f; if ( g_Lua->Get( 3 )->IsNumber() ) fov = g_Lua->Get( 3 )->GetFloat();
		float znear = 1.0f; if ( g_Lua->Get( 4 )->IsNumber() ) znear = g_Lua->Get( 4 )->GetFloat();
		float zfar = 10000.0f; if ( g_Lua->Get( 5 )->IsNumber() ) zfar = g_Lua->Get( 5 )->GetFloat();
		Passion::Vector up( 0.0f, 0.0f, 1.0f ); if ( g_Lua->Get( 6 )->IsTable() ) up = GetVector( 6 );

		g_Render->Start3D( GetVector( 1 ), GetVector( 2 ), fov, znear, zfar, up );

		return 0;
	}

	SCRIPT_FUNCTION( End3D )
	{
		g_Render->End3D();
		return 0;
	}

	SCRIPT_FUNCTION( SetTransform )
	{
		if ( g_Lua->Get( 1 )->IsNil() )
		{
			g_Render->SetTransform( Passion::Matrix() );
		} else {
			if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Matrix" ) ) )
				g_Lua->Error( 1, "Matrix" );

			g_Render->SetTransform( *reinterpret_cast<Passion::Matrix*>( g_Lua->Get( 1 )->GetUserData() ) );
		}

		return 0;
	}

	SCRIPT_FUNCTION( SetDrawColor )
	{
		g_Render->SetDrawColor( GetColor( 1 ) );
		return 0;
	}

	SCRIPT_FUNCTION( SetTexture )
	{
		if ( g_Lua->Get( 1 )->IsUserData() && g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Texture" ) ) )
			g_Render->SetTexture( *reinterpret_cast<Passion::Texture*>( g_Lua->Get( 1 )->GetUserData() ), g_Lua->Get( 2 )->GetInteger() );
		else if ( g_Lua->Get( 1 )->IsNil() || ( g_Lua->Get( 1 )->IsNumber() && g_Lua->Get( 1 )->GetInteger() == 0 ) )
			g_Render->SetTexture();
		else
			g_Lua->Error( 1, "Texture" );

		return 0;
	}

	SCRIPT_FUNCTION( SetRenderTarget )
	{
		if ( g_Lua->Get( 1 )->IsUserData() && g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "RenderTarget" ) ) )
			g_Render->SetRenderTarget( *reinterpret_cast<Passion::BaseRenderTarget**>( g_Lua->Get( 1 )->GetUserData() ) );
		else
			g_Render->SetRenderTarget();

		return 0;
	}

	SCRIPT_FUNCTION( SetShader )
	{
		if ( g_Lua->Get( 1 )->IsUserData() && g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "Shader" ) ) )
			g_Render->SetProgram( *reinterpret_cast<Passion::Shader*>( g_Lua->Get( 1 )->GetUserData() ) );
		else if ( g_Lua->Get( 1 )->IsNil() )
			g_Render->SetProgram();
		else
			g_Lua->Error( 1, "Shader" );

		return 0;
	}

	SCRIPT_FUNCTION( DrawPoint )
	{
		g_Render->DrawPoint( GetVector( 1 ) );
		return 0;
	}

	SCRIPT_FUNCTION( DrawLine )
	{
		g_Render->DrawLine( GetVector( 1 ), GetVector( 2 ) );
		return 0;
	}

	SCRIPT_FUNCTION( DrawTriangle )
	{
		g_Render->DrawTriangle( GetVector( 1 ), GetVector( 2 ), GetVector( 3 ) );
		return 0;
	}

	SCRIPT_FUNCTION( DrawQuad )
	{
		g_Render->DrawQuad( GetVector( 1 ), GetVector( 2 ), GetVector( 3 ), GetVector( 4 ) );
		return 0;
	}

	SCRIPT_FUNCTION( DrawRect )
	{
		g_Render->DrawRect( g_Lua->Get( 1 )->GetFloat(), g_Lua->Get( 2 )->GetFloat(), g_Lua->Get( 3 )->GetFloat(), g_Lua->Get( 4 )->GetFloat() );
		return 0;
	}

	SCRIPT_FUNCTION( DrawBox )
	{
		g_Render->DrawBox( GetVector( 1 ), GetVector( 2 ) );
		return 0;
	}

	SCRIPT_FUNCTION( WorldToScreen )
	{
		Passion::Vector pos = g_Render->WorldToScreen( GetVector( 1 ) );

		std::auto_ptr<BaseScriptValue> vec = g_Lua->NewTable();
		vec->GetMember( "x" )->Set( pos.x );
		vec->GetMember( "y" )->Set( pos.y );
		vec->GetMember( "z" )->Set( pos.z );

		g_Lua->Push( vec );

		return 1;
	}

	SCRIPT_FUNCTION( ScreenToWorld )
	{
		Passion::Vector pos = g_Render->ScreenToWorld( g_Lua->Get( 1 )->GetFloat(), g_Lua->Get( 2 )->GetFloat() );

		std::auto_ptr<BaseScriptValue> vec = g_Lua->NewTable();
		vec->GetMember( "x" )->Set( pos.x );
		vec->GetMember( "y" )->Set( pos.y );
		vec->GetMember( "z" )->Set( pos.z );

		g_Lua->Push( vec );

		return 1;
	}

	static void Bind()
	{
		// Library
		std::auto_ptr<BaseScriptValue> render = g_Lua->NewTable();

		render->GetMember( "SupportsShaders" )->Set( SupportsShaders );
		render->GetMember( "SupportsRenderTargets" )->Set( SupportsRenderTargets );

		render->GetMember( "SetWireframeEnabled" )->Set( SetWireframeEnabled );
		render->GetMember( "SetDepthEnabled" )->Set( SetDepthEnabled );
		render->GetMember( "SetColorEnabled" )->Set( SetColorEnabled );
		render->GetMember( "SetStencilEnabled" )->Set( SetStencilEnabled );
		render->GetMember( "SetCullingEnabled" )->Set( SetCullingEnabled );
		render->GetMember( "SetScissorEnabled" )->Set( SetScissorEnabled );
		render->GetMember( "SetAlphaBlendingEnabled" )->Set( SetAlphaBlendingEnabled );
		render->GetMember( "SetTexturingEnabled" )->Set( SetTexturingEnabled );

		render->GetMember( "Clear" )->Set( Clear );
		render->GetMember( "ClearZ" )->Set( ClearZ );
		render->GetMember( "ClearStencil" )->Set( ClearStencil );

		render->GetMember( "SetViewport" )->Set( SetViewport );
		render->GetMember( "SetScissor" )->Set( SetScissor );

		render->GetMember( "SetStencilCompareFunction" )->Set( SetStencilCompareFunction );
		render->GetMember( "SetStencilOperations" )->Set( SetStencilOperations );

		render->GetMember( "Start2D" )->Set( Start2D );
		render->GetMember( "End2D" )->Set( End2D );
		render->GetMember( "Start3D" )->Set( Start3D );
		render->GetMember( "End3D" )->Set( End3D );

		render->GetMember( "SetTransform" )->Set( SetTransform );
		render->GetMember( "SetDrawColor" )->Set( SetDrawColor );
		render->GetMember( "SetTexture" )->Set( SetTexture );
		render->GetMember( "SetRenderTarget" )->Set( SetRenderTarget );
		render->GetMember( "SetShader" )->Set( SetShader );

		render->GetMember( "DrawPoint" )->Set( DrawPoint );
		render->GetMember( "DrawLine" )->Set( DrawLine );
		render->GetMember( "DrawTriangle" )->Set( DrawTriangle );
		render->GetMember( "DrawQuad" )->Set( DrawQuad );
		render->GetMember( "DrawRect" )->Set( DrawRect );
		render->GetMember( "DrawBox" )->Set( DrawBox );

		render->GetMember( "WorldToScreen" )->Set( WorldToScreen );
		render->GetMember( "ScreenToWorld" )->Set( ScreenToWorld );

		g_Lua->Globals()->GetMember( "render" )->Set( render );
	}
};
