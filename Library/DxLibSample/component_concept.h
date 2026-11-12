#pragma once
#include <type_traits>

namespace DxLibSample
{
    class Component;
}

template<typename T>
concept ComponentT = std::is_base_of_v<DxLibSample::Component, T>;