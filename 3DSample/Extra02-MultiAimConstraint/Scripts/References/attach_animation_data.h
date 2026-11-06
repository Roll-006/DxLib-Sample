#pragma once
#include <string>

struct AttachAnimationData
{
    std::string animationName;
    float currentTime = 0.f;
    int attachIndex = -1;
};