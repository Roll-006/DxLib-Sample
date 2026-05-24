#pragma once

class Gamepad
{
public:
	/// @brief インスタンスを取得
	static Gamepad& GetInstance()
	{
		// ローカル静的オブジェクトにより実装
		// ※オブジェクトの解放タイミングを任意に決めることはできない
		static Gamepad instance;
		return instance;
	}

	void Update();

	/// <summary>
	/// 長押ししているかを判定
	/// </summary>
	/// <param name="gamepadIndex">ゲームパッドの接続番号</param>
	/// <param name="gamepadButton">ゲームパッドボタン</param>
	/// <returns>true : 長押ししている, false : 長押ししていない</returns>
	bool IsPressed(const int gamepadIndex, const int gamepadButton) const;

	/// <summary>
	/// このフレームで入力したかを判定
	/// </summary>
	/// <param name="gamepadIndex">ゲームパッドの接続番号</param>
	/// <param name="gamepadButton">ゲームパッドボタン</param>
	/// <returns>true : このフレームでした, false : 入力していない</returns>
	bool WasPressedThisFrame(const int gamepadIndex, const int gamepadButton) const;

	/// <summary>
	/// このフレームで離されたかを判定
	/// </summary>
	/// <param name="gamepadIndex">ゲームパッドの接続番号</param>
	/// <param name="gamepadButton">ゲームパッドボタン</param>
	/// <returns>true : このフレームで離された, false : その他</returns>
	bool WasReleasedThisFrame(const int gamepadIndex, const int gamepadButton) const;

	/// <summary>
	/// 入力されてからの回数を取得
	/// MEMO : 一度入力されたら次0になることはない
	/// </summary>
	/// <param name="gamepadIndex">ゲームパッドの接続番号</param>
	/// <param name="gamepadButton">ゲームパッドボタン</param>
	/// <returns>0 : 入力なし, 1以上 : 入力されてからの回数, -1以下 : 押されて離されてからの回数</returns>
	int GetInputCount(const int gamepadIndex, const int gamepadButton) const;

	/// <summary>
	/// 接続されているゲームパッドの数を取得
	/// </summary>
	/// <returns>接続されているゲームパッドの数</returns>
	int GetGamepadNum() const { return _gamepadNum; }

private:
	Gamepad();

	void UpdateGamepadNum();

	/// <summary>
	/// XInputの状態を更新する
	/// </summary>
	void UpdateXInputState();

private:
	int _gamepadNum;
	std::vector<std::array<int, 16>> _states;
	std::vector<XINPUT_STATE> _xInput;
};
