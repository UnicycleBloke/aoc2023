#include "utils.h"


template <typename T>
auto part1(const string& dirs, T& net)
{
    aoc::timer timer;

    string pos   = "AAA";
    size_t steps = 0;

    while (pos != "ZZZ")
    {
        for (auto d: dirs)
        {
            switch (d)
            {
                case 'L': pos = net[pos].left;  break; 
                case 'R': pos = net[pos].right; break; 
            }
            ++steps;

            if (pos == "ZZZ") break;
        }
    }

    return steps;
}


template <typename T>
auto part2_impl(const string& dirs, T& net, string pos)
{
    size_t steps = 0;

    while (pos[2] != 'Z')
    {
        for (auto d: dirs)
        {
            switch (d)
            {
                case 'L': pos = net[pos].left;  break; 
                case 'R': pos = net[pos].right; break; 
            }
            ++steps;

            if (pos[2] == 'Z') break;
        }
    }

    return steps;
}


template <typename T>
auto part2(const string& dirs, T& net)
{
    aoc::timer timer;

    // Find all the starting positions.
    vector<string> pos;
    for (auto [k, v]: net)
    {
        if (k[2] == 'A') pos.push_back(k);
    }

    // Just do Part 1 for each path to find its period. Doing them all at 
    // once as the instructions say will take forever to run.
    vector<size_t> steps;
    for (auto i: aoc::range{pos.size()})
    {
        steps.push_back(part2_impl(dirs, net, pos[i]));
    }

    // Took a while to get this right. Just finding the GCD and dividing at the 
    // end gave a number that was too high. There is something I haven't quite
    // understood about combining GCDs. Doing the thing step wise was fine.
    size_t result = steps[0];
    for (auto i: aoc::range{1U, steps.size()})
    {
        auto gcd = aoc::gcd<size_t>(result, steps[i]);
        result = result * steps[i] / gcd;
    }

    return result;
}


struct Dir
{
    string left;
    string right;
};


void run(const char* filename)
{
    auto lines2 = aoc::read_lines<string, string, string>(filename, R"((\w+) = \((\w+), (\w+)\))");
    map<string, Dir> net;
    for (auto [a, b, c]: lines2)
        net[a] = Dir{b, c};

    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    auto dirs  = lines[0];

    auto p1 = part1(dirs, net);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 16531U);

    auto p2 = part2(dirs, net);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 24035773251517U);
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
