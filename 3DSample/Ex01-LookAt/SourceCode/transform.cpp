#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "transform.h"

Transform::Transform() : 
	_position	(0.0f, 0.0f, 0.0f),
	_rotation	(0.0f, 0.0f, 0.0f),
	_scale		(1.0f, 1.0f, 1.0f),
	_matrix		(Matrix4x4::GetIdentity()),
	_isDirty	(true)
{
	
}

void Transform::LookAt(const Vector3& target, const Vector3& up)
{
	_matrix = Matrix4x4::LookAt(_position, target, up);
}

Matrix4x4 Transform::GetMatrix()
{
	UpdateMatrix();

	return _matrix;
}

Vector3 Transform::GetRight()
{
	UpdateMatrix();

	return _matrix.GetRight();
}

Vector3 Transform::GetUp()
{
	UpdateMatrix();

	return _matrix.GetUp();
}

Vector3 Transform::GetForward()
{
	UpdateMatrix();

	return _matrix.GetForward();
}

void Transform::UpdateMatrix()
{
	if (_isDirty)
	{
		_matrix = Matrix4x4::CreateTRS(_position, _rotation, _scale);
		_isDirty = false;
	}
}
