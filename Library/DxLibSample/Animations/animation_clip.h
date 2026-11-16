#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "DxLib.h"

namespace DxLibSample::Animations
{
    struct AnimationClip
    {
        int animationHandle = -1;
        bool isLooping;
        bool isBakeIntoPoseY;
        bool isBakeIntoPoseXZ;
        float speed;
        int animationIndex = -1;
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, AnimationClip& animationClip)
    {
        auto path = std::string();
    
        json.at("animationPath").       get_to(path);
        json.at("isLooping").           get_to(animationClip.isLooping);
        json.at("isBakeIntoPoseY").     get_to(animationClip.isBakeIntoPoseY);
        json.at("isBakeIntoPoseXZ").    get_to(animationClip.isBakeIntoPoseXZ);
        json.at("speed").               get_to(animationClip.speed);
        json.at("animationIndex").      get_to(animationClip.animationIndex);
    
        animationClip.animationHandle = MV1LoadModel(path.c_str());
    }

    inline void to_json(nlohmann::json& json, const AnimationClip& animationClip)
    {
        json =
        {
            { "isLooping",          animationClip.isLooping },
            { "isBakeIntoPoseY",    animationClip.isBakeIntoPoseY },
            { "isBakeIntoPoseXZ",   animationClip.isBakeIntoPoseXZ },
            { "speed",              animationClip.speed },
            { "animationIndex",     animationClip.animationIndex },
        };
    }
    #pragma endregion
}