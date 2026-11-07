#pragma once
#include <type_traits>

namespace Core
{
    class Component;
}

template<typename T>
concept ComponentT = std::is_base_of_v<Core::Component, T>;