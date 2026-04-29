#include <string>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "math.h"
#include "game_object.h"
#include "component_factory.h"
#include "transform.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("Transform", [](GameObject& obj) { return obj.AddComponent<Transform>(); });
		return true;
	}();
}

Transform::Transform() :
	_position	(0.0f, 0.0f, 0.0f),
	_rotation	(0.0f, 0.0f, 0.0f),
	_scale		(1.0f, 1.0f, 1.0f),
	_matrix		(Matrix4x4::GetIdentity()),
	_isDirty	(false)
{
	
}

void Transform::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
}

void Transform::Update()
{
	if (!_enabled) { return; }
}

void Transform::LateUpdate()
{
	if (!_enabled) { return; }
}

void Transform::Render() const
{
	if (!_enabled) { return; }
}

void Transform::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);

	_isDirty = true;
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
