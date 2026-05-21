#include <math.hpp>
#include "transform.h"
#include "model.h"
#include "model_renderer.h"

ModelRenderer::ModelRenderer(Model& model) :
	_model(model)
{

}

void ModelRenderer::Render() const
{
	MV1DrawModel(_model.GetModelHandle());
}
