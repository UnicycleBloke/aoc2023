#include "utils.h"


template <typename T, typename U>
size_t accept_or_reject(T& flows, const string& name, const U& part)
{
    auto [x, m, a, s] = part;

    if (name == "R")
        return 0;
    else if (name == "A")
        return x + m + a + s;

    const auto& rules = flows[name];
    for (const auto& rule: rules)
    {
        if ((rule.size() >= 2) && (rule[1] == '>'))
        {
            auto [var, num, dst] = aoc::parse_line<char, int, string>(R"((\w).(\d+):(\w+))", rule);
            switch (var)
            {
                case 'x': if (x > num) return accept_or_reject(flows, dst, part); break;
                case 'm': if (m > num) return accept_or_reject(flows, dst, part); break;
                case 'a': if (a > num) return accept_or_reject(flows, dst, part); break;
                case 's': if (s > num) return accept_or_reject(flows, dst, part); break;
            }
        }
        else if ((rule.size() >= 2) && (rule[1] == '<'))
        {
            auto [var, num, dst] = aoc::parse_line<char, int, string>(R"((\w).(\d+):(\w+))", rule);
            switch (var)
            {
                case 'x': if (x < num) return accept_or_reject(flows, dst, part); break;
                case 'm': if (m < num) return accept_or_reject(flows, dst, part); break;
                case 'a': if (a < num) return accept_or_reject(flows, dst, part); break;
                case 's': if (s < num) return accept_or_reject(flows, dst, part); break;
            }
        }
        else 
        {
            return accept_or_reject(flows, rule, part);
        }
    }

    return 0;
}


struct Range
{
    size_t min_x{1};
    size_t min_m{1};
    size_t min_a{1};
    size_t min_s{1};

    size_t max_x{4000};
    size_t max_m{4000};
    size_t max_a{4000};
    size_t max_s{4000};
};


template <typename T>
size_t accept_or_reject2(T& flows, const string& name, Range& range)
{
    if (name == "R")
        return 0;

    if (name == "A")
    {
        auto [min_x, min_m, min_a, min_s, max_x, max_m, max_a, max_s] = range;
        return (max_x-min_x+1) * (max_m-min_m+1) * (max_a-min_a+1) * (max_s-min_s+1);
    }

    size_t result = 0;
    Range range3 = range;

    const auto& rules = flows[name];
    for (const auto& rule: rules)
    {
        if ((rule.size() >= 2) && (rule[1] == '>'))
        {
            auto [var, num, dst] = aoc::parse_line<char, size_t, string>(R"((\w).(\d+):(\w+))", rule);
            switch (var)
            {
                case 'x':
                {
                    Range range2 = range3;
                    range2.min_x = max(range2.min_x, num+1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.max_x = min(range3.max_x, num);
                    break;
                }
                case 'm':
                {
                    Range range2 = range3;
                    range2.min_m = max(range2.min_m, num+1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.max_m = min(range3.max_m, num);
                    break;
                }
                case 'a':
                {
                    Range range2 = range3;
                    range2.min_a = max(range2.min_a, num+1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.max_a = min(range3.max_a, num);
                    break;
                }
                case 's':
                {
                    Range range2 = range3;
                    range2.min_s = max(range2.min_s, num+1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.max_s = min(range3.max_s, num);
                    break;
                }
            }
        }
        else if ((rule.size() >= 2) && (rule[1] == '<'))
        {
            auto [var, num, dst] = aoc::parse_line<char, size_t, string>(R"((\w).(\d+):(\w+))", rule);
            switch (var)
            {
                case 'x':
                {
                    Range range2 = range3;
                    range2.max_x = min(range2.max_x, num-1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.min_x = max(range3.min_x, num);
                    break;
                }
                case 'm':
                {
                    Range range2 = range3;
                    range2.max_m = min(range2.max_m, num-1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.min_m = max(range3.min_m, num);
                    break;
                }
                case 'a':
                {
                    Range range2 = range3;
                    range2.max_a = min(range2.max_a, num-1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.min_a = max(range3.min_a, num);
                    break;
                }
                case 's':
                {
                    Range range2 = range3;
                    range2.max_s = min(range2.max_s, num-1);
                    result += accept_or_reject2(flows, dst, range2);
                    range3.min_s = max(range3.min_s, num);
                    break;
                }
            }
        }
        else 
        {
            result += accept_or_reject2(flows, rule, range);
        }
    }

    return result;
}


template <typename T, typename U>
auto part1(T& flows, const U& parts)
{
    aoc::timer timer;

    size_t result = 0;

    for (auto part: parts)
    {
        result += accept_or_reject(flows, "in", part);
    }

    return result;
}


template <typename T, typename U>
auto part2(T& flows, const U& parts)
{
    aoc::timer timer;

    Range range;
    return accept_or_reject2(flows, "in", range);
}


void run(const char* filename)
{
    auto f = aoc::read_lines<string,string>(filename, R"((\w+)\{(.+)\})");
    //for (auto [a, b]: flows) cout << a << " " << b << "\n";
    map<string, vector<string>> flows;
    for (auto [name, rules]: f)
        flows[name] = aoc::split(rules, ",");

    auto parts = aoc::read_lines<int,int,int,int>(filename, R"(\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\})");
    //for (auto [x, m, a, s]: parts) cout << x << " " << m << " " << a << " " << s << "\n";

    auto p1 = part1(flows, parts);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(flows, parts);
    cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 0);
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
