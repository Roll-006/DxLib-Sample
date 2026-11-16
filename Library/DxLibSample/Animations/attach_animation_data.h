#pragma once
#include <string>

namespace DxLibSample::Animations
{
    struct AttachAnimationData
    {
        std::string animationName;
        int attachIndex = -1;
        float currentTime = 0.f;
        float totalTime = 0.f;
        float playRate = 0.f;
    };
}