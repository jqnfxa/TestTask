#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cstddef>

constexpr std::size_t HISTORY_SIZE = 10000;
constexpr int DUPLICATE_PERCENT = 20;

struct Point
{
    float x, y, z;
};

std::string point_to_string(const Point &p)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1)
       << p.x << " " << p.y << " " << p.z;
    return ss.str();
}

int main(int argc, char *argv[])
{
    std::size_t num_rows = 1'000'000;
    std::string filename = "Test_data_duplicate.txt";

    if (argc > 1)
    {
        try
        {
            num_rows = static_cast<std::size_t>(std::stod(argv[1]));
        }
        catch (...)
        {
            std::cerr << "Invalid number format. Usage: ./generator <count> [filename]\n";
            return 1;
        }
    }
    if (argc > 2)
    {
        filename = argv[2];
    }

    std::cout << "Generating " << num_rows << " lines to '" << filename << "'...\n";
    std::cout << "Target Duplicate Rate: " << DUPLICATE_PERCENT << "%\n";

    std::ofstream out(filename);
    if (!out)
    {
        std::cerr << "Error opening file for writing.\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1000.0f, 1000.0f);
    std::uniform_int_distribution<int> chance(1, 100);
    std::uniform_int_distribution<size_t> history_dist(0, HISTORY_SIZE - 1);

    std::vector<std::string> history;
    history.reserve(HISTORY_SIZE);
    out.sync_with_stdio(false);

    for (size_t i = 0; i < num_rows; ++i)
    {
        bool do_duplicate = (chance(gen) <= DUPLICATE_PERCENT) && !history.empty();

        if (do_duplicate)
        {
            size_t idx = history_dist(gen) % history.size();
            out << history[idx] << "\n";
        }
        else
        {
            Point p{dist(gen), dist(gen), dist(gen)};
            std::string line = point_to_string(p);
            out << line << "\n";

            if (history.size() < HISTORY_SIZE)
            {
                history.push_back(line);
            }
            else
            {
                history[history_dist(gen)] = line;
            }
        }

        if (i % 1000000 == 0 && i > 0)
        {
            std::cout << "Generated " << i << " rows\r" << std::flush;
        }
    }

    std::cout << "\nFile size: " << std::filesystem::file_size(filename) / (1024 * 1024) << " MB\n";
    return 0;
}
