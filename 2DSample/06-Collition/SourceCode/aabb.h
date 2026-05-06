#pragma once

/// <summary>
/// Axis-Aligned Bounding Box : 軸平行境界ボックス
/// オブジェクトの回転を無視して常にWorldのXY軸に平行なボックスです。
/// </summary>
class AABB
{
public:
	AABB(Transform& transform, const math::Vector2& size, const math::Vector2& offset = { 0.0f, 0.0f });

	void Update();
	void Draw() const;

	math::Vector2 GetCenter()	const { return _center; }
	math::Vector2 GetMax()		const { return _max; }
	math::Vector2 GetMin()		const { return _min; }
	math::Vector2 GetSize()		const { return math::Vector2::StoreFromSIMD(_size.LoadToSIMD() * _transform.scale.LoadToSIMD()); }
	math::Vector2 GetExtents()	const { return math::Vector2::StoreFromSIMD(_extents.LoadToSIMD() * _transform.scale.LoadToSIMD()); }

private:
	void UpdateCollisionInfo();

private:
	math::Vector2 _center;
	math::Vector2 _max;
	math::Vector2 _min;
	math::Vector2 _size;
	math::Vector2 _extents;
	math::Vector2 _offset;
	Transform& _transform;
};
