#include "utils.h"


bool is_symbol(char c)
{
    if (c == '.') return false;
    if (isdigit(c)) return false;
    return true;
}


// Is this location adjacent to a symbol?
template <typename T>
bool is_valid(const T& rows, size_t r, size_t c)
{
    if (is_symbol(rows[r-1][c+1])) return true;
    if (is_symbol(rows[r-1][c]))   return true;
    if (is_symbol(rows[r-1][c-1])) return true;

    if (is_symbol(rows[r  ][c+1])) return true;
    //if (is_symbol(rows[r  ][c]))   return true;
    if (is_symbol(rows[r  ][c-1])) return true;

    if (is_symbol(rows[r+1][c+1])) return true;
    if (is_symbol(rows[r+1][c]))   return true;
    if (is_symbol(rows[r+1][c-1])) return true;

    return false;
}


// Is this location adjacent to a *?
template <typename T>
bool has_star(const T& rows, size_t r, size_t c)
{
    if ((rows[r-1][c+1]) == '*') return true;
    if ((rows[r-1][c  ]) == '*') return true;
    if ((rows[r-1][c-1]) == '*') return true;

    if ((rows[r  ][c+1]) == '*') return true;
    //if ((rows[r  ][c  ]) == '*') return true;
    if ((rows[r  ][c-1]) == '*') return true;

    if ((rows[r+1][c+1]) == '*') return true;
    if ((rows[r+1][c  ]) == '*') return true;
    if ((rows[r+1][c-1]) == '*') return true;

    return false;
}


// Concatenate digit sequences and keep a record of 
// whether any digits are adjacent to a symbol.
template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    size_t total = 0;

    auto rows = input.size();
    auto cols = input[0].size();
    for (auto r: aoc::range(rows))
    {
        const auto& row = input[r];

        size_t num   = 0;
        bool   valid = false;

        for (auto c: aoc::range(cols))
        {
            if (isdigit(row[c]))
            {
               num = num * 10 + (row[c] - '0');
               valid |= is_valid(input, r, c);
            }
            else 
            {
                if (valid) total += num;
                num   = 0;
                valid = false;
            }
       }
    }

    return total;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    auto rows = input.size();
    auto cols = input[0].size();

    struct Number
    {
        size_t row;
        size_t col;
        size_t len;
        int    num;
    };
    vector<Number> numbers;

    // Concatenate digit sequences and keep a record of 
    // whether any digits are adjacent to a gear symbol.
    // Store location and other information in a vector of Number.
    for (auto r: aoc::range(rows))
    {
        const auto& row = input[r];

        int    num   = 0;
        size_t len   = 0;
        bool   valid = false;

        for (auto c: aoc::range(cols))
        {
            if (isdigit(row[c]))
            {
               num = num * 10 + (row[c] - '0');
               len += 1;
               valid |= has_star(input, r, c);
            }
            else 
            {
                if (valid)
                {
                    numbers.push_back(Number{r, c-len, len, num});
                }

                num   = 0;
                len   = 0;
                valid = false;
            }
       }
    }

    // Now we need a map from gear symbol locations to numbers.
    using Gear = pair<size_t, size_t>;
    map<Gear, vector<int>> gears;
    for (auto r: aoc::range(rows))
    {
        for (auto c: aoc::range(cols))
        {
            if ((input[r][c]) == '*') gears[{r,c}] = {};
        }
    }

    // Each number is adjacent to one or more gear symbols.
    // Create a vector of adjacent numbers for each gear symbols.
    for (auto n: numbers)
    {
        for (auto r: aoc::range(n.row-1, n.row+2))
        {
            for (auto c: aoc::range(n.col-1, n.col+n.len+1))
            {
                if ((input[r][c]) == '*') gears[{r,c}].push_back(n.num);
            }
        }
    }

    // Finally, we only care about the gear symbols with exactly two adjacent numbers.
    size_t total = 0;
    for (auto [k, v]: gears)
    {
        if (v.size() == 2)
            total += v[0] * v[1];
    }

    return total;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    
    // Extend the data with a border of '.' to make the tests on adjacent symbols simpler.
    string blank(lines[0].size() + 2, '.');
    vector<string> rows;
    rows.push_back(blank);
    for (auto line: lines)
        rows.push_back("." + line + ".");
    rows.push_back(blank);

    auto p1 = part1(rows);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 536576U);

    auto p2 = part2(rows);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 75741499U);
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
