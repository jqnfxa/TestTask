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
    template <std::random_access_iterator RandomIt, class Compare>
    void sift_down(RandomIt first, std::ptrdiff_t n, std::ptrdiff_t index, Compare comp)
    {
        while (true)
        {
            std::ptrdiff_t largest = index;
            std::ptrdiff_t left_child = 2 * index + 1;
            std::ptrdiff_t right_child = 2 * index + 2;

            if (left_child < n && comp(*(first + largest), *(first + left_child)))
            {
                largest = left_child;
            }
            if (right_child < n && comp(*(first + largest), *(first + right_child)))
            {
                largest = right_child;
            }

            if (largest != index)
            {
                std::iter_swap(first + index, first + largest);
                index = largest;
            }
            else
            {
                break;
            }
        }
    }

    template <std::random_access_iterator RandomIt, class Compare>
    void make_heap_manual(RandomIt first, RandomIt last, Compare comp)
    {
        const auto n = std::distance(first, last);
        for (std::ptrdiff_t i = n / 2 - 1; i >= 0; --i)
        {
            sift_down(first, n, i, comp);
        }
    }

    template <std::random_access_iterator RandomIt, class Compare>
    void heap_sort_impl(RandomIt first, RandomIt last, Compare comp)
    {
        if (first == last)
        {
            return;
        }

        const auto n = std::distance(first, last);
        make_heap_manual(first, last, comp);

        for (std::ptrdiff_t i = n - 1; i > 0; --i)
        {
            std::iter_swap(first, first + i);
            sift_down(first, i, 0, comp);
        }
    }
}
