#include <string>
#include <sstream>
#include <unordered_set>
#include <charconv>

#include "filter.hpp"
#include "hash.hpp"
#include "structs.hpp"


namespace task2
{
    inline const char *skip_whitespace(const char *ptr, const char *end)
    {
        while (ptr != end && std::isspace(*ptr))
        {
            ++ptr;
        }

        return ptr;
    }

    void filter_duplicates(std::istream &input, std::ostream &output)
    {
        std::unordered_set<geometry::Point3, hash::Point3Hash> seen;
        seen.reserve(1024 * 64);

        std::string line;
        line.reserve(64);

        geometry::Point3 point{};

        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }

            const char *ptr = line.data();
            const char *end = line.data() + line.size();

            /* X */
            ptr = skip_whitespace(ptr, end);
            auto res = std::from_chars(ptr, end, point.x);
            if (res.ec != std::errc())
            {
                continue;
            }
            ptr = res.ptr;

            /* Y */
            ptr = skip_whitespace(ptr, end);
            res = std::from_chars(ptr, end, point.y);
            if (res.ec != std::errc())
            {
                continue;
            }
            ptr = res.ptr;

            /* Z */
            ptr = skip_whitespace(ptr, end);
            res = std::from_chars(ptr, end, point.z);
            if (res.ec != std::errc())
            {
                continue;
            }

            if (seen.insert(point).second)
            {
                output << line << '\n';
            }
        }
    }
}
