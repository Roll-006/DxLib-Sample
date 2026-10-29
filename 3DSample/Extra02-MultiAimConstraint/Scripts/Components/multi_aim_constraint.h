#pragma once
#include "../Core/component.h"

class GameObject;

class MultiAimConstraint final : public Component
{
public:
	MultiAimConstraint();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	void CreateOffsetRotationInverseMatrix();
	void AimBone() const;

private:
	static constexpr float		kMinLimit = -180.0f;
	static constexpr float		kMaxLimit = 180.0f;
	
	int							_modelHandle;
	std::string					_constrainedBoneName;	// sourceObjectの影響を受けるボーンの名前
	int							_constrainedBoneIndex;	// sourceObjectの影響を受けるボーンのインデックス
	int							_parentBoneIndex;		// constrainedBoneの親ボーンのインデックス
	Vector3						_aimAxis;				// ボーンの前方向をsourceObjectに向けるためのローカルaim軸
	Vector3						_upAxis;				// ボーンの上方向に向けるためのローカルup軸
	std::string					_sourceObjectName;		// ボーンの向きに影響を与えるゲームオブジェクトの名前
	std::weak_ptr<Transform>	_sourceObject;			// ボーンの向きに影響を与えるゲームオブジェクト
	bool						_constrainedAxisX;		// コンストレイントをX軸に適用するかどうか
	bool						_constrainedAxisY;		// コンストレイントをY軸に適用するかどうか
	bool						_constrainedAxisZ;		// コンストレイントをZ軸に適用するかどうか
	float						_minLimit;				// 回転軸に適用する最小値
	float						_maxLimit;				// 回転軸に適用する最大値
	Quaternion					_initialBoneRotation;	// ボーンの初期回転量
	Quaternion					_resultRotation;
	Matrix						_offsetRotationInverseMatrix;
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
	json.at("constrainedAxisX").	get_to(multiAimConstraint._constrainedAxisX);
	json.at("constrainedAxisY").	get_to(multiAimConstraint._constrainedAxisY);
	json.at("constrainedAxisZ").	get_to(multiAimConstraint._constrainedAxisZ);
	json.at("minLimit").			get_to(multiAimConstraint._minLimit);
	json.at("maxLimit").			get_to(multiAimConstraint._maxLimit);
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
		{ "constrainedAxisX",		multiAimConstraint._constrainedAxisX },
		{ "constrainedAxisY",		multiAimConstraint._constrainedAxisY },
		{ "constrainedAxisZ",		multiAimConstraint._constrainedAxisZ },
		{ "minLimit",				multiAimConstraint._minLimit },
		{ "maxLimit",				multiAimConstraint._maxLimit },
	};
}
#pragma endregion