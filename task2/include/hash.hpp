#pragma once

#include <cstddef>
#include "structs.hpp"


namespace task2::hash
{
    struct Point3Hash
    {
        std::size_t operator()(const task2::geometry::Point3 &p) const noexcept
        {
            std::size_t h1 = std::hash<float>{}(p.x);
            std::size_t h2 = std::hash<float>{}(p.y);
            std::size_t h3 = std::hash<float>{}(p.z);
            std::size_t seed = 0;

            seed ^= h1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}
