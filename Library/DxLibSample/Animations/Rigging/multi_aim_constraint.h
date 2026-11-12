#pragma once
#include "rig_constraint.h"

namespace DxLibSample
{
	class GameObject;
}

namespace DxLibSample::Animations::Rigging
{
	class MultiAimConstraint final : public RigConstraint
	{
	public:
		MultiAimConstraint();

		void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
		void Start()		override;
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
		/// <param name="originToSourceObjectLocal">追尾対象までのローカル方向ベクトル</param>
		/// <returns>角度制限を行ったローカルクォータニオン</returns>
		[[nodiscard]] Quaternion CalcConstrainedLocalRotation(const Quaternion& originLocalRotation, const Vector3& originToSourceObjectLocal);
		
		/// <summary>
		/// 各軸を有効状態を適用する。
		/// </summary>
		/// <param name="originLocalRotation">回転させるボーンのローカルクォータニオン</param>
		/// <param name="targetLocalRotation">回転目標のローカルクォータニオン</param>
		/// <returns>有効状態が適用されたローカルクォータニオン</returns>
		[[nodiscard]] Quaternion ApplyAxisConstraint(const Quaternion& originLocalRotation, const Quaternion& targetLocalRotation) const;

		/// <summary>
		/// 傾きを計算する。
		/// </summary>
		/// <param name="from">回転元のaim軸</param>
		/// <param name="to">回転目標のaim軸</param>
		/// <returns></returns>
		[[nodiscard]] Quaternion CalcSwingRotation(const Vector3& from, const Vector3& to);
	
	private:
		static constexpr float kMaxLimit = 180.f;
	
		int								_modelHandle;
		std::string						_constrainedBoneName;	// sourceObjectの影響を受けるボーンの名前
		int								_constrainedBoneIndex;	// sourceObjectの影響を受けるボーンのインデックス
		int								_parentBoneIndex;		// constrainedBoneの親ボーンのインデックス
		Vector3							_aimAxis;				// ボーンの前方向をsourceObjectに向けるためのローカルaim軸
		Vector3							_upAxis;				// ボーンの上方向に向けるためのローカルup軸
		std::string						_sourceObjectName;		// ボーンの向きに影響を与えるゲームオブジェクトの名前
		std::weak_ptr<DxLibSample::Transform>	_sourceObject;			// ボーンの向きに影響を与えるゲームオブジェクト
		Vector3							_offset;				// 回転をずらす量
		bool 							_constrainedXAxis;		// X軸回転を有効にするかどうか
		bool 							_constrainedYAxis;		// Y軸回転を有効にするかどうか
		bool 							_constrainedZAxis;		// Z軸回転を有効にするかどうか
		float							_limit;					// 回転軸に適用する上限値
		Matrix							_offsetRotationMatrix;	// ボーン独自の軸を適用するためのオフセット
		Quaternion						_initialLocalRotation;  // オフセットを除いた親基準の初期回転
		Vector3							_initialAimAxis;		// 親空間での初期aim方向
		Vector3							_initialUpAxis;			// 親空間での初期up方向
		Vector3							_lastSwingAxis;         // 直前のスイング軸

		friend void from_json	(const nlohmann::json& json, MultiAimConstraint& multiAimConstraint);
		friend void to_json		(nlohmann::json& json, const MultiAimConstraint& multiAimConstraint);
	};

	#pragma region from / to JSON
	inline void from_json(const nlohmann::json& json, MultiAimConstraint& multiAimConstraint)
	{
		json.at("enabled").				get_to(multiAimConstraint._enabled);
		json.at("weight").				get_to(multiAimConstraint._weight);
		json.at("constrainedBoneName").	get_to(multiAimConstraint._constrainedBoneName);
		json.at("aimAxis").				get_to(multiAimConstraint._aimAxis);
		json.at("upAxis").				get_to(multiAimConstraint._upAxis);
		json.at("sourceObjectName").	get_to(multiAimConstraint._sourceObjectName);
		json.at("offset").				get_to(multiAimConstraint._offset);
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
			{ "weight",				multiAimConstraint._weight },
			{ "constrainedBoneName",	multiAimConstraint._constrainedBoneName },
			{ "aimAxis",				multiAimConstraint._aimAxis },
			{ "upAxis",				multiAimConstraint._upAxis },
			{ "sourceObjectName",		multiAimConstraint._sourceObjectName },
			{ "offset",				multiAimConstraint._offset },
			{ "constrainedXAxis",		multiAimConstraint._constrainedXAxis },
			{ "constrainedYAxis",		multiAimConstraint._constrainedYAxis },
			{ "constrainedZAxis",		multiAimConstraint._constrainedZAxis },
			{ "limit",				multiAimConstraint._limit },
		};
	}
	#pragma endregion
}