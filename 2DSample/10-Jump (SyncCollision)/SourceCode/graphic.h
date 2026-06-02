#pragma once

class Graphic
{
public:
	Graphic(const std::string& graphicPath);
	Graphic(const int graphicHandle);
	~Graphic();

	int GetGraphicHandle()		const { return _graphicHandle; }
	Vector2 GetGraphicSize()	const { return _graphicSize; }

private:
	int		_graphicHandle;
	Vector2 _graphicSize;
};
