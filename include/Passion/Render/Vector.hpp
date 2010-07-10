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

#ifndef PASSION_VECTOR_HPP
#define PASSION_VECTOR_HPP

#include <cmath>

namespace Passion
{
	struct Matrix;

	struct Vector
	{
		Vector( float X = 0.0f, float Y = 0.0f, float Z = 0.0f ) { x = X; y = Y; z = Z; }

		Vector operator+( Vector vec );
		Vector operator-( Vector vec );
		Vector operator-();
		Vector operator*( float n );
		Vector operator*( Matrix m );
		Vector operator/( float n );

		void operator+=( Vector v );
		void operator-=( Vector v );
		void operator*=( float n );
		void operator/=( float n );

		bool operator==( Vector v );
		bool operator<( Vector v );

		float Distance( Vector v );
		float Length();
		float LengthSqr();

		float Dot( Vector v );
		Vector Cross( Vector v );

		Vector Normal();
		void Normalize();

		float x, y, z;
	};

	inline Vector Vector::operator+( Vector vec )
	{
		return Vector( x + vec.x, y + vec.y, z + vec.z );
	}

	inline Vector Vector::operator-( Vector vec )
	{
		return Vector( x - vec.x, y - vec.y, z - vec.z );
	}

	inline Vector Vector::operator-()
	{
		return Vector( -x, -y, -z );
	}

	inline Vector Vector::operator*( float n )
	{
		return Vector( x * n, y * n, z * n );
	}

	inline Vector Vector::operator/( float n )
	{
		return Vector( x / n, y / n, z / n );
	}

	inline void Vector::operator+=( Vector v )
	{
		x += v.x; y += v.y; z += v.z;
	}

	inline void Vector::operator-=( Vector v )
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	inline void Vector::operator*=( float n )
	{
		x *= n; y *= n; z *= n;
	}

	inline void Vector::operator/=( float n )
	{
		x /= n; y /= n; z /= n;
	}

	inline bool Vector::operator==( Vector v )
	{
		return LengthSqr() == v.LengthSqr();
	}

	inline bool Vector::operator<( Vector v )
	{
		return LengthSqr() < v.LengthSqr();
	}

	inline float Vector::Distance( Vector v )
	{
		return ( *this - v ).Length();
	}

	inline float Vector::Length()
	{
		return sqrt( x*x + y*y + z*z );
	}

	inline float Vector::LengthSqr()
	{
		return x*x + y*y + z*z;
	}

	inline float Vector::Dot( Vector v )
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline Vector Vector::Cross( Vector v )
	{
		return Vector( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	inline Vector Vector::Normal()
	{
		float length = Length();
		return Vector( x / length, y / length, z / length );
	}

	inline void Vector::Normalize()
	{
		float length = Length();
		x /= length; y/= length; z /= length;
	}
}

#endif