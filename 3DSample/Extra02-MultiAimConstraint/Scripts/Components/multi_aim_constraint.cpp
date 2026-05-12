#include <string>
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
	_constrainedBoneName	(""),
	_constrainedBoneIndex	(-1),
	_parentBoneIndex		(-1),
	_aimAxis				(0.0f, 0.0f, 1.0f),
	_upAxis					(0.0f, 1.0f, 0.0f),
	_sourceObjectName		(""),
	_sourceObject			(),
	_transform				()
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

void MultiAimConstraint::AimBone()
{
	const auto worldMat	= ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));
	
	// ワールド回転行列を計算
	auto lookAt	= Matrix::CreateWorld(worldMat.Translation(), _sourceObject.lock()->GetWorldPosition() - worldMat.Translation(), Vector3::Up);
	auto worldRMat = CreateRotationMatrix(lookAt);
	
	// 指定されたaim軸及びup軸で姿勢が制御されるように変換
	auto invOffsetRMat	= Matrix::Identity;
	invOffsetRMat.Right(_aimAxis.Cross(_upAxis));
	invOffsetRMat.Up(_upAxis);
	invOffsetRMat.Forward(_aimAxis);
	invOffsetRMat = invOffsetRMat.Transpose();
	worldRMat = invOffsetRMat * worldRMat;

	// ローカル回転行列に変換
	auto parentWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelHandle, _parentBoneIndex));
	const auto invParentWorldRMat = CreateRotationMatrix(parentWorldMat).Transpose();
	auto localMat = worldRMat * invParentWorldRMat;

	// 座標を設定してローカル行列を生成
	const auto mat = ToMatrix(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
	localMat.m[3][0] = mat.m[3][0];
	localMat.m[3][1] = mat.m[3][1];
	localMat.m[3][2] = mat.m[3][2];

	MV1SetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex, ToDxLibMatrix(localMat));
}
