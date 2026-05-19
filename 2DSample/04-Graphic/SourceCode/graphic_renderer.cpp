#include <math.hpp>
#include "transform.h"
#include "graphic_renderer.h"

GraphicRenderer::GraphicRenderer(Transform& transform, const std::string& graphicPath) :
	_graphicHandle		(LoadGraph(graphicPath.c_str())),
	_originGraphicSize	(Vector2::Zero),
	_transform			(transform)
{
	GetGraphSizeF(_graphicHandle, &_originGraphicSize.x, &_originGraphicSize.y);
}

void GraphicRenderer::Draw() const
{
	DrawRotaGraph3(
		static_cast<int>(_transform.position.x),
		static_cast<int>(_transform.position.y),
		static_cast<int>(_originGraphicSize.x * 0.5f),
		static_cast<int>(_originGraphicSize.y * 0.5f),
		static_cast<double>(_transform.scale.x),
		static_cast<double>(_transform.scale.y),
		_transform.rotation,
		_graphicHandle,
		TRUE,
		FALSE,
		FALSE);
}
