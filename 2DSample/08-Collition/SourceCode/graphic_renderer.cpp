#include <math.hpp>
#include "transform.h"
#include "graphic.h"
#include "graphic_renderer.h"

GraphicRenderer::GraphicRenderer(Transform& transform) : 
	_graphic	(),
	_isFlipX	(false),
	_isFlipY	(false),
	_transform	(transform)
{

}

void GraphicRenderer::Draw() const
{
	const auto graphic = _graphic.lock();
	if (!graphic) { return; }

	DrawRotaGraph3(
		static_cast<int>(_transform.position.x),
		static_cast<int>(_transform.position.y),
		static_cast<int>(graphic->GetGraphicSize().x * 0.5f),
		static_cast<int>(graphic->GetGraphicSize().y * 0.5f),
		static_cast<double>(_transform.scale.x),
		static_cast<double>(_transform.scale.y),
		static_cast<double>(_transform.rotation),
		graphic->GetGraphicHandle(),
		TRUE,
		_isFlipX,
		_isFlipY);
}
