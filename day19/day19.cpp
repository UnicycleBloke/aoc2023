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
    pair<size_t, size_t> x{0, 4000};
    pair<size_t, size_t> m{0, 4000};
    pair<size_t, size_t> a{0, 4000};
    pair<size_t, size_t> s{0, 4000};
};


// Pretty sure I made this much harder than it needed to be. Very fiddly making sure the 
// volume shrinkage is cumulative during the application of rules for a work flow.
template <typename T>
size_t accept_or_reject2(T& flows, const string& name, Range& range)
{
    if (name == "R")
        return 0;

    if (name == "A")
    {
        auto [x, m, a, s] = range;
        auto [x1, x2] = x;
        auto [m1, m2] = m;
        auto [a1, a2] = a;
        auto [s1, s2] = s;
        auto vol = (x2-x1) * (m2-m1) * (a2-a1) * (s2-s1);
        return vol;
    }

    size_t result = 0;
    Range range2 = range;

    auto intersect = [](pair<size_t, size_t> a, pair<size_t, size_t> b)
    {
        auto [a1, a2] = a;
        auto [b1, b2] = b;

        auto c1 = max(a1, b1);
        auto c2 = min(a2, b2);
        c1 = min(c1, c2);
        return make_pair(c1, c2); 
    };

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
                    Range range3 = range2;
                    range3.x = intersect(range2.x, {num, 4000});
                    range2.x = intersect(range2.x, {0, num});
                    result += accept_or_reject2(flows, dst, range3);
                    break;
                }
                case 'm':
                {
                    Range range3 = range2;
                    range3.m = intersect(range2.m, {num, 4000});
                    range2.m = intersect(range2.m, {0, num});
                    result += accept_or_reject2(flows, dst, range3);
                    break;
                }
                case 'a':
                {
                    Range range3 = range2;
                    range3.a = intersect(range2.a, {num, 4000});
                    range2.a = intersect(range2.a, {0, num});
                    result += accept_or_reject2(flows, dst, range3);
                    break;
                }
                case 's':
                {
                    Range range3 = range2;
                    range3.s = intersect(range2.s, {num, 4000});
                    range2.s = intersect(range2.s, {0, num});
                    result += accept_or_reject2(flows, dst, range3);
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
                    Range range3 = range2;
                    range3.x = intersect(range2.x, {0, num-1});
                    range2.x = intersect(range2.x, {num-1, 4000});
                    result += accept_or_reject2(flows, dst, range3);
                    break;
                }
                case 'm':
                {
                    Range range3 = range2;
                    range3.m = intersect(range2.m, {0, num-1});
                    range2.m = intersect(range2.m, {num-1, 4000});
                    result += accept_or_reject2(flows, dst, range3);
                    break;
                }
                case 'a':
                {
                    Range range3 = range2;
                    range3.a = intersect(range2.a, {0, num-1});
                    range2.a = intersect(range2.a, {num-1, 4000});
                    result += accept_or_reject2(flows, dst, range3);
                    break;
                }
                case 's':
                {
                    Range range3 = range2;
                    range3.s = intersect(range2.s, {0, num-1});
                    range2.s = intersect(range2.s, {num-1, 4000});
                    result += accept_or_reject2(flows, dst, range3);
                    break;
                }
            }
        }
        else 
        {
            result += accept_or_reject2(flows, rule, range2);
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
