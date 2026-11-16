#pragma once
#include <DxLibSample/component.h>

namespace DxLibSample
{
    class GameObject;
}

namespace Components
{
	// MEMO: 仮カメラ
	class Camera final : public DxLibSample::Component
	{
	public:
		Camera();

		void Initialize(const std::shared_ptr<DxLibSample::GameObject>& gameObject) override;
		void Start()		override;
		void Update()       override;
		void LateUpdate()   override;

		void Deserialize(const nlohmann::json& json) override;

	private:
		float _near;
		float _far;
		float _fov;
		std::string _targetName;
		Vector3 _offset;
		Vector3 _eulerAngles;
		std::shared_ptr<DxLibSample::Transform> _target;
		std::weak_ptr<DxLibSample::Transform> _transform;

		friend void from_json	(const nlohmann::json& json, Camera& cameraApplier);
		friend void to_json		(nlohmann::json& json, const Camera& cameraApplier);
	};

	#pragma region from / to JSON
	inline void from_json(const nlohmann::json& json, Camera& cameraApplier)
	{
		json.at("enabled").		get_to(cameraApplier._enabled);
		json.at("near").		get_to(cameraApplier._near);
		json.at("far").			get_to(cameraApplier._far);
		json.at("fov").			get_to(cameraApplier._fov);
		json.at("targetName").	get_to(cameraApplier._targetName);
		json.at("offset").		get_to(cameraApplier._offset);
		json.at("eulerAngles").	get_to(cameraApplier._eulerAngles);
	}

	inline void to_json(nlohmann::json& json, const Camera& cameraApplier)
	{
		json =
		{
			{ "enabled",		cameraApplier._enabled },
			{ "near",			cameraApplier._near },
			{ "far",			cameraApplier._far },
			{ "fov",			cameraApplier._fov },
			{ "targetName",		cameraApplier._targetName },
			{ "offset",			cameraApplier._offset },
			{ "eulerAngles",	cameraApplier._eulerAngles },
		};
	}
	#pragma endregion
}