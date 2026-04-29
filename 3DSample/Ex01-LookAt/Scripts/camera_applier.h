#pragma once
#include "component.h"

class GameObject;

class CameraApplier final : public Component
{
public:
	CameraApplier();
	~CameraApplier();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	std::weak_ptr<Transform> _transform;

	friend void from_json	(const nlohmann::json& json, CameraApplier& cameraApplier);
	friend void to_json		(nlohmann::json& json, const CameraApplier& cameraApplier);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, CameraApplier& cameraApplier)
{
	json.at("enabled").get_to(cameraApplier._enabled);
}

inline void to_json(nlohmann::json& json, const CameraApplier& cameraApplier)
{
	json =
	{
		{ "enabled", cameraApplier._enabled },
	};
}
#pragma endregion
