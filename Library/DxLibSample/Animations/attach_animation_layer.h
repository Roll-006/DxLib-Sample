#pragma once
#include "attach_animation_data.h"

namespace DxLibSample::Animations
{
    struct AttachAnimationLayer
    {
        AttachAnimationData animation;
        float weight;
        Vector3 previousRootPosition;
        float previousTime = 0.f;
        bool hasPreviousRootPosition;
    };
}