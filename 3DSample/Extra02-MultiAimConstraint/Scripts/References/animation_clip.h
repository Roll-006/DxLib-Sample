#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "DxLib.h"

struct AnimationClip
{
    int animationHandle = -1;
    bool isLooping;
    float speed;
    float transitionDuration;
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
    json.at("transitionDuration").  get_to(animationClip.transitionDuration);
    json.at("animationIndex").      get_to(animationClip.animationIndex);
    
    animationClip.animationHandle = MV1LoadModel(path.c_str());
}

inline void to_json(nlohmann::json& json, const AnimationClip& animationClip)
{
    json =
    {
        { "isLooping",          animationClip.isLooping },
        { "speed",              animationClip.speed },
        { "transitionDuration", animationClip.transitionDuration },
        { "animationIndex",     animationClip.animationIndex },
    };
}
#pragma endregion