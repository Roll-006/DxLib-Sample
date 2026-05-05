#pragma once
#include <numbers>
#include <DirectXMath.h>
#include <DxLib.h>
#include <nlohmann/json.hpp>

using namespace DirectX;

namespace math
{
	static constexpr float kDeg2Rad = static_cast<float>(std::numbers::pi) / 180.0f;		// ディグリーをラジアンに変換 (変換対象と掛け算を行う)
	static constexpr float kRad2Deg = 180.0f / static_cast<float>(std::numbers::pi);		// ラジアンをディグリーに変換 (変換対象と掛け算を行う)
	static constexpr float kEpsilon = 1e-05f;

	struct Vector2
	{
		float x;
		float y;

		#pragma region constructor
		Vector2() = default;
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}
		constexpr Vector2(const DirectX::XMFLOAT2& v) noexcept : x(v.x), y(v.y) {}
		#pragma endregion

		#pragma region operator
		Vector2& operator=(const Vector2&) = default;
		Vector2& operator=(Vector2&&) = default;

		operator DirectX::XMFLOAT2() const noexcept { return { x, y }; }
		#pragma endregion

		#pragma region store / load
		[[nodiscard]] static Vector2 StoreFromSIMD(const DirectX::XMVECTOR& v)
		{
			auto result = DirectX::XMFLOAT2();
			XMStoreFloat2(&result, v);
			return result;
		}

		[[nodiscard]] DirectX::XMVECTOR LoadToSIMD() const
		{
			return XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(this));
		}
		#pragma endregion
	};

	struct Vector3
	{
		float x;
		float y;
		float z;

		#pragma region constructor
		Vector3() = default;
		Vector3(const Vector3&) = default;
		Vector3(Vector3&&) = default;
		constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
		constexpr Vector3(const VECTOR& v) noexcept : x(v.x), y(v.y), z(v.z) {}
		constexpr Vector3(const DirectX::XMFLOAT3& v) noexcept : x(v.x), y(v.y), z(v.z) {}
		#pragma endregion

		#pragma region operator
		Vector3& operator=(const Vector3&) = default;
		Vector3& operator=(Vector3&&) = default;

		operator VECTOR() const noexcept { return { x, y, z }; }
		operator DirectX::XMFLOAT3() const noexcept { return { x, y, z }; }
		#pragma endregion

		#pragma region store / load
		[[nodiscard]] static Vector3 StoreFromSIMD(const DirectX::XMVECTOR& v)
		{
			auto result = DirectX::XMFLOAT3();
			XMStoreFloat3(&result, v);
			return result;
		}

		[[nodiscard]] DirectX::XMVECTOR LoadToSIMD() const
		{
			return XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(this));
		}
		#pragma endregion
	};

	struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		#pragma region constructor
		Quaternion() = default;
		Quaternion(const Quaternion&) = default;
		Quaternion(Quaternion&&) = default;
		constexpr Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
		constexpr Quaternion(const FLOAT4& q) noexcept : x(q.x), y(q.y), z(q.z), w(q.w) {}
		constexpr Quaternion(const DirectX::XMFLOAT4& q) noexcept : x(q.x), y(q.y), z(q.z), w(q.w) {}
		#pragma endregion

		#pragma region operator
		Quaternion& operator=(const Quaternion&) = default;
		Quaternion& operator=(Quaternion&&) = default;

		operator FLOAT4() const noexcept { return { x, y, z, w }; }
		operator DirectX::XMFLOAT4() const noexcept { return { x, y, z, w }; }
		#pragma endregion

		#pragma region store / load
		[[nodiscard]] static Quaternion StoreFromSIMD(const DirectX::XMVECTOR& v)
		{
			auto result = DirectX::XMFLOAT4();
			XMStoreFloat4(&result, v);
			return result;
		}

		[[nodiscard]] DirectX::XMVECTOR LoadToSIMD() const
		{
			return XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4*>(this));
		}
		#pragma endregion
	};

	struct Matrix4x4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};

		#pragma region constructor
		Matrix4x4() = default;
		Matrix4x4(const Matrix4x4&) = default;
		Matrix4x4(Matrix4x4&&) = default;

		constexpr Matrix4x4(float m00, float m01, float m02, float m03,
							float m10, float m11, float m12, float m13,
							float m20, float m21, float m22, float m23,
							float m30, float m31, float m32, float m33) noexcept :
			_11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23),
			_41(m30), _42(m31), _43(m32), _44(m33) {}

		constexpr Matrix4x4(const MATRIX& mat) noexcept : 
			_11(mat.m[0][0]), _12(mat.m[0][1]), _13(mat.m[0][2]), _14(mat.m[0][3]),
			_21(mat.m[1][0]), _22(mat.m[1][1]), _23(mat.m[1][2]), _24(mat.m[1][3]),
			_31(mat.m[2][0]), _32(mat.m[2][1]), _33(mat.m[2][2]), _34(mat.m[2][3]),
			_41(mat.m[3][0]), _42(mat.m[3][1]), _43(mat.m[3][2]), _44(mat.m[3][3]) {}

		constexpr Matrix4x4(const DirectX::XMFLOAT4X4& mat) noexcept :
			_11(mat._11), _12(mat._12), _13(mat._13), _14(mat._14),
			_21(mat._21), _22(mat._22), _23(mat._23), _24(mat._24),
			_31(mat._31), _32(mat._32), _33(mat._33), _34(mat._34),
			_41(mat._41), _42(mat._42), _43(mat._43), _44(mat._44) {}
		#pragma endregion

		#pragma region operator
		Matrix4x4& operator=(const Matrix4x4&) = default;
		Matrix4x4& operator=(Matrix4x4&&) = default;

		operator MATRIX() const noexcept
		{
			MATRIX mat{};

			for (size_t i = 0; i < 4; ++i)
			{
				for (size_t j = 0; j < 4; ++j)
				{
					mat.m[i][j] = m[i][j];
				}
			}

			return mat;
		}

		operator DirectX::XMFLOAT4X4() const noexcept
		{
			return{ _11, _12, _13, _14,
					_21, _22, _23, _24,
					_31, _32, _33, _34,
					_41, _42, _43, _44 };
		}

		float  operator() (size_t row, size_t column) const noexcept { return m[row][column]; }
		float& operator() (size_t row, size_t column) noexcept { return m[row][column]; }
		#pragma endregion

		#pragma region store / load
		[[nodiscard]] static Matrix4x4 StoreFromSIMD(const DirectX::XMMATRIX& v)
		{
			auto result = DirectX::XMFLOAT4X4();
			XMStoreFloat4x4(&result, v);
			return result;
		}

		[[nodiscard]] DirectX::XMMATRIX LoadToSIMD() const
		{
			return XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(this));
		}
		#pragma endregion
	};

	#pragma region from / to JSON
	inline void from_json(const nlohmann::json& j, Vector2& v)
	{
		j.at("x").get_to(v.x);
		j.at("y").get_to(v.y);
	}

	inline void to_json(nlohmann::json& j, const Vector2& v)
	{
		j =
		{
			{ "x", v.x },
			{ "y", v.y }
		};
	}

	inline void from_json(const nlohmann::json& j, Vector3& v)
	{
		j.at("x").get_to(v.x);
		j.at("y").get_to(v.y);
		j.at("z").get_to(v.z);
	}

	inline void to_json(nlohmann::json& j, const Vector3& v)
	{
		j = 
		{
			{ "x", v.x },
			{ "y", v.y },
			{ "z", v.z }
		};
	}

	inline void from_json(const nlohmann::json& j, Quaternion& q)
	{
		j.at("x").get_to(q.x);
		j.at("y").get_to(q.y);
		j.at("z").get_to(q.z);
		j.at("w").get_to(q.w);
	}

	inline void to_json(nlohmann::json& j, const Quaternion& q)
	{
		j =
		{
			{ "x", q.x },
			{ "y", q.y },
			{ "z", q.z },
			{ "w", q.w }
		};
	}

	inline void from_json(const nlohmann::json& j, Matrix4x4& mat)
	{
		const auto& m = j.at("m");

		for (size_t i = 0; i < 4; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				mat.m[i][j] = m[i][j];
			}
		}
	}

	inline void to_json(nlohmann::json& j, const Matrix4x4& mat)
	{
		j =
		{{ "m",
			{{ mat._11, mat._12, mat._13, mat._14 },
			 { mat._21, mat._22, mat._23, mat._24 },
			 { mat._31, mat._32, mat._33, mat._34 },
			 { mat._41, mat._42, mat._43, mat._44 }}}};
	}
	#pragma endregion
}
