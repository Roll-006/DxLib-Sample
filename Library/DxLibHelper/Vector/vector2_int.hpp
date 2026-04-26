#pragma once
#include <cmath>
#include <nlohmann/json.hpp>

struct Vector2Int
{
    int x;
	int y;


	Vector2Int operator+() const				{ return *this; }
	Vector2Int operator-() const				{ return { -x, -y }; }

    Vector2Int& operator+=(const Vector2Int& v)	{ x += v.x; y += v.y; return *this; }
    Vector2Int& operator-=(const Vector2Int& v)	{ x -= v.x; y -= v.y; return *this; }
    Vector2Int& operator*=(const Vector2Int& v)	{ x *= v.x; y *= v.y; return *this; }

	template<typename T>
	Vector2Int& operator*=(const T scale)		{ x *= scale; y *= scale; return *this; }

	template<typename T>
	Vector2Int& operator/=(const T scale)		{ x /= scale; y /= scale; return *this; }


	[[nodiscard]] static Vector2Int GetZero()	{ return {  0,  0 }; }
	[[nodiscard]] static Vector2Int GetLeft()	{ return { -1,  0 }; }
	[[nodiscard]] static Vector2Int GetRight()	{ return {  1,  0 }; }
	[[nodiscard]] static Vector2Int GetDown()	{ return {  0, -1 }; }
	[[nodiscard]] static Vector2Int GetUp()		{ return {  0,  1 }; }

	/// @brief 大きさを取得する
	/// @return 大きさ
	[[nodiscard]] float GetMagnitude() const
	{
		return sqrtf(static_cast<float>(x * x + y * y));
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
	[[nodiscard]] Vector2Int GetNormalized() const
	{
		auto v = *this;
		v.Normalize();
		return v;
	}
};


inline Vector2Int operator+(const Vector2Int& v1, const Vector2Int& v2)	{ return { v1.x + v2.x, v1.y + v2.y }; }
inline Vector2Int operator-(const Vector2Int& v1, const Vector2Int& v2)	{ return { v1.x - v2.x, v1.y - v2.y }; }
inline Vector2Int operator*(const Vector2Int& v1, const Vector2Int& v2)	{ return { v1.x * v2.x, v1.y * v2.y }; }

template<typename T>
inline Vector2Int operator*(const Vector2Int& v, const T scale)			{ return { v.x * scale, v.y * scale }; }

template<typename T>
inline Vector2Int operator*(const T scale, const Vector2Int& v)			{ return v * scale; }

template<typename T>
inline Vector2Int operator/(const Vector2Int& v, const T scale)			{ return { v.x / scale, v.y / scale }; }

inline bool operator==(const Vector2Int& v1, const Vector2Int& v2)		{ return v1.x == v2.x && v1.y == v2.y; }
inline bool operator!=(const Vector2Int& v1, const Vector2Int& v2)		{ return !(v1 == v2); }


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Vector2Int& v)
{
	j_data.at("x").get_to(v.x);
	j_data.at("y").get_to(v.y);
}

inline void to_json(nlohmann::json& j_data, const Vector2Int& v)
{
	j_data = 
	{
		{ "x", v.x },
		{ "y", v.y }
	};
}
#pragma endregion
