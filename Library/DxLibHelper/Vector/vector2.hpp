#pragma once
#include <cmath>
#include <nlohmann/json.hpp>

struct Vector2
{
    float x;
	float y;


	Vector2 operator+() const				{ return *this; }
	Vector2 operator-()	const				{ return { -x, -y }; }

    Vector2& operator+=(const Vector2& v)	{ x += v.x; y += v.y; return *this; }
    Vector2& operator-=(const Vector2& v)	{ x -= v.x; y -= v.y; return *this; }
    Vector2& operator*=(const Vector2& v)	{ x *= v.x; y *= v.y; return *this; }

	template<typename T>
	Vector2& operator*=(const T scale)		{ x *= scale; y *= scale; return *this; }

	template<typename T>
	Vector2& operator/=(const T scale)		{ x /= scale; y /= scale; return *this; }


	[[nodiscard]] static Vector2 GetZero()	{ return {  0.0f,  0.0f }; }
	[[nodiscard]] static Vector2 GetLeft()	{ return { -1.0f,  0.0f }; }
	[[nodiscard]] static Vector2 GetRight() { return {  1.0f,  0.0f }; }
	[[nodiscard]] static Vector2 GetDown()	{ return {  0.0f, -1.0f }; }
	[[nodiscard]] static Vector2 GetUp()	{ return {  0.0f,  1.0f }; }

	/// @brief 大きさを取得する
	/// @return 大きさ
	[[nodiscard]] float GetMagnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	/// @brief 大きさの二乗を取得する
	/// @return 大きさの二乗
	[[nodiscard]] float GetSqrMagnitude() const
	{
		return x * x + y * y;
	}

	/// @brief 自身を正規化する
	void Normalize()
	{
		auto magnitude = GetMagnitude();
		if (magnitude != 0.0f)
		{
			*this /= magnitude;
		}
	}

	/// @brief 正規化したベクトルを取得する (自身に影響はない)
	/// @return 正規化後のベクトル
	[[nodiscard]] Vector2 GetNormalized() const
	{
		auto v = *this;
		v.Normalize();
		return v;
	}
};


inline Vector2 operator+(const Vector2& v1, const Vector2& v2)	{ return { v1.x + v2.x, v1.y + v2.y }; }
inline Vector2 operator-(const Vector2& v1, const Vector2& v2)	{ return { v1.x - v2.x, v1.y - v2.y }; }
inline Vector2 operator*(const Vector2& v1, const Vector2& v2)	{ return { v1.x * v2.x, v1.y * v2.y }; }

template<typename T>
inline Vector2 operator*(const Vector2& v, const T scale)		{ return { v.x * scale, v.y * scale }; }

template<typename T>
inline Vector2 operator*(const T scale, const Vector2& v)		{ return v * scale; }

template<typename T>
inline Vector2 operator/(const Vector2& v, const T scale)		{ return { v.x / scale, v.y / scale }; }

inline bool operator==(const Vector2& v1, const Vector2& v2)	{ return v1.x == v2.x && v1.y == v2.y; }
inline bool operator!=(const Vector2& v1, const Vector2& v2)	{ return !(v1 == v2); }


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Vector2& v)
{
	j_data.at("x").get_to(v.x);
	j_data.at("y").get_to(v.y);
}

inline void to_json(nlohmann::json& j_data, const Vector2& v)
{
	j_data = 
	{
		{ "x", v.x },
		{ "y", v.y }
	};
}
#pragma endregion
