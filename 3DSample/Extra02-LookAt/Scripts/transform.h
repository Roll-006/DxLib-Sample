#pragma once
#include "component.h"

class GameObject;

/// <summary>
/// オブジェクトの座標、回転、スケールを保持している。
/// また、親子関係も保持している。
/// </summary>
class Transform final : public Component
{
public:
	Transform();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const	override;

	void Deserialize(const nlohmann::json& json) override;

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

	friend void from_json	(const nlohmann::json& json, Transform& transform);
	friend void to_json		(nlohmann::json& json, const Transform& transform);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, Transform& transform)
{
	json.at("enabled").	get_to(transform._enabled);
	json.at("position").get_to(transform._position);
	json.at("rotation").get_to(transform._rotation);
	json.at("scale").	get_to(transform._scale);
}

inline void to_json(nlohmann::json& json, const Transform& transform)
{
	json =
	{
		{ "enabled",	transform._enabled },
		{ "position",	transform._position },
		{ "rotation",	transform._rotation },
		{ "scale",		transform._scale },
	};
}
#pragma endregion
