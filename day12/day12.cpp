#include "utils.h"


bool matches2(const string& input, size_t pos, size_t len)
{
    // Ensure that the preceding character can be a . (need . or ?)
    if ((pos > 0) && (input[pos-1] == '#')) return false;
    // Can't match if the group overruns the input
    if ((pos + len) > input.size()) return false;
    // Ensure that the group items can all be # (need # or ?)
    for (int i = 0; i < len; ++i) if (input[pos+i] == '.') return false;   
    // If we are the end of the input there is no need for a following .
    if ((pos + len) == input.size()) return true;
    // Ensure that the following character can be a . (need . or ?)
    return (input[pos + len] != '#');
}


// After a day of barking up the wrong tree I remembered that memoising is a good solution.
// I really must get this technique into my regular vocabulary.
using Memo    = pair<size_t, size_t>;
using MemoMap = map<Memo, size_t>;


size_t calculate(const string& input, const vector<size_t>& groups, size_t pos, size_t grp, MemoMap& memo_map)
{
    Memo memo = make_pair(pos, grp);
    if (memo_map.find(memo) != memo_map.end()) return memo_map[memo];

    if (grp >= groups.size())
    {
        for (auto p: aoc::range(pos, input.size())) 
            if (input[p] == '#') return 0;
        return 1;
    }

    size_t result = 0;
    while (pos < input.size())
    {
        if (matches2(input, pos, groups[grp]))
        {
            result += calculate(input, groups, pos + groups[grp] + 1, grp + 1, memo_map);
        }

        if (input[pos] == '#') break;
        ++pos;
    }

    memo_map[memo] = result;
    return result;
}


size_t calculate(const string& s0, const string& s1)
{
    auto t = aoc::split(s1, ",");

    vector<size_t> groups;
    ostringstream os;
    os << "\\.*";
    for (auto c: t)
    {
        groups.push_back(stoi(c));
        for (auto d: aoc::range(stoi(c))) os << '#';
        os << "\\.+";
    }
    auto corrupt = s0 + ".";
    auto pattern = os.str(); 

    MemoMap memo_map;
    return calculate(corrupt, groups, 0, 0, memo_map);
}


template <typename T>
auto part1(const T& lines)
{
    aoc::timer timer;

    size_t result = 0;
    for (auto line: lines)
    {
        auto s  = aoc::split(line, " ");
        auto s0 = s[0];
        auto s1 = s[1];
        result += calculate(s0, s1);    
    }    

    return result;
}


template <typename T>
auto part2(T& lines)
{
    aoc::timer timer;

    size_t result = 0;
    for (auto line: lines)
    {
        auto s  = aoc::split(line, " ");
        auto s0 = s[0] + "?" + s[0] + "?" + s[0] + "?" + s[0] + "?" + s[0];
        auto s1 = s[1] + "," + s[1] + "," + s[1] + "," + s[1] + "," + s[1];
        result += calculate(s0, s1);    
    }    

    return result;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 7191U);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 6512849198636U);
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
