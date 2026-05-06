#pragma once
#include "../Core/component.h"

class GameObject;

class PlayerMover final : public Component
{
public:
	PlayerMover();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	void Move();

private:
	float _moveSpeed;
	std::weak_ptr<PlayerController> _playerController;
	std::weak_ptr<Transform> _transform;

	friend void from_json	(const nlohmann::json& json, PlayerMover& playerMover);
	friend void to_json		(nlohmann::json& json, const PlayerMover& playerMover);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, PlayerMover& playerMover)
{
	json.at("enabled").		get_to(playerMover._enabled);
	json.at("moveSpeed").	get_to(playerMover._moveSpeed);
}

inline void to_json(nlohmann::json& json, const PlayerMover& playerMover)
{
	json =
	{
		{ "enabled",	playerMover._enabled },
		{ "moveSpeed",	playerMover._moveSpeed },
	};
}
#pragma endregion
