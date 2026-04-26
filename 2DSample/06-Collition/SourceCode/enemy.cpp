#include <DxLib.h>
#include <Vector/vector2.hpp>
#include "transform.h"
#include "animator.h"
#include "aabb.h"
#include "enemy.h"

Enemy::Enemy() : 
	_transform	(Transform()),
	_animator	(Animator(_transform)),
	_aabb		(AABB(_transform, Vector2(14.0f, 42.0f), Vector2(-3.0f, 11.0f)))
{
	_transform.position = kFirstPos;
	_transform.scale	= kScale;

	_animator.LoadAnim(AnimationClip({}, "Idle", "../../Assets/Animations/Skeleton/Skeleton_01_White_Idle.png", Vector2(768, 64), 8, 0.2f, true, false));
	_animator.AttachAnim("Idle");
}

void Enemy::Update()
{
	_animator.Update();
	_aabb.Update();
}

void Enemy::Draw() const
{
	_animator.Draw();
	_aabb.Draw();
}
