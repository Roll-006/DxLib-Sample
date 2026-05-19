#pragma once

class GraphicRenderer
{
public:
	GraphicRenderer(Transform& transform);

	void Draw() const;

	bool IsFlipX() const { return _isFlipX; }
	bool IsFlipY() const { return _isFlipY; }

	/// <summary>
	/// 描画する画像を設定
	/// </summary>
	/// <param name="graphic">描画する画像のshared_ptr</param>
	void SetGraphic(const std::shared_ptr<Graphic>& graphic) { _graphic = graphic; }

	void SetFlipX(const bool isFlipX) { _isFlipX = isFlipX; }
	void SetFlipY(const bool isFlipY) { _isFlipY = isFlipY; }

private:
	std::weak_ptr<Graphic> _graphic;
	bool _isFlipX;
	bool _isFlipY;
	Transform& _transform;
};
