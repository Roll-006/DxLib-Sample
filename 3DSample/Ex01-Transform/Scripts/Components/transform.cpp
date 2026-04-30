#include <string>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../Core/math.h"
#include "../Core/game_object.h"
#include "../Core/component_factory.h"
#include "transform.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("Transform", [](GameObject& obj) { return obj.GetComponent<Transform>(); });
		return true;
	}();
}

Transform::Transform() :
	_localPosition	(0.0f, 0.0f, 0.0f),
	_localRotation	(0.0f, 0.0f, 0.0f),
	_localScale		(1.0f, 1.0f, 1.0f),
	_worldMatrix	(Matrix4x4::GetIdentity()),
	_isDirty		(false),
	_parent			(),
	_children		()
{
	
}

void Transform::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;

	OnDirty();
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
}

void Transform::LookAt(const Vector3& target, const Vector3& up)
{
	_worldMatrix = Matrix4x4::LookAt(_localPosition, target, up);
}

#pragma region Getter
Vector3 Transform::GetWorldPosition()
{
	UpdateMatrix();

	return _worldMatrix.GetTranslation();
}

//Vector3 Transform::GetWorldRotation()
//{
//	UpdateMatrix();
//}

Vector3 Transform::GetWorldScale()
{
	UpdateMatrix();

	return _worldMatrix.GetScale();
}

Matrix4x4 Transform::GetWorldMatrix()
{
	UpdateMatrix();

	return _worldMatrix;
}

Vector3 Transform::GetRight()
{
	UpdateMatrix();

	return _worldMatrix.GetRight();
}

Vector3 Transform::GetUp()
{
	UpdateMatrix();

	return _worldMatrix.GetUp();
}

Vector3 Transform::GetForward()
{
	UpdateMatrix();

	return _worldMatrix.GetForward();
}

std::shared_ptr<Transform> Transform::GetParent() const
{
	return _parent.lock();
}

std::shared_ptr<Transform> Transform::GetChild(const int index) const
{
	return index < _children.size() ? _children.at(index).lock() : nullptr;
}
#pragma endregion

#pragma region Setter
void Transform::SetLocalPosition(const Vector3& position)
{
	_localPosition = position;

	OnDirty();
}

void Transform::SetLocalRotation(const Vector3& rotation)
{
	_localRotation = rotation;
	
	OnDirty();
}

void Transform::SetLocalScale(const Vector3& scale)
{
	_localScale = scale;
	
	OnDirty();
}

void Transform::SetLocalScale(const float scale)
{
	_localScale = { scale, scale, scale };

	OnDirty();
}

void Transform::SetParent(const std::shared_ptr<Transform>& parent)
{
	_parent = parent;
	parent->AddChild(std::dynamic_pointer_cast<Transform>(shared_from_this()));
}
#pragma endregion

void Transform::AddChild(const std::shared_ptr<Transform>& child)
{
	_children.emplace_back(child);
}

void Transform::OnDirty()
{
	_isDirty = true;

	for (const auto& child : _children)
	{
		child.lock()->OnDirty();
	}
}

void Transform::UpdateMatrix()
{
	if (!_isDirty) { return; }
	
	const auto localMat = GetLocalMatrix();

	if (const auto& parent = _parent.lock())
	{
		_worldMatrix = parent->GetWorldMatrix() * localMat;
	}
	else
	{
		_worldMatrix = localMat;
	}

	_isDirty = false;

	for (const auto& child : _children)
	{
		child.lock()->UpdateMatrix();
	}
}
