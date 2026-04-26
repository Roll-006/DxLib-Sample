#pragma once
#include <array>
#include <Vector/vector3.hpp>

struct Matrix4x4;
Matrix4x4 operator*(const Matrix4x4& mat1, const Matrix4x4& mat2);

struct Matrix4x4
{
	std::array<std::array<float, 4>, 4> m;

	Matrix4x4(const std::array<std::array<float, 4>, 4> m) : 
		m(m)
	{ }

	Matrix4x4(
		const float m00, const float m01, const float m02, const float m03, 
		const float m10, const float m11, const float m12, const float m13, 
		const float m20, const float m21, const float m22, const float m23, 
		const float m30, const float m31, const float m32, const float m33) :
		m{{	{m00, m01, m02, m03},
			{m10, m11, m12, m13},
			{m20, m21, m22, m23},
			{m30, m31, m32, m33} }}
	{ }

	Matrix4x4(const MATRIX& mat) :
		m()
	{
		// バイト数を指定しオブジェクトをコピー
		std::memcpy(m.data(), mat.m, sizeof(float) * 16);
	}


	operator MATRIX() const
	{
		auto result = MATRIX();
		std::memcpy(result.m, m.data(), sizeof(float) * 16);
		return result;
	}


	Matrix4x4& operator*=(const Matrix4x4& mat)
	{
		const auto tmpMat = *this;

		m[0][0] = tmpMat.m[0][0] * mat.m[0][0] + tmpMat.m[0][1] * mat.m[1][0] + tmpMat.m[0][2] * mat.m[2][0] + tmpMat.m[0][3] * mat.m[3][0];
		m[0][1] = tmpMat.m[0][0] * mat.m[0][1] + tmpMat.m[0][1] * mat.m[1][1] + tmpMat.m[0][2] * mat.m[2][1] + tmpMat.m[0][3] * mat.m[3][1];
		m[0][2] = tmpMat.m[0][0] * mat.m[0][2] + tmpMat.m[0][1] * mat.m[1][2] + tmpMat.m[0][2] * mat.m[2][2] + tmpMat.m[0][3] * mat.m[3][2];
		m[0][3] = tmpMat.m[0][0] * mat.m[0][3] + tmpMat.m[0][1] * mat.m[1][3] + tmpMat.m[0][2] * mat.m[2][3] + tmpMat.m[0][3] * mat.m[3][3];
		
		m[1][0] = tmpMat.m[1][0] * mat.m[0][0] + tmpMat.m[1][1] * mat.m[1][0] + tmpMat.m[1][2] * mat.m[2][0] + tmpMat.m[1][3] * mat.m[3][0];
		m[1][1] = tmpMat.m[1][0] * mat.m[0][1] + tmpMat.m[1][1] * mat.m[1][1] + tmpMat.m[1][2] * mat.m[2][1] + tmpMat.m[1][3] * mat.m[3][1];
		m[1][2] = tmpMat.m[1][0] * mat.m[0][2] + tmpMat.m[1][1] * mat.m[1][2] + tmpMat.m[1][2] * mat.m[2][2] + tmpMat.m[1][3] * mat.m[3][2];
		m[1][3] = tmpMat.m[1][0] * mat.m[0][3] + tmpMat.m[1][1] * mat.m[1][3] + tmpMat.m[1][2] * mat.m[2][3] + tmpMat.m[1][3] * mat.m[3][3];
		
		m[2][0] = tmpMat.m[2][0] * mat.m[0][0] + tmpMat.m[2][1] * mat.m[1][0] + tmpMat.m[2][2] * mat.m[2][0] + tmpMat.m[2][3] * mat.m[3][0];
		m[2][1] = tmpMat.m[2][0] * mat.m[0][1] + tmpMat.m[2][1] * mat.m[1][1] + tmpMat.m[2][2] * mat.m[2][1] + tmpMat.m[2][3] * mat.m[3][1];
		m[2][2] = tmpMat.m[2][0] * mat.m[0][2] + tmpMat.m[2][1] * mat.m[1][2] + tmpMat.m[2][2] * mat.m[2][2] + tmpMat.m[2][3] * mat.m[3][2];
		m[2][3] = tmpMat.m[2][0] * mat.m[0][3] + tmpMat.m[2][1] * mat.m[1][3] + tmpMat.m[2][2] * mat.m[2][3] + tmpMat.m[2][3] * mat.m[3][3];
		
		m[3][0] = tmpMat.m[3][0] * mat.m[0][0] + tmpMat.m[3][1] * mat.m[1][0] + tmpMat.m[3][2] * mat.m[2][0] + tmpMat.m[3][3] * mat.m[3][0];
		m[3][1] = tmpMat.m[3][0] * mat.m[0][1] + tmpMat.m[3][1] * mat.m[1][1] + tmpMat.m[3][2] * mat.m[2][1] + tmpMat.m[3][3] * mat.m[3][1];
		m[3][2] = tmpMat.m[3][0] * mat.m[0][2] + tmpMat.m[3][1] * mat.m[1][2] + tmpMat.m[3][2] * mat.m[2][2] + tmpMat.m[3][3] * mat.m[3][2];
		m[3][3] = tmpMat.m[3][0] * mat.m[0][3] + tmpMat.m[3][1] * mat.m[1][3] + tmpMat.m[3][2] * mat.m[2][3] + tmpMat.m[3][3] * mat.m[3][3];

		return *this;
	}


	/// @brief ゼロ行列を取得
	[[nodiscard]] static Matrix4x4 GetZero()
	{
		return{	0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f };
	}

	/// @brief 単位行列を取得
	[[nodiscard]] static Matrix4x4 GetIdentity()
	{
		return{	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };
	}

	/// @brief 平行移動行列を生成
	[[nodiscard]] static Matrix4x4 CreateTranslation(const Vector3& translation)
	{
		return{ 1.0f,			0.0f,			0.0f,			0.0f,
				0.0f,			1.0f,			0.0f,			0.0f,
				0.0f,			0.0f,			1.0f,			0.0f,
				translation.x,	translation.y,	translation.z,	1.0f };
	}

	/// @brief X軸(ピッチ軸回転)に対する回転行列を生成 (ラジアン角指定)
	[[nodiscard]] static Matrix4x4 CreateXRotation(const float rotation)
	{
		const auto sin = std::sin(rotation);
		const auto cos = std::cos(rotation);

		return{ 1.0f,	0.0f,	0.0f,	0.0f,
				0.0f,	cos,	sin,	0.0f,
				0.0f,	-sin,	cos,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f };
	}

	/// @brief Y軸(ヨー軸)に対する回転行列を生成 (ラジアン角指定)
	[[nodiscard]] static Matrix4x4 CreateYRotation(const float rotation)
	{
		const auto sin = std::sin(rotation);
		const auto cos = std::cos(rotation);

		return{ cos,	0.0f,	-sin,	0.0f,
				0.0f,	1.0f,	0.0f,	0.0f,
				sin,	0.0f,	cos,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f };
	}

	/// @brief Z軸(ロール軸)に対する回転行列を生成 (ラジアン角指定)
	[[nodiscard]] static Matrix4x4 CreateZRotation(const float rotation)
	{
		const auto sin = std::sin(rotation);
		const auto cos = std::cos(rotation);

		return{ cos,	sin,	0.0f,	0.0f,
				-sin,	cos,	0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f };
	}

	/// @brief 回転行列(X→Y→Z)を生成 (ラジアン角指定)
	[[nodiscard]] static Matrix4x4 CreateXYZRotation(const Vector3& rotation)
	{
		return CreateXRotation(rotation.x) * CreateYRotation(rotation.y) * CreateZRotation(rotation.z);
	}

	/// @brief 回転行列(Z→X→Y)を生成 (ラジアン角指定)
	[[nodiscard]] static Matrix4x4 CreateZXYRotation(const Vector3& rotation)
	{
		return CreateZRotation(rotation.z) * CreateXRotation(rotation.x) * CreateYRotation(rotation.y);
	}
	
	/// @brief right, up, forwardから回転行列を生成
	[[nodiscard]] static Matrix4x4 CreateRotation(const Vector3& right, const Vector3& up, const Vector3& forward)
	{
		return{ right.x,	right.y,	right.z,	0.0f,
				up.x,		up.y,		up.z,		0.0f,
				forward.x,	forward.y,	forward.z,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f };
	}

	/// @brief スケール行列を生成
	[[nodiscard]] static Matrix4x4 CreateScale(const Vector3& scale)
	{
		return{ scale.x,	0.0f,		0.0f,		0.0f,
				0.0f,		scale.y,	0.0f,		0.0f,
				0.0f,		0.0f,		scale.z,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f };
	}

	/// @brief TRS(平行移動, 回転, スケール)行列を生成
	[[nodiscard]] static Matrix4x4 CreateTRS(const Vector3& t, const Vector3& r, const Vector3& s)
	{
		return CreateScale(s) * CreateZXYRotation(r) * CreateTranslation(t);
	}

	// @brief 指定の位置から指定の位置を見るような行列を取得
	[[nodiscard]] static Matrix4x4 LookAt(const Vector3& from, const Vector3& to, const Vector3& up)
	{
		const auto forward	= (to - from).GetNormalized();
		const auto right	= Vector3::GetCross(up, forward);
		const auto newUp	= Vector3::GetCross(forward, right);

		return CreateRotation(right, newUp, forward) * CreateTranslation(from);
	}

	/// @brief 平行移動成分を取得
	[[nodiscard]] Vector3 GetTranslation() const
	{
		return { m[3][0], m[3][1], m[3][2] };
	}

	/// @brief スケール成分を取得
	[[nodiscard]] Vector3 GetScale() const
	{
		const auto scale_x = Vector3(m[0][0], m[0][1], m[0][2]);
		const auto scale_y = Vector3(m[1][0], m[1][1], m[1][2]);
		const auto scale_z = Vector3(m[2][0], m[2][1], m[2][2]);

		return { scale_x.GetMagnitude(), scale_y.GetMagnitude(), scale_z.GetMagnitude() };
	}

	/// @brief 平行移動成分を取得
	[[nodiscard]] Matrix4x4 GetTMatrix() const
	{
		return{ 1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				m[3][0],	m[3][1],	m[3][2],	1.0f };
	}

	/// @brief 回転及びスケール成分を取得
	[[nodiscard]] Matrix4x4 GetRSMatrix() const
	{
		return{ m[0][0],	m[0][1],	m[0][2],	0.0f,
				m[1][0],	m[1][1],	m[1][2],	0.0f,
				m[2][0],	m[2][1],	m[2][2],	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f };
	}

	/// @brief 回転成分を取得
	[[nodiscard]] Matrix4x4 GetRMatrix() const
	{
		const auto scale = GetScale();
		const auto rs_m	= GetRSMatrix();

		auto rot_m = rs_m;
		rot_m.m[0][0] /= scale.x; rot_m.m[0][1] /= scale.x; rot_m.m[0][2] /= scale.x;
		rot_m.m[1][0] /= scale.y; rot_m.m[1][1] /= scale.y; rot_m.m[1][2] /= scale.y;
		rot_m.m[2][0] /= scale.z; rot_m.m[2][1] /= scale.z; rot_m.m[2][2] /= scale.z;
		return rot_m;
	}

	/// @brief スケール成分を取得
	//[[nodiscard]] Matrix4x4 GetSMatrix() const;

	/// @brief rightを取得
	[[nodiscard]] Vector3 GetRight() const
	{
		return GetRMatrix().MultiplyVector(Vector3::GetRight());
	}

	/// @brief upを取得
	[[nodiscard]] Vector3 GetUp() const
	{
		return GetRMatrix().MultiplyVector(Vector3::GetUp());
	}

	/// @brief forwardを取得
	[[nodiscard]] Vector3 GetForward() const
	{
		return GetRMatrix().MultiplyVector(Vector3::GetForward());
	}

	/// @brief 逆行列を取得
	//[[nodiscard]] Matrix4x4 Inverse() const
	//{

	//}

	/// @brief 転置行列を取得
	[[nodiscard]] Matrix4x4 GetTranspose() const
	{
		return{ m[0][0],	m[1][0],	m[2][0],	m[3][0],
				m[0][1],	m[1][1],	m[2][1],	m[3][1],
				m[0][2],	m[1][2],	m[2][2],	m[3][2],
				m[0][3],	m[1][3],	m[2][3],	m[3][3] };
	}

	/// @brief TRS(平行移動, 回転, スケール)を行列で変換する
	[[nodiscard]] Vector3 MultiplyPoint(const Vector3& v) const
	{
		return{	v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2] };
	}

	/// @brief RS(回転, スケール)を行列で変換する
	[[nodiscard]] Vector3 MultiplyVector(const Vector3& v) const
	{
		return{	v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] };
	}

	/// @brief 平行移動成分を設定
	void SetTranslation(const Vector3& translation)
	{
		m[3][0] = translation.x;
		m[3][1] = translation.y;
		m[3][2] = translation.z;
	}

	/// @brief 行列に回転成分を設定
	void SetRotation(const Matrix4x4& rotMat)
	{
		const auto scale = GetScale();

		// 回転を代入しスケールを復元
		m[0][0] = rotMat.m[0][0] * scale.x;
		m[1][0] = rotMat.m[1][0] * scale.y;
		m[2][0] = rotMat.m[2][0] * scale.z;

		m[0][1] = rotMat.m[0][1] * scale.x;
		m[1][1] = rotMat.m[1][1] * scale.y;
		m[2][1] = rotMat.m[2][1] * scale.z;

		m[0][2] = rotMat.m[0][2] * scale.x;
		m[1][2] = rotMat.m[1][2] * scale.y;
		m[2][2] = rotMat.m[2][2] * scale.z;
	}

	/// @brief 行列にスケール成分を設定
	void SetScale(const Vector3& scale)
	{
		// 軸を正規化してスケールを掛ける
		const auto current_scale = GetScale();

		if (current_scale.x != 0.0f)
		{
			m[0][0] = (m[0][0] / current_scale.x) * scale.x;
			m[0][1] = (m[0][1] / current_scale.x) * scale.x;
			m[0][2] = (m[0][2] / current_scale.x) * scale.x;
		}

		if (current_scale.y != 0.0f)
		{
			m[1][0] = (m[1][0] / current_scale.y) * scale.y;
			m[1][1] = (m[1][1] / current_scale.y) * scale.y;
			m[1][2] = (m[1][2] / current_scale.y) * scale.y;
		}

		if (current_scale.z != 0.0f)
		{
			m[2][0] = (m[2][0] / current_scale.z) * scale.z;
			m[2][1] = (m[2][1] / current_scale.z) * scale.z;
			m[2][2] = (m[2][2] / current_scale.z) * scale.z;
		}
	}
};


inline Matrix4x4 operator*(const Matrix4x4& mat1, const Matrix4x4& mat2)
{
	return{
		mat1.m[0][0] * mat2.m[0][0] + mat1.m[0][1] * mat2.m[1][0] + mat1.m[0][2] * mat2.m[2][0] + mat1.m[0][3] * mat2.m[3][0],
		mat1.m[0][0] * mat2.m[0][1] + mat1.m[0][1] * mat2.m[1][1] + mat1.m[0][2] * mat2.m[2][1] + mat1.m[0][3] * mat2.m[3][1],
		mat1.m[0][0] * mat2.m[0][2] + mat1.m[0][1] * mat2.m[1][2] + mat1.m[0][2] * mat2.m[2][2] + mat1.m[0][3] * mat2.m[3][2],
		mat1.m[0][0] * mat2.m[0][3] + mat1.m[0][1] * mat2.m[1][3] + mat1.m[0][2] * mat2.m[2][3] + mat1.m[0][3] * mat2.m[3][3],

		mat1.m[1][0] * mat2.m[0][0] + mat1.m[1][1] * mat2.m[1][0] + mat1.m[1][2] * mat2.m[2][0] + mat1.m[1][3] * mat2.m[3][0],
		mat1.m[1][0] * mat2.m[0][1] + mat1.m[1][1] * mat2.m[1][1] + mat1.m[1][2] * mat2.m[2][1] + mat1.m[1][3] * mat2.m[3][1],
		mat1.m[1][0] * mat2.m[0][2] + mat1.m[1][1] * mat2.m[1][2] + mat1.m[1][2] * mat2.m[2][2] + mat1.m[1][3] * mat2.m[3][2],
		mat1.m[1][0] * mat2.m[0][3] + mat1.m[1][1] * mat2.m[1][3] + mat1.m[1][2] * mat2.m[2][3] + mat1.m[1][3] * mat2.m[3][3],

		mat1.m[2][0] * mat2.m[0][0] + mat1.m[2][1] * mat2.m[1][0] + mat1.m[2][2] * mat2.m[2][0] + mat1.m[2][3] * mat2.m[3][0],
		mat1.m[2][0] * mat2.m[0][1] + mat1.m[2][1] * mat2.m[1][1] + mat1.m[2][2] * mat2.m[2][1] + mat1.m[2][3] * mat2.m[3][1],
		mat1.m[2][0] * mat2.m[0][2] + mat1.m[2][1] * mat2.m[1][2] + mat1.m[2][2] * mat2.m[2][2] + mat1.m[2][3] * mat2.m[3][2],
		mat1.m[2][0] * mat2.m[0][3] + mat1.m[2][1] * mat2.m[1][3] + mat1.m[2][2] * mat2.m[2][3] + mat1.m[2][3] * mat2.m[3][3],

		mat1.m[3][0] * mat2.m[0][0] + mat1.m[3][1] * mat2.m[1][0] + mat1.m[3][2] * mat2.m[2][0] + mat1.m[3][3] * mat2.m[3][0],
		mat1.m[3][0] * mat2.m[0][1] + mat1.m[3][1] * mat2.m[1][1] + mat1.m[3][2] * mat2.m[2][1] + mat1.m[3][3] * mat2.m[3][1],
		mat1.m[3][0] * mat2.m[0][2] + mat1.m[3][1] * mat2.m[1][2] + mat1.m[3][2] * mat2.m[2][2] + mat1.m[3][3] * mat2.m[3][2],
		mat1.m[3][0] * mat2.m[0][3] + mat1.m[3][1] * mat2.m[1][3] + mat1.m[3][2] * mat2.m[2][3] + mat1.m[3][3] * mat2.m[3][3] };
}

inline bool operator==(const Matrix4x4& mat1, const Matrix4x4& mat2) { return mat1.m == mat2.m; }
inline bool operator!=(const Matrix4x4& mat1, const Matrix4x4& mat2) { return !(mat1 == mat2); }


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Matrix4x4& mat)
{
	mat.m = j_data.get<std::array<std::array<float, 4>, 4>>();
}

inline void to_json(nlohmann::json& j_data, const Matrix4x4& mat)
{
	j_data = 
	{
		{"m", mat.m}
	};
}
#pragma endregion
