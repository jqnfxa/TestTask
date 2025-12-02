#pragma once

#include <concepts>


namespace task1::traits
{
template<typename It, typename Comp>
concept random_access_sortable =
    std::random_access_iterator<It> &&
    std::sortable<It, Comp>;
}
