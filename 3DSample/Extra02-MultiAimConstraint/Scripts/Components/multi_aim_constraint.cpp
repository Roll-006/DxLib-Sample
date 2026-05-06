#include <string>
#include <JSON/json_loader.hpp>
#include <Math/math.hpp>
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
	const auto worldMat	= static_cast<math::Matrix4x4>(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex)).LoadToSIMD();
	
	// ワールド回転行列を計算
	const auto lookAt	= math::Matrix4x4::LookAt(worldMat.r[3], _sourceObject.lock()->GetWorldPosition().LoadToSIMD(), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	auto worldRMat		= math::Matrix4x4::GetRMatrix(lookAt);
	
	// 指定されたaim軸及びup軸で姿勢が制御されるように変換
	const auto upAxis			= _upAxis.LoadToSIMD();
	const auto aimAxis			= _aimAxis.LoadToSIMD();
	const auto invOffsetRMat	= XMMatrixTranspose(math::Matrix4x4::CreateRMatrix(XMVector3Cross(upAxis, aimAxis), upAxis, aimAxis));
	worldRMat = invOffsetRMat * worldRMat;

	// ローカル回転行列に変換
	const auto parentWorldRMat		= math::Matrix4x4::GetRMatrix(static_cast<math::Matrix4x4>(MV1GetFrameLocalWorldMatrix(_modelHandle, _parentBoneIndex)).LoadToSIMD());
	const auto invParentWorldRMat	= XMMatrixTranspose(parentWorldRMat);
	auto localMat = worldRMat * invParentWorldRMat;

	// 座標を設定してローカル行列を生成
	localMat.r[3] = static_cast<math::Matrix4x4>(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex)).LoadToSIMD().r[3];

	MV1SetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex, math::Matrix4x4::StoreFromSIMD(localMat));
}
