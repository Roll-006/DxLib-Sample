#pragma once

class Player
{
public:
	void Update();
	void Render() const;

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
	static constexpr float			kSpeed			= 500.0f;
	static constexpr int			kRadius			= 50;
	static constexpr unsigned int	kColor			= 0xffffff;
	static constexpr float			kJumpPower		= -1000.0f;
<<<<<<< HEAD:2DSample/05-1-Jump/SourceCode/player.h
=======
	static constexpr int			kMaxJumpNum		= 2;
>>>>>>> f6fd91f0c5688c20eac593b66c33923a445441b1:2DSample/09-2-Jump (DoubleJump)/SourceCode/player.h

	bool	_isGrounded = false;
	Vector2	_moveVelocity;
	Vector2	_fallVelocity;
	Vector2 _position = kFirstPosition;
};
