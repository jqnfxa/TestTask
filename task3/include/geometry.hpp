#pragma once

#include <vector>
#include <cmath>
#include <compare>


namespace task3::geometry
{
    struct Point
    {
        double x, y;
        bool operator==(const Point&) const = default;
    };

    double dist_sq(Point p1, Point p2);
    double length_sq(Point p1, Point p2);

    double cross_product(Point a, Point b, Point c);
    double triangle_area(Point a, Point b, Point c);

    bool segments_intersect(Point p1, Point p2, Point p3, Point p4);
}
