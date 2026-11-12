#pragma once
#include "../../component.h"

namespace DxLibSample
{
    class GameObject;
}

namespace DxLibSample::Animations::Rigging
{
    class BoneRenderer final : public Component
    {
    public:
        BoneRenderer();

        void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
        void Start()		override;
        void Update()       override;
        void LateUpdate()   override;
        void Render() const override;

        void Deserialize(const nlohmann::json& json) override;

    private:
        void DrawBone() const;
        void DrawTripod(const Matrix& mat) const;
        
    private:
        static constexpr float kBoneMinSize = 0.01f;
        static constexpr float kBoneMaxSize = 1.f;
        static constexpr int kMinSmoothness = 1;
        static constexpr int kMaxSmoothness = 5;
        static constexpr float kTripodMinSize = 0.01f;
        static constexpr float kTripodMaxSize = 1.f;
        
        Model::ModelData _modelData;
        bool _enabledBone;
        std::string _rootBoneName;
        float _boneSize;
        unsigned _color;
        int _smoothness;
        bool _enabledTripod;
        float _tripodSize;
        
        friend void from_json	(const nlohmann::json& json, BoneRenderer& boneRenderer);
        friend void to_json		(nlohmann::json& json, const BoneRenderer& boneRenderer);
    };

#pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, BoneRenderer& boneRenderer)
    {
        json.at("enabled").         get_to(boneRenderer._enabled);
        json.at("enabledBone").     get_to(boneRenderer._enabledBone);
        json.at("boneSize").        get_to(boneRenderer._boneSize);
        json.at("color").           get_to(boneRenderer._color);
        json.at("enabledTripod").   get_to(boneRenderer._enabledTripod);
        json.at("tripodSize").      get_to(boneRenderer._tripodSize);
    }

    inline void to_json(nlohmann::json& json, const BoneRenderer& boneRenderer)
    {
        json =
        {
            { "enabled",        boneRenderer._enabled },
            { "enabledBone",    boneRenderer._enabledBone },
            { "boneSize",       boneRenderer._boneSize },
            { "color",          boneRenderer._color },
            { "enabledTripod",  boneRenderer._enabledTripod },
            { "tripodSize",     boneRenderer._tripodSize },
        };
    }
#pragma endregion
}