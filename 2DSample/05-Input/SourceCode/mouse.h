#pragma once

class Mouse
{
public:
	/// @brief インスタンスを取得
	static Mouse& GetInstance()
	{
		// ローカル静的オブジェクトにより実装
		// ※オブジェクトの解放タイミングを任意に決めることはできない
		static Mouse instance;
		return instance;
	}

	void Update();

	/// <summary>
	/// 長押ししているかを判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>true : 長押ししている, false : 長押ししていない</returns>
	//bool IsPressed(int mouseButton) const;

	/// <summary>
	/// このフレームで入力したかを判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>true : このフレームでした, false : 入力していない</returns>
	//bool WasPressedThisFrame(int mouseButton) const;

	/// <summary>
	/// このフレームで離されたかを判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>true : このフレームで離された, false : その他</returns>
	//bool WasReleasedThisFrame(int mouseButton) const;

	/// <summary>
	/// 入力されてからの回数を取得
	/// MEMO : 一度入力されたら次0になることはない
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>0 : 入力なし, 1以上 : 入力されてからの回数, -1以下 : 押されて離されてからの回数</returns>
	//int GetInputCount(int mouseButton) const;

	/// <summary>
	/// スクリーン上のマウス座標を取得
	/// </summary>
	/// <returns>マウス座標</returns>
	Vector2Int GetMousePos() const { return _currentMousePos; }

	/// <summary>
	/// マウスの移動量を取得
	/// </summary>
	/// <returns>マウスの移動量</returns>
	Vector2Int GetMouseDelta() const { return _mouseDelta; }

private:
	Mouse();
	~Mouse();

private:
	Vector2Int _currentMousePos;
	Vector2Int _prevMousePos;
	Vector2Int _mouseDelta;
};
