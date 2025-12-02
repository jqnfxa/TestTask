#pragma once

#include "traits.hpp"
#include <algorithm>
#include <cstddef>
#include <execution>
#include <functional>
#include <future>
#include <iterator>

namespace task1::algorithm::implementation
{
    constexpr std::ptrdiff_t INSERTION_SORT_THRESHOLD = 32;
    constexpr std::ptrdiff_t PARALLELIZATION_THRESHOLD = 2048;

    template <std::random_access_iterator RandomIt, class Compare>
    requires std::sortable<RandomIt, Compare>
    auto insertion_sort(RandomIt first, RandomIt last, Compare comp)
        -> void
    {
        for (auto it = first; it != last; ++it)
        {
            const auto insertion = std::upper_bound(first, it, *it, comp);
            std::rotate(insertion, it, std::next(it));
        }
    }

    template <std::random_access_iterator RandomIt, class Compare>
    requires std::sortable<RandomIt, Compare>
    decltype(auto) select_pivot(RandomIt first, RandomIt last, Compare comp)
    {
        auto mid = first + (last - first) / 2;
        auto last_el = last - 1;

        if (comp(*mid, *first))
        {
            std::iter_swap(mid, first);
        }
        if (comp(*last_el, *first))
        {
            std::iter_swap(last_el, first);
        }
        if (comp(*last_el, *mid))
        {
            std::iter_swap(last_el, mid);
        }

        return *mid;
    }

    template <std::random_access_iterator RandomIt, class Compare>
    requires std::sortable<RandomIt, Compare>
    auto partition_dnf(RandomIt first, RandomIt last,
                       typename std::iterator_traits<RandomIt>::value_type pivot,
                       Compare comp)
        -> std::pair<RandomIt, RandomIt>
    {
        auto current = first;
        auto greater_start = last;

        while (current != greater_start)
        {
            if (comp(*current, pivot))
            {
                std::iter_swap(current, first);
                ++first;
                ++current;
            }
            else if (comp(pivot, *current))
            {
                --greater_start;
                std::iter_swap(current, greater_start);
            }
            else
            {
                ++current;
            }
        }

        return std::make_pair(first, greater_start);
    }

    template <class ExecutionPolicy, std::random_access_iterator RandomIt, class Compare>
    requires std::sortable<RandomIt, Compare>
    auto quick_sort_impl(RandomIt first, RandomIt last, Compare comp)
        -> void
    {
        const auto len = std::distance(first, last);
        if (len <= INSERTION_SORT_THRESHOLD)
        {
            insertion_sort(first, last, comp);
            return;
        }

        auto pivot = select_pivot(first, last, comp);
        auto [left_bound, right_bound] = partition_dnf(first, last, pivot, comp);

        constexpr bool is_parallel_policy =
            std::is_same_v<ExecutionPolicy, std::execution::parallel_policy> ||
            std::is_same_v<ExecutionPolicy, std::execution::parallel_unsequenced_policy>;

        if constexpr (is_parallel_policy)
        {
            if (len > PARALLELIZATION_THRESHOLD)
            {
                auto future = std::async(std::launch::async, [=]() {
                    quick_sort_impl<ExecutionPolicy>(first, left_bound, comp);
                });
               quick_sort_impl<ExecutionPolicy>(right_bound, last, comp);

                future.wait();
                return;
            }
        }

        quick_sort_impl<ExecutionPolicy>(first, left_bound, comp);
        quick_sort_impl<ExecutionPolicy>(right_bound, last, comp);
    }
}
