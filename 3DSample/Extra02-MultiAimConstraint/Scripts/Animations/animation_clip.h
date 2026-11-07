#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "DxLib.h"

namespace Animations
{
    struct AnimationClip
    {
        int animationHandle = -1;
        bool isLooping;
        float speed;
        float totalTime;
        int animationIndex = -1;
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, AnimationClip& animationClip)
    {
        auto path = std::string();
    
        json.at("animationPath").       get_to(path);
        json.at("isLooping").           get_to(animationClip.isLooping);
        json.at("speed").               get_to(animationClip.speed);
        json.at("animationIndex").      get_to(animationClip.animationIndex);
    
        animationClip.animationHandle = MV1LoadModel(path.c_str());
    }

    inline void to_json(nlohmann::json& json, const AnimationClip& animationClip)
    {
        json =
        {
            { "isLooping",          animationClip.isLooping },
            { "speed",              animationClip.speed },
            { "animationIndex",     animationClip.animationIndex },
        };
    }
    #pragma endregion
}