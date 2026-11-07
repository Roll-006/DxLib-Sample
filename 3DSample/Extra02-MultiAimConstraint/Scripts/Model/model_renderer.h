#pragma once
#include "../Core/component.h"
#include "model_data.h"

namespace Core
{
	class GameObject;
}

namespace Model
{
	class ModelRenderer final : public Core::Component
	{
	public:
		ModelRenderer();
		~ModelRenderer() override;

		void Initialize(const std::shared_ptr<Core::GameObject>& gameObject) override;
		void Start()		override;
		void Update()       override;
		void LateUpdate()   override;
		void Render() const override;

		void Deserialize(const nlohmann::json& json) override;

		[[nodiscard]] ModelData GetModelData() const { return _modelData; }

	private:
		ModelData _modelData;
		std::string _modelPath;
		std::weak_ptr<Core::Transform> _transform;

		friend void from_json	(const nlohmann::json& json, ModelRenderer& modelRenderer);
		friend void to_json		(nlohmann::json& json, const ModelRenderer& modelRenderer);
	};

	#pragma region from / to JSON
	inline void from_json(const nlohmann::json& json, ModelRenderer& modelRenderer)
	{
		json.at("enabled").		get_to(modelRenderer._enabled);
		json.at("modelPath").	get_to(modelRenderer._modelPath);
	}

	inline void to_json(nlohmann::json& json, const ModelRenderer& modelRenderer)
	{
		json =
		{
			{ "enabled",	modelRenderer._enabled },
			{ "modelPath",	modelRenderer._modelPath },
		};
	}
	#pragma endregion
}