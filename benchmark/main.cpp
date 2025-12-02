#include <algorithm>
#include <chrono>
#include <execution>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "algorithm.hpp"

using namespace std::chrono;

enum class DataType
{
    Integers,
    Strings
};

template <typename T>
std::vector<T> generate_data(std::size_t size, DataType type)
{
    std::vector<T> data;
    data.reserve(size);

    std::random_device rd;
    std::mt19937 gen(rd());

    if constexpr (std::is_same_v<T, int>)
    {
        std::uniform_int_distribution<> dis(-100000, 100000);
        for (std::size_t i = 0; i < size; ++i)
        {
            data.push_back(dis(gen));
        }
    }
    else
    {
        const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);
        std::uniform_int_distribution<> len_dis(5, 15);

        for (std::size_t i = 0; i < size; ++i)
        {
            std::string s;
            const auto len = static_cast<std::size_t>(len_dis(gen));
            s.resize(len);

            for (std::size_t j = 0; j < len; ++j)
            {
                s[j] = charset[dis(gen)];
            }
            data.push_back(std::move(s));
        }
    }

    return data;
}

template <typename Func>
long long measure_ms(Func func)
{
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start).count();
}

void print_result(const std::string &name, long long ms)
{
    std::cout << std::left << std::setw(35) << name
              << std::right << std::setw(10) << ms << " ms" << std::endl;
}


template <typename T>
void run_benchmark_suite(size_t size, const std::string &type_name)
{
    std::cout << " Benchmark " << size << " " << type_name << "\n";

    auto master_data = generate_data<T>(size, std::is_same_v<T, int> ? DataType::Integers : DataType::Strings);

    /* Quick sort */
    {
        std::cout << "Quick sort\n";

        auto d1 = master_data;
        print_result("std::sort", measure_ms([&]()
                                             { std::sort(d1.begin(), d1.end()); }));

        auto d2 = master_data;
        print_result("task1::quick_sort (seq)", measure_ms([&]() {
            task1::algorithm::quick_sort(d2.begin(), d2.end()); }));

        auto d3 = master_data;
        print_result("task1::quick_sort (par)", measure_ms([&]() {
            task1::algorithm::quick_sort(std::execution::par, d3.begin(), d3.end()); }));
    }

    /* Stable sort (merge sort )*/
    {
        std::cout << "\nStable sort\n";

        auto d1 = master_data;
        print_result("std::stable_sort", measure_ms([&]() {
            std::stable_sort(d1.begin(), d1.end()); }));

        auto d2 = master_data;
        print_result("task1::stable_sort (seq)", measure_ms([&]() {
            task1::algorithm::stable_sort(d2.begin(), d2.end()); }));

        auto d3 = master_data;
        print_result("task1::stable_sort (par)", measure_ms([&]() {
            task1::algorithm::stable_sort(std::execution::par, d3.begin(), d3.end()); }));
    }

    /* Heap sort */
    {
        std::cout << "\nHeap sort\n";

        auto d1 = master_data;
        print_result("std::make_heap + sort_heap", measure_ms([&]() {
            std::make_heap(d1.begin(), d1.end());
            std::sort_heap(d1.begin(), d1.end());
        }));

        auto d2 = master_data;
        print_result("task1::heap_sort (seq)", measure_ms([&]() {
            task1::algorithm::heap_sort(d2.begin(), d2.end()); }));
    }

    std::cout << '\n';
}

int main()
{
    run_benchmark_suite<int>(1'000'000, "Integers");
    run_benchmark_suite<std::string>(1'000'000, "Strings");

    run_benchmark_suite<int>(10'000'000, "Integers");
    run_benchmark_suite<std::string>(10'000'000, "Strings");
    return 0;
}
