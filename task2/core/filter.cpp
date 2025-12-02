#include <string>
#include <sstream>
#include <unordered_set>

#include "filter.hpp"
#include "hash.hpp"
#include "structs.hpp"


namespace task2
{
    void filter_duplicates(std::istream &input, std::ostream &output)
    {
        std::unordered_set<geometry::Point3, hash::Point3Hash> seen;
        seen.reserve(1000);

        std::string line;
        geometry::Point3 temp_point{};

        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::stringstream ss(line);
            if (ss >> temp_point.x >> temp_point.y >> temp_point.z)
            {
                if (seen.insert(temp_point).second)
                {
                    output << line << '\n';
                }
            }
        }
    }
}
