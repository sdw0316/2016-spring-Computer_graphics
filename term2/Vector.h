#pragma once

#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>

class Vec3
{
public:
	double x;
	double y;
	double z;
public:
	Vec3(void)
	{
		x = y = z = 0.0;
	}
	Vec3(double x0, double y0, double z0)
	{
		x = x0;
		y = y0;
		z = z0;
	}
	~Vec3(void)
	{
	}
public:
	/*position method*/
	void setX(double value) {
		x = value;
	}
	void setY(double value) {
		y = value;
	}
	void setZ(double value) {
		z = value;
	}
	double getX() {
		return x;
	}
	double getY() {
		return y;
	}
	double getZ() {
		return z;
	}
	Vec3 rotateZ(double angle) {
		Vec3 result;
		result.x = x*cos(angle) - y*sin(angle);
		result.y = x*sin(angle) + y*cos(angle);
		result.z = z;
		return result;
	}
	/*vector operation*/
	double innerProduct(Vec3 v) {
		return (x * v.x + y * v.y + z * v.z);
	}
	Vec3 crossProduct(Vec3 v) {
		Vec3 result;
		result.x = y*v.z - z*v.y;
		result.y = z*v.x - x*v.z;
		result.z = x*v.y - y*v.x;
		return result;
	}
public:
	void operator=(Vec3 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vec3 operator+(Vec3 v)
	{
		Vec3 res;
		res.x = x + v.x;
		res.y = y + v.y;
		res.z = z + v.z;
		return res;
	}
	Vec3 operator-(Vec3 v)
	{
		Vec3 res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	}
	Vec3 operator/(double v)
	{
		Vec3 res;
		res.x = x / v;
		res.y = y / v;
		res.z = z / v;
		return res;
	}
	Vec3 operator*(double v)
	{
		Vec3 res;
		res.x = x * v;
		res.y = y * v;
		res.z = z * v;
		return res;
	}
	Vec3 &operator+=(Vec3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;;
		return *this;
	}
	Vec3 &operator-=(Vec3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;;
		return *this;
	}
	double length()
	{
		return sqrt(x*x + y*y + z*z);
	}
	double dist()
	{
		return sqrt(x*x + y*y + z*z);
	}
	double dist(Vec3 p)
	{
		return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y) + (z - p.z)*(z - p.z));
	}
};
