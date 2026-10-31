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
	/// <summary>
	/// ボーン独自の軸を適用するためのオフセット行列を生成する。
	/// </summary>
	void CreateOffsetRotationMatrix();

	/// <summary>
	/// ボーンを対象に向ける。
	/// </summary>
	void AimAtSourceObject();

	/// <summary>
	/// 回転をボーンに適用する。
	/// </summary>
	/// <param name="resultLocalRotation">ボーンに適用させるローカルクォータニオン</param>
	void ApplyRotationBone(const Quaternion& resultLocalRotation) const;

	/// <summary>
	/// 角度制限を行ったローカルクォータニオンを計算する。
	/// </summary>
	/// <param name="originLocalRotation">回転させるボーンのローカルクォータニオン</param>
	/// <param name="localToSourceObject">追尾対象までのローカル方向ベクトル</param>
	/// <returns></returns>
	Quaternion CalcConstrainedLocalRotation(const Quaternion& originLocalRotation, const Vector3& localToSourceObject);

	/// <summary>
	/// 傾きを計算する。
	/// </summary>
	/// <param name="from">回転元のaim軸</param>
	/// <param name="to">回転目標のaim軸</param>
	/// <returns></returns>
	Quaternion CalcSwingRotation(const Vector3& from, const Vector3& to);

	void RenderDebugLine() const;
	
private:
	static constexpr float		kLimit = 180.f;
	static constexpr float		kFollowSpeed = 8.f;
	
	int							_modelHandle;
	std::string					_constrainedBoneName;	// sourceObjectの影響を受けるボーンの名前
	int							_constrainedBoneIndex;	// sourceObjectの影響を受けるボーンのインデックス
	int							_parentBoneIndex;		// constrainedBoneの親ボーンのインデックス
	Vector3						_aimAxis;				// ボーンの前方向をsourceObjectに向けるためのローカルaim軸
	Vector3						_upAxis;				// ボーンの上方向に向けるためのローカルup軸
	std::string					_sourceObjectName;		// ボーンの向きに影響を与えるゲームオブジェクトの名前
	std::weak_ptr<Transform>	_sourceObject;			// ボーンの向きに影響を与えるゲームオブジェクト
	bool						_constrainedXAxis;		// コンストレイントをX軸に適用するかどうか
	bool						_constrainedYAxis;		// コンストレイントをY軸に適用するかどうか
	bool						_constrainedZAxis;		// コンストレイントをZ軸に適用するかどうか
	float						_limit;					// 回転軸に適用する上限値
	Matrix						_offsetRotationMatrix;	// ボーン独自の軸を適用するためのオフセット
	Quaternion					_initialLocalRotation;  // オフセットを除いた親基準の初期回転
	Vector3						_initialAimAxis;		// 親空間での初期aim方向
	Vector3						_initialUpAxis;			// 親空間での初期up方向
	Vector3						_lastSwingAxis;         // 直前のスイング軸
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
	json.at("constrainedXAxis").	get_to(multiAimConstraint._constrainedXAxis);
	json.at("constrainedYAxis").	get_to(multiAimConstraint._constrainedYAxis);
	json.at("constrainedZAxis").	get_to(multiAimConstraint._constrainedZAxis);
	json.at("limit").				get_to(multiAimConstraint._limit);
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
		{ "constrainedXAxis",		multiAimConstraint._constrainedXAxis },
		{ "constrainedYAxis",		multiAimConstraint._constrainedYAxis },
		{ "constrainedZAxis",		multiAimConstraint._constrainedZAxis },
		{ "limit",					multiAimConstraint._limit },
	};
}
#pragma endregion