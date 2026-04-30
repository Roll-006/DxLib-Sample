#include <DxLib.h>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <JSON/json_loader.hpp>
#include <string>
#include "math.h"
#include "transform.h"
#include "component_factory.h"
#include "model_renderer.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("ModelRenderer", [](GameObject& obj) { return obj.AddComponent<ModelRenderer>(); });
		return true;
	}();
}

ModelRenderer::ModelRenderer() :
	_modelHandle(-1),
	_modelPath	(""),
	_transform	()
{

}

ModelRenderer::~ModelRenderer()
{
	MV1DeleteModel(_modelHandle);
}

void ModelRenderer::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform	= GetComponent<Transform>();
}

void ModelRenderer::Update()
{
	if (!_enabled) { return; }
}

void ModelRenderer::LateUpdate()
{
	if (!_enabled) { return; }
}

void ModelRenderer::Render() const
{
	MV1SetMatrix(_modelHandle, _transform.lock()->GetMatrix());
	MV1DrawModel(_modelHandle);
}

void ModelRenderer::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);

	_modelHandle = MV1LoadModel(_modelPath.c_str());
}
