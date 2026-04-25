#include <DxLib.h>
#include <Vector/vector2.hpp>
#include "debug.h"
#include "transform.h"
#include "aabb.h"
#include "collision.h"

bool collision::IsColliding(const AABB& aabb1, const AABB& aabb2)
{
    // 衝突していない場合の否定
    return !(
        aabb1.GetMax().x < aabb2.GetMin().x ||
        aabb1.GetMin().x > aabb2.GetMax().x ||
        aabb1.GetMax().y < aabb2.GetMin().y ||
        aabb1.GetMin().y > aabb2.GetMax().y);
}
