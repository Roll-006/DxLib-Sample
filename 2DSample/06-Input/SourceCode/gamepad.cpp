#include <array>
#include <vector>
#include <DxLib.h>
#include "gamepad.h"

Gamepad::Gamepad() :
	_gamepadNum	(0),
	_xInput		(),
	_states		()
{

}

void Gamepad::Update()
{
	UpdateGamepadNum();
	UpdateXInputState();
}

bool Gamepad::IsPressed(const int gamepadIndex, const int gamepadButton) const
{
	if (gamepadIndex >= _states.size()) { return false; }

	return _states.at(gamepadIndex).at(gamepadButton) >= 2;
}

bool Gamepad::WasPressedThisFrame(const int gamepadIndex, const int gamepadButton) const
{
	if (gamepadIndex >= _states.size()) { return false; }

	return _states.at(gamepadIndex).at(gamepadButton) == 1;
}

bool Gamepad::WasReleasedThisFrame(const int gamepadIndex, const int gamepadButton) const
{
	if (gamepadIndex >= _states.size()) { return false; }

	return _states.at(gamepadIndex).at(gamepadButton) == -1;
}

int Gamepad::GetInputCount(const int gamepadIndex, const int gamepadButton) const
{
	if (gamepadIndex >= _states.size()) { return false; }

	return _states.at(gamepadIndex).at(gamepadButton);
}

void Gamepad::UpdateGamepadNum()
{
	const auto gamepadNum = GetJoypadNum();

	if (_gamepadNum == gamepadNum) { return; }
	
	_gamepadNum = gamepadNum;
	_xInput.resize(_gamepadNum);
	_states.resize(_gamepadNum);
}

void Gamepad::UpdateXInputState()
{
	for (size_t i = 0; i < _xInput.size(); ++i)
	{
		GetJoypadXInputState(DX_INPUT_PAD1 + i, &_xInput.at(i));

		for (size_t j = 0; j < _states.at(i).size(); ++j)
		{
			auto& state = _states.at(i).at(j);
			
			// 入力判定
			const auto wasPressedThisFrame = _xInput.at(i).Buttons[j];

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
}
