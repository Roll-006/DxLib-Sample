#include <nlohmann/json.hpp>

struct CameraRendering
{
	int priority;
};

#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, CameraRendering& cameraRendering)
{
	json.at("priority").get_to(cameraRendering.priority);
}

inline void to_json(nlohmann::json& json, const CameraRendering& cameraRendering)
{
	json =
	{
		{ "priority", cameraRendering.priority },
	};
}
#pragma endregion
