#pragma once

#include<iostream>
#include<math.h>

template <typename T>
class Vector3 {
public:
	T x;
	T y;
	T z;

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(T a, T b, T c)
	{
		x = a;
		y = b;
		z = c;
	}
};

// Vec3 cross product
template<typename T>
Vector3<T> cross(const Vector3<T> a, const Vector3<T> b) {
	return Vector3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
// Vec3 dot product
template<typename T>
float dot(const Vector3<T> a, const Vector3<T> b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
// vec3 Magnitude
template<typename T>
float magnitude(const Vector3<T> a) {
	return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}
// Angle between vectors
template<typename T>
float angleBetween(const Vector3<T> a, const Vector3<T> b)
{
	float angle = dot(a, b);
	angle /= (magnitude(a) * magnitude(b));
	return angle = acosf(angle);
}
// Projection of a onto b
template<typename T>
Vector3<T> projection(const Vector3<T> a, const Vector3<T> b)
{
	Vector3<T> bn = b / magnitude(b);
	return bn * dot(a, bn);
}
// normalization of vector - unit vector in original vector's direction
template<typename T>
Vector3<T> normalize(const Vector3<T> v) 
{
	T len = magnitude(v);
	return Vector3<T>(v.x / len, v.y / len, v.z / len);
}




// Overload comparision operators
template<typename T>
bool operator ==(const Vector3<T>& left, const Vector3<T>& right) {
	return (left.x == right.x && left.y == right.y && left.z == right.z);
}
template<typename T>
bool operator !=(const Vector3<T>& left, const Vector3<T>& right) {
	return (left.x != right.x || left.y != right.y || left.z != right.z);
}

// Memberwise addition
template<typename T>
Vector3<T> operator +(const Vector3<T>& left, const Vector3<T>& right)
{
	return Vector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}
// Memberwise opposite
template<typename T>
Vector3<T> operator -(const Vector3<T>& right)
{
	return Vector3<T>(-right.x, -right.y, -right.z);
}
// Memberwise subtraction
template<typename T>
Vector3<T> operator -(const Vector3<T>& left, const Vector3<T>& right)
{
	return Vector3<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}
// Memberwise multiplication by scalar
template<typename T>
Vector3<T> operator *(const Vector3<T>& left, T right)
{
	return Vector3<T>(left.x * right, left.y * right, left.z * right);
}
template<typename T>
Vector3<T> operator *(T left, const Vector3<T>& right)
{
	return Vector3<T>(left * right.x, left * right.y, left * right.z);
}
// Memberwise division by scalar
template<typename T>
Vector3<T> operator /(const Vector3<T>& left, T right)
{
	return Vector3<T>(left.x / right, left.y / right, left.z / right);
}


template<typename T>
std::ostream& operator<<(std::ostream& os, Vector3<T> const& right) {
	return os << "(" << right.x << "," << right.y << "," << right.z << ")";
}
//Create common aliases
using Vec3i = Vector3<int>;
using Vector3i = Vector3<int>;
using Vec3f = Vector3<float>;
using Vector3f = Vector3<float>;
