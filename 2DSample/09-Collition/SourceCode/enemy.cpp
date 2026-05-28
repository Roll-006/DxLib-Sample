#include <math.hpp>
#include "transform.h"
#include "graphic.h"
#include "graphic_renderer.h"
#include "animator.h"
#include "aabb.h"
#include "enemy.h"

Enemy::Enemy() : 
	_transform		(Transform()),
	_graphicRenderer(GraphicRenderer(_transform)),
	_animator		(Animator(_transform, _graphicRenderer)),
	_aabb			(AABB(_transform, Vector2(14.0f, 42.0f), Vector2(-3.0f, 11.0f)))
{
	_transform.position = kFirstPosition;
	_transform.scale	= kScale;

	_animator.LoadAnim("../../Assets/Animations/Skeleton/Skeleton_01_White_Idle.png", AnimationClip("Idle", 8, 0.2f, true, false));
	_animator.AttachAnim("Idle");
}

void Enemy::Update()
{
	_animator.Update();
	_aabb.Update();
}

void Enemy::Render() const
{
	_graphicRenderer.Render();
	_aabb.Render();
}
