#pragma once
#include "component.h"

class GameObject;

class MultiAimConstraint final : public Component
{
public:
	MultiAimConstraint();
	~MultiAimConstraint();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	void AimBone();

private:
	int							_modelHandle;
	std::string					_constrainedBoneName;	// sourceObjectの影響を受けるボーンの名前
	int							_constrainedBoneIndex;	// sourceObjectの影響を受けるボーンのインデックス
	int							_parentBoneIndex;		// constrainedBoneの親ボーンのインデックス
	Vector3						_aimAxis;				// ボーンの前方向をsourceObjectに向けるためのローカルaim軸
	Vector3						_upAxis;				// ボーンの上方向に向けるためのローカルup軸
	std::string					_sourceObjectName;		// ボーンの向きに影響を与えるゲームオブジェクトの名前
	std::weak_ptr<Transform>	_sourceObject;			// ボーンの向きに影響を与えるゲームオブジェクト
	std::weak_ptr<Transform>	_transform;

	friend void from_json	(const nlohmann::json& json, MultiAimConstraint& multiAimConstraint);
	friend void to_json		(nlohmann::json& json, const MultiAimConstraint& multiAimConstraint);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, MultiAimConstraint& multiAimConstraint)
{
	json.at("enabled").				get_to(multiAimConstraint._enabled);
	json.at("constrainedBoneName").	get_to(multiAimConstraint._constrainedBoneName);
	json.at("aimAxis").				get_to(multiAimConstraint._aimAxis);
	json.at("upAxis").				get_to(multiAimConstraint._upAxis);
	json.at("sourceObjectName").	get_to(multiAimConstraint._sourceObjectName);
}

inline void to_json(nlohmann::json& json, const MultiAimConstraint& multiAimConstraint)
{
	json =
	{
		{ "enabled",				multiAimConstraint._enabled },
		{ "constrainedBoneName",	multiAimConstraint._constrainedBoneName },
		{ "aimAxis",				multiAimConstraint._aimAxis },
		{ "upAxis",					multiAimConstraint._upAxis },
		{ "sourceObjectName",		multiAimConstraint._sourceObjectName },
	};
}
#pragma endregion
