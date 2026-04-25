#include <DxLib.h>
#include <Vector/vector2.hpp>
#include "debug.h"
#include "transform.h"
#include "aabb.h"

AABB::AABB(Transform& transform, const Vector2& size, const Vector2& offset) :
	_transform	(transform),
	_center		(0.0f, 0.0f),
	_max		(0.0f, 0.0f),
	_min		(0.0f, 0.0f),
	_size		(size),
	_extents	(size / 2),
	_offset		(offset)
{
	UpdateCollisionInfo();
}

void AABB::Update()
{
	UpdateCollisionInfo();
}

void AABB::Draw() const
{
	DrawBox(
		static_cast<int>(_min.x),
		static_cast<int>(_min.y),
		static_cast<int>(_max.x), 
		static_cast<int>(_max.y), 
		debug::kColliderColor,
		FALSE, 
		debug::kColliderLineThickness);
}

void AABB::UpdateCollisionInfo()
{
	_center = _transform.position + _offset * _transform.scale;
	_max	= _center + GetExtents();
	_min	= _center - GetExtents();
}
