#pragma once
#include <type_traits>
class Component;

template<typename T>
concept ComponentT = std::is_base_of_v<Component, T>;
