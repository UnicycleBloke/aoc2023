#include "utils.h"


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress);

    int p1 = 0;
    int p2 = 0;

    for (const auto& line: lines)
    {
        constexpr const char* red   = "red";
        constexpr const char* green = "green";
        constexpr const char* blue  = "blue";

        map<string, int> bag;
        bag[red]   = 0;
        bag[green] = 0;
        bag[blue]  = 0;

        auto game = aoc::split(line, ":");
        auto id   = stoi(aoc::split(game[0], " ")[1]);
        auto sets = aoc::split(game[1], ";");

        for (const auto& set: sets)
        {
            auto cubes = aoc::split(set, ",");
            for (const auto& cube: cubes)
            {
                auto subset = aoc::split(cube, " ");
                auto colour = subset[1];
                auto count  = stoi(subset[0]);

                bag[colour] = max(bag[colour], count);
            }
        }

        if ((bag[red] <= 12) && (bag[green] <= 13) && (bag[blue] <= 14))
        {
            p1 += id;
        }
        p2 += bag[red] * bag[green] * bag[blue];
    } 

    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 2'600);

    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 86'036);
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
