#pragma once

namespace Input
{
	enum class CursorLockModeType
	{
		kNone,		// カーソルの動作を変更しない
		kLocked,	// カーソルを画面中央に固定する
		kConfined,	// カーソルを表示する
	};
}