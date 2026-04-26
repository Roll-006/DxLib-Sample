#pragma once

/// <summary>
/// オブジェクトの座標、回転、スケールを保持している。
/// 親子関係は考慮していない。
/// </summary>
class Transform
{
public:
	Transform();

	/// <summary>
	/// 対象の方向を見る
	/// </summary>
	/// <param name="target">注視する対象</param>
	/// <param name="up">upベクトル</param>
	void LookAt(const Vector3& target, const Vector3& up = Vector3::GetUp());

	Vector3		GetPosition()	const { return _position; }
	Vector3		GetRotation()	const { return _rotation; }
	Vector3		GetScale()		const { return _scale; }
	Matrix4x4	GetMatrix();
	Vector3		GetRight();
	Vector3		GetUp();
	Vector3		GetForward();

	void SetPosition(const Vector3& position)	{ _position = position;					_isDirty = true; }
	void SetRotation(const Vector3& rotation)	{ _rotation = rotation;					_isDirty = true; }
	void SetScale	(const Vector3& scale)		{ _scale	= scale;					_isDirty = true; }
	void SetScale	(const float scale)			{ _scale	= { scale, scale, scale };	_isDirty = true; }
	
private:
	void UpdateMatrix();

private:
	Vector3		_position;
	Vector3		_rotation;
	Vector3		_scale;
	Matrix4x4	_matrix;
	bool		_isDirty;
};
