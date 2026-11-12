#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace DxLibSample::Animations
{
    enum class FloatTransitionType
    {
        kGreater,
        kLess,
    };

    struct FloatTransitionData
    {
        FloatTransitionType type;
        float value;
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, FloatTransitionData& floatTransitionData)
    {
        json.at("type").    get_to(floatTransitionData.type);
        json.at("value").   get_to(floatTransitionData.value);
    }

    inline void to_json(nlohmann::json& json, const FloatTransitionData& floatTransitionData)
    {
        json =
        {
            { "type",   floatTransitionData.type },
            { "value",  floatTransitionData.value },
        };
    }
    #pragma endregion


    enum class BooleanTransitionType
    {
        kFalse,
        kTrue,
    };

    struct BooleanTransitionData
    {
        BooleanTransitionType type;
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, BooleanTransitionData& booleanTransitionData)
    {
        json.at("type").get_to(booleanTransitionData.type);
    }

    inline void to_json(nlohmann::json& json, const BooleanTransitionData& booleanTransitionData)
    {
        json =
        {
            { "type", booleanTransitionData.type },
        };
    }
    #pragma endregion


    enum class IntegerTransitionType
    {
        kGreater,
        kLess,
        kEquals,
        kNotEqual,
    };

    struct IntegerTransitionData
    {
        IntegerTransitionType type;
        int value;
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, IntegerTransitionData& integerTransitionData)
    {
        json.at("type").    get_to(integerTransitionData.type);
        json.at("value").   get_to(integerTransitionData.value);
    }

    inline void to_json(nlohmann::json& json, const IntegerTransitionData& integerTransitionData)
    {
        json =
        {
            { "type",   integerTransitionData.type },
            { "value",  integerTransitionData.value },
        };
    }
    #pragma endregion
    
    
    struct AnimationTransition
    {
        std::string originAnimationName;
        std::string targetAnimationName;
        float transitionDuration;
        std::unordered_map<std::string, FloatTransitionData> floats;
        std::unordered_map<std::string, BooleanTransitionData> booleans;
        std::unordered_map<std::string, IntegerTransitionData> integers;
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, AnimationTransition& animationTransition)
    {
        json.at("originAnimationName"). get_to(animationTransition.originAnimationName);
        json.at("targetAnimationName"). get_to(animationTransition.targetAnimationName);
        json.at("transitionDuration").  get_to(animationTransition.transitionDuration);
        json.at("floats").              get_to(animationTransition.floats);
        json.at("booleans").            get_to(animationTransition.booleans);
        json.at("integers").            get_to(animationTransition.integers);
    }

    inline void to_json(nlohmann::json& json, const AnimationTransition& animationTransition)
    {
        json =
        {
            { "originAnimationName",    animationTransition.originAnimationName },
            { "targetAnimationName",    animationTransition.targetAnimationName },
            { "transitionDuration",     animationTransition.transitionDuration },
            { "floats",                 animationTransition.floats },
            { "booleans",               animationTransition.booleans },
            { "integers",               animationTransition.integers },
        };
    }
    #pragma endregion
}