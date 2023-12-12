#include "utils.h"


struct Report
{
    string input;
    string corrupt;
    string pattern;
};


int matches(string corrupt, size_t index, const string& pattern)
{
    int result = 0;

    if (index >= corrupt.size())
    {
        regex  re(pattern);
        smatch m;
        return regex_match(corrupt, m, re) ? 1 : 0;
    }

    while ((index < corrupt.size()) && (corrupt[index] != '?')) ++index;
    
    corrupt[index] = '.';
    result += matches(corrupt, index, pattern);

    corrupt[index] = '#';
    result += matches(corrupt, index, pattern);

    return result;
}


template <typename T>
auto part1(const T& reports)
{
    aoc::timer timer;

    int total_ways = 0;
    for (const auto& r: reports)
    {
        auto ways = matches(r.corrupt, 0, r.pattern);
        cout << "ways = " << ways << '\n';
        total_ways += ways;
    }    

    return total_ways / 2;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    vector<Report> reports;
    for (auto line: lines)
    {
        auto s = aoc::split(line, " ");
        auto t = aoc::split(s[1], ",");
        ostringstream os;

        os << "\\.*";
        for (auto c: t)
        {
            for (auto d: aoc::range(stoi(c))) os << '#';
            os << "\\.+";
        }

        reports.push_back({line, s[0] + ".", os.str()});
    }

    auto p1 = part1(reports);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 0);

    vector<Report> reports2;
    for (auto line: lines)
    {
        auto s  = aoc::split(line, " ");
        auto s0 = s[0] + "?" + s[0] + "?" + s[0] + "?" + s[0] + "?" + s[0];
        auto s1 = s[1] + "," + s[1] + "," + s[1] + "," + s[1] + "," + s[1];    

        cout << s0 << ' ' << s1 << '\n';    

        auto t = aoc::split(s1, ",");
        ostringstream os;

        os << "\\.*";
        for (auto c: t)
        {
            for (auto d: aoc::range(stoi(c))) os << '#';
            os << "\\.+";
        }

        reports2.push_back({line, s0 + ".", os.str()});
    }

    auto p2 = part1(reports2);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 0);
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
