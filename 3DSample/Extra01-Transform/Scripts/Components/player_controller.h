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

private:
	void Move();

private:
	float	_moveSpeed;
	float	_moveT;
	Vector3 _moveDir;
	std::weak_ptr<Transform> _mainCameraTransform;
	std::weak_ptr<Transform> _transform;

	friend void from_json	(const nlohmann::json& json, PlayerController& playerController);
	friend void to_json		(nlohmann::json& json, const PlayerController& playerController);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, PlayerController& playerController)
{
	json.at("enabled").		get_to(playerController._enabled);
	json.at("moveSpeed").	get_to(playerController._moveSpeed);
	json.at("moveT").		get_to(playerController._moveT);
}

inline void to_json(nlohmann::json& json, const PlayerController& playerController)
{
	json =
	{
		{ "enabled",	playerController._enabled },
		{ "moveSpeed",	playerController._moveSpeed },
		{ "moveT",		playerController._moveT },
	};
}
#pragma endregion
