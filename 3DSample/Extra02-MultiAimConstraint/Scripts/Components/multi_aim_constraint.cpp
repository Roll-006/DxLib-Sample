#include <json_loader.hpp>
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
	_aimAxis				(0.0f, 0.0f, 1.0f),
	_upAxis					(0.0f, 1.0f, 0.0f),
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
	_limit					= std::clamp(_limit, 0.0f, kLimit);
	
	CreateOffsetRotationMatrix();
	
	auto boneMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	auto mat = _offsetRotationMatrix.Transpose() * boneMat;
	_initialBoneRotation = Quaternion::CreateFromRotationMatrix(CreateRotationMatrix(mat));
}

void MultiAimConstraint::Update()
{
	if (!_enabled) { return; }

	AimBone();
}

void MultiAimConstraint::LateUpdate()
{
	if (!_enabled) { return; }
}

void MultiAimConstraint::Render() const
{
	if (!_enabled) { return; }
}

void MultiAimConstraint::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void MultiAimConstraint::CreateOffsetRotationMatrix()
{
	// 指定されたaim軸及びup軸で姿勢が制御されるように行列を生成
	_offsetRotationMatrix = Matrix::Identity;
	_offsetRotationMatrix.Right(_aimAxis.Cross(_upAxis));
	_offsetRotationMatrix.Up(_upAxis);
	_offsetRotationMatrix.Forward(_aimAxis);
	_offsetRotationMatrix = _offsetRotationMatrix.Transpose();
}

void MultiAimConstraint::AimBone() const
{
	auto originWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	auto mat = _offsetRotationMatrix.Transpose() * originWorldMat;
	auto originWorldRotation = Quaternion::CreateFromRotationMatrix(CreateRotationMatrix(mat));
	
	// 制限した回転を計算
	auto forward = _sourceObject.lock()->GetWorldPosition() - originWorldMat.Translation();
	forward.Normalize();
	auto targetWorldRotation = Quaternion::LookRotation(forward, Vector3::Up);
	const auto resultWorldRotationMat = CalcConstrainedRotationMatrix(originWorldRotation, targetWorldRotation);
	
	// ローカル回転行列に変換
	auto parentWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _parentBoneIndex));
	const auto invParentWorldRotationMat = CreateRotationMatrix(parentWorldMat).Transpose();
	const auto resultLocalRotationMat = resultWorldRotationMat * invParentWorldRotationMat;
	
	// 座標を設定してローカル行列を生成
	auto resultLocalMat = resultLocalRotationMat;
	const auto originLocalMat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
	resultLocalMat.m[3][0] = originLocalMat.m[3][0];
	resultLocalMat.m[3][1] = originLocalMat.m[3][1];
	resultLocalMat.m[3][2] = originLocalMat.m[3][2];

	MV1SetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex, ToDxLibMatrix(resultLocalMat));
}

Matrix MultiAimConstraint::CalcConstrainedRotationMatrix(const Quaternion& origin, const Quaternion& target) const
{
	Quaternion resultWorldRotation;
	Quaternion::Slerp(origin, target, 4.0f * Time::GetInstance().GetDeltaTime(), resultWorldRotation);
	
	auto diffAngle = Quaternion::Angle(_initialBoneRotation, resultWorldRotation) * math::kRad2Deg;
	if (diffAngle > 180.0f) { diffAngle = 360.0f - diffAngle; }
	if (diffAngle > _limit)
	{
		resultWorldRotation = origin;
	}
	
	return _offsetRotationMatrix * Matrix::CreateFromQuaternion(resultWorldRotation);
}