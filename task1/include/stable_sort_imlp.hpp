#pragma once

#include <algorithm>
#include <cstddef>
#include <execution>
#include <functional>
#include <future>
#include <iterator>
#include "traits.hpp"


namespace task1::algorithm::implementation
{
    constexpr std::ptrdiff_t STABLE_INSERTION_THRESHOLD = 32;
    constexpr std::ptrdiff_t STABLE_PARALLEL_THRESHOLD = 2048;

    template <class ExecutionPolicy, std::random_access_iterator RandomIt, class Compare>
    void stable_sort_impl(RandomIt first, RandomIt last, Compare comp)
    {
        auto const len = std::distance(first, last);
        if (len <= STABLE_INSERTION_THRESHOLD)
        {
            insertion_sort(first, last, comp);
            return;
        }

        auto mid = first + len / 2;
        constexpr bool is_parallel_policy =
            std::is_same_v<ExecutionPolicy, std::execution::parallel_policy> ||
            std::is_same_v<ExecutionPolicy, std::execution::parallel_unsequenced_policy>;

        if constexpr (is_parallel_policy)
        {
            if (len > STABLE_PARALLEL_THRESHOLD)
            {
                auto future = std::async(std::launch::async, [=]() {
                    stable_sort_impl<ExecutionPolicy>(first, mid, comp);
                });
                stable_sort_impl<ExecutionPolicy>(mid, last, comp);
                future.wait();

                std::inplace_merge(first, mid, last, comp);
                return;
            }
        }

        stable_sort_impl<ExecutionPolicy>(first, mid, comp);
        stable_sort_impl<ExecutionPolicy>(mid, last, comp);
        std::inplace_merge(first, mid, last, comp);
    }
}
