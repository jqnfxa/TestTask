#pragma once

#include <vector>
#include "geometry.hpp"


namespace task3::algorithm
{
    std::vector<geometry::Point>
    compute_concave_hull(
        const std::vector<geometry::Point> &points,
        double gamma
    );
}
