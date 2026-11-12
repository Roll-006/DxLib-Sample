#include <math.hpp>
#include "../../Core/transform.h"
#include "../../Core/component_factory.h"
#include "../../Model/model_renderer.h"
#include "bone_renderer.h"

using namespace Core;
using namespace Animations::Rigging;
using namespace Model;

namespace
{
    const bool registered = []()
    {
        ComponentFactory::Register("BoneRenderer", [](GameObject& obj) { return obj.AddComponent<BoneRenderer>(); });
        return true;
    }();
}

BoneRenderer::BoneRenderer() : 
    _enabledBone    (true),
    _boneSize       (kBoneMinSize),
    _color          (0xffffff),
    _smoothness     (kMinSmoothness),
    _enabledTripod  (true),
    _tripodSize     (kTripodMinSize)
{
	
}

void BoneRenderer::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
    _gameObject	= gameObject;
    
    _modelData = GetComponent<ModelRenderer>()->GetModelData();
    _rootBoneName = MV1GetFrameName(_modelData.modelHandle, 0);
    _boneSize = std::clamp(_boneSize, kBoneMinSize, kBoneMaxSize);
    _smoothness = static_cast<int>(Math::Remap(_boneSize, kBoneMinSize, kBoneMaxSize, static_cast<float>(kMinSmoothness), static_cast<float>(kMaxSmoothness)));
    _tripodSize = std::clamp(_tripodSize, kTripodMinSize, kTripodMaxSize);
}

void BoneRenderer::Start()
{
    
}

void BoneRenderer::Update()
{
    if (!_enabled) return;
}

void BoneRenderer::LateUpdate()
{
    if (!_enabled) return;
}

void BoneRenderer::Render() const
{
    if (!_enabled) return;
    
    DrawBone();
}

void BoneRenderer::Deserialize(const nlohmann::json& json)
{
    from_json(json, *this);
}

void BoneRenderer::DrawBone() const
{
    if (!_enabledBone && !_enabledTripod) return;
    
    const auto rootIndex = MV1SearchFrame(_modelData.modelHandle, _rootBoneName.c_str());
    if (rootIndex <= -1) return;
    
    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    // 子を辿る再帰関数 (フレームindexで辿る)
    std::function<void(int)> Traverse;

    Traverse = [&](const auto parentIndex)
    {
        const auto parentMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelData.modelHandle, parentIndex));
        const auto parentPosition = parentMat.Translation();

        const auto childCount = MV1GetFrameChildNum(_modelData.modelHandle, parentIndex);
        for (auto i = 0; i < childCount; ++i)
        {
            const auto childIndex = MV1GetFrameChild(_modelData.modelHandle, parentIndex, i);
            if (childIndex <= -1) continue;

            const auto childMat = ToMatrix(MV1GetFrameLocalWorldMatrix(_modelData.modelHandle, childIndex));
            const auto childPosition = childMat.Translation();
            const auto distance = (childPosition - parentPosition).Length();
            const auto radius = distance * _boneSize;

            // 関節・ボーン・XYZ軸の描画
            if (_enabledBone) DrawSphere3D(ToDxLibVector(parentPosition), radius, _smoothness, _color, _color, FALSE);
            if (_enabledBone) DrawCone3D(ToDxLibVector(childPosition), ToDxLibVector(parentPosition), radius, _smoothness, _color, _color, FALSE);
            if (_enabledTripod) DrawTripod(parentMat);
            
            // 子がいない場合、関節・XYZ軸のみ描画して再帰しない
            if (MV1GetFrameChildNum(_modelData.modelHandle, childIndex) == 0)
            {
                if (_enabledBone) DrawSphere3D(ToDxLibVector(childPosition), radius, _smoothness, _color, _color, FALSE);
                if (_enabledTripod) DrawTripod(childMat);
                continue;
            }

            // 子がいるため再帰
            Traverse(childIndex);
        }
    };

    Traverse(rootIndex);
    
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

void BoneRenderer::DrawTripod(const Matrix& mat) const
{
    const auto position = mat.Translation();
    const auto rotationMat = CreateRotationMatrix(mat);
    const auto right = rotationMat.Right();
    const auto up = rotationMat.Up();
    const auto forward = rotationMat.Forward();
    
    DrawLine3D(ToDxLibVector(position), ToDxLibVector(position + right * _tripodSize),      ToDxLibColor(kRed));
    DrawLine3D(ToDxLibVector(position), ToDxLibVector(position + up * _tripodSize),         ToDxLibColor(kGreen));
    DrawLine3D(ToDxLibVector(position), ToDxLibVector(position + forward * _tripodSize),    ToDxLibColor(kBlue));
}