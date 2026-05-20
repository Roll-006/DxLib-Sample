#include <math.hpp>
#include "window.h"
#include "mouse.h"

Mouse::Mouse() :
	_state			(0),
	_lockState		(CursorLockModeType::kNone),
	_currentPos		(Vector2::Zero),
	_prevPos		(Vector2::Zero),
	_delta			(Vector2::Zero),
	_currentScroll	(Vector2::Zero),
	_prevScroll		(Vector2::Zero),
	_scroll			(Vector2::Zero)
{
	// MOUSE_INPUT_6～MOUSE_INPUT_8を使用する場合は、下記の関数を実行する必要があります
	// SetUseDirectInputFlag(TRUE);

	SetCursorLockState(_lockState);

	UpdateMousePos();
	UpdateScroll();
}

void Mouse::Update()
{
	UpdateMouseButtonState();
	UpdateMousePos();
	UpdateScroll();
}

void Mouse::SetCursorLockState(const CursorLockModeType lockState)
{
	_lockState = lockState;

	switch (_lockState)
	{
	case CursorLockModeType::kNone:
	case CursorLockModeType::kLocked:
		SetMouseDispFlag(FALSE);
		break;

	case CursorLockModeType::kConfined:
		SetMouseDispFlag(TRUE);
		break;

	default:
		_lockState = CursorLockModeType::kNone;
		SetMouseDispFlag(FALSE);
		break;
	}
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

	for (size_t i = 0; i < _state.size(); ++i)
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
	_currentPos = Vector2(static_cast<float>(x), static_cast<float>(y));

	// 移動量を計算
	_delta = _currentPos - _prevPos;

	// カーソル固定中は画面中央に設定
	if (_lockState == CursorLockModeType::kLocked)
	{
		_currentPos = window::kSize * 0.5f;
		SetMousePoint(static_cast<float>(_currentPos.x), static_cast<float>(_currentPos.y));
	}
}

void Mouse::UpdateScroll()
{
	// データを保存
	_prevScroll = _currentScroll;

	// マウスホイールの移動量を取得
	_currentScroll += Vector2(GetMouseHWheelRotVolF(), GetMouseWheelRotVolF());

	// 移動量を計算
	_scroll = _currentScroll - _prevScroll;
}
