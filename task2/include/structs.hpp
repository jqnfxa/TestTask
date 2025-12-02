#pragma once


namespace task2::geometry
{
    struct Point3
    {
        float x, y, z;
        bool operator==(const Point3 &) const = default;
    };
}
