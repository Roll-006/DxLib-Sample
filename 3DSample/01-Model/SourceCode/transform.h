#pragma once

/// <summary>
/// オブジェクトの座標、回転、スケールを保持している。
/// </summary>
class Transform
{
public:
	Transform();

	#pragma region Getter
	Matrix	GetMatrix()	const { return XMMatrixAffineTransformation(scale, Quaternion(0, 0, 0, 0), rotation, position); }

	Vector3	GetRight()		{ return Vector3::TransformNormal(Vector3::Right,		Matrix::CreateFromQuaternion(rotation)); }
	Vector3	GetUp()			{ return Vector3::TransformNormal(Vector3::Up,			Matrix::CreateFromQuaternion(rotation)); }
	Vector3	GetForward()	{ return Vector3::TransformNormal(Vector3::Backward,	Matrix::CreateFromQuaternion(rotation)); }
	#pragma endregion

	#pragma region Setter
	void SetEulerAngles (const Vector3& eulerAnglesDeg);
	void SetMatrix		(Matrix& matrix);
	#pragma endregion

public:
	Vector3		position;
	Quaternion	rotation;
	Vector3		scale;

private:
	Matrix _matrix;
};
