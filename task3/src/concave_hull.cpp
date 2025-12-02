#include <algorithm>
#include <execution>
#include <limits>
#include <mutex>
#include <ranges>
#include "concave_hull.hpp"


namespace task3::algorithm
{
    namespace
    {
        inline bool compare_by_coord(const geometry::Point &a, const geometry::Point &b)
        {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
        }

        std::vector<geometry::Point> get_convex_hull(std::vector<geometry::Point> pts)
        {
            const auto n = pts.size();
            if (n <= 2)
            {
                return pts;
            }

            std::sort(
                std::execution::par_unseq,
                pts.begin(), pts.end(), compare_by_coord
            );

            std::vector<geometry::Point> hull;
            hull.reserve(n);

            /* Lower */
            for (const auto &p : pts)
            {
                while (
                    hull.size() >= 2 &&
                    cross_product(hull[hull.size() - 2], hull.back(), p) <= 0.0
                )
                {
                    hull.pop_back();
                }

                hull.push_back(p);
            }

            /* Upper */
            const auto lower_size = hull.size();
            for (const auto &p : pts | std::views::reverse | std::views::drop(1))
            {
                while (
                    hull.size() > lower_size &&
                    cross_product(hull[hull.size() - 2], hull.back(), p) <= 0.0
                )
                {
                    hull.pop_back();
                }

                hull.push_back(p);
            }

            if (!hull.empty())
            {
                hull.pop_back();
            }
            return hull;
        }

        struct Candidate
        {
            std::size_t original_index;
            double area;
            geometry::Point p;
        };
    }

    std::vector<geometry::Point>
    compute_concave_hull(
        const std::vector<geometry::Point> &input_points,
        double gamma
    )
    {
        if (input_points.size() < 3)
        {
            return input_points;
        }

        std::vector<geometry::Point> H = get_convex_hull(input_points);
        std::vector<geometry::Point> G;
        G.reserve(input_points.size());

        {
            auto H_sorted = H;
            std::sort(H_sorted.begin(), H_sorted.end(), compare_by_coord);

            for (const auto &p : input_points)
            {
                if (!std::binary_search(
                    H_sorted.begin(), H_sorted.end(), p, compare_by_coord))
                {
                    G.push_back(p);
                }
            }
        }

        while (!G.empty())
        {
            double max_dist_sq = -1.0;
            std::size_t m = 0;
            std::size_t h_len = H.size();

            for (std::size_t i = 0; i < h_len; ++i)
            {
                double d = dist_sq(H[i], H[(i + 1) % h_len]);
                if (d > max_dist_sq)
                {
                    max_dist_sq = d;
                    m = i;
                }
            }

            geometry::Point p_b = H[m];
            geometry::Point p_e = H[(m + 1) % h_len];
            double d0_sq = max_dist_sq;

            std::vector<Candidate> candidates;
            std::mutex candidates_mutex;

            std::for_each(
                std::execution::par,
                G.begin(), G.end(), [&](const geometry::Point &pt) {
                    double d1_sq = dist_sq(p_b, pt);
                    double d2_sq = dist_sq(p_e, pt);

                    if ((d1_sq + d2_sq - d0_sq) < (gamma * std::min(d1_sq, d2_sq)))
                    {
                        double area = triangle_area(p_b, p_e, pt);
                        std::lock_guard<std::mutex> lock(candidates_mutex);
                        candidates.push_back({0, area, pt});
                    }
                }
            );

            if (candidates.empty())
            {
                break;
            }

            std::sort(
                std::execution::par_unseq,
                candidates.begin(), candidates.end(),
                [](const Candidate &a, const Candidate &b) {
                    return a.area < b.area;
                }
            );

            int best_cand_idx = -1;
            for (std::size_t i = 0; i < candidates.size(); ++i)
            {
                const auto pt = candidates[i].p;
                bool intersects = false;

                for (std::size_t k = 0; k < h_len; ++k)
                {
                    if (k == m)
                    {
                        continue;
                    }

                    geometry::Point h1 = H[k];
                    geometry::Point h2 = H[(k + 1) % h_len];

                    if (segments_intersect(p_b, pt, h1, h2) ||
                        segments_intersect(pt, p_e, h1, h2))
                    {
                        intersects = true;
                        break;
                    }
                }

                if (!intersects)
                {
                    best_cand_idx = static_cast<int>(i);
                    break;
                }
            }

            if (best_cand_idx != -1)
            {
                const auto pt = candidates[best_cand_idx].p;
                H.insert(H.begin() + m + 1, pt);

                if (auto it = std::find(G.begin(), G.end(), pt); it != G.end())
                {
                    *it = G.back();
                    G.pop_back();
                }
            }
            else
            {
                break;
            }
        }

        return H;
    }
}
