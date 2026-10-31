#pragma once
#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "../Core/time.h"
#include "../System/keyboard.h"
#include "../Core/component.h"

class GameObject;

class AimTargetMovement final : public Component
{
public:
	AimTargetMovement();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const override;

	void Deserialize(const nlohmann::json& json) override;

private:
	void Move() const;

private:
	float _moveSpeed;
	std::weak_ptr<Transform> _transform;

	friend void from_json	(const nlohmann::json& json, AimTargetMovement& aimTargetMovement);
	friend void to_json		(nlohmann::json& json, const AimTargetMovement& aimTargetMovement);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, AimTargetMovement& aimTargetMovement)
{
	json.at("enabled").		get_to(aimTargetMovement._enabled);
	json.at("moveSpeed").	get_to(aimTargetMovement._moveSpeed);
}

inline void to_json(nlohmann::json& json, const AimTargetMovement& aimTargetMovement)
{
	json =
	{
		{ "enabled",	aimTargetMovement._enabled },
		{ "moveSpeed",	aimTargetMovement._moveSpeed },
	};
}
#pragma endregion