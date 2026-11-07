#pragma once
#include <string>

namespace Animations
{
    struct AttachAnimationData
    {
        std::string animationName;
        float currentTime = 0.f;
        int attachIndex = -1;
    };
}