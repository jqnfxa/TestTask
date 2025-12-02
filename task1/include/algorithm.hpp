#pragma once

#include <cstddef>
#include <algorithm>
#include <future>
#include <functional>
#include <execution>
#include <iterator>
#include "traits.hpp"
#include "quick_sort_impl.hpp"


namespace task1::algorithm
{
    template <class ExecutionPolicy, std::random_access_iterator RandomIt, class Compare>
    requires std::is_execution_policy_v<std::decay_t<ExecutionPolicy>> &&
             traits::random_access_sortable<RandomIt, Compare>
    void quick_sort(ExecutionPolicy &&, RandomIt first, RandomIt last, Compare comp)
    {
        implementation::quick_sort_impl<std::decay_t<ExecutionPolicy>>(first, last, comp);
    }

    template <class ExecutionPolicy, std::random_access_iterator RandomIt>
    requires std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>
    void quick_sort(ExecutionPolicy &&policy, RandomIt first, RandomIt last)
    {
        using T = typename std::iterator_traits<RandomIt>::value_type;
        quick_sort(std::forward<ExecutionPolicy>(policy), first, last, std::less<T>{});
    }

    template<std::random_access_iterator RandomIt, class Compare>
    requires traits::random_access_sortable<RandomIt, Compare>
    void quick_sort(RandomIt first, RandomIt last, Compare comp)
    {
        implementation::quick_sort_impl<std::execution::sequenced_policy>(first, last, comp);
    }

    template<std::random_access_iterator RandomIt>
    void quick_sort(RandomIt first, RandomIt last)
    {
        using T = typename std::iterator_traits<RandomIt>::value_type;
        quick_sort(first, last, std::less<T>{});
    }

    template <std::random_access_iterator RandomIt, typename Compare>
    requires std::sortable<RandomIt, Compare>
    void stable_sort(RandomIt first, RandomIt last, Compare comp)
    {
    }

    template <std::random_access_iterator RandomIt>
    void stable_sort(RandomIt first, RandomIt last)
    {
        using T = typename std::iterator_traits<RandomIt>::value_type;
        stable_sort(first, last, std::less<T>{});
    }

    template <std::random_access_iterator RandomIt, typename Compare>
    requires std::sortable<RandomIt, Compare>
    void heap_sort(RandomIt first, RandomIt last, Compare comp)
    {
    }

    template <std::random_access_iterator RandomIt>
    void heap_sort(RandomIt first, RandomIt last)
    {
        using T = typename std::iterator_traits<RandomIt>::value_type;
        heap_sort(first, last, std::less<T>{});
    }
}
