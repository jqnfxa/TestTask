#include <gtest/gtest.h>
#include <concave_hull.hpp>
#include <geometry.hpp>

using namespace task3::geometry;
using namespace task3::algorithm;

TEST(Task3_Geometry, Intersection)
{
    EXPECT_TRUE(segments_intersect({0, 0}, {10, 10}, {0, 10}, {10, 0}));
    EXPECT_FALSE(segments_intersect({0, 0}, {10, 0}, {0, 1}, {10, 1}));
    EXPECT_FALSE(segments_intersect({0, 0}, {5, 5}, {5, 5}, {10, 0}));
    EXPECT_FALSE(segments_intersect({0, 0}, {10, 0}, {5, 0}, {5, 5}));
}

TEST(Task3_Hull, BasicSquare)
{
    std::vector<Point> pts = {
        {0, 0}, {10, 0}, {10, 10}, {0, 10},
        {5, 5}
    };

    auto hull = compute_concave_hull(pts, 0.0);
    EXPECT_EQ(hull.size(), 4);

    auto concave = compute_concave_hull(pts, 5.0);
    EXPECT_EQ(concave.size(), 5);
}

TEST(Task3_Hull, ArticleExampleLogic)
{
    std::vector<Point> pts = {
        {0, 0}, {10, 0}, {10, 10}, {0, 10},
        {5, 2}
    };

    auto result = compute_concave_hull(pts, 1.5);
    bool found = false;
    for (auto &p : result)
    {
        if (p == Point{5, 2})
        {
            found = true;
        }
    }
    EXPECT_TRUE(found);
}
