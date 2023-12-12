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

    //cout << index << ' ' << corrupt << " .\n";

    if (index >= corrupt.size())
    {
        //cout << corrupt << " => " << pattern << '\n';

        regex  re(pattern);
        smatch m;
        bool is_match = regex_match(corrupt, m, re);
        if (is_match)
        {
            //cout << corrupt << " => " << pattern << '\n';
        }
        
        return is_match ? 1 : 0;
    }

    while ((index < corrupt.size()) && (corrupt[index] != '?')) ++index;
    
    corrupt[index] = 'x';
    //cout << corrupt << " .\n";
    result += matches(corrupt, index, pattern);

    corrupt[index] = '#';
    //cout << corrupt << " #\n";
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


// vector<tuple<Args...>>
//auto lines = aoc::read_lines<int,int,int,int>(filename, R"((\d+)-(\d+),(\d+)-(\d+))");

// vector<string>    
//auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

// Replace all substrings matching "search" with "replace".
//std::string replace(std::string source, const std::string& search, const std::string& replace);

// Split a delimited string of tokens into a vector of string tokens. Trims substrings by default and drops trimmed 
// tokens which are empty by default. Not convinced how useful the option are, but you never know.
//std::vector<std::string> split(std::string source, std::string delim, Blanks allow_blanks = Blanks::Suppress, Trim trim_subs = Trim::Yes);

// vector (size_type n, const value_type& val = value_type(),
//vector<int> row(COLS, 0);
//vector<vector<int>> grid(ROWS, row);
//grid[row][col];


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    vector<Report> reports;
    for (auto line: lines)
    {
        auto s = aoc::split(line, " ");
        auto t = aoc::split(s[1], ",");
        ostringstream os;

        // os << "\\.*";
        // for (auto c: t)
        // {
        //     for (auto d: aoc::range(stoi(c))) os << '#';
        //     os << "\\.+";
        // }

        os << "x*";
        for (auto c: t)
        {
            for (auto d: aoc::range(stoi(c))) os << '#';
            os << "x+";
        }

        auto x = aoc::replace(s[0], ".", "x");
        reports.push_back({line, x + "x", os.str()});
        //reports.push_back({line + ".", s[0], os.str()});

        cout << s[0] << ' ' << s[1] << ' ' << os.str() << '\n';
    }

    auto p1 = part1(reports);
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
