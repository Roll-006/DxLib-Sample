#pragma once
#include <cmath>
#include <nlohmann/json.hpp>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3 operator+() const				{ return *this; }
	Vector3 operator-()						{ x = -x; y = -y; z = -z; return *this; }

	Vector3& operator+=(const Vector3& v)	{ x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(const Vector3& v)	{ x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(const Vector3& v)	{ x *= v.x; y *= v.y; z *= v.z; return *this; }

	template<typename T>
	Vector3& operator*=(const T scale)		{ x *= scale; y *= scale; z *= scale; return *this; }

	template<typename T>
	Vector3& operator/=(const T scale)		{ x /= scale; y /= scale; z /= scale; return *this; }

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
	j_data = nlohmann::json
	{
		{ "x", v.x },
		{ "y", v.y },
		{ "z", v.z }
	};
}
#pragma endregion
