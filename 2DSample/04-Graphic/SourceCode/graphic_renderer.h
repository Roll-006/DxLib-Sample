#pragma once

class GraphicRenderer
{
public:
	GraphicRenderer(Transform& transform, const std::string& graphicPath);

	void Draw() const;

	int GetGraphicHandle() const { return _graphicHandle; }
	Vector2 GetOriginGraphicSize() const { return _originGraphicSize; }
	Vector2 GetGraphicSize() const { return _originGraphicSize * _transform.scale; }

private:
	int _graphicHandle;
	Vector2 _originGraphicSize;
	Transform& _transform;
};
