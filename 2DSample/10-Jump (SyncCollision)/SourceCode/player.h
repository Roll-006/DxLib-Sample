#pragma once

class Player
{
public:
	Player();

	void Update();
	void Render() const;

	AABB GetCollider() const { return _aabb; }

private:
	/// <summary>
	/// 重力を適用する
	/// </summary>
	void ApplyGravity();

	/// <summary>
	/// 地面に着地したかの状態を更新する
	/// </summary>
	void UpdateGroundedState();

	void Move();
	void Jump();

	void UpdatePosition();

private:
	static constexpr Vector2		kFirstPosition	= { 500.0f, 500.0f };
	static constexpr Vector2		kScale			= { 10.0f, 10.0f };
	static constexpr float			kSpeed			= 500.0f;
	static constexpr int			kRadius			= 100;
	static constexpr unsigned int	kColor			= 0xffffff;
	static constexpr float			kJumpPower		= -1000.0f;

	bool			_isGrounded;
	Vector2			_moveVelocity;
	Vector2			_fallVelocity;
	Transform		_transform;
	GraphicRenderer _graphicRenderer;
	Animator		_animator;
	AABB			_aabb;
};
