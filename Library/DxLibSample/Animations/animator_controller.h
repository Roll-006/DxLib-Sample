#pragma once
#include "../Model/model_data.h"
#include "animation_clip.h"
#include "animation_parameters.h"
#include "animation_transition.h"
#include "attach_animation_layer.h"

namespace DxLibSample::Animations
{
    class AnimatorController
    {
    public:
        AnimatorController();
    
        void Initialize(const Model::ModelData& modelData);
        void UpdateAnimation();
        void ApplyRootMotion(bool isApplyRootMotion, int animationRootIndex, const std::shared_ptr<Transform>& transform);

        void SetFloat(const std::string& name, float value);
        void SetBool(const std::string& name, bool value);
        void SetInteger(const std::string& name, int value);
        void SetTrigger(const std::string& name);
        void ResetTrigger(const std::string& name);
    
        [[nodiscard]] AnimationParameters GetAnimationParameters() const { return _animationParameters; }

    private:
        /// <summary>
        /// アニメーションを遷移させる。
        /// </summary>
        void Translate();

        /// <summary>
        /// アニメーションのブレンドを行う。
        /// </summary>
        void BlendAnimation();

        /// <summary>
        /// アニメーションを再生する。
        /// </summary>
        void PlayAnimation();

        /// <summary>
        /// アニメーションをアタッチする。
        /// </summary>
        /// <param name="attachAnimationName">アタッチするアニメーションの名前</param>
        /// <returns>true: アタッチ成功, false: アタッチ失敗</returns>
        bool AttachAnimation(const std::string& attachAnimationName);

        /// <summary>
        /// アニメーションをデタッチする。
        /// </summary>
        /// <param name="attachAnimation">デタッチするアニメーション</param>
        void DetachAnimation(AttachAnimationData& attachAnimation) const;
        
        /// <summary>
        /// 再生中のアニメーションのトリガーをすべてリセットする。
        /// </summary>
        void ResetAllTriggers();
        
        /// <summary>
        /// Float型の遷移条件が有効か判定する。
        /// </summary>
        /// <param name="checkAnimation">遷移可能か判定するアニメーション</param>
        /// <returns>true: 成功, false: 失敗</returns>
        [[nodiscard]] bool CheckFloatTransition(const AnimationTransition& checkAnimation) const;

        /// <summary>
        /// Bool型の遷移条件が有効か判定する。
        /// </summary>
        /// <param name="checkAnimation">遷移可能か判定するアニメーション</param>
        /// <returns>true: 成功, false: 失敗</returns>
        [[nodiscard]] bool CheckBooleanTransition(const AnimationTransition& checkAnimation) const;

        /// <summary>
        /// Int型の遷移条件が有効か判定する。
        /// </summary>
        /// <param name="checkAnimation">遷移可能か判定するアニメーション</param>
        /// <returns>true: 成功, false: 失敗</returns>
        [[nodiscard]] bool CheckIntegerTransition(const AnimationTransition& checkAnimation) const;

        /// <summary>
        /// Trigerの遷移条件が有効か判定する。
        /// </summary>
        /// <param name="checkAnimation">遷移可能か判定するアニメーション</param>
        /// <returns>true: 成功, false: 失敗</returns>
        [[nodiscard]] bool CheckTriggerTransition(const AnimationTransition& checkAnimation) const;
        
        /// <summary>
        /// レイヤーのインデックスからブレンド率の重みを取得する。
        /// </summary>
        /// <param name="index">レイヤーのインデックス</param>
        /// <returns>ブレンド率の重み</returns>
        [[nodiscard]] float GetWeight(size_t index) const;

        /// <summary>
        /// ブレンド率を適用する。
        /// </summary>
        void ApplyLayerWeights() const;

        /// <summary>
        /// 超過したレイヤーを取り除く。
        /// </summary>
        void RemoveExcessLayers();

        /// <summary>
        /// 各レイヤーのルート座標を取得する。
        /// </summary>
        /// <param name="animationRootIndex">アニメーションのルートインデックス</param>
        /// <param name="modelInvMat">モデルの逆行列</param>
        /// <returns>各レイヤーのルート座標</returns>
        [[nodiscard]] std::vector<Vector3> GetLayerRootPositions(int animationRootIndex, const Matrix& modelInvMat) const;

        /// <summary>
        /// 固定座標を更新する。
        /// </summary>
        /// <param name="rootModelMat">ルートモデル行列</param>
        void UpdateLockPosition(const Matrix& rootModelMat);
        
        /// <summary>
        /// GameObject の XZ 座標にルートモーションの移動量を適用する。
        /// </summary>
        /// <param name="isApplyRootMotion">ルートモーションを適用するかどうか</param>
        /// <param name="transform">GameObject のトランスフォーム</param>
        /// <param name="layerRootPositions">各レイヤーのルート座標</param>
        /// <param name="modelMat">モデルの行列</param>
        void ApplyGameObjectXZPosition(bool isApplyRootMotion, const std::shared_ptr<Transform>& transform, const std::vector<Vector3>& layerRootPositions, const Matrix& modelMat) const;
        
        /// <summary>
        /// GameObject の Y 座標にルートモーションの移動量を適用する。
        /// </summary>
        /// <param name="isApplyRootMotion">ルートモーションを適用するかどうか</param>
        /// <param name="transform">GameObject のトランスフォーム</param>
        /// <param name="layerRootPositions">各レイヤーのルート座標</param>
        /// <param name="modelMat">モデルの行列</param>
        void ApplyGameObjectYPosition(bool isApplyRootMotion, const std::shared_ptr<Transform>& transform, const std::vector<Vector3>& layerRootPositions, const Matrix& modelMat);
        
        [[nodiscard]] Matrix CalcResultWorldMatrix(bool isApplyRootMotion, const std::vector<Vector3>& layerRootPositions, const Matrix& modelMat, const Matrix& rootModelMat) const;
        
        /// <summary>
        /// 遷移元のアニメーション名を取得する。
        /// </summary>
        /// <returns>遷移元のアニメーション名</returns>
        [[nodiscard]] std::vector<std::string> GetTransitionOrigins() const;

        /// <summary>
        /// 遷移元のアニメーションレイヤーを検索する。
        /// </summary>
        /// <param name="origin">遷移元のアニメーション名</param>
        /// <returns>遷移元のアニメーションレイヤー。見つからなかった場合は std::nullopt</returns>
        [[nodiscard]] std::optional<AttachAnimationLayer> FindOriginLayer(const std::string& origin) const;

    private:
        static constexpr int kMaxAttachLayers = 4;
        
        Model::ModelData                                _modelData;
        std::string                                     _defaultAnimationName;
        AnimationParameters                             _animationParameters;
        std::vector<AnimationTransition>                _animationTransitions;
        std::unordered_map<std::string, AnimationClip>  _animationClips;
        AnimationTransition                             _currentTransition;
        std::vector<AttachAnimationLayer>               _attachLayers;
        float                                           _blendRate;
        Vector3                                         _lockPosition;
        bool                                            _isXZLocked;
        float                                           _appliedRootY;
    
        friend void from_json	(const nlohmann::json& json, AnimatorController& animatorController);
        friend void to_json		(nlohmann::json& json, const AnimatorController& animatorController);
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, AnimatorController& animatorController)
    {
        json.at("defaultAnimationName").get_to(animatorController._defaultAnimationName);
        json.at("animationParameters"). get_to(animatorController._animationParameters);
        json.at("animationTransitions").get_to(animatorController._animationTransitions);
        json.at("animationClips").      get_to(animatorController._animationClips);
    }

    inline void to_json(nlohmann::json& json, const AnimatorController& animatorController)
    {
        json =
        {
            { "defaultAnimationName",   animatorController._defaultAnimationName },
            { "animationParameters",    animatorController._animationParameters },
            { "animationTransitions",   animatorController._animationTransitions },
            { "animationClips",         animatorController._animationClips },
        };
    }
    #pragma endregion
}