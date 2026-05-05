#include <string>
#include <JSON/json_loader.hpp>
#include <Math/math.hpp>
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
	_localPosition	(0.0f, 0.0f, 0.0f),
	_localRotation	(math::Quaternion::StoreFromSIMD(XMQuaternionIdentity())),
	_localScale		(1.0f, 1.0f, 1.0f),
	_worldMatrix	(math::Matrix4x4::StoreFromSIMD(XMMatrixIdentity())),
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

void Transform::LookAt(const math::Vector3& target, const math::Vector3& up)
{
	const auto pos			= GetWorldPosition().LoadToSIMD();
	const auto targetVector = target.LoadToSIMD();
	const auto upVector		= up.LoadToSIMD();

	const auto forward		= XMVector3Normalize(targetVector - pos);
	const auto right		= XMVector3Normalize(XMVector3Cross(upVector, forward));
	const auto newUp		= XMVector3Cross(forward, right);

	const auto scale		= GetWorldScale();

	// ワールド行列を計算
	auto worldMat = XMMatrixIdentity();
	worldMat.r[0] = right	* scale.x;
	worldMat.r[1] = newUp	* scale.y;
	worldMat.r[2] = forward	* scale.z;
	worldMat.r[3] = XMVectorSetW(pos, 1.0f);

	// ローカル行列を計算
	const auto localMat = _parent.expired() ? worldMat : worldMat * XMMatrixInverse(nullptr, _parent.lock()->GetWorldMatrix().LoadToSIMD());

	SetLocalMatrix(math::Matrix4x4::StoreFromSIMD(localMat));
}

#pragma region Getter
math::Matrix4x4 Transform::GetLocalMatrix() const
{
	return math::Matrix4x4::StoreFromSIMD(XMMatrixAffineTransformation(_localScale.LoadToSIMD(), XMVectorZero(), _localRotation.LoadToSIMD(), _localPosition.LoadToSIMD()));
}

math::Vector3 Transform::GetWorldPosition()
{
	UpdateMatrix();

	return { _worldMatrix._41, _worldMatrix._42, _worldMatrix._43 };
}

math::Quaternion Transform::GetWorldRotation()
{
	UpdateMatrix();

	return math::Quaternion::StoreFromSIMD(XMQuaternionRotationMatrix(_worldMatrix.LoadToSIMD()));
}

math::Vector3 Transform::GetWorldScale()
{
	UpdateMatrix();

	const auto scaleX = math::Vector3(_worldMatrix._11, _worldMatrix._12, _worldMatrix._13);
	const auto scaleY = math::Vector3(_worldMatrix._21, _worldMatrix._22, _worldMatrix._23);
	const auto scaleZ = math::Vector3(_worldMatrix._31, _worldMatrix._32, _worldMatrix._33);

	return{ XMVectorGetX(XMVector3Length(scaleX.LoadToSIMD())), 
			XMVectorGetX(XMVector3Length(scaleY.LoadToSIMD())), 
			XMVectorGetX(XMVector3Length(scaleZ.LoadToSIMD())) };
}

math::Matrix4x4 Transform::GetWorldMatrix()
{
	UpdateMatrix();

	return _worldMatrix;
}

math::Vector3 Transform::GetRight()
{
	UpdateMatrix();

	const auto quaternion	= XMQuaternionRotationMatrix(_worldMatrix.LoadToSIMD());
	const auto rotationMat	= XMMatrixRotationQuaternion(quaternion);
	
	return math::Vector3::StoreFromSIMD(XMVector3TransformNormal(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotationMat));
}

math::Vector3 Transform::GetUp()
{
	UpdateMatrix();

	const auto quaternion	= XMQuaternionRotationMatrix(_worldMatrix.LoadToSIMD());
	const auto rotationMat	= XMMatrixRotationQuaternion(quaternion);

	return math::Vector3::StoreFromSIMD(XMVector3TransformNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationMat));
}

math::Vector3 Transform::GetForward()
{
	UpdateMatrix();

	const auto quaternion	= XMQuaternionRotationMatrix(_worldMatrix.LoadToSIMD());
	const auto rotationMat	= XMMatrixRotationQuaternion(quaternion);

	return math::Vector3::StoreFromSIMD(XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMat));
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
void Transform::SetLocalPosition(const math::Vector3& position)
{
	_localPosition = position;

	OnDirty();
}

void Transform::SetLocalRotation(const math::Quaternion& rotation)
{
	_localRotation = rotation;
	
	OnDirty();
}

void Transform::SetLocalEulerAngles(const math::Vector3& eulerAnglesDeg)
{
	_localRotation = math::Quaternion::StoreFromSIMD(XMQuaternionRotationRollPitchYaw(eulerAnglesDeg.x * math::kDeg2Rad, eulerAnglesDeg.y * math::kDeg2Rad, eulerAnglesDeg.z * math::kDeg2Rad));

	OnDirty();
}

void Transform::SetLocalScale(const math::Vector3& scale)
{
	_localScale = scale;
	
	OnDirty();
}

void Transform::SetLocalScale(const float scale)
{
	_localScale = { scale, scale, scale };

	OnDirty();
}

void Transform::SetLocalMatrix(const math::Matrix4x4& matrix)
{
	XMVECTOR scale, rotation, translation;
	XMMatrixDecompose(&scale, &rotation, &translation, matrix.LoadToSIMD());
	_localScale		= math::Vector3::StoreFromSIMD(scale);
	_localRotation	= math::Quaternion::StoreFromSIMD(rotation);
	_localPosition	= math::Vector3::StoreFromSIMD(translation);

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
		_worldMatrix = math::Matrix4x4::StoreFromSIMD(localMat.LoadToSIMD() * parent->GetWorldMatrix().LoadToSIMD());
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
