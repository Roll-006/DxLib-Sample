#pragma once
#include <DxLibSample/component.h>

namespace DxLibSample
{
	class GameObject;
}

namespace Components
{
	class AimTargetMovement final : public DxLibSample::Component
	{
	public:
		AimTargetMovement();

		void Initialize(const std::shared_ptr<DxLibSample::GameObject>& gameObject) override;
		void Start()		override;
		void Update()       override;
		void LateUpdate()   override;
		void Render() const override;

		void Deserialize(const nlohmann::json& json) override;

	private:
		void Move() const;

	private:
		float _moveSpeed;
		std::weak_ptr<DxLibSample::Transform> _transform;

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
}