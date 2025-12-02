#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>
#include <algorithm.hpp>


std::vector<int> generate_random_ints(std::size_t size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10000, 10000);

    std::vector<int> data(size);
    for (auto &val : data)
    {
        val = dis(gen);
    }

    return data;
}

std::vector<std::string> generate_random_strings(std::size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);
    std::uniform_int_distribution<> len_dis(1, 20);

    std::vector<std::string> data(size);
    for (auto &str : data)
    {
        const auto len = static_cast<std::size_t>(len_dis(gen));
        str.resize(len);
        for (std::size_t i = 0; i < len; ++i)
        {
            str[i] = charset[dis(gen)];
        }
    }
    return data;
}

template <typename SortFunc>
void RunSortTests(SortFunc sort_func, const std::string &algo_name)
{
    /* Empty */
    {
        std::vector<int> data;
        sort_func(data.begin(), data.end());
        EXPECT_TRUE(data.empty()) << algo_name << " failed on empty vector";
    }

    /* Sorted */
    {
        std::vector<int> data = {1, 2, 3, 4, 5};
        std::vector<int> expected = data;
        sort_func(data.begin(), data.end());
        EXPECT_EQ(data, expected) << algo_name << " failed on sorted vector";
    }

    /* Reversed */
    {
        std::vector<int> data = {5, 4, 3, 2, 1};
        std::vector<int> expected = {1, 2, 3, 4, 5};
        sort_func(data.begin(), data.end());
        EXPECT_EQ(data, expected) << algo_name << " failed on reverse sorted vector";
    }

    /* Random shuffle (int) */
    {
        auto data = generate_random_ints(1000);
        auto copy_std = data;

        sort_func(data.begin(), data.end());
        std::sort(copy_std.begin(), copy_std.end());

        EXPECT_EQ(data, copy_std) << algo_name << " results differ from std::sort (int)";
    }

    /* Random shuffle strings */
    {
        auto data = generate_random_strings(100);
        auto copy_std = data;

        sort_func(data.begin(), data.end());
        std::sort(copy_std.begin(), copy_std.end());

        EXPECT_EQ(data, copy_std) << algo_name << " results differ from std::sort (string)";
    }
}

template <typename SortFunc>
void RunComparatorTests(SortFunc sort_func, const std::string &algo_name)
{
    /* Random (int) descending order */
    {
        auto data = generate_random_ints(1000);
        auto expected = data;
        std::sort(expected.begin(), expected.end(), std::greater<int>{});
        sort_func(data.begin(), data.end(), std::greater<int>{});

        EXPECT_EQ(data, expected) << algo_name << " failed with custom comparator";
    }
}

/* Suite */
TEST(Task1_QuickSort, Standard)
{
    RunSortTests([](auto f, auto l) { task1::algorithm::quick_sort(f, l); }, "QuickSort");
}
TEST(Task1_QuickSort, WithComparator)
{
    RunComparatorTests([](auto f, auto l, auto c) { task1::algorithm::quick_sort(f, l, c); }, "QuickSort");
}
TEST(Task1_QuickSort, Parallel_Execution)
{
    std::vector<int> data = generate_random_ints(5000);
    auto expected = data;
    std::sort(expected.begin(), expected.end());
    task1::algorithm::quick_sort(std::execution::par, data.begin(), data.end());
    EXPECT_EQ(data, expected);
}

TEST(Task1_StableSort, Standard)
{
    RunSortTests([](auto f, auto l) { task1::algorithm::stable_sort(f, l); }, "StableSort");
}
TEST(Task1_StableSort, WithComparator)
{
    RunComparatorTests([](auto f, auto l, auto c) { task1::algorithm::stable_sort(f, l, c); }, "StableSort");
}

TEST(Task1_HeapSort, Standard)
{
    RunSortTests([](auto f, auto l) { task1::algorithm::heap_sort(f, l); }, "HeapSort");
}
TEST(Task1_HeapSort, WithComparator)
{
    RunComparatorTests([](auto f, auto l, auto c) { task1::algorithm::heap_sort(f, l, c); }, "HeapSort");
}
