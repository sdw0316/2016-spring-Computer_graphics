#pragma once
#include <math.h>

class VECTOR {
public:
	float Magnitude();
	float InnerProduct(VECTOR v);
	VECTOR CrossProduct(VECTOR v);
	void Normalize();

	float x;
	float y;
	float z;

	VECTOR() {
	}

	VECTOR(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	VECTOR operator +(VECTOR v) {
		VECTOR result = (*this);
		result.x += v.x;
		result.y += v.y;
		result.z += v.z;

		return result;
	}

	VECTOR operator -(VECTOR v) {
		VECTOR result = (*this);
		result.x -= v.x;
		result.y -= v.y;
		result.z -= v.z;

		return result;
	}

	friend VECTOR operator * (VECTOR v, float val)
	{
		v.x *= val;
		v.y *= val;
		v.z *= val;
		return v;
	}

	friend VECTOR operator * (float val, VECTOR v)
	{
		v.x *= val;
		v.y *= val;
		v.z *= val;
		return v;
	}
};

float VECTOR::Magnitude() {
	return sqrt(x*x + y*y + z*z);
}

float VECTOR::InnerProduct(VECTOR v) {
	return (x * v.x + y * v.y + z * v.z);
}

VECTOR VECTOR::CrossProduct(VECTOR v) {
	VECTOR result;
	result.x = y*v.z - z*v.y;
	result.y = z*v.x - x*v.z;
	result.z = x*v.y - y*v.x;
	return result;
}

void VECTOR::Normalize() {
	float w = Magnitude();
	if (w < 0.00001) return;
	x /= w;
	y /= w;
	z /= w;
}

