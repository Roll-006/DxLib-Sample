#pragma once
#include <numbers>
#include <DirectXMath.h>
#include <DxLib.h>
#include <nlohmann/json.hpp>

using namespace DirectX;

namespace math
{
	#pragma region constant
	static constexpr float kDeg2Rad = static_cast<float>(std::numbers::pi) / 180.0f;		// ディグリーをラジアンに変換 (変換対象と掛け算を行う)
	static constexpr float kRad2Deg = 180.0f / static_cast<float>(std::numbers::pi);		// ラジアンをディグリーに変換 (変換対象と掛け算を行う)
	static constexpr float kEpsilon = 1.0e-5f;
	#pragma endregion

	struct Vector2
	{
		float x;
		float y;

		#pragma region constructor
		Vector2() = default;
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}
		constexpr Vector2(const DirectX::XMFLOAT2& vector) noexcept : x(vector.x), y(vector.y) {}
		#pragma endregion

		#pragma region operator
		Vector2& operator=(const Vector2&) = default;
		Vector2& operator=(Vector2&&) = default;

		operator DirectX::XMFLOAT2() const noexcept { return { x, y }; }
		#pragma endregion

		#pragma region store / load
		[[nodiscard]] static Vector2 StoreFromSIMD(const DirectX::XMVECTOR& vector)
		{
			auto result = DirectX::XMFLOAT2();
			XMStoreFloat2(&result, vector);
			return result;
		}

		[[nodiscard]] DirectX::XMVECTOR LoadToSIMD() const
		{
			return XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(this));
		}
		#pragma endregion

		#pragma region function
		[[nodiscard]] static float GetLength(const DirectX::XMVECTOR& vector)
		{
			return XMVectorGetX(XMVector2Length(vector));
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
		constexpr Vector3(const DxLib::VECTOR& vector) noexcept : x(vector.x), y(vector.y), z(vector.z) {}
		constexpr Vector3(const DirectX::XMFLOAT3& vector) noexcept : x(vector.x), y(vector.y), z(vector.z) {}
		#pragma endregion

		#pragma region operator
		Vector3& operator=(const Vector3&) = default;
		Vector3& operator=(Vector3&&) = default;

		operator DxLib::VECTOR() const noexcept { return { x, y, z }; }
		operator DirectX::XMFLOAT3() const noexcept { return { x, y, z }; }
		#pragma endregion

		#pragma region store / load
		[[nodiscard]] static Vector3 StoreFromSIMD(const DirectX::XMVECTOR& vector)
		{
			auto result = DirectX::XMFLOAT3();
			XMStoreFloat3(&result, vector);
			return result;
		}

		[[nodiscard]] DirectX::XMVECTOR LoadToSIMD() const
		{
			return XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(this));
		}
		#pragma endregion

		#pragma region function
		[[nodiscard]] static float GetLength(const DirectX::XMVECTOR& vector)
		{
			return XMVectorGetX(XMVector3Length(vector));
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
		constexpr Quaternion(const DxLib::FLOAT4& quaternion) noexcept : x(quaternion.x), y(quaternion.y), z(quaternion.z), w(quaternion.w) {}
		constexpr Quaternion(const DirectX::XMFLOAT4& quaternion) noexcept : x(quaternion.x), y(quaternion.y), z(quaternion.z), w(quaternion.w) {}
		#pragma endregion

		#pragma region operator
		Quaternion& operator=(const Quaternion&) = default;
		Quaternion& operator=(Quaternion&&) = default;

		operator DxLib::FLOAT4() const noexcept { return { x, y, z, w }; }
		operator DirectX::XMFLOAT4() const noexcept { return { x, y, z, w }; }
		#pragma endregion

		#pragma region store / load
		[[nodiscard]] static Quaternion StoreFromSIMD(const DirectX::XMVECTOR& quaternion)
		{
			auto result = DirectX::XMFLOAT4();
			XMStoreFloat4(&result, quaternion);
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

		constexpr Matrix4x4(const DxLib::MATRIX& matrix) noexcept :
			_11(matrix.m[0][0]), _12(matrix.m[0][1]), _13(matrix.m[0][2]), _14(matrix.m[0][3]),
			_21(matrix.m[1][0]), _22(matrix.m[1][1]), _23(matrix.m[1][2]), _24(matrix.m[1][3]),
			_31(matrix.m[2][0]), _32(matrix.m[2][1]), _33(matrix.m[2][2]), _34(matrix.m[2][3]),
			_41(matrix.m[3][0]), _42(matrix.m[3][1]), _43(matrix.m[3][2]), _44(matrix.m[3][3]) {}

		constexpr Matrix4x4(const DirectX::XMFLOAT4X4& matrix) noexcept :
			_11(matrix._11), _12(matrix._12), _13(matrix._13), _14(matrix._14),
			_21(matrix._21), _22(matrix._22), _23(matrix._23), _24(matrix._24),
			_31(matrix._31), _32(matrix._32), _33(matrix._33), _34(matrix._34),
			_41(matrix._41), _42(matrix._42), _43(matrix._43), _44(matrix._44) {}
		#pragma endregion

		#pragma region operator
		Matrix4x4& operator=(const Matrix4x4&) = default;
		Matrix4x4& operator=(Matrix4x4&&) = default;

		operator DxLib::MATRIX() const noexcept
		{
			DxLib::MATRIX matrix{};

			for (size_t i = 0; i < 4; ++i)
			{
				for (size_t j = 0; j < 4; ++j)
				{
					matrix.m[i][j] = m[i][j];
				}
			}

			return matrix;
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
		[[nodiscard]] static Matrix4x4 StoreFromSIMD(const DirectX::XMMATRIX& matrix)
		{
			auto result = DirectX::XMFLOAT4X4();
			XMStoreFloat4x4(&result, matrix);
			return result;
		}

		[[nodiscard]] DirectX::XMMATRIX LoadToSIMD() const
		{
			return XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(this));
		}
		#pragma endregion

		#pragma region function
		/// <summary>
		/// 平行移動行列を生成
		/// </summary>
		/// <param name="translation">平行移動成分</param>
		/// <returns>平行移動行列</returns>
		[[nodiscard]] static DirectX::XMMATRIX CreateTMatrix(const DirectX::XMVECTOR& translation)
		{
			auto result = DirectX::XMMatrixIdentity();
			result.r[3] = XMVectorSetW(translation, 1.0f);
			return result;
		}

		/// <summary>
		/// 回転行列を生成
		/// </summary>
		/// <param name="right">right軸</param>
		/// <param name="up">up軸</param>
		/// <param name="forward">forward軸</param>
		/// <returns>回転行列</returns>
		[[nodiscard]] static DirectX::XMMATRIX CreateRMatrix(const DirectX::XMVECTOR& right, const DirectX::XMVECTOR& up, const DirectX::XMVECTOR& forward)
		{
			auto result = DirectX::XMMatrixIdentity();
			result.r[0] = DirectX::XMVectorSetW(DirectX::XMVector3Normalize(right),		0.0f);
			result.r[1] = DirectX::XMVectorSetW(DirectX::XMVector3Normalize(up),		0.0f);
			result.r[2] = DirectX::XMVectorSetW(DirectX::XMVector3Normalize(forward),	0.0f);
			return result;
		}

		/// <summary>
		/// 平行移動行列を取得
		/// </summary>
		/// <param name="matrix">行列</param>
		/// <returns>平行移動行列</returns>
		[[nodiscard]] static DirectX::XMMATRIX GetTMatrix(const DirectX::XMMATRIX& matrix)
		{
			auto result = DirectX::XMMatrixIdentity();
			result.r[3] = DirectX::XMVectorSetW(matrix.r[3], 1.0f);
			return result;
		}

		/// <summary>
		/// 回転行列を取得
		/// </summary>
		/// <param name="matrix">行列</param>
		/// <returns>回転行列</returns>
		[[nodiscard]] static DirectX::XMMATRIX GetRMatrix(const DirectX::XMMATRIX& matrix)
		{
			auto result = DirectX::XMMatrixIdentity();
			result.r[0] = DirectX::XMVectorSetW(DirectX::XMVector3Normalize(matrix.r[0]), 0.0f);
			result.r[1] = DirectX::XMVectorSetW(DirectX::XMVector3Normalize(matrix.r[1]), 0.0f);
			result.r[2] = DirectX::XMVectorSetW(DirectX::XMVector3Normalize(matrix.r[2]), 0.0f);
			return result;
		}

		/// <summary>
		/// 平行移動成分を取得
		/// </summary>
		/// <param name="matrix">行列</param>
		/// <returns>平行移動成分</returns>
		[[nodiscard]] static DirectX::XMVECTOR GetTranslation(const DirectX::XMMATRIX& matrix)
		{
			DirectX::XMVECTOR scale, rotation, translation;
			DirectX::XMMatrixDecompose(&scale, &rotation, &translation, matrix);
			return translation;
		}

		/// <summary>
		/// 回転成分(クォータニオン)を取得
		/// </summary>
		/// <param name="matrix">行列</param>
		/// <returns>回転成分</returns>
		[[nodiscard]] static DirectX::XMVECTOR GetRotation(const DirectX::XMMATRIX& matrix)
		{
			DirectX::XMVECTOR scale, rotation, translation;
			DirectX::XMMatrixDecompose(&scale, &rotation, &translation, matrix);
			return rotation;
		}

		/// <summary>
		/// 拡大縮小成分を取得
		/// </summary>
		/// <param name="matrix">行列</param>
		/// <returns>拡大縮小成分</returns>
		[[nodiscard]] static DirectX::XMVECTOR GetScale(const DirectX::XMMATRIX& matrix)
		{
			DirectX::XMVECTOR scale, rotation, translation;
			DirectX::XMMatrixDecompose(&scale, &rotation, &translation, matrix);
			return scale;
		}

		/// <summary>
		/// 指定の位置(from)から指定の位置(to)を見るような行列を取得
		/// </summary>
		/// <param name="from">自身の位置</param>
		/// <param name="to">見る対象の位置</param>
		/// <param name="up">up軸</param>
		/// <returns></returns>
		[[nodiscard]] static DirectX::XMMATRIX LookAt(const DirectX::XMVECTOR& from, const DirectX::XMVECTOR& to, const DirectX::XMVECTOR& up)
		{
			const auto forward	= DirectX::XMVector3Normalize(to - from);
			const auto right	= DirectX::XMVector3Cross(DirectX::XMVector3Normalize(up), forward);
			const auto newUp	= DirectX::XMVector3Cross(forward, right);

			return CreateRMatrix(right, newUp, forward) * CreateTMatrix(from);
		}
		#pragma endregion
	};

	#pragma region from / to JSON
	inline void from_json(const nlohmann::json& j, Vector2& vector)
	{
		j.at("x").get_to(vector.x);
		j.at("y").get_to(vector.y);
	}

	inline void to_json(nlohmann::json& j, const Vector2& vector)
	{
		j =
		{
			{ "x", vector.x },
			{ "y", vector.y }
		};
	}

	inline void from_json(const nlohmann::json& j, Vector3& vector)
	{
		j.at("x").get_to(vector.x);
		j.at("y").get_to(vector.y);
		j.at("z").get_to(vector.z);
	}

	inline void to_json(nlohmann::json& j, const Vector3& vector)
	{
		j = 
		{
			{ "x", vector.x },
			{ "y", vector.y },
			{ "z", vector.z }
		};
	}

	inline void from_json(const nlohmann::json& j, Quaternion& quaternion)
	{
		j.at("x").get_to(quaternion.x);
		j.at("y").get_to(quaternion.y);
		j.at("z").get_to(quaternion.z);
		j.at("w").get_to(quaternion.w);
	}

	inline void to_json(nlohmann::json& j, const Quaternion& quaternion)
	{
		j =
		{
			{ "x", quaternion.x },
			{ "y", quaternion.y },
			{ "z", quaternion.z },
			{ "w", quaternion.w }
		};
	}

	inline void from_json(const nlohmann::json& j, Matrix4x4& matrix)
	{
		const auto& m = j.at("m");

		for (size_t i = 0; i < 4; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				matrix.m[i][j] = m[i][j];
			}
		}
	}

	inline void to_json(nlohmann::json& j, const Matrix4x4& matrix)
	{
		j =
		{{ "m",
			{{ matrix._11, matrix._12, matrix._13, matrix._14 },
			 { matrix._21, matrix._22, matrix._23, matrix._24 },
			 { matrix._31, matrix._32, matrix._33, matrix._34 },
			 { matrix._41, matrix._42, matrix._43, matrix._44 }}}};
	}
	#pragma endregion
}
