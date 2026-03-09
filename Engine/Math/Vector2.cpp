#include "Vector2.h"
#include <iostream>

namespace Wanted
{
	Vector2 Vector2::Zero(0, 0);
	Vector2 Vector2::One(1, 1);
	Vector2 Vector2::Up(0, 1);
	Vector2 Vector2::Right(1, 0);

	Vector2::Vector2()
	{
	}

	Vector2::Vector2(int x, int y)
		: x(x), y(y)
	{
	}
	
	Vector2::~Vector2()
	{
		if (string)
		{

			delete[] string;
			string = nullptr;
		}
	}
	
	const char* Vector2::ToString()
	{
		// 기존 문자열이 있다면 제거.
		if (string)
		{
			delete[] string;
			string = nullptr;
		}

		string = new char[128];
		memset(string, 0, sizeof(char) * 128);
		sprintf_s(string, 128, "(%d, %d)", x, y);

		return string;
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}

	Vector2 Vector2::operator*(const float other) const
	{
		return Vector2(x* other, y * other);
	}

	Vector2::operator COORD() const
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord;
	}

	float Vector2::Length()
	{
		return sqrtf(x * x + y * y);
	}

	Vector2 Vector2::Normalize() {
		 float length = sqrt(x * x + y * y);

		 if (length > 0.0f) {
			 float invLength = 1.0f / length; // 나눗셈은 딱 한 번 진행(연산비용이 크므로)
			 return Vector2(x * invLength, y * invLength); // 이후는 빠른 곱셈
		 }

		 return Vector2(0, 0);
	}

	float Vector2::Dot(Vector2 b) {
		return (x * b.x) + (y * b.y);
	}
}