#pragma once
#include <cmath>
#include <nlohmann/json.hpp>
#include <DxLib.h>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{ }

	Vector3(const float x, const float y, const float z) : 
		x(x),
		y(y),
		z(z)
	{ }

	Vector3(const VECTOR& v) : 
		x(v.x),
		y(v.y),
		z(v.z)
	{ }


	operator VECTOR() const { return { x, y, z }; }

	Vector3 operator+() const				{ return *this; }
	Vector3 operator-()	const				{ return { -x, -y, -z }; }

	Vector3& operator+=(const Vector3& v)	{ x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(const Vector3& v)	{ x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(const Vector3& v)	{ x *= v.x; y *= v.y; z *= v.z; return *this; }

	template<typename T>
	Vector3& operator*=(const T scale)		{ x *= scale; y *= scale; z *= scale; return *this; }

	template<typename T>
	Vector3& operator/=(const T scale)		{ x /= scale; y /= scale; z /= scale; return *this; }


	[[nodiscard]] static Vector3 GetZero()		{ return {  0.0f,  0.0f,  0.0f }; }
	[[nodiscard]] static Vector3 GetLeft()		{ return { -1.0f,  0.0f,  0.0f }; }
	[[nodiscard]] static Vector3 GetRight()		{ return {  1.0f,  0.0f,  0.0f }; }
	[[nodiscard]] static Vector3 GetDown()		{ return {  0.0f, -1.0f,  0.0f }; }
	[[nodiscard]] static Vector3 GetUp()		{ return {  0.0f,  1.0f,  0.0f }; }
	[[nodiscard]] static Vector3 GetBack()		{ return {  0.0f,  0.0f, -1.0f }; }
	[[nodiscard]] static Vector3 GetForward()	{ return {  0.0f,  0.0f,  1.0f }; }

	/// @brief 内積を取得
	[[nodiscard]] static float GetDot(const Vector3& v1, const Vector3& v2)
	{
		return{ v1.x * v2.x + v1.y * v2.y + v1.z * v2.z };
	}

	/// @brief 外積を取得
	[[nodiscard]] static Vector3 GetCross(const Vector3& v1, const Vector3& v2)
	{
		return{ v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x };
	}

	/// @brief 大きさを取得する
	/// @return 大きさ
	[[nodiscard]] float GetMagnitude() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	/// @brief 大きさの二乗を取得する
	/// @return 大きさの二乗
	[[nodiscard]] float GetSqrMagnitude() const
	{
		return x * x + y * y + z * z;
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
	[[nodiscard]] Vector3 GetNormalized() const
	{
		auto v = *this;
		v.Normalize();
		return v;
	}
};


inline Vector3 operator+(const Vector3& v1, const Vector3& v2)	{ return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2)	{ return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; }
inline Vector3 operator*(const Vector3& v1, const Vector3& v2)	{ return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z }; }

template<typename T>
inline Vector3 operator*(const Vector3& v, const T scale)		{ return { v.x * scale, v.y * scale, v.z * scale }; }

template<typename T>
inline Vector3 operator*(const T scale, const Vector3& v)		{ return v * scale; }

template<typename T>
inline Vector3 operator/(const Vector3& v, const T scale)		{ return { v.x / scale, v.y / scale, v.z / scale }; }

inline bool operator==(const Vector3& v1, const Vector3& v2)	{ return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
inline bool operator!=(const Vector3& v1, const Vector3& v2)	{ return !(v1 == v2); }


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Vector3& v)
{
	j_data.at("x").get_to(v.x);
	j_data.at("y").get_to(v.y);
	j_data.at("z").get_to(v.z);
}

inline void to_json(nlohmann::json& j_data, const Vector3& v)
{
	j_data = 
	{
		{ "x", v.x },
		{ "y", v.y },
		{ "z", v.z }
	};
}
#pragma endregion
