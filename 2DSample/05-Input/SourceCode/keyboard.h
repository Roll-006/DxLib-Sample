#pragma once

class Keyboard
{
public:
	/// @brief インスタンスを取得
	static Keyboard& GetInstance()
	{
		// ローカル静的オブジェクトにより実装
		// ※オブジェクトの解放タイミングを任意に決めることはできない
		static Keyboard instance;
		return instance;
	}

	void Update();

	/// <summary>
	/// 長押ししているかを判定
	/// </summary>
	/// <param name="keyCode">キーコード</param>
	/// <returns>true : 長押ししている, false : 長押ししていない</returns>
	bool IsPressed(int keyCode) const { return _keyState.at(keyCode) >= 2; }

	/// <summary>
	/// このフレームで入力したかを判定
	/// </summary>
	/// <param name="keyCode">キーコード</param>
	/// <returns>true : このフレームでした, false : 入力していない</returns>
	bool WasPressedThisFrame(int keyCode) const { return _keyState.at(keyCode) == 1; }

	/// <summary>
	/// このフレームで離されたかを判定
	/// </summary>
	/// <param name="keyCode">キーコード</param>
	/// <returns>true : このフレームで離された, false : その他</returns>
	bool WasReleasedThisFrame(int keyCode) const { return _keyState.at(keyCode) == -1; }

	/// <summary>
	/// 入力されてからの回数を取得
	/// MEMO : 一度入力されたら次0になることはない
	/// </summary>
	/// <param name="keyCode">キーコード</param>
	/// <returns>0 : 入力なし, 1以上 : 入力されてからの回数, -1以下 : 押されて離されてからの回数</returns>
	int GetInputCount(int keyCode) const { return _keyState.at(keyCode); }

private:
	Keyboard();
	~Keyboard();

private:
	std::array<int, 256> _keyState;
};
