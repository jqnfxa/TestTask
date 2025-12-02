#include <algorithm>
#include "geometry.hpp"

namespace task3::geometry
{
    double dist_sq(Point p1, Point p2)
    {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return dx * dx + dy * dy;
    }

    double length_sq(Point p1, Point p2)
    {
        return dist_sq(p1, p2);
    }

    double cross_product(Point a, Point b, Point c)
    {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    double triangle_area(Point a, Point b, Point c)
    {
        return std::abs(cross_product(a, b, c)) / 2.0;
    }

    int ccw(Point a, Point b, Point c)
    {
        double cp = cross_product(a, b, c);
        const double epsilon = 1e-9;
        if (cp > epsilon)
        {
            return 1;
        }
        if (cp < -epsilon)
        {
            return -1;
        }

        return 0;
    }

    bool segments_intersect(Point p1, Point p2, Point p3, Point p4)
    {
        return (ccw(p1, p2, p3) * ccw(p1, p2, p4) < 0 &&
                ccw(p3, p4, p1) * ccw(p3, p4, p2) < 0);
    }
}
