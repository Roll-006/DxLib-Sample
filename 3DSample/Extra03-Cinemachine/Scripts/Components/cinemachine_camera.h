#pragma once
#include "../Core/component.h"

class GameObject;

class CinemachineCamera final : public Component
{
public:
	CinemachineCamera();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	std::weak_ptr<Transform> _transform;

	friend void from_json	(const nlohmann::json& json, CinemachineCamera& cinemachineCamera);
	friend void to_json		(nlohmann::json& json, const CinemachineCamera& cinemachineCamera);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, CinemachineCamera& cinemachineCamera)
{
	json.at("enabled").get_to(cinemachineCamera._enabled);
}

inline void to_json(nlohmann::json& json, const CinemachineCamera& cinemachineCamera)
{
	json =
	{
		{ "enabled", cinemachineCamera._enabled },
	};
}
#pragma endregion
