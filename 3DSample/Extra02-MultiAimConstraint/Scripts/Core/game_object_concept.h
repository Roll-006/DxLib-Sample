#pragma once
#include <type_traits>

namespace Core
{
    class GameObject;
}

template<typename T>
concept GameObjectT = std::is_base_of_v<Core::GameObject, T>;