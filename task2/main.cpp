#include <filesystem>
#include <fstream>
#include <iostream>

#include "filter.hpp"


int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    const std::filesystem::path input_path = argv[1];
    const std::filesystem::path output_path = argv[2];

    std::ifstream input_file(input_path);
    if (!input_file.is_open())
    {
        std::cerr << "[-]: Error: Could not open input file: " << input_path << '\n';
        return 1;
    }

    std::ofstream output_file(output_path);
    if (!output_file.is_open())
    {
        std::cerr << "[-] Error: Could not open output file: " << output_path << '\n';
        return 1;
    }

    try
    {
        task2::filter_duplicates(input_file, output_file);
        std::cout << "[+] Done: " << output_path << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << "[-] Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
