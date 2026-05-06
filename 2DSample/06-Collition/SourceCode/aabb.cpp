#include <Math/math.hpp>
#include "debug.h"
#include "transform.h"
#include "aabb.h"

AABB::AABB(Transform& transform, const math::Vector2& size, const math::Vector2& offset) :
	_transform	(transform),
	_center		(0.0f, 0.0f),
	_max		(0.0f, 0.0f),
	_min		(0.0f, 0.0f),
	_size		(size),
	_extents	(math::Vector2::StoreFromSIMD(size.LoadToSIMD() / 2)),
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
	_center = math::Vector2::StoreFromSIMD(_transform.position.LoadToSIMD() + _offset.LoadToSIMD() * _transform.scale.LoadToSIMD());

	const auto extents	= GetExtents().LoadToSIMD();
	const auto center	= _center.LoadToSIMD();

	_max	= math::Vector2::StoreFromSIMD(center + extents);
	_min	= math::Vector2::StoreFromSIMD(center - extents);
}
