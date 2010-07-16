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

// Headers
#include "CDefaultScene.hpp"
#include <cmath>

CDefaultScene::CDefaultScene( Passion::IBaseRender* render )
{
	m_render = render;

	cloud = m_render->LoadTexture( "textures/cloud.png", false );
	logo = m_render->LoadTexture( "textures/logo.png", false );
	
	for ( unsigned int x = 0; x < 3; x++ )
		for ( unsigned int y = 0; y < 3; y++ )
			clouds[x*3+y] = Passion::Vector( 20.0f + x * 500.0f + (float)rand() / (float)RAND_MAX * 250.0f, 20.0f + y * 290.0f + (float)rand() / (float)RAND_MAX * 150.0f );
}

void CDefaultScene::Update( float time )
{
	for ( unsigned int i = 0; i < 9; i++ )
	{
		if ( clouds[i].x > 1500.0f ) clouds[i].x = -600.0f;
		clouds[i].x += time * 30.0f;
	}
}

void CDefaultScene::Draw()
{
	m_render->Start2D();
		m_render->SetTransform( Passion::Matrix() );
		m_render->SetTexture();
			
		float r1 = 0.45f; float g1 = 0.73f; float b1 = 0.86f;
		float r2 = 0.20f; float g2 = 0.60f; float b2 = 0.80f;
		for ( unsigned y = 0; y <= 720; y++ )
		{
			float percent = (float)y / 720.0f;
			m_render->SetDrawColor( Passion::Color( percent * ( r2 - r1 ) + r1, percent * ( g2 - g1 ) + g1, percent * ( b2 - b1 ) + b1 ) );
			m_render->DrawLine( Passion::Vector( 0.0f, y ), Passion::Vector( 1280.0f, y ) );
		}
	m_render->End2D();

	m_render->Start3D( Passion::Vector( 700.0f + sin( clouds[0].x / 100.0f ) * 200.0f, 100.0f, -800.0f ), Passion::Vector( 640.0f, 400.0f, 0.0f ), 45.0f, 1.0f, 10000.0f, Passion::Vector( 0.0f, 0.0f, -1.0f ) );
		m_render->SetTexture( logo );
		m_render->SetDrawColor( Passion::Color( 1.0f, 1.0f, 1.0f, 0.5f ) );
		m_render->DrawRect( 392.0f, 312.0f, 496.0f, 158.0f );
		
		m_render->SetDrawColor( Passion::Color() );
		m_render->SetTexture( cloud );

		for ( unsigned int i = 0; i < 9; i++ )
		{
			Passion::Matrix depth;
			depth.Translate( Passion::Vector( 0.0f, 0.0f, sin( ( clouds[i].x + clouds[i].y ) / 200.0f ) * 20.0f ) );
			m_render->SetTransform( depth );

			m_render->DrawRect( clouds[i].x, clouds[i].y + sin( clouds[i].x / 10.0f ), 243.0f + sin( ( clouds[i].x + clouds[i].y ) / 50.0f ) * 10.0f, 135.0f + sin( ( clouds[i].x + clouds[i].y ) / 50.0f ) * 5.0f );
		}
	m_render->End3D();
}