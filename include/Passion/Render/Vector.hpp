#ifndef PASSION_VECTOR_HPP
#define PASSION_VECTOR_HPP

#include <cmath>

namespace Passion
{
	struct Vector
	{
	public:
		Vector( float X = 0.0f, float Y = 0.0f, float Z = 0.0f ) { x = X; y = Y; z = Z; }

		Vector operator+( Vector& vec );
		Vector operator-( Vector& vec );
		Vector operator*( float n );

		float Length();
		float LengthSqr();

		Vector Normal();

		float x, y, z;
	};

	inline Vector Vector::operator+( Vector& vec )
	{
		return Vector( x + vec.x, y + vec.y, z + vec.z );
	}

	inline Vector Vector::operator-( Vector& vec )
	{
		return Vector( x - vec.x, y - vec.y, z - vec.z );
	}

	inline Vector Vector::operator*( float n )
	{
		return Vector( x * n, y * n, z * n );
	}

	inline float Vector::Length()
	{
		return sqrt( x*x + y*y + z*z );
	}

	inline float Vector::LengthSqr()
	{
		return x*x + y*y + z*z;
	}

	inline Vector Vector::Normal()
	{
		float length = Length();
		return Vector( x / length, y / length, z / length );
	}
}

#endif