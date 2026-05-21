#pragma once

class Player
{
public:
	Player();

	void Update();
	void Render() const;

private:
	static constexpr Vector3		kScale	= { 0.1f, 0.1f, 0.1f };
	static constexpr float			kSpeed	= 500.0f;
	static constexpr int			kRadius	= 100;
	static constexpr unsigned int	kColor	= 0xffffff;

	Transform		_transform;
	Model			_model;
	ModelRenderer	_modelRenderer;
};
