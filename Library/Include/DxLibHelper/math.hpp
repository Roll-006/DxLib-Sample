#pragma once
#include <numbers>
#include <SimpleMath.h>
#include <DxLib.h>
#include <nlohmann/json.hpp>

using namespace DirectX::SimpleMath;

namespace math
{
	#pragma region constant
	static constexpr float kDeg2Rad = static_cast<float>(std::numbers::pi) / 180.0f;		// ディグリーをラジアンに変換 (変換対象と掛け算を行う)
	static constexpr float kRad2Deg = 180.0f / static_cast<float>(std::numbers::pi);		// ラジアンをディグリーに変換 (変換対象と掛け算を行う)
	static constexpr float kEpsilon = 1.0e-5f;
	#pragma endregion
}

namespace DirectX
{
	namespace SimpleMath
	{
		#pragma region Rectangle
		#pragma region from / to JSON
		inline void from_json(const nlohmann::json& j, Rectangle& rectangle)
		{
			j.at("x").		get_to(rectangle.x);
			j.at("y").		get_to(rectangle.y);
			j.at("width").	get_to(rectangle.width);
			j.at("height").	get_to(rectangle.height);
		}

		inline void to_json(nlohmann::json& j, const Rectangle& rectangle)
		{
			j =
			{
				{ "x",		rectangle.x },
				{ "y",		rectangle.y },
				{ "width",	rectangle.width },
				{ "height", rectangle.height },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Vector2
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
				{ "y", vector.y },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Vector3
		#pragma region conversion
		inline Vector3 ToVector3(const DxLib::VECTOR& vector) { return { vector.x, vector.y, vector.z }; }
		inline DxLib::VECTOR ToDxLibVector(const Vector3& vector) { return { vector.x, vector.y, vector.z }; }
		#pragma endregion

		#pragma region from / to JSON
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
				{ "z", vector.z },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Vector4
		#pragma region conversion
		inline Vector4 ToVector4(const DxLib::FLOAT4& float4) { return { float4.x, float4.y, float4.z, float4.w }; }
		inline DxLib::FLOAT4 ToDxLibFloat4(const Vector4& vector) { return { vector.x, vector.y, vector.z, vector.w }; }
		#pragma endregion

		#pragma region from / to JSON
		inline void from_json(const nlohmann::json& j, Vector4& vector)
		{
			j.at("x").get_to(vector.x);
			j.at("y").get_to(vector.y);
			j.at("z").get_to(vector.z);
			j.at("w").get_to(vector.w);
		}

		inline void to_json(nlohmann::json& j, const Vector4& vector)
		{
			j =
			{
				{ "x", vector.x },
				{ "y", vector.y },
				{ "z", vector.z },
				{ "w", vector.w },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Matrix
		#pragma region conversion
		inline Matrix ToMatrix(const DxLib::MATRIX& matrix)
		{
			Matrix result;
			memcpy(&result, &matrix, sizeof(Matrix));
			return result;
		}

		inline DxLib::MATRIX ToDxLibMatrix(const Matrix& matrix)
		{
			DxLib::MATRIX result;
			memcpy(&result, &matrix, sizeof(DxLib::MATRIX));
			return result;
		}
		#pragma endregion

		#pragma region from / to JSON
		inline void from_json(const nlohmann::json& j, Matrix& matrix)
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

		inline void to_json(nlohmann::json& j, const Matrix& matrix)
		{
			j =
			{{"m",
				{{ matrix._11, matrix._12, matrix._13, matrix._14 },
				 { matrix._21, matrix._22, matrix._23, matrix._24 },
				 { matrix._31, matrix._32, matrix._33, matrix._34 },
				 { matrix._41, matrix._42, matrix._43, matrix._44 }}}};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Plane
		#pragma region conversion
		inline Plane ToPlane(const DxLib::FLOAT4& float4) { return { float4.x, float4.y, float4.z, float4.w }; }
		inline DxLib::FLOAT4 ToDxLibFloat4(const Plane& plane) { return { plane.x, plane.y, plane.z, plane.w }; }
		#pragma endregion

		#pragma region from / to JSON
		inline void from_json(const nlohmann::json& j, Plane& plane)
		{
			j.at("x").get_to(plane.x);
			j.at("y").get_to(plane.y);
			j.at("z").get_to(plane.z);
			j.at("w").get_to(plane.w);
		}

		inline void to_json(nlohmann::json& j, const Plane& plane)
		{
			j =
			{
				{ "x", plane.x },
				{ "y", plane.y },
				{ "z", plane.z },
				{ "w", plane.w },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Quaternion
		#pragma region conversion
		inline Quaternion ToQuaternion(const DxLib::FLOAT4& float4) { return { float4.x, float4.y, float4.z, float4.w }; }
		inline DxLib::FLOAT4 ToDxLibFloat4(const Quaternion& quaternion) { return { quaternion.x, quaternion.y, quaternion.z, quaternion.w }; }
		#pragma endregion

		#pragma region from / to JSON
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
				{ "w", quaternion.w },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Color
		#pragma region conversion
		inline Color ToColor(const DxLib::COLOR_F& colorF) { return { colorF.r, colorF.g, colorF.b, colorF.a }; }
		inline DxLib::COLOR_F ToDxLibColorF(const Color& color) { return { color.x, color.y, color.z, color.w }; }
		#pragma endregion

		#pragma region from / to JSON
		inline void from_json(const nlohmann::json& j, Color& color)
		{
			j.at("x").get_to(color.x);
			j.at("y").get_to(color.y);
			j.at("z").get_to(color.z);
			j.at("w").get_to(color.w);
		}

		inline void to_json(nlohmann::json& j, const Color& color)
		{
			j =
			{
				{ "x", color.x },
				{ "y", color.y },
				{ "z", color.z },
				{ "w", color.w },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Ray
		#pragma region from / to JSON
		inline void from_json(const nlohmann::json& j, Ray& ray)
		{
			j.at("position").	get_to(ray.position);
			j.at("direction").	get_to(ray.direction);
		}

		inline void to_json(nlohmann::json& j, const Ray& ray)
		{
			j =
			{
				{ "position",	ray.position },
				{ "direction",	ray.direction },
			};
		}
		#pragma endregion
		#pragma endregion


		#pragma region Viewport
		#pragma region from / to JSON
		inline void from_json(const nlohmann::json& j, Viewport& viewport)
		{
			j.at("x").			get_to(viewport.x);
			j.at("y").			get_to(viewport.y);
			j.at("width").		get_to(viewport.width);
			j.at("height").		get_to(viewport.height);
			j.at("minDepth").	get_to(viewport.minDepth);
			j.at("maxDepth").	get_to(viewport.maxDepth);
		}

		inline void to_json(nlohmann::json& j, const Viewport& viewport)
		{
			j =
			{
				{ "x",			viewport.x },
				{ "y",			viewport.y },
				{ "width",		viewport.width },
				{ "height",		viewport.height },
				{ "minDepth",	viewport.minDepth },
				{ "maxDepth",	viewport.maxDepth },
			};
		}
		#pragma endregion
		#pragma endregion
	}
}

