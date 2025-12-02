#pragma once

#include <functional>
#include "traits.hpp"


namespace task1::algorithm
{
    template<std::random_access_iterator RandomIt, typename Compare>
    requires traits::random_access_sortable<RandomIt, Compare>
    void quick_sort(RandomIt first, RandomIt last, Compare comp)
    {
    }

    template<std::random_access_iterator RandomIt>
    void quick_sort(RandomIt first, RandomIt last)
    {
        using T = typename std::iterator_traits<RandomIt>::value_type;
        quick_sort(first, last, std::less<T>{});
    }

    template<std::random_access_iterator RandomIt, typename Compare>
    requires std::sortable<RandomIt, Compare>
    void stable_sort(RandomIt first, RandomIt last, Compare comp)
    {
    }

    template<std::random_access_iterator RandomIt>
    void stable_sort(RandomIt first, RandomIt last)
    {
        using T = typename std::iterator_traits<RandomIt>::value_type;
        stable_sort(first, last, std::less<T>{});
    }

    template<std::random_access_iterator RandomIt, typename Compare>
    requires std::sortable<RandomIt, Compare>
    void heap_sort(RandomIt first, RandomIt last, Compare comp)
    {
    }

    template<std::random_access_iterator RandomIt>
    void heap_sort(RandomIt first, RandomIt last)
    {
        using T = typename std::iterator_traits<RandomIt>::value_type;
        heap_sort(first, last, std::less<T>{});
    }
}
