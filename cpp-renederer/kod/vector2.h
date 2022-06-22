#pragma once

#include<iostream>

template <typename T>
class Vector2 {
public:
	T x;
	T y;

	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(T a) {
		x = a;
		y = a;
	}
	Vector2(T a, T b)
	{
		x = a;
		y = b;
	}
};

// Overload comparision operators
template<typename T>
bool operator ==(const Vector2<T>& left, const Vector2<T>& right) {
	return (left.x == right.x && left.y == right.y);
}
template<typename T>
bool operator !=(const Vector2<T>& left, const Vector2<T>& right) {
	return (left.x != right.x || left.y == right.y);
}

// Memberwise addition
template<typename T>
Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.x + right.x, left.y + right.y);
}
// Memberwise opposite
template<typename T>
Vector2<T> operator -(const Vector2<T>& right)
{
	return Vector2<T>(-right.x, -right.y);
}
// Memberwise subtraction
template<typename T>
Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.x - right.x, left.y - right.y);
}
// Memberwise multiplication by scalar
template<typename T>
Vector2<T> operator *(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.x * right, left.y * right);
}
template<typename T>
Vector2<T> operator *(T left, const Vector2<T>& right)
{
	return Vector2<T>(left * right.x, left * right.y);
}
// Memberwise division by scalar
template<typename T>
Vector2<T> operator /(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.x / right, left.y / right);
}


template<typename T>
std::ostream& operator<<(std::ostream& os, Vector2<T> const& right) {
	return os << "(" << right.x << "," << right.y << ")";
}
//Create common aliases
using Vec2i = Vector2<int>;
using Vec2f = Vector2<float>;
using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
