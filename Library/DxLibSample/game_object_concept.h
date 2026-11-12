#pragma once
#include <type_traits>

namespace DxLibSample
{
    class GameObject;
}

template<typename T>
concept GameObjectT = std::is_base_of_v<DxLibSample::GameObject, T>;