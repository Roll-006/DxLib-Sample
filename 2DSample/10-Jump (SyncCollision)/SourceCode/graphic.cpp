#include <math.hpp>
#include "graphic.h"

Graphic::Graphic(const std::string& graphicPath) : 
	_graphicHandle	(LoadGraph(graphicPath.c_str())),
	_graphicSize	(Vector2::Zero)
{
	GetGraphSizeF(_graphicHandle, &_graphicSize.x, &_graphicSize.y);
}

Graphic::Graphic(const int graphicHandle) :
	_graphicHandle	(graphicHandle),
	_graphicSize	(Vector2::Zero)
{
	GetGraphSizeF(_graphicHandle, &_graphicSize.x, &_graphicSize.y);
}

Graphic::~Graphic()
{
	if (_graphicHandle != -1)
	{
		DeleteGraph(_graphicHandle);
	}
}
