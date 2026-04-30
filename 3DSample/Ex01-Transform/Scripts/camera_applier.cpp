#include <DxLib.h>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <JSON/json_loader.hpp>
#include <string>
#include "math.h"
#include "transform.h"
#include "component_factory.h"
#include "camera_applier.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("CameraApplier", [](GameObject& obj) { return obj.AddComponent<CameraApplier>(); });
		return true;
	}();
}

CameraApplier::CameraApplier()
{
	// 仮
	SetCameraNearFar(1.0f, 100.0f);
	SetupCamera_Perspective(60.0f * math::kDeg2Rad);
}

CameraApplier::~CameraApplier()
{

}

void CameraApplier::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform = GetComponent<Transform>();
}

void CameraApplier::Update()
{
	if (!_enabled) { return; }
}

void CameraApplier::LateUpdate()
{
	if (!_enabled) { return; }

	// カメラの位置及び姿勢を設定
	const auto transform = _transform.lock();
	SetCameraPositionAndTargetAndUpVec(transform->GetPosition(), transform->GetForward(), transform->GetUp());

	if (transform->GetParent() == nullptr)
	{
		int a = 0;
	}
}

void CameraApplier::Render() const
{
	if (!_enabled) { return; }
}

void CameraApplier::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}
