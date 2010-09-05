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

#ifndef PASSION_MATRIX_HPP
#define PASSION_MATRIX_HPP

#include <cmath>

namespace Passion
{
	struct Matrix
	{
		Matrix();
		Matrix( float* m );

		Vector operator*( Vector v );
		Matrix operator*( Matrix m );
		void operator*=( Matrix m );

		void Translate( Vector v );
		void Scale( Vector v );

		void RotateX( float ang );
		void RotateY( float ang );
		void RotateZ( float ang );

		float m[4][4];
	};

	inline Matrix::Matrix()
	{
		for ( short c = 0; c < 4; c++ )
			for ( short r = 0; r < 4; r++ )
				if ( r == c )
					m[c][r] = 1;
				else
					m[c][r] = 0;
	}

	inline Matrix::Matrix( float* v )
	{
		for ( short c = 0; c < 4; c++ )
			for ( short r = 0; r < 4; r++ )
				m[c][r] = v[r*4+c];
	}

	inline Vector Matrix::operator*( Vector v )
	{
		float x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
		float y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
		float z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];

		return Vector( x, y, z );
	}

	inline Matrix Matrix::operator*( Matrix mat )
	{
		Matrix res;

		for ( short c = 0; c < 4; c++ )
			for ( short r = 0; r < 4; r++ )
				res.m[c][r] = (*this).m[0][r] * mat.m[c][0] + (*this).m[1][r] * mat.m[c][1] +
					(*this).m[2][r] * mat.m[c][2] + (*this).m[3][r] * mat.m[c][3];

		return res;
	}

	inline void Matrix::operator*=( Matrix m )
	{
		*this = *this * m;
	}

	inline void Matrix::Translate( Vector v )
	{
		float mat[] = {
			1, 0, 0, v.x,
			0, 1, 0, v.y,
			0, 0, 1, v.z,
			0, 0, 0, 1
		};
		*this *= Matrix( mat );
	}

	inline void Matrix::Scale( Vector v )
	{
		float mat[] = {
			v.x, 0, 0, 0,
			0, v.y, 0, 0,
			0, 0, v.z, 0,
			0, 0, 0, 1
		};
		*this *= Matrix( mat );
	}

	inline void Matrix::RotateX( float ang )
	{
		float mat[] = {
			1, 0, 0, 0,
			0, cos( ang ), -sin( ang ), 0,
			0, sin( ang ), cos( ang ), 0,
			0, 0, 0, 1
		};
		*this *= Matrix( mat );
	}

	inline void Matrix::RotateY( float ang )
	{
		float mat[] = {
			cos( ang ), 0, sin( ang ), 0,
			0, 1, 0, 0,
			-sin( ang ), 0, cos( ang ), 0,
			0, 0, 0, 1
		};
		*this *= Matrix( mat );
	}

	inline void Matrix::RotateZ( float ang )
	{
		float mat[] = {
			cos( ang ), sin( ang ), 0, 0,
			-sin( ang ), cos( ang ), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		*this *= Matrix( mat );
	}

	inline Vector Vector::operator*( Matrix m )
	{
		return m.operator*( *this );
	}
}

#endif
