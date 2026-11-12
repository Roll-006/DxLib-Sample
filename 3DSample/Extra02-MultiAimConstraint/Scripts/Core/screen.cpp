#include <math.hpp>
#include "screen.h"
#include "../Input/keyboard.h"

using namespace Core;

Screen::Screen():
    _screenSize             (kFullHD),
    _isFullScreen		    (true),
    _changeScreenModeCount  (_isFullScreen ? 1 : 0)
{
#ifdef _DEBUG
    _isFullScreen = false;
    _changeScreenModeCount = 0;
#endif

    SetResolution(_screenSize, _isFullScreen);
}

void Screen::UpdateScreenMode()
{
    const auto keyboard = Input::Keyboard::I();
    
    // [ALT + ENTER]
    if (keyboard.IsPressed(KEY_INPUT_LALT) && keyboard.WasPressedThisFrame(KEY_INPUT_RETURN))
    {
        ++_changeScreenModeCount;
        _isFullScreen = _changeScreenModeCount % 2 == 0;
        ChangeWindowMode(_isFullScreen ? FALSE : TRUE);
    }
}

void Screen::SetResolution(const Vector2& screenSize, bool isFullScreen)
{
    _screenSize = screenSize;
    SetGraphMode(static_cast<int>(_screenSize.x), static_cast<int>(_screenSize.y), kColorBitDepth);
    
    ++_changeScreenModeCount;
    _isFullScreen = isFullScreen;
    ChangeWindowMode(_isFullScreen ? FALSE : TRUE);
}