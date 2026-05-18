#pragma once
#include "../Core/component.h"

class GameObject;

class CinemachineBrain final : public Component
{
public:
	CinemachineBrain();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	float _blendTime;
	float _blendTimer;
	std::vector<std::shared_ptr<Transform>> _cinemachines;
	std::weak_ptr<Transform> _mainCameraTransform;

	friend void from_json	(const nlohmann::json& json, CinemachineBrain& cinemachineBrain);
	friend void to_json		(nlohmann::json& json, const CinemachineBrain& cinemachineBrain);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, CinemachineBrain& cinemachineBrain)
{
	json.at("enabled").		get_to(cinemachineBrain._enabled);
	json.at("blendTime").	get_to(cinemachineBrain._blendTime);
}

inline void to_json(nlohmann::json& json, const CinemachineBrain& cinemachineBrain)
{
	json =
	{
		{ "enabled",	cinemachineBrain._enabled },
		{ "blendTime",	cinemachineBrain._blendTime },
	};
}
#pragma endregion
