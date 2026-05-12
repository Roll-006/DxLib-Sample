#pragma once
#include "../Core/component.h"

class GameObject;

class PlayerController final : public Component
{
public:
	PlayerController();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

	Vector3 GetMoveDir() const { return _moveDir; }
	bool IsMoving() const { return _isMoving; }

private:
	void Control();

private:
	float			_moveT;
	Vector3	_moveDir;
	bool			_isMoving;
	std::weak_ptr<Transform> _mainCameraTransform;

	friend void from_json	(const nlohmann::json& json, PlayerController& playerController);
	friend void to_json		(nlohmann::json& json, const PlayerController& playerController);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, PlayerController& playerController)
{
	json.at("enabled").	get_to(playerController._enabled);
	json.at("moveT").	get_to(playerController._moveT);
}

inline void to_json(nlohmann::json& json, const PlayerController& playerController)
{
	json =
	{
		{ "enabled",	playerController._enabled },
		{ "moveT",		playerController._moveT },
	};
}
#pragma endregion
