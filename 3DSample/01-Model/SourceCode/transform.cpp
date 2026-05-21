#include <math.hpp>
#include "transform.h"

Transform::Transform() :
	position	(Vector3::Zero),
	rotation	(Quaternion::Identity),
	scale		(Vector3::One),
	_matrix		(Matrix::Identity)
{
	
}

#pragma region Setter
void Transform::SetEulerAngles(const Vector3& eulerAnglesDeg)
{
	rotation = Quaternion::CreateFromYawPitchRoll(eulerAnglesDeg * math::kDeg2Rad);
}

void Transform::SetMatrix(Matrix& matrix)
{
	_matrix = matrix;

	Vector3 s, t;
	Quaternion r;
	_matrix.Decompose(s, r, t);
	scale		= s;
	rotation	= r;
	position	= t;
}
#pragma endregion
