#include <array>
#include <Math/math.hpp>
#include "mouse.h"

Mouse::Mouse() :
	_state			(0),
	_currentPos		(0.0f, 0.0f),
	_prevPos		(0.0f, 0.0f),
	_delta			(0.0f, 0.0f),
	_currentScroll	(0.0f, 0.0f),
	_prevScroll		(0.0f, 0.0f),
	_scroll			(0.0f, 0.0f)
{
	// MOUSE_INPUT_6～MOUSE_INPUT_8を使用する場合は、下記の関数を実行する必要があります
	// SetUseDirectInputFlag(TRUE);

	UpdateMousePos();
	UpdateScroll();
}

void Mouse::Update()
{
	UpdateMouseButtonState();
	UpdateMousePos();
	UpdateScroll();
}

int Mouse::GetIndex(const int mouseButton) const
{
	for (int i = 0; i < _state.size(); ++i)
	{
		if (mouseButton == (1 << i)) { return i; }
	}

	return -1;
}

void Mouse::UpdateMouseButtonState()
{
	// GetHitKeyStateAllEx関数の仕様に合わせて実装

	const auto inputState = GetMouseInput();

	for (int i = 0; i < _state.size(); ++i)
	{
		auto& state = _state.at(i);

		// 入力判定
		// MEMO : ボタンの定義にはビットが利用されており(MOUSE_INPUT_1 : 0000 0001, MOUSE_INPUT_5 : 0001 0000)、
		//        [1 << i] はループのたびに [0000 0001] をiビット桁移動させてボタンを表しています。
		//        ビットについて [https://www.sejuku.net/blog/24512]
		const auto wasPressedThisFrame = inputState & (1 << i);

		// 入力されたことがなくこのフレームでも入力されなかった場合、次の要素へ移動
		if (state == 0 && !wasPressedThisFrame) { continue; }

		// 入力があった場合は値を加算
		if (wasPressedThisFrame)
		{
			// このフレームで入力した場合は強制的に1にする
			if (state <= 0)
			{
				state = 1;
			}
			else
			{
				++state;
			}
		}
		// 入力がなかった場合は値を減算
		else
		{
			// このフレームで離された場合は強制的に-1にする
			if (state >= 0)
			{
				state = -1;
			}
			else
			{
				--state;
			}
		}
	}
}

void Mouse::UpdateMousePos()
{
	// データを保存
	_prevPos = _currentPos;

	// 現在の座標を更新
	int x, y;
	GetMousePoint(&x, &y);
	_currentPos = math::Vector2(x, y);

	// 移動量を計算
	_delta = math::Vector2::StoreFromSIMD(DirectX::XMVectorSubtract(_currentPos.LoadToSIMD(), _prevPos.LoadToSIMD()));
}

void Mouse::UpdateScroll()
{
	// データを保存
	_prevScroll = _currentScroll;

	// マウスホイールの移動量を取得
	_currentScroll = math::Vector2::StoreFromSIMD(DirectX::XMVectorAdd(_currentScroll.LoadToSIMD(), DirectX::XMVectorSet(GetMouseHWheelRotVolF(), GetMouseWheelRotVolF(), 0.0f, 0.0f)));

	// 移動量を計算
	_scroll = math::Vector2::StoreFromSIMD(DirectX::XMVectorSubtract(_currentScroll.LoadToSIMD(), _prevScroll.LoadToSIMD()));
}
