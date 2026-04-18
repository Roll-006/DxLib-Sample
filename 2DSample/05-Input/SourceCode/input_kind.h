#pragma once

enum class InputStateKind
{
	kNone,		// 入力なし
	kSingle,	// 単押し
	kHold,		// 長押し
	kPrev,		// 1フレーム前に単押し
};
