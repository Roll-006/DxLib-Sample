#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "model_renderer.h"
#include "multi_aim_constraint.h"

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
	_constrainedAxisX		(true),
	_constrainedAxisY		(true),
	_constrainedAxisZ		(true),
	_minLimit				(kMinLimit),
	_maxLimit				(kMaxLimit),
	_initialBoneRotation	(Quaternion::Identity)
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
	_minLimit				= std::clamp(_minLimit, kMinLimit, kMaxLimit);
	_maxLimit				= std::clamp(_maxLimit, kMinLimit, kMaxLimit);
	
	auto boneMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	_initialBoneRotation = Quaternion::CreateFromRotationMatrix(CreateRotationMatrix(boneMat));
	
	CreateOffsetRotationInverseMatrix();
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

void MultiAimConstraint::CreateOffsetRotationInverseMatrix()
{
	// 指定されたaim軸及びup軸で姿勢が制御されるように行列を生成
	_offsetRotationInverseMatrix = Matrix::Identity;
	_offsetRotationInverseMatrix.Right(_aimAxis.Cross(_upAxis));
	_offsetRotationInverseMatrix.Up(_upAxis);
	_offsetRotationInverseMatrix.Forward(_aimAxis);
	_offsetRotationInverseMatrix = _offsetRotationInverseMatrix.Transpose();
}

void MultiAimConstraint::AimBone() const
{
	auto originWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	auto originWorldRotation = Quaternion::CreateFromRotationMatrix(CreateRotationMatrix(originWorldMat));

	// ワールド回転行列を計算
	auto targetWorldRotation = Quaternion::LookRotation(_sourceObject.lock()->GetWorldPosition() - originWorldMat.Translation(), Vector3::Up);
	
	
	
	// 角度制限
	Quaternion resultRotation;
	originWorldRotation.RotateTowards(targetWorldRotation, 0.05f, resultRotation);
	
	const auto diffAngle = Quaternion::Angle(_initialBoneRotation, resultRotation) * math::kRad2Deg;
	
	
	if (diffAngle > _maxLimit || diffAngle < _minLimit)
	{
		resultRotation = originWorldRotation;
	}
	
	auto resultWorldRMat = Matrix::CreateFromQuaternion(resultRotation);
	
	printfDx("diffAngle : %f\n", diffAngle);
	printfDx("_initialBoneRotation : %f, %f, %f\n", _initialBoneRotation.ToEuler().x, _initialBoneRotation.ToEuler().y, _initialBoneRotation.ToEuler().z);
	printfDx("resultRotation : %f, %f, %f\n", resultRotation.ToEuler().x, resultRotation.ToEuler().y, resultRotation.ToEuler().z);
	
	resultWorldRMat = _offsetRotationInverseMatrix * resultWorldRMat;

	// ローカル回転行列に変換
	auto parentWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _parentBoneIndex));
	const auto invParentWorldRMat = CreateRotationMatrix(parentWorldMat).Transpose();
	auto localMat = resultWorldRMat * invParentWorldRMat;

	// 座標を設定してローカル行列を生成
	const auto mat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
	localMat.m[3][0] = mat.m[3][0];
	localMat.m[3][1] = mat.m[3][1];
	localMat.m[3][2] = mat.m[3][2];

	MV1SetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex, ToDxLibMatrix(localMat));
}
