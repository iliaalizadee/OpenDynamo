#pragma once

#include <concepts>

namespace OpenDynamo
{
    template<typename T>
    concept Floating = std::same_as<T, float> || std::same_as<T, double>;
}