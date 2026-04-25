#pragma once

/// <summary>
/// Axis-Aligned Bounding Box : 軸平行境界ボックス
/// オブジェクトの回転を無視して常にWorldのXY軸に平行なボックスです。
/// </summary>
class AABB
{
public:
	AABB(Transform& transform, const Vector2& size, const Vector2& offset = { 0.0f, 0.0f });

	void Update();
	void Draw() const;

	Vector2 GetCenter()		const { return _center; }
	Vector2 GetMax()		const { return _max; }
	Vector2 GetMin()		const { return _min; }
	Vector2 GetSize()		const { return _size * _transform.scale; }
	Vector2 GetExtents()	const { return _extents * _transform.scale; }

private:
	void UpdateCollisionInfo();

private:
	Vector2 _center;
	Vector2 _max;
	Vector2 _min;
	Vector2 _size;
	Vector2 _extents;
	Vector2 _offset;
	Transform& _transform;
};
