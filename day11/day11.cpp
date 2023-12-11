#include "utils.h"


struct Star
{ 
    long row; 
    long col; 
};


template <typename T>
auto solve(const T& input)
{
    aoc::timer timer;

    long total = 0;
    for (auto i: aoc::range(input.size()))
    {
        auto s1 = input[i];
        for (auto j: aoc::range(i + 1U, input.size()))
        {
            auto s2 = input[j];

            auto path = abs(s1.row - s2.row) + abs(s1.col - s2.col);
            total += path;
        }
    }

    return total;
}


set<size_t> find_empty_cols(const vector<string>& lines)
{
    set<size_t> result;
    for (auto col: aoc::range(lines[0].size()))
    {   
        bool empty = true;     
        for (auto row: aoc::range(lines.size()))
        {
            if (lines[row][col] != '.')
            {
                empty = false;
                break; 
            }
        }        
        if (empty) result.insert(col); 
    }
    return result;
}


set<size_t> find_empty_rows(const vector<string>& lines)
{
    set<size_t> result;
    for (auto row: aoc::range(lines.size()))
    {
        bool empty = true;     
        for (auto col: aoc::range(lines[0].size()))
        {
            if (lines[row][col] != '.')
            {
                empty = false; 
                break; 
            }
        }        
        if (empty) result.insert(row); 
    }
    return result;
}


vector<Star> make_stars(const vector<string>& lines, 
    set<size_t>& empty_cols, set<size_t>& empty_rows, long grow)
{
    vector<Star> stars;

    size_t row2 = 0;
    for (auto row: aoc::range(lines.size()))
    {
        size_t col2 = 0;
        for (auto col: aoc::range(lines[0].size()))
        {
            if (lines[row][col] == '#') 
                stars.push_back({(long)row2, (long)col2});
            col2 += (empty_cols.find(col) != empty_cols.end()) ? grow : 1;
        }
        row2 += (empty_rows.find(row) != empty_rows.end()) ? grow : 1;
    }

    return stars;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);
    set<size_t> empty_cols = find_empty_cols(lines);
    set<size_t> empty_rows = find_empty_rows(lines);

    auto p1 = solve(make_stars(lines, empty_cols, empty_rows, 2));
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 9684228);

    auto p2 = solve(make_stars(lines, empty_cols, empty_rows, 1'000'000));
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 483844716556);
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
