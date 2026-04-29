#include <DxLib.h>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <JSON/json_loader.hpp>
#include <string>
#include "math.h"
#include "transform.h"
#include "model_renderer.h"
#include "component_factory.h"
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
	_aimAxis				(0, 0, 1),
	_upAxis					(0, 1, 0),
	_sourceObjectName		(""),
	_sourceObject			(),
	_transform				()
{

}

MultiAimConstraint::~MultiAimConstraint()
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

	//TODO : ボーンの進行方向とupを指定できるようにする必要あり

	// 回転させるボーンの行列を取得
	auto frameWorldMat = static_cast<Matrix4x4>(MV1GetFrameLocalWorldMatrix(_modelHandle, _constrainedBoneIndex));

	// 対象の方向を見る
	frameWorldMat = Matrix4x4::LookAt(frameWorldMat.GetTranslation(), _sourceObject.lock()->GetPosition(), Vector3::GetUp());
	
	// ローカル行列に変換
	const auto parentWorldRMat	= static_cast<Matrix4x4>(MV1GetFrameLocalWorldMatrix(_modelHandle, _parentBoneIndex)).GetRMatrix();
	const auto frameLocalRMat	= frameWorldMat.GetRMatrix() * MInverse(parentWorldRMat);
	
	// ローカル行列に回転行列を設定
	auto frameLocalMat = static_cast<Matrix4x4>(MV1GetFrameLocalMatrix(_modelHandle, _constrainedBoneIndex));
	frameLocalMat.SetRotation(frameLocalRMat);
	
	// ローカル行列として適用
	MV1SetFrameUserLocalMatrix(_modelHandle, _constrainedBoneIndex, frameLocalMat);
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
