#pragma once
#include "input_state_kind.h"

class Input
{
public:
	/// @brief インスタンスを取得
	static Input& GetInstance()
	{
		// ローカル静的オブジェクトにより実装
		// ※オブジェクトの解放タイミングを任意に決めることはできない
		static Input instance;
		return instance;
	}

	void Update();

	/// <summary>
	/// 入力の状態を取得
	/// </summary>
	/// <param name="keyCode">キーコード</param>
	/// <returns>入力の状態</returns>
	InputStateKind GetKeyState(int keyCode) const;

	/// <summary>
	/// 入力されてからの回数を取得
	/// MEMO : 一度入力されたら次0になることはない
	/// </summary>
	/// <param name="keyCode">キーコード</param>
	/// <returns>0 : 入力なし, 1以上 : 入力されてからの回数, -1以下 : 押されて離されてからの回数</returns>
	int GetKeyCount(int keyCode) const { return _keyState.at(keyCode); }

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
	Input();
	~Input();

private:
	std::array<int, 256> _keyState;
	Vector2Int	_currentMousePos;
	Vector2Int	_prevMousePos;
	Vector2Int	_mouseDelta;
};
