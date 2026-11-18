#include <math.hpp>
#include "../../transform.h"
#include "../../component_factory.h"
#include "../../Model/model_renderer.h"
#include "bone_renderer.h"

using namespace DxLibSample;
using namespace DxLibSample::Animations::Rigging;
using namespace DxLibSample::Model;

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
    
    _smoothness = static_cast<int>(Math::Remap(_boneSize, kBoneMinSize, kBoneMaxSize, static_cast<float>(kMinSmoothness), static_cast<float>(kMaxSmoothness)));
}

void BoneRenderer::Start()
{
    _modelRenderer = GetComponentInChildren<ModelRenderer>();
    _modelData = _modelRenderer->GetModelData();
    _rootIndex = MV1SearchFrame(_modelData.modelHandle, MV1GetFrameName(_modelData.modelHandle, 0));
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
}

void BoneRenderer::DebugRender() const
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
    if (_rootIndex <= -1) return;
    if (!_enabledBone && !_enabledTripod) return;
    
    MV1SetMatrix(_modelData.modelHandle, ToDxLibMatrix(_modelRenderer->GetTransform()->GetWorldMatrix()));
    
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

    Traverse(_rootIndex);
    
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

void BoneRenderer::DrawTripod(const Matrix& mat) const
{
    const auto position = mat.Translation();
    const auto rotMat = CreateRotationMatrix(mat);
    const auto right = rotMat.Right();
    const auto up = rotMat.Up();
    const auto forward = rotMat.Forward();
    
    DrawLine3D(ToDxLibVector(position), ToDxLibVector(position + right * _tripodSize),      ToDxLibColor(kRed));
    DrawLine3D(ToDxLibVector(position), ToDxLibVector(position + up * _tripodSize),         ToDxLibColor(kGreen));
    DrawLine3D(ToDxLibVector(position), ToDxLibVector(position + forward * _tripodSize),    ToDxLibColor(kBlue));
}