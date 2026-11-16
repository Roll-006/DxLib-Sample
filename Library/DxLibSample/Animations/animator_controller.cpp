#include "../transform.h"
#include "animator_controller.h"
#include "math.hpp"
#include "../game_time.h"

using namespace DxLibSample;
using namespace DxLibSample::Animations;
using namespace DxLibSample::Model;

AnimatorController::AnimatorController() : 
    _blendRate      (1.f),
    _isXZLocked     (false),
    _appliedRootY   (0.f)
{
    
}

void AnimatorController::Initialize(const ModelData& modelData)
{
    _modelData = modelData;
    AttachAnimation(_defaultAnimationName);
}

void AnimatorController::UpdateAnimation()
{
    Translate();
    BlendAnimation();
    PlayAnimation();
}

void AnimatorController::ApplyRootMotion(bool isApplyRootMotion, int animationRootIndex, const std::shared_ptr<Transform>& transform)
{
    if (_attachLayers.empty()) return;

    MV1ResetFrameUserLocalMatrix(_modelData.modelHandle, animationRootIndex);
    MV1ResetFrameUserLocalWorldMatrix(_modelData.modelHandle, animationRootIndex);

    const auto modelMat = ToMatrix(MV1GetMatrix(_modelData.modelHandle));
    const auto modelInvMat = modelMat.Invert();
    const auto layerRootPositions = GetLayerRootPositions(animationRootIndex, modelInvMat);
    const auto rootWorldMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelData.modelHandle, animationRootIndex));
    const auto rootModelMat = rootWorldMat * modelInvMat;
    
    UpdateLockPosition(rootModelMat);
    ApplyGameObjectXZPosition(isApplyRootMotion, transform, layerRootPositions, modelMat);
    ApplyGameObjectYPosition(isApplyRootMotion, transform, layerRootPositions, modelMat);

    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        auto& layer = _attachLayers[i];
        layer.previousRootPosition = layerRootPositions[i];
        layer.previousTime = layer.animation.currentTime;
        layer.hasPreviousRootPosition = true;
    }
    
    const auto resultWorldMat = CalcResultWorldMatrix(isApplyRootMotion, layerRootPositions, modelMat, rootModelMat);
    const auto parentIndex = MV1GetFrameParent(_modelData.modelHandle, animationRootIndex);
    const auto parentWorldMat = parentIndex >= 0 ? ToMatrix(MV1GetFrameLocalWorldMatrix(_modelData.modelHandle, parentIndex)) : modelMat;
    MV1SetFrameUserLocalMatrix(_modelData.modelHandle, animationRootIndex, ToDxLibMatrix(resultWorldMat * parentWorldMat.Invert()));
}

void AnimatorController::SetFloat(const std::string& name, float value)
{
    if (!_animationParameters.floats.contains(name)) return;
    _animationParameters.floats.at(name) = value;
}

void AnimatorController::SetBool(const std::string& name, bool value)
{
    if (!_animationParameters.booleans.contains(name)) return;
    _animationParameters.booleans.at(name) = value;
}

void AnimatorController::SetInteger(const std::string& name, int value)
{
    if (!_animationParameters.integers.contains(name)) return;
    _animationParameters.integers.at(name) = value;
}

void AnimatorController::SetTrigger(const std::string& name)
{
    if (!_animationParameters.triggers.contains(name)) return;
    _animationParameters.triggers.at(name) = true;
}

void AnimatorController::ResetTrigger(const std::string& name)
{
    if (!_animationParameters.triggers.contains(name)) return;
    _animationParameters.triggers.at(name) = false;
}

void AnimatorController::Translate()
{
    if (_attachLayers.empty()) return;

    const auto isBlending = _attachLayers.size() >= 2 && _blendRate < 1.f;

    for (const auto origin : GetTransitionOrigins())
    {
        const auto originLayer = FindOriginLayer(origin);
        if (!originLayer) continue;

        for (const auto& transition : _animationTransitions)
        {
            if (transition.originAnimationName != origin) continue;

            // 同じ条件の遷移はさせない
            if (isBlending &&
                transition.originAnimationName == _currentTransition.originAnimationName &&
                transition.targetAnimationName == _currentTransition.targetAnimationName) continue;

            if (transition.hasExitTime && originLayer->animation.playRate < transition.exitTime) continue;

            if (!CheckFloatTransition(transition))   continue;
            if (!CheckBooleanTransition(transition)) continue;
            if (!CheckIntegerTransition(transition)) continue;
            if (!CheckTriggerTransition(transition)) continue;

            if (!AttachAnimation(transition.targetAnimationName)) continue;

            _currentTransition = transition;
            ResetAllTriggers();
            return;
        }
    }
}

void AnimatorController::BlendAnimation()
{
    if (_attachLayers.size() < 2)
    {
        _blendRate = 1.f;
        return;
    }

    if (_currentTransition.transitionDuration <= 0.f)
    {
        _blendRate = 1.f;
    }
    else
    {
        const auto magnitude = _currentTransition.isFixedDuration ? 1.f : _attachLayers[_attachLayers.size() - 2].animation.totalTime;
        const auto duration = GameTime::I().GetDeltaTime() / (_currentTransition.transitionDuration * magnitude);
        _blendRate = std::min(_blendRate + duration, 1.f);
    }

    if (_blendRate < 1.f) return;

    for (size_t i = 0; i < _attachLayers.size() - 1; ++i)
    {
        DetachAnimation(_attachLayers[i].animation);
    }
    _attachLayers.erase(_attachLayers.begin(), _attachLayers.end() - 1);
}

void AnimatorController::PlayAnimation()
{
    for (auto& layer : _attachLayers)
    {
        auto& attach = layer.animation;
        if (attach.attachIndex < 0) continue;

        const auto& clip = _animationClips.at(attach.animationName);

        attach.currentTime += clip.speed * GameTime::I().GetDeltaTime();
        if (clip.isLooping)
        {
            if (attach.currentTime > attach.totalTime) attach.currentTime -= attach.totalTime;
        }
        else if (attach.currentTime > attach.totalTime)
        {
            attach.currentTime = attach.totalTime;
        }
        attach.playRate = attach.currentTime / attach.totalTime;

        MV1SetAttachAnimTime(_modelData.modelHandle, attach.attachIndex, attach.currentTime);
    }

    ApplyLayerWeights();
}

bool AnimatorController::AttachAnimation(const std::string& attachAnimationName)
{
    if (!_attachLayers.empty() && _attachLayers.back().animation.animationName == attachAnimationName) return false;

    auto& clip = _animationClips.at(attachAnimationName);

    const auto attachIndex = MV1AttachAnim(_modelData.modelHandle, clip.animationIndex, clip.animationHandle, TRUE);
    if (attachIndex < 0) return false;

    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        _attachLayers[i].weight = GetWeight(i);
    }
    
    AttachAnimationLayer attachLayer;
    attachLayer.animation.animationName = attachAnimationName;
    attachLayer.animation.attachIndex   = attachIndex;
    attachLayer.animation.currentTime   = 0.f;
    attachLayer.animation.totalTime     = MV1GetAttachAnimTotalTime(_modelData.modelHandle, attachIndex);
    attachLayer.animation.playRate      = 0.f;

    // 他のアニメーションがある場合、ブレンド率を0から設定
    const auto hasOtherAnimations = !_attachLayers.empty();
    _blendRate = hasOtherAnimations ? 0.f : 1.f;
    _attachLayers.emplace_back(attachLayer);

    RemoveExcessLayers();
    
    return true;
}

void AnimatorController::DetachAnimation(AttachAnimationData& attachAnimation) const
{
    if (attachAnimation.attachIndex < 0) return;

    MV1DetachAnim(_modelData.modelHandle, attachAnimation.attachIndex);
    attachAnimation.attachIndex = -1;
}

void AnimatorController::ResetAllTriggers()
{
    for (const auto& name : _currentTransition.triggers)
    {
        ResetTrigger(name);
    }
}

bool AnimatorController::CheckFloatTransition(const AnimationTransition& checkAnimation) const
{
    for (const auto& [name, floatTransition] : checkAnimation.floats)
    {
        if (!_animationParameters.floats.contains(name)) return false;
            
        switch (floatTransition.type)
        {
        case FloatTransitionType::kGreater:
            if (_animationParameters.floats.at(name) <= floatTransition.value) return false;
            break;
                
        case FloatTransitionType::kLess:
            if (_animationParameters.floats.at(name) >= floatTransition.value) return false;
            break;
        }
    }
    
    return true;
}

bool AnimatorController::CheckBooleanTransition(const AnimationTransition& checkAnimation) const
{
    for (const auto& [name, booleanTransition] : checkAnimation.booleans)
    {
        if (!_animationParameters.booleans.contains(name)) return false;
            
        const auto expected = booleanTransition.type == BooleanTransitionType::kTrue;
        if (_animationParameters.booleans.at(name) != expected) return false;
    }
    
    return true;
}

bool AnimatorController::CheckIntegerTransition(const AnimationTransition& checkAnimation) const
{
    for (const auto& [name, integerTransition] : checkAnimation.integers)
    {
        if (!_animationParameters.integers.contains(name)) return false;
            
        switch (integerTransition.type)
        {
        case IntegerTransitionType::kGreater:
            if (_animationParameters.integers.at(name) <= integerTransition.value) return false;
            break;
                
        case IntegerTransitionType::kLess:
            if (_animationParameters.integers.at(name) >= integerTransition.value) return false;
            break;
            
        case IntegerTransitionType::kEquals:
            if (_animationParameters.integers.at(name) != integerTransition.value) return false;
            break;
                
        case IntegerTransitionType::kNotEqual:
            if (_animationParameters.integers.at(name) == integerTransition.value) return false;
            break;
        }
    }
    
    return true;
}

bool AnimatorController::CheckTriggerTransition(const AnimationTransition& checkAnimation) const
{
    for (const auto& name : checkAnimation.triggers)
    {
        if (!_animationParameters.triggers.contains(name)) return false;
        if (!_animationParameters.triggers.at(name)) return false;
    }
    
    return true;
}

float AnimatorController::GetWeight(size_t index) const
{
    // ブレンド先 (末尾レイヤー) はブレンド率をそのまま反映
    if (index + 1 == _attachLayers.size()) return _blendRate;
    
    return _attachLayers[index].weight * (1.f - _blendRate);
}

void AnimatorController::ApplyLayerWeights() const
{
    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        MV1SetAttachAnimBlendRate(_modelData.modelHandle, _attachLayers[i].animation.attachIndex, GetWeight(i));
    }
}

void AnimatorController::RemoveExcessLayers()
{
    while (_attachLayers.size() > kMaxAttachLayers)
    {
        // 遷移元と遷移先以外 (index: 0 ～ size() - 3) の削除を許可する
        size_t minIndex = 0;
        for (size_t i = 1; i < _attachLayers.size() - 2; ++i)
        {
            // weight が最小のレイヤーを検索
            if (_attachLayers[i].weight < _attachLayers[minIndex].weight) minIndex = i;
        }

        const auto lostWeight = _attachLayers[minIndex].weight;
        
        DetachAnimation(_attachLayers[minIndex].animation);
        _attachLayers.erase(_attachLayers.begin() + minIndex);

        // 遷移先以外の weight の合計を取得
        auto sum = 0.f;
        for (size_t i = 0; i < _attachLayers.size() - 1; ++i) sum += _attachLayers[i].weight;
        
        // 捨てた weight を遷移先以外のレイヤーへ分ける (全体の weight は1を保つ)
        if (sum > 0.f)
        {
            for (size_t i = 0; i < _attachLayers.size() - 1; ++i)
            {
                _attachLayers[i].weight += lostWeight * (_attachLayers[i].weight / sum);
            }
        }
    }
}

std::vector<Vector3> AnimatorController::GetLayerRootPositions(int animationRootIndex, const Matrix& modelInvMat) const
{
    std::vector<Vector3> positions(_attachLayers.size());

    // レイヤーが1つの場合は直接取得
    if (_attachLayers.size() == 1)
    {
        positions[0] = (ToMatrix(MV1GetFrameLocalWorldMatrix(_modelData.modelHandle, animationRootIndex)) * modelInvMat).Translation();
        return positions;
    }

    // DxLib はアニメーション個別でルート座標を取得することができないため、該当のアニメーションの weight を1にして座標を取得する
    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        for (size_t j = 0; j < _attachLayers.size(); ++j)
        {
            MV1SetAttachAnimBlendRate(_modelData.modelHandle, _attachLayers[j].animation.attachIndex, i == j ? 1.f : 0.f);
        }
        positions[i] = (ToMatrix(MV1GetFrameLocalWorldMatrix(_modelData.modelHandle, animationRootIndex)) * modelInvMat).Translation();
    }

    // weight を戻す
    ApplyLayerWeights();
    
    return positions;
}

void AnimatorController::UpdateLockPosition(const Matrix& rootModelMat)
{
    // ルートの初期座標を保存
    if (!_isXZLocked)
    {
        _lockPosition = rootModelMat.Translation();
        _isXZLocked = true;
    }

    // Y座標固定するかを設定
    auto isYLocked = false;
    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        const auto& clip = _animationClips.at(_attachLayers[i].animation.animationName);
        if (!clip.isBakeIntoPoseY && GetWeight(i) > Math::kEpsilon)
        {
            isYLocked = true;
            break;
        }
    }

    if (!isYLocked) _lockPosition.y = rootModelMat.Translation().y;
}

void AnimatorController::ApplyGameObjectXZPosition(bool isApplyRootMotion, const std::shared_ptr<Transform>& transform, const std::vector<Vector3>& layerRootPositions, const Matrix& modelMat) const
{
    if (!isApplyRootMotion) return;
    
    Vector3 delta;
    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        const auto& layer = _attachLayers[i];
        const auto& clip  = _animationClips.at(layer.animation.animationName);

        if (clip.isBakeIntoPoseXZ) continue;
        if (!layer.hasPreviousRootPosition) continue;
        if (layer.animation.currentTime < layer.previousTime) continue;

        delta += (layerRootPositions[i] - layer.previousRootPosition) * GetWeight(i);
    }

    auto worldDelta = Vector3::TransformNormal(delta, modelMat);
    worldDelta.y = 0.f;
    transform->SetWorldPosition(transform->GetWorldPosition() + worldDelta);
}

void AnimatorController::ApplyGameObjectYPosition(bool isApplyRootMotion, const std::shared_ptr<Transform>& transform, const std::vector<Vector3>& layerRootPositions, const Matrix& modelMat)
{
    auto targetY = 0.f;
    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        const auto& clip = _animationClips.at(_attachLayers[i].animation.animationName);
        if (clip.isBakeIntoPoseY) continue;
        targetY += (layerRootPositions[i].y - _lockPosition.y) * GetWeight(i);
    }

    const auto delta = targetY - _appliedRootY;

    if (std::abs(delta) > Math::kEpsilon)
    {
        const auto worldDelta = Vector3::TransformNormal(Vector3(0.f, delta, 0.f), modelMat);
        transform->SetWorldPosition(transform->GetWorldPosition() + Vector3(0.f, worldDelta.y, 0.f));
        _appliedRootY = targetY;
    }
}

Matrix AnimatorController::CalcResultWorldMatrix(bool isApplyRootMotion, const std::vector<Vector3>& layerRootPositions, const Matrix& modelMat, const Matrix& rootModelMat) const
{
    auto resultModelMat = rootModelMat;
    auto targetPosition = _lockPosition;

    for (size_t i = 0; i < _attachLayers.size(); ++i)
    {
        const auto& clip = _animationClips.at(_attachLayers[i].animation.animationName);
        const auto weight = GetWeight(i);

        if (!isApplyRootMotion && clip.isBakeIntoPoseXZ)
        {
            targetPosition.x += (layerRootPositions[i].x - _lockPosition.x) * weight;
            targetPosition.z += (layerRootPositions[i].z - _lockPosition.z) * weight;
        }

        if (clip.isBakeIntoPoseY)
        {
            targetPosition.y += (layerRootPositions[i].y - _lockPosition.y) * weight;
        }
    }

    resultModelMat._41 = targetPosition.x;
    resultModelMat._43 = targetPosition.z;
    resultModelMat._42 = targetPosition.y - _appliedRootY;
    
    return resultModelMat * modelMat;
}

std::vector<std::string> AnimatorController::GetTransitionOrigins() const
{
    const auto next = _attachLayers.back().animation.animationName;
    
    // 単独もしくはブレンド済みの場合、次の State を起点とする
    if (_attachLayers.size() < 2 || _blendRate >= 1.f) return { next };

    const auto current = _attachLayers[_attachLayers.size() - 2].animation.animationName;

    switch (_currentTransition.interruptionSourceType)
    {
    case InterruptionSourceType::kNone: return { };
    case InterruptionSourceType::kCurrentState: return { current };
    case InterruptionSourceType::kNextState: return { next };
    case InterruptionSourceType::kCurrentStateThenNextState: return { current, next };
    case InterruptionSourceType::kNextStateThenCurrentState: return { next, current };
    default: return { };
    }
}

std::optional<AttachAnimationLayer> AnimatorController::FindOriginLayer(const std::string& origin) const
{
    if (_attachLayers.empty()) return std::nullopt;
    
    const auto next = _attachLayers.back();
    if (next.animation.animationName == origin) return next;

    if (_attachLayers.size() >= 2)
    {
        const auto current = _attachLayers[_attachLayers.size() - 2];
        if (current.animation.animationName == origin) return current;
    }
    
    // next, current 以外は起点としない
    return std::nullopt;
}