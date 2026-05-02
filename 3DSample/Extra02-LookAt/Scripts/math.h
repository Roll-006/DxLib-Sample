#pragma once
#include <DxLib.h>

namespace math
{
	static constexpr float kDeg2Rad = DX_PI_F / 180.0f;		// ディグリーをラジアンに変換 (変換対象と掛け算を行う)
	static constexpr float kRad2Deg = 180.0f / DX_PI_F;		// ラジアンをディグリーに変換 (変換対象と掛け算を行う)
}