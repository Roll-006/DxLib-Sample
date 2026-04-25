#pragma once

class Game
{
public:
	/// <summary>
	/// ゲームループ処理を行う
	/// </summary>
	/// <returns>0 : 初期化成功, その他 : 初期化失敗</returns>
	int Run() const;

private:
	/// <summary>
	/// ゲームの設定を行う
	/// </summary>
	/// <returns>0 : 初期化成功, その他 : 初期化失敗</returns>
	int SetUpDxLib() const;

	/// <summary>
	/// ゲームループを続けるかを判定
	/// </summary>
	/// <returns>true : 継続, false : 終了</returns>
	bool ShouldRun() const;
};
