#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace Animations
{
    struct AnimationParameters
    {
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, bool> booleans;
        std::unordered_map<std::string, int> integers;
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, AnimationParameters& animationParameters)
    {
        json.at("floats").      get_to(animationParameters.floats);
        json.at("booleans").    get_to(animationParameters.booleans);
        json.at("integers").    get_to(animationParameters.integers);
    }

    inline void to_json(nlohmann::json& json, const AnimationParameters& animationParameters)
    {
        json =
        {
            { "floats",     animationParameters.floats },
            { "booleans",   animationParameters.booleans },
            { "integers",   animationParameters.integers },
        };
    }
    #pragma endregion
}