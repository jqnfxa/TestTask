#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "concave_hull.hpp"
#include "geometry.hpp"


std::vector<task3::geometry::Point> read_points(std::istream &input)
{
    std::vector<task3::geometry::Point> points;
    task3::geometry::Point p;

    while (input >> p.x >> p.y)
    {
        points.push_back(p);
    }

    return points;
}

void write_hull(std::ostream &output, const std::vector<task3::geometry::Point> &hull)
{
    output << std::fixed << std::setprecision(6);
    for (const auto &p : hull)
    {
        output << p.x << " " << p.y << '\n';
    }
}

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc < 3 || argc > 4)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> [gamma]\n";
        std::cerr << "  gamma (optional): detail level. Default is 1.0.\n";
        return 1;
    }

    const std::filesystem::path input_path = argv[1];
    const std::filesystem::path output_path = argv[2];

    double gamma = 1.5;
    if (argc == 4)
    {
        try
        {
            gamma = std::stod(argv[3]);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "[-] Error: Invalid gamma value. Must be a number.\n";
            return 1;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "[-] Error: Gamma value is out of range.\n";
            return 1;
        }
    }

    std::ifstream input_file(input_path);
    if (!input_file.is_open())
    {
        std::cerr << "[-]: Error: Could not open input file: " << input_path << '\n';
        return 1;
    }

    std::vector<task3::geometry::Point> points;
    try
    {
        points = read_points(input_file);
        std::cout << "[+] Read " << points.size() << " points from input file.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "[-] Error reading points: " << e.what() << '\n';
        return 1;
    }
    input_file.close();

    if (points.size() < 3)
    {
        std::cerr << "[-] Error: Need at least 3 points to compute a hull.\n";
        return 1;
    }

    std::vector<task3::geometry::Point> hull;
    try
    {
        std::cout << "[+] Computing concave hull with gamma = " << gamma << '\n';
        hull = task3::algorithm::compute_concave_hull(points, gamma);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[-] Error during hull computation: " << e.what() << '\n';
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
        write_hull(output_file, hull);
        std::cout << "[+] Done: Wrote " << hull.size() << " hull vertices to " << output_path << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << "[-] Error writing to output file: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
