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
        json.at("weight").          get_to(sourceObject.weight);
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