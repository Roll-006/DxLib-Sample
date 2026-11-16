#pragma once

namespace DxLibSample::Input
{
	enum class CursorLockModeType : uint8_t
	{
		kNone,		// カーソルの動作を変更しない
		kLocked,	// カーソルを画面中央に固定する
		kConfined,	// カーソルを表示する
	};
}