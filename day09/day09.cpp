#include "utils.h"


template <typename T>
auto find_next(const T& diffs, size_t index)
{
    if (index >= diffs.size()) return 0L;
    return diffs[index].back() + find_next(diffs, index + 1U);
}


template <typename T>
auto find_prev(const T& diffs, size_t index)
{
    if (index >= diffs.size()) return 0L;
    return diffs[index].front() - find_prev(diffs, index + 1U);
}


template <typename T>
auto extrapolate(const T& scan)
{
    vector<vector<int64_t>> diffs;
    diffs.push_back(scan);

    bool all_zeroes = false;
    while (!all_zeroes)
    {
        const auto& row = diffs[diffs.size()-1U];

        all_zeroes = true;

        vector<int64_t> diff;
        for (auto i: aoc::range(1U, row.size()))
        {
            int64_t d = row[i] - row[i-1];
            if (d != 0) all_zeroes = false;
            diff.push_back(d);
        }

        diffs.push_back(diff);
    }

    return diffs;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;
 
    int64_t total = 0;
    for (const auto& scan: input)
    {
        auto diffs = extrapolate(scan);
        total += find_next(diffs, 0);
    }

    return total;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    int64_t total = 0;
    for (const auto& scan: input)
    {
        auto diffs = extrapolate(scan);
        total += find_prev(diffs, 0);
    }

    return total;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    vector<vector<int64_t>> input;
    for (const auto& line: lines)
    {        
        auto s = aoc::split(line, " ");
     
        vector<int64_t> v;
        for (const auto& i: s)
        {
            int64_t val = stoll(i);
            v.push_back(val);
        }
        input.push_back(v);
    }

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 1901217887);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 905);
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    try
    {
        if (argc < 2)
        {
            cout << "Provide input file name\n";
            return -1;
        }

        fs::path path{argv[1]};
        if (!fs::is_regular_file(path))
        {
            cout << "Path '" << argv[1] << "' does not exist or is not a file\n";
            return -1;
        }

        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}
