#pragma once
#include <string>

namespace DxLibSample::Animations
{
    struct AttachAnimationData
    {
        std::string animationName;
        float currentTime = 0.f;
        int attachIndex = -1;
    };
}