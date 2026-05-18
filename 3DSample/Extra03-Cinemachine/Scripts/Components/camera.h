#pragma once
#include "../Core/component.h"
#include "../References/camera_projection.h"
#include "../References/camera_rendering.h"

class GameObject;

class Camera final : public Component
{
public:
	Camera();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	CameraProjection			_projection;
	CameraRendering				_rendering;
	std::weak_ptr<Transform>	_transform;

	friend void from_json	(const nlohmann::json& json, Camera& camera);
	friend void to_json		(nlohmann::json& json, const Camera& camera);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, Camera& camera)
{
	json.at("enabled").		get_to(camera._enabled);
	json.at("projection").	get_to(camera._projection);
	json.at("rendering").	get_to(camera._rendering);
}

inline void to_json(nlohmann::json& json, const Camera& camera)
{
	json =
	{
		{ "enabled",	camera._enabled },
		{ "projection", camera._projection },
		{ "rendering",	camera._rendering },
	};
}
#pragma endregion
