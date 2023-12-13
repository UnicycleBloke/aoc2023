#include "utils.h"


using Grid = vector<string>;


bool cols_match(const Grid& grid, size_t col1, size_t col2)
{
    for (auto row: aoc::range(grid.size()))
        if (grid[row][col1] != grid[row][col2]) return false;
    return true;
}


bool rows_match(const Grid& grid, size_t row1, size_t row2)
{
    for (auto col: aoc::range(grid[0].size()))
        if (grid[row1][col] != grid[row2][col]) return false;
    return true;
}


int vert_reflection(const Grid& grid, int excl = 0)
{
    for (auto col: aoc::range(grid[0].size() - 1))
    {
        int col1 = col;
        int col2 = col + 1;
        
        bool match = true;
        while ((col1 >= 0) && (col2 < grid[0].size()))
        {
            if (!cols_match(grid, col1, col2))
            {
                match = false;
                break;
            }

            --col1;
            ++col2;
        }

        if (match && ((col+1) != excl)) 
            return col + 1;
    }
    return 0;
}


int horz_reflection(const Grid& grid, int excl = 0)
{
    for (auto row: aoc::range(grid.size() - 1))
    {
        int row1 = row;
        int row2 = row + 1;
        
        bool match = true;
        while ((row1 >= 0) && (row2 < grid.size()))
        {
            if (!rows_match(grid, row1, row2))
            {
                match = false;
                break;
            }
            --row1;
            ++row2;
        }

        if (match && ((row+1) != excl)) 
            return row + 1;
    }
    return 0;
}


int reflection2(Grid& grid, int horz, int vert)
{
    for (auto row: aoc::range(grid.size()))
    {
        for (auto col: aoc::range(grid[0].size()))
        {
            grid[row][col] = (grid[row][col] == '.') ? '#' : '.';

            int vert2 = vert_reflection(grid, vert);
            if (vert2) return vert2;

            int horz2 = horz_reflection(grid, horz);
            if (horz2) return horz2 * 100;

            grid[row][col] = (grid[row][col] == '.') ? '#' : '.';
        }
    }
    return 0;
}



template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int result = 0;
    for (auto& grid: input)
    {
        result += vert_reflection(grid);
        result += horz_reflection(grid) * 100;
    }

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
  
    int result = 0;
    for (auto& grid: input)
    {
        int horz = horz_reflection(grid);
        int vert = vert_reflection(grid);
        result += reflection2(grid, horz, vert);
    }

    return result;
}


void run(const char* filename)
{
    std::vector<std::vector<std::string>> groups = aoc::read_groups(filename);

    auto p1 = part1(groups);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 34100);

    auto p2 = part2(groups);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 33106);
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
