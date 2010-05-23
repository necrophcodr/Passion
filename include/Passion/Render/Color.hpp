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

#ifndef PASSION_COLOR_HPP
#define PASSION_COLOR_HPP

namespace Passion
{
	struct Color
	{
		float R, G, B, A;

		Color( float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f );

		bool operator==( const Color& color );
		bool operator!=( const Color& color );
	};

	inline Color::Color( float r, float g, float b, float a )
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	inline bool Color::operator==( const Color& color )
	{
		return R == color.R && G == color.G && B == color.B && A == color.A;
	}

	inline bool Color::operator!=( const Color& color )
	{
		return !(operator==( color ) );
	}
}

#endif