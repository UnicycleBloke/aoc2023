#include "utils.h"


int64_t dist_from_hex(const string& str)
{
    int64_t x;   
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;
    return x;
}


char dir_from_hex(char hex)
{
    switch (hex)
    {
        case '0': return 'R';
        case '1': return 'D';
        case '2': return 'L';
        case '3': return 'U';
    }
    return 'U';
}


template <typename T>
auto solve(T& input, bool is_part2)
{
    aoc::timer timer;

    int64_t row = 0;
    int64_t col = 0;

    using Point = pair<int64_t, int64_t>;
    vector<Point> points;
    points.push_back({row, col});
   
    for (auto line: input)
    {
        auto s    = aoc::split(line, " ");
        auto dir  = s[0][0];
        auto dist = stoi(s[1]);
        if (is_part2)
        {
            dist = dist_from_hex(s[2].substr(2, 5));
            dir  = dir_from_hex(s[2][7]);
        }

        switch (dir)
        {
            case 'L': col -= dist; break;
            case 'R': col += dist; break;
            case 'U': row -= dist; break;
            case 'D': row += dist; break;
        }

        points.push_back({row, col});
    }
    points.push_back({0, 0});

    // This uses the shoelace formula with a simple modification to deal with lattice points. 
    // The area of a shape is the overall footprint if it is shifted 1 step R, then D, then L, then U.
    // I reasoned that extruding the shape by a half cell all the way around would have the same effect.
    // I have added the length of the periphery, which amounts to the same thing. This addition is halved
    // at the end, so that works out. The result was off by one for the test data and the input for Part1, 
    // so I added 1 at the end. Probably need to think a bit more about this...
    int64_t area = 0;
    int64_t edge = 0;
    for (auto i: aoc::range(points.size()-1))
    {
        auto [x1, y1] = points[i];
        auto [x2, y2] = points[i+1];

        area += (y1 + y2) * (x1 - x2); 
        edge += abs(x1 - x2) + abs(y1 - y2);
    }

    return (abs(area) + abs(edge)) / 2 + 1;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    auto p1 = solve(lines, false);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 56923);

    auto p2 = solve(lines, true);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 66296566363189);
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
