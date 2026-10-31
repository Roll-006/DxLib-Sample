#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "model_renderer.h"
#include "multi_aim_constraint.h"
#include "../Core/time.h"

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
	_limit					(kLimit)
{
	
}

void MultiAimConstraint::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject		= gameObject;
	_transform		= GetComponent<Transform>();
	_sourceObject	= gameObject->Find(_sourceObjectName);
	
	_modelHandle			= GetComponent<ModelRenderer>()->GetModelHandle();
	_constrainedBoneIndex	= MV1SearchFrame(_modelHandle, _constrainedBoneName.c_str());
	_parentBoneIndex		= MV1GetFrameParent(_modelHandle, _constrainedBoneIndex);
	_limit					= std::clamp(_limit, 0.f, kLimit);
	
	CreateOffsetRotationMatrix();
	
	const auto localMat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
 
	// オフセットを除いた、親基準の初期回転
	_initialLocalRotation = Quaternion::CreateFromRotationMatrix(CreateRotationMatrix(_offsetRotationMatrix.Transpose() * localMat));
	
	// 親空間での初期aim軸及びup軸
	_initialAimAxis = Vector3::TransformNormal(_aimAxis, localMat);
	_initialUpAxis = Vector3::TransformNormal(_upAxis, localMat);
	
	// スイング軸が不定になったときのフォールバック用
	_lastSwingAxis = _initialAimAxis.Cross(_initialUpAxis);
	if (_lastSwingAxis.LengthSquared() <= math::kEpsilon) { _lastSwingAxis = Vector3::Up; }
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
	
	RenderDebugLine();
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
	const auto originWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	const auto parentWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _parentBoneIndex));
	
	// ターゲット方向を取得
	auto worldToSourceObject = _sourceObject.lock()->GetWorldPosition() - originWorldMat.Translation();
	if (worldToSourceObject.LengthSquared() <= math::kEpsilon) return;
	worldToSourceObject.Normalize();
	
	// ターゲット方向をローカル空間に変換
	const auto parentWorldInvRotMat = CreateRotationMatrix(parentWorldMat).Transpose();
	const auto localToSourceObject = Vector3::TransformNormal(worldToSourceObject, parentWorldInvRotMat);
	
	const auto originLocalRotMat = CreateRotationMatrix(GetLocalMatrix(_offsetRotationMatrix.Transpose() * originWorldMat, parentWorldMat));
	const auto originLocalRotation = Quaternion::CreateFromRotationMatrix(originLocalRotMat);
	
	const auto resultLocalRotation = CalcConstrainedLocalRotation(originLocalRotation, localToSourceObject);
	
	ApplyRotationBone(resultLocalRotation);
}

void MultiAimConstraint::ApplyRotationBone(const Quaternion& resultLocalRotation) const
{
	const auto resultLocalRotMat = _offsetRotationMatrix * Matrix::CreateFromQuaternion(resultLocalRotation);
	const auto originLocalMat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
	
	auto resultLocalMat = resultLocalRotMat;
	resultLocalMat.m[3][0] = originLocalMat.m[3][0];
	resultLocalMat.m[3][1] = originLocalMat.m[3][1];
	resultLocalMat.m[3][2] = originLocalMat.m[3][2];
 
	MV1SetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex, ToDxLibMatrix(resultLocalMat));
}

Quaternion MultiAimConstraint::CalcConstrainedLocalRotation(const Quaternion& originLocalRotation, const Vector3& localToSourceObject)
{
	const auto swing = CalcSwingRotation(_initialAimAxis, localToSourceObject);
	const auto targetLocalRotation = _initialLocalRotation * swing;
	const auto t = kFollowSpeed * Time::GetInstance().GetDeltaTime();
	
	Quaternion resultLocalRotation;
	Quaternion::Slerp(originLocalRotation, targetLocalRotation, t, resultLocalRotation);
	return resultLocalRotation;
}

Quaternion MultiAimConstraint::CalcSwingRotation(const Vector3& from, const Vector3& to)
{
	// from・to = cos θ
	const auto angleRad = std::acos(std::clamp(from.Dot(to), -1.f, 1.f));
	
	// 同方向であった場合は回転させない
	if (angleRad <= math::kEpsilon) return Quaternion::Identity;
	
	// fromとtoに垂直な軸を取得し、回転軸とする
	auto axis = from.Cross(to);
	
	// 逆方向であった場合は軸が定まらないため前回の軸を使用する
	if (axis.LengthSquared() <= math::kEpsilon)
	{
		axis = _lastSwingAxis;
	}
	_lastSwingAxis = axis;
	
	return Quaternion::CreateFromAxisAngle(axis, std::min(angleRad, _limit / math::kRad2Deg));
}

void MultiAimConstraint::RenderDebugLine() const
{
	const auto mat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	const auto rotMat = CreateRotationMatrix(mat);
	const auto forward = Vector3::TransformNormal(Vector3::Forward, rotMat);
	DrawLine3D(ToDxLibVector(mat.Translation()), ToDxLibVector(mat.Translation() + forward * 30.f), 0xffffff);
}