#include <nlohmann/json.hpp>

struct CameraProjection
{
	float fieldOfView;
	float clippingPlanesNear;
	float clippingPlanesFar;
};

#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, CameraProjection& cameraProjection)
{
	json.at("fieldOfView").			get_to(cameraProjection.fieldOfView);
	json.at("clippingPlanesNear").	get_to(cameraProjection.clippingPlanesNear);
	json.at("clippingPlanesFar").	get_to(cameraProjection.clippingPlanesFar);
}

inline void to_json(nlohmann::json& json, const CameraProjection& cameraProjection)
{
	json =
	{
		{ "fieldOfView",		cameraProjection.fieldOfView },
		{ "clippingPlanesNear", cameraProjection.clippingPlanesNear },
		{ "clippingPlanesFar",	cameraProjection.clippingPlanesFar },
	};
}
#pragma endregion
