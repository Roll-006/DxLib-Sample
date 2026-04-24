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
	bool IsPressed(uint8_t mouseButton) const { return _state.at(GetIndex(mouseButton)) >= 2; }

	/// <summary>
	/// このフレームで入力したかを判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>true : このフレームでした, false : 入力していない</returns>
	bool WasPressedThisFrame(uint8_t mouseButton) const { return _state.at(GetIndex(mouseButton)) == 1; }

	/// <summary>
	/// このフレームで離されたかを判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>true : このフレームで離された, false : その他</returns>
	bool WasReleasedThisFrame(uint8_t mouseButton) const { return _state.at(GetIndex(mouseButton)) == -1; }

	/// <summary>
	/// 入力されてからの回数を取得
	/// MEMO : 一度入力されたら次0になることはない
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>0 : 入力なし, 1以上 : 入力されてからの回数, -1以下 : 押されて離されてからの回数</returns>
	int GetInputCount(uint8_t mouseButton) const { return _state.at(GetIndex(mouseButton)); }

	/// <summary>
	/// スクリーン上のマウス座標を取得
	/// </summary>
	/// <returns>マウス座標</returns>
	Vector2Int GetPos() const { return _currentPos; }

	/// <summary>
	/// マウスの移動量を取得
	/// </summary>
	/// <returns>マウスの移動量</returns>
	Vector2Int GetDelta() const { return _delta; }

	/// <summary>
	/// マウスホイールの移動量を取得
	/// </summary>
	/// <returns>マウスホイールの移動量</returns>
	Vector2 GetScroll() const { return _scroll; }

private:
	Mouse();

	/// <summary>
	/// マウスボタンの識別コードからインデックスを取得
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>インデックス</returns>
	int GetIndex(uint8_t mouseButton) const;

private:
	std::array<int, 8> _state;
	Vector2Int _currentPos;
	Vector2Int _prevPos;
	Vector2Int _delta;
	Vector2 _scroll;
};
