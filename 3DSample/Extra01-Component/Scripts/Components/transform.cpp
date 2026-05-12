#include <string>
#include <json_loader.hpp>
#include <math.hpp>
#include "../Core/game_object.h"
#include "../Core/component_factory.h"
#include "../Core/scene.h"
#include "transform.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("Transform", [](GameObject& obj) { return obj.GetTransform(); });
		return true;
	}();
}

Transform::Transform() :
	_localPosition	(Vector3::Zero),
	_localRotation	(Quaternion::Identity),
	_localScale		(Vector3::One),
	_worldMatrix	(Matrix::Identity),
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

std::shared_ptr<Transform> Transform::Find(const std::string& name) const
{
	for (const auto& child : _children)
	{
		if (const auto gameObject = child->_gameObject.lock(); gameObject->GetName() == name)
		{
			return gameObject->GetTransform();
		}
	}

	return nullptr;
}

void Transform::DetachChildren()
{
	for (const auto& child : _children)
	{
		child->SetParent(nullptr);
	}
}

void Transform::LookAt(const Vector3& target, const Vector3& up)
{
	// ワールド行列を計算
	auto lookAtMat = Matrix::CreateWorld(GetWorldPosition(), target - GetWorldPosition(), up);

	Vector3 scale, translation;
	Quaternion rotation;
	lookAtMat.Decompose(scale, rotation, translation);

	const auto worldMat = Matrix(XMMatrixAffineTransformation(GetWorldScale(), Quaternion(0, 0, 0, 0), rotation, translation));

	// ローカル行列を計算
	auto localMat = _parent.expired() ? worldMat : worldMat * _parent.lock()->GetWorldMatrix().Invert();

	SetLocalMatrix(localMat);
}

#pragma region Getter
Matrix Transform::GetLocalMatrix() const
{
	return XMMatrixAffineTransformation(_localScale, Quaternion(0, 0, 0, 0), _localRotation, _localPosition);
}

Vector3 Transform::GetWorldPosition()
{
	UpdateMatrix();

	return _worldMatrix.Translation();
}

Quaternion Transform::GetWorldRotation()
{
	UpdateMatrix();

	Vector3 scale, translation;
	Quaternion rotation;
	_worldMatrix.Decompose(scale, rotation, translation);
	return rotation;
}

Vector3 Transform::GetWorldScale()
{
	UpdateMatrix();

	Vector3 scale, translation;
	Quaternion rotation;
	_worldMatrix.Decompose(scale, rotation, translation);
	return scale;
}

Matrix Transform::GetWorldMatrix()
{
	UpdateMatrix();

	return _worldMatrix;
}

Vector3 Transform::GetRight()
{
	UpdateMatrix();

	return Vector3::TransformNormal(Vector3::Right, Matrix::CreateFromQuaternion(GetWorldRotation()));
}

Vector3 Transform::GetUp()
{
	UpdateMatrix();

	return Vector3::TransformNormal(Vector3::Up, Matrix::CreateFromQuaternion(GetWorldRotation()));
}

Vector3 Transform::GetForward()
{
	UpdateMatrix();

	// MEMO : SimpleMathのForwardの定義は{ 0.f, 0.f, -1.f }であることに注意
	return Vector3::TransformNormal(Vector3::Backward, Matrix::CreateFromQuaternion(GetWorldRotation()));
}

std::shared_ptr<Transform> Transform::GetParent() const
{
	return _parent.lock();
}

std::shared_ptr<Transform> Transform::GetChild(const int index) const
{
	return index < _children.size() ? _children.at(index) : nullptr;
}

std::shared_ptr<Transform> Transform::GetRoot()
{
	auto current = std::static_pointer_cast<Transform>(shared_from_this());

	while (true)
	{
		const auto parent = current->GetParent();
		if (!parent) { break; }
		current = parent;
	}

	return current;
}

int Transform::GetSiblingIndex()
{
	const auto transform = std::static_pointer_cast<Transform>(shared_from_this());
	const auto parent = GetParent();
	auto index = 0;

	// 親がいる場合は親から、いない場合はSceneから兄弟を取得
	const auto sibling = parent ? parent->GetChildren() : _gameObject.lock()->GetScene()->GetRoots();

	for (size_t i = 0; i < sibling.size(); ++i)
	{
		if (sibling.at(i) == transform)
		{
			index = static_cast<int>(i);
			break;
		}
	}

	return index;
}
#pragma endregion

#pragma region Setter
void Transform::SetLocalPosition(const Vector3& position)
{
	_localPosition = position;

	OnDirty();
}

void Transform::SetLocalRotation(const Quaternion& rotation)
{
	_localRotation = rotation;
	
	OnDirty();
}

void Transform::SetLocalEulerAngles(const Vector3& eulerAnglesDeg)
{
	_localRotation = Quaternion::CreateFromYawPitchRoll(eulerAnglesDeg * math::kDeg2Rad);

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

void Transform::SetLocalMatrix(Matrix& matrix)
{
	Vector3 scale, translation;
	Quaternion rotation;
	matrix.Decompose(scale, rotation, translation);
	_localScale		= scale;
	_localRotation	= rotation;
	_localPosition	= translation;

	OnDirty();
}

void Transform::SetParent(const std::shared_ptr<Transform>& parent)
{
	if (parent)
	{
		_parent = parent;
		_parent.lock()->AddChild(std::static_pointer_cast<Transform>(shared_from_this()));
	}
	else
	{
		_parent.lock()->RemoveChild(GetSiblingIndex());
		_parent = parent;
	}

}
#pragma endregion

void Transform::AddChild(const std::shared_ptr<Transform>& child)
{
	_children.emplace_back(child);
}

void Transform::RemoveChild(const int index)
{
	_children.erase(_children.begin() + index);
}

void Transform::OnDirty()
{
	_isDirty = true;

	for (const auto& child : _children)
	{
		child->OnDirty();
	}
}

void Transform::UpdateMatrix()
{
	if (!_isDirty) { return; }
	
	const auto localMat = GetLocalMatrix();

	if (const auto& parent = _parent.lock())
	{
		_worldMatrix = localMat * parent->GetWorldMatrix();
	}
	else
	{
		// 親がいない場合はローカルをそのままワールドにする
		_worldMatrix = localMat;
	}

	_isDirty = false;

	for (const auto& child : _children)
	{
		child->UpdateMatrix();
	}
}
