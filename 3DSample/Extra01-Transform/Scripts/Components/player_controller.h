#pragma once
#include "../Core/component.h"

class GameObject;

class PlayerController final : public Component
{
public:
	PlayerController();
	~PlayerController();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

	int GetModelHandle() const { return _modelHandle; }

private:
	int _modelHandle;
	std::string _modelPath;
	std::weak_ptr<Transform> _transform;

	friend void from_json	(const nlohmann::json& json, PlayerController& playerController);
	friend void to_json		(nlohmann::json& json, const PlayerController& playerController);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, PlayerController& playerController)
{
	json.at("enabled").get_to(playerController._enabled);
}

inline void to_json(nlohmann::json& json, const PlayerController& playerController)
{
	json =
	{
		{ "enabled", playerController._enabled },
	};
}
#pragma endregion
