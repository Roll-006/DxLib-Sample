#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

namespace DxLibSample
{
    class Transform;
}

namespace DxLibSample::Animations::Rigging
{
    struct SourceObject
    {
        std::string sourceObjectName;
        std::weak_ptr<Transform> sourceObject;
        float weight;
    };
    
    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, SourceObject& sourceObject)
    {
        json.at("sourceObjectName").get_to(sourceObject.sourceObjectName);
        sourceObject.weight = std::clamp(json.at("weight").get<float>(), 0.f, 1.f);
    }

    inline void to_json(nlohmann::json& json, const SourceObject& sourceObject)
    {
        json =
        {
            { "sourceObjectName",   sourceObject.sourceObjectName },
            { "weight",		        sourceObject.weight },
        };
    }
    #pragma endregion
}