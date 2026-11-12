#include <math.hpp>
#include "../../Core/transform.h"
#include "../../Core/component_factory.h"
#include "../../Model/model_renderer.h"
#include "multi_aim_constraint.h"
#include "../animator.h"

using namespace Core;
using namespace Animations::Rigging;

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("MultiAimConstraint", [](GameObject& obj) { return obj.AddComponent<MultiAimConstraint>(); });
		return true;
	}();
}

MultiAimConstraint::MultiAimConstraint() : 
	_modelHandle			(-1),
	_constrainedBoneIndex	(-1),
	_parentBoneIndex		(-1),
	_aimAxis				(Vector3::Backward),
	_upAxis					(Vector3::Up),
 	_constrainedXAxis		(true),
 	_constrainedYAxis		(true),
 	_constrainedZAxis		(true),
	_limit					(kMaxLimit)
{
	
}

void MultiAimConstraint::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject	= gameObject;
}

void MultiAimConstraint::Start()
{
	_sourceObject = _gameObject.lock()->Find(_sourceObjectName);
	
	auto animator = GetComponent<Animator>();
	for (auto parent = GetTransform()->GetParent(); !animator && parent; parent = parent->GetParent())
	{
		animator = parent->GetComponent<Animator>();
	}
	
	if (animator) _modelHandle = animator->GetModelData().modelHandle;
	
	_constrainedBoneIndex	= MV1SearchFrame(_modelHandle, _constrainedBoneName.c_str());
	_parentBoneIndex		= MV1GetFrameParent(_modelHandle, _constrainedBoneIndex);
	_limit					= std::clamp(_limit, 0.f, kMaxLimit);
	
	CreateOffsetRotationMatrix();
	
	const auto localMat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
 
	// オフセットを除いた、初期回転
	_initialLocalRotation = Quaternion::CreateFromRotationMatrix(CreateRotationMatrix(_offsetRotationMatrix.Transpose() * localMat));
	
	// 初期aim軸及びup軸
	_initialAimAxis = Vector3::TransformNormal(_aimAxis, localMat);
	_initialUpAxis = Vector3::TransformNormal(_upAxis, localMat);
	
	// スイング軸が定まらなくなった場合の補助軸
	_lastSwingAxis = _initialAimAxis.Cross(_initialUpAxis);
	if (_lastSwingAxis.LengthSquared() <= Math::kEpsilon) { _lastSwingAxis = Vector3::Up; }
}

void MultiAimConstraint::Update()
{
	if (!_enabled) return;

	AimAtSourceObject();
}

void MultiAimConstraint::LateUpdate()
{
	if (!_enabled) return;
}

void MultiAimConstraint::Render() const
{
	if (!_enabled) return;
}

void MultiAimConstraint::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void MultiAimConstraint::CreateOffsetRotationMatrix()
{
	// 指定されたaim軸及びup軸で姿勢が制御されるように行列を生成
	_offsetRotationMatrix.Right(_aimAxis.Cross(_upAxis));
	_offsetRotationMatrix.Up(_upAxis);
	_offsetRotationMatrix.Forward(_aimAxis);
	_offsetRotationMatrix = _offsetRotationMatrix.Transpose();
}

void MultiAimConstraint::AimAtSourceObject()
{
	// ボーンをリセットし、weight が0の場合は処理を行わない
	MV1ResetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex);
	if (GetEffectiveWeight() <= Math::kEpsilon) return;
	
	const auto originWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	const auto parentWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _parentBoneIndex));

	auto originToSourceObjectWorld = _sourceObject.lock()->GetWorldPosition() - originWorldMat.Translation();
	if (originToSourceObjectWorld.LengthSquared() <= Math::kEpsilon) return;
	originToSourceObjectWorld.Normalize();

	const auto parentWorldInvRotMat = CreateRotationMatrix(parentWorldMat).Transpose();
	const auto originToSourceObjectLocal = Vector3::TransformNormal(originToSourceObjectWorld, parentWorldInvRotMat);

	const auto originLocalMat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
	const auto originLocalRotation = Quaternion::CreateFromRotationMatrix(CreateRotationMatrix(_offsetRotationMatrix.Transpose() * originLocalMat));
	const auto resultLocalRotation = CalcConstrainedLocalRotation(originLocalRotation, originToSourceObjectLocal);

	ApplyRotationBone(resultLocalRotation);
}

void MultiAimConstraint::ApplyRotationBone(const Quaternion& resultLocalRotation) const
{
	const auto resultLocalRotMat = _offsetRotationMatrix * Matrix::CreateFromQuaternion(resultLocalRotation);
	const auto originLocalMat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
	const auto resultLocalMat = resultLocalRotMat * Matrix::CreateTranslation(originLocalMat.Translation());
 
	MV1SetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex, ToDxLibMatrix(resultLocalMat));
}

Quaternion MultiAimConstraint::CalcConstrainedLocalRotation(const Quaternion& originLocalRotation, const Vector3& originToSourceObjectLocal)
{
	const auto swing = CalcSwingRotation(_initialAimAxis, originToSourceObjectLocal);
	auto aimTargetLocalRotation = _initialLocalRotation * swing * Quaternion::CreateFromYawPitchRoll(_offset * Math::kDeg2Rad);
	aimTargetLocalRotation = ApplyAxisConstraint(originLocalRotation, aimTargetLocalRotation);
	
	Quaternion result;
	Quaternion::Slerp(originLocalRotation, aimTargetLocalRotation, GetEffectiveWeight(), result);
	return result;
}

Quaternion MultiAimConstraint::ApplyAxisConstraint(const Quaternion& originLocalRotation, const Quaternion& targetLocalRotation) const
{
	if (_constrainedXAxis && _constrainedYAxis && _constrainedZAxis) return targetLocalRotation;

	const auto toBoneLocal = [this](const Quaternion& rotation)
	{
		return Quaternion::CreateFromRotationMatrix(_offsetRotationMatrix * Matrix::CreateFromQuaternion(rotation));
	};

	const auto originEulerAngles = toBoneLocal(originLocalRotation).ToEuler();
	auto targetEulerAngles = toBoneLocal(targetLocalRotation).ToEuler();

	if (!_constrainedXAxis) targetEulerAngles.x = originEulerAngles.x;
	if (!_constrainedYAxis) targetEulerAngles.y = originEulerAngles.y;
	if (!_constrainedZAxis) targetEulerAngles.z = originEulerAngles.z;

	return Quaternion::CreateFromYawPitchRoll(targetEulerAngles);
}

Quaternion MultiAimConstraint::CalcSwingRotation(const Vector3& from, const Vector3& to)
{
	// from・to = cos θ
	const auto rad = std::acos(std::clamp(from.Dot(to), -1.f, 1.f));
	
	// 同方向であった場合は回転させない
	if (rad <= Math::kEpsilon) return Quaternion::Identity;
	
	// fromとtoに垂直な軸を取得し、回転軸とする
	auto axis = from.Cross(to);
	
	// 逆方向であった場合は軸が定まらないため前回の軸を使用する
	if (axis.LengthSquared() <= Math::kEpsilon)
	{
		axis = _lastSwingAxis;
	}
	_lastSwingAxis = axis;
	
	return Quaternion::CreateFromAxisAngle(axis, std::min(rad, _limit / Math::kRad2Deg));
}