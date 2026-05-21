#include <math.hpp>
#include "time.h"
#include "keyboard.h"
#include "transform.h"
#include "model.h"
#include "model_renderer.h"
#include "player.h"

Player::Player() : 
	_transform		(Transform()),
	_model			(Model("../../Assets/Models/Swat/mesh.mv1", _transform)),
	_modelRenderer	(ModelRenderer(_model))
{
	_transform.scale = kScale;
}

void Player::Update()
{
	_model.ApplyMatrix();
}

void Player::Render() const
{
	_modelRenderer.Render();
}
