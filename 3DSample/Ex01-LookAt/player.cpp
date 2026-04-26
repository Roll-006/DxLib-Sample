#include <DxLib.h>
#include <string>
#include <Vector/vector3.hpp>
#include "transform.h"
#include "modeler.h"
#include "player.h"

Player::Player() : 
	_transform	(Transform()),
	_modeler	(Modeler("Assets/Models/Swat/mesh.mv1", _transform))
{

}

void Player::Update()
{

}

void Player::Draw() const
{

}
