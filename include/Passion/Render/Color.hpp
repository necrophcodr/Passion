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