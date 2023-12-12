#include "utils.h"


struct Report
{
    string corrupt;
    string pattern;
    vector<int> groups;
};


int matches(string corrupt, size_t index, const string& pattern, const vector<int>& groups)
{
    int result = 0;

    // check whether the first groups match (up to index)
    vector<int> g;
    size_t i = 0;
    int    num = 0;
    while (i < index)
    {
        if (corrupt[i] == '#')
        {
            ++num;
        }
        else if (corrupt[i] == '.')
        {
            if (num > 0) g.push_back(num);
            num = 0;
        }
        ++i;
    }
    for (auto i: aoc::range(g.size()))
        if (g[i] != groups[i]) return 0;


    if (index >= corrupt.size())
    {
        regex  re(pattern);
        smatch m;
        return regex_match(corrupt, m, re) ? 1 : 0;
    }

    while ((index < corrupt.size()) && (corrupt[index] != '?')) ++index;
    
    corrupt[index] = '.';
    result += matches(corrupt, index, pattern, groups);

    corrupt[index] = '#';
    result += matches(corrupt, index, pattern, groups);

    return result;
}


template <typename T>
auto part1(const T& lines)
{
    aoc::timer timer;

    vector<Report> reports;
    for (auto line: lines)
    {
        auto s = aoc::split(line, " ");
        auto t = aoc::split(s[1], ",");

        Report report;
        ostringstream os;
        os << "\\.*";
        for (auto c: t)
        {
            report.groups.push_back(stoi(c));
            for (auto d: aoc::range(stoi(c))) os << '#';
            os << "\\.+";
        }
        report.corrupt = s[0] + ".";
        report.pattern = os.str(); 
        reports.push_back(report);
    }

    int total_ways = 0;
    for (const auto& r: reports)
    {
        auto ways = matches(r.corrupt, 0, r.pattern, r.groups);
        cout << "ways = " << ways << '\n';
        total_ways += ways;
    }    

    return total_ways / 2;
}


template <typename T>
auto part2(T& lines)
{
    aoc::timer timer;

    vector<Report> reports;
    for (auto line: lines)
    {
        auto s  = aoc::split(line, " ");
        auto s0 = s[0] + "?" + s[0] + "?" + s[0] + "?" + s[0] + "?" + s[0];
        auto s1 = s[1] + "," + s[1] + "," + s[1] + "," + s[1] + "," + s[1];    
        auto t = aoc::split(s1, ",");

        Report report;
        ostringstream os;
        os << "\\.*";
        for (auto c: t)
        {
            report.groups.push_back(stoi(c));
            for (auto d: aoc::range(stoi(c))) os << '#';
            os << "\\.+";
        }
        report.corrupt = s0 + ".";
        report.pattern = os.str(); 
        reports.push_back(report);
    }

    int total_ways = 0;
    for (const auto& r: reports)
    {
        auto ways = matches(r.corrupt, 0, r.pattern, r.groups);
        cout << "ways = " << ways << '\n';
        total_ways += ways;
    }    

    return total_ways / 2;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 0);

    auto p2 = part2(lines);
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
