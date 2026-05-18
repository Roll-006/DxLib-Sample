#pragma once
#include <type_traits>
class GameObject;

template<typename T>
concept GameObjectT = std::is_base_of_v<GameObject, T>;
