#include "utils.h"


template <typename T>
auto tilt_north(T& input)
{
    // A bit long-winded but gets the job done.
    for (auto c: aoc::range(input[0].size()))
    {
        for (auto r: aoc::range(input.size()))
        {
            if (input[r][c] == '.')
            {
                auto r2 = r + 1;
                while ((r2 < input.size()) && (input[r2][c] == '.'))
                {
                    ++r2;
                }
                if ((r2 < input.size()) && (input[r2][c] == 'O'))
                {
                    input[r2][c] = '.';
                    input[r][c]  = 'O';
                }
            }
        }
    }
}


template <typename T>
auto tilt_south(T& input)
{
    // It was taking too long to work out the logic, so I learned from an earlier day.
    // Just reverse the input!
    reverse(input.begin(), input.end());
    tilt_north(input);
    reverse(input.begin(), input.end());
}


template <typename T>
auto tilt_west(T& input)
{
    // Easy duplicate of tilt_north.
    for (auto r: aoc::range(input.size()))
    {
        for (auto c: aoc::range(input[0].size()))
        {
            if (input[r][c] == '.')
            {
                auto c2 = c + 1;
                while ((c2 < input[0].size()) && (input[r][c2] == '.'))
                {
                    ++c2;
                }
                if ((c2 < input[0].size()) && (input[r][c2] == 'O'))
                {
                    input[r][c2] = '.';
                    input[r][c]  = 'O';
                }
            }
        }
    }
}


template <typename T>
auto tilt_east(T& input)
{
    // Reverse trick a bit more fiddly but well worth it.
    for (auto r: aoc::range(input.size()))
        reverse(input[r].begin(), input[r].end());
    tilt_west(input);
    for (auto r: aoc::range(input.size()))
        reverse(input[r].begin(), input[r].end());
}


template <typename T>
auto calculate_load(const T& input)
{
    size_t load = 0;
    for (auto c: aoc::range(input[0].size()))
    {
        for (auto r: aoc::range(input.size()))
        {
            if (input[r][c] == 'O')
                load += input.size() - r;
        }
    }
    return load;
}


template <typename T>
auto part1(T input)
{
    aoc::timer timer;
   
    tilt_north(input);
    return calculate_load(input);
}


bool matches(const vector<size_t>& loads, size_t off, size_t len)
{
    // A little nod to memory safety.
    if ((off + len + len) > loads.size()) return false;

    for (auto i: aoc::range(len))
    {
        if (loads[off + i] != loads[off + i + len])
            return false;
    }
    return true;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    // A quick test shows that load values fall into a repeating cycle after a while. 
    // Could extract the offset and repeat length manually but ... 

    // Calculate enough loads to show a few cycles of the repeating pattern.
    // Finger in the air...
    vector<size_t> loads;
    for (auto cycle: aoc::range(1000))
    {
        tilt_north(input);
        tilt_west(input);
        tilt_south(input);
        tilt_east(input);

        auto load = calculate_load(input);
        loads.push_back(load);
    }

    // Scan to work out the offset and length of the repeating pattern. 
    for (auto off: aoc::range(loads.size()))
    {
        for (auto len: aoc::range(1U, loads.size() - off))
        {
            // This code doesn't do proper range checking. We assume that the 
            // vector is long enough to contain three repeats of the pattern. 
            // Naughty programer!! 
            
            // Checking multiple patterns to eliminate spurious short lengths 
            // due to a repeating subset - this affected the sample data.
            // It's not a general solution but did the job.
            if ( matches(loads, off, len) && 
                 matches(loads, off + len, len)   &&
                 matches(loads, off + 2*len, len) )
            {
                size_t index = (1'000'000'000U - off) % len + off - 1;
                return loads[index];
            }
        }
    }

    return size_t{};
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 110565U);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 89845U);
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
