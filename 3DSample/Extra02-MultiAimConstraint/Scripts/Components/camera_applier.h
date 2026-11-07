#pragma once
#include "../Core/component.h"

namespace Core
{
    class GameObject;
}

namespace Components
{
	class CameraApplier final : public Core::Component
	{
	public:
		CameraApplier();

		void Initialize(const std::shared_ptr<Core::GameObject>& gameObject) override;
		void Start()		override;
		void Update()       override;
		void LateUpdate()   override;
		void Render() const override;

		void Deserialize(const nlohmann::json& json) override;

	private:
		std::weak_ptr<Core::Transform> _transform;

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
}