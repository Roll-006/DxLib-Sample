#pragma once

class Mouse final
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
	bool IsPressed(const int mouseButton) const { return _state.at(GetIndex(mouseButton)) >= 2; }

	/// <summary>
	/// このフレームで入力したかを判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>true : このフレームでした, false : 入力していない</returns>
	bool WasPressedThisFrame(const int mouseButton) const { return _state.at(GetIndex(mouseButton)) == 1; }

	/// <summary>
	/// このフレームで離されたかを判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>true : このフレームで離された, false : その他</returns>
	bool WasReleasedThisFrame(const int mouseButton) const { return _state.at(GetIndex(mouseButton)) == -1; }

	/// <summary>
	/// 入力されてからの回数を取得
	/// MEMO : 一度入力されたら次0になることはない
	/// </summary>
	/// <param name="mouseButton">マウスボタン</param>
	/// <returns>0 : 入力なし, 1以上 : 入力されてからの回数, -1以下 : 押されて離されてからの回数</returns>
	int GetInputCount(const int mouseButton) const { return _state.at(GetIndex(mouseButton)); }

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
	int GetIndex(const int mouseButton) const;

	/// <summary>
	/// マウスボタンの状態を更新する
	/// </summary>
	void UpdateMouseButtonState();

private:
	std::array<int, 8> _state;		// マウスボタンの状態を格納
	Vector2Int	_currentPos;		// 現在の座標
	Vector2Int	_prevPos;			// 以前の座標
	Vector2Int	_delta;				// 座標の移動量
	Vector2		_currentScroll;		// 現在のホイール
	Vector2		_prevScroll;		// 以前のホイール
	Vector2		_scroll;			// ホイールの移動量
};
