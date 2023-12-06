#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int64_t result = 1;

    for (auto r: aoc::range(input.time.size()))
    {
        auto time = input.time[r];
        auto dist = input.dist[r];

        int64_t ways = 0;
        for (auto c: aoc::range(0, time))
        {
            auto d = c * (time - c);
            if (d > dist) ++ways;
        }

        result *= ways;
    }

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    // Charge time is c
    // Record distance is Dist
    // Time allowed for the race is Time
    // Actual distance d = c * (Time - c)
    // We want d > Dist
    // Find the zeroes of Dist - d 
    // Solve the quadratic: c*c - Time*c + Dist = 0

    int64_t Time = input.time2;
    int64_t Dist = input.dist2;

    // I have assumed that there are no issues with rounding down (that worked).
    // I guess the input was designed such that the discriminant is a whole number.
    int64_t tmin = (Time - sqrt(Time*Time - 4*Dist)) / 2;
    int64_t tmax = (Time + sqrt(Time*Time - 4*Dist)) / 2;

    // The number of ways to win. Thought I might be off by one here but it worked.
    return tmax - tmin;
}


struct Input
{
    vector<int> time;
    vector<int> dist;

    int64_t time2;
    int64_t dist2;
};


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);
    auto time  = aoc::split(lines[0], " ");
    auto dist  = aoc::split(lines[1], " ");

    Input input;
    ostringstream t2;
    ostringstream d2;
    for (auto i: aoc::range(1U, time.size()))
    { 
        input.time.push_back(stoi(time[i]));
        input.dist.push_back(stoi(dist[i]));

        t2 << time[i];
        d2 << dist[i];
    }

    input.time2 = stoll(t2.str()); 
    input.dist2 = stoll(d2.str()); 

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 1312850);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 36749103);
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
