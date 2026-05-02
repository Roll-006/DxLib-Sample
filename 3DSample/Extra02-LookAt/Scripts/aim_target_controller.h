#pragma once
#include "component.h"

class GameObject;

class AimTargetController final : public Component
{
public:
	AimTargetController();
	~AimTargetController();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	void Move();

private:
	float _moveSpeed;
	std::weak_ptr<Transform> _transform;

	friend void from_json	(const nlohmann::json& json, AimTargetController& aimTargetController);
	friend void to_json		(nlohmann::json& json, const AimTargetController& aimTargetController);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, AimTargetController& aimTargetController)
{
	json.at("enabled").		get_to(aimTargetController._enabled);
	json.at("moveSpeed").	get_to(aimTargetController._moveSpeed);
}

inline void to_json(nlohmann::json& json, const AimTargetController& aimTargetController)
{
	json =
	{
		{ "enabled",	aimTargetController._enabled },
		{ "moveSpeed",	aimTargetController._moveSpeed },
	};
}
#pragma endregion
