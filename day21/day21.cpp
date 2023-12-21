#include "utils.h"


void step_grid(vector<string>& grid)
{
    auto grid2 = grid;

    auto rows = grid.size();
    auto cols = grid[0].size();

    for (size_t r = 1; r < (rows-1); ++r)
    {
        for (size_t c = 1; c < (cols-1); ++c)
        {
            if ((grid[r][c] == 'S') || (grid[r][c] == 'O'))
            {
                grid2[r][c] = '.';
                if (grid[r][c-1] != '#') grid2[r][c-1] = 'O';
                if (grid[r][c+1] != '#') grid2[r][c+1] = 'O';
                if (grid[r-1][c] != '#') grid2[r-1][c] = 'O';
                if (grid[r+1][c] != '#') grid2[r+1][c] = 'O';
            }            
        }
    }
    grid = grid2;
}


auto duplicate_grid(const vector<string>& grid, int copies)
{
    vector<string> grid2;
    for (const auto& s: grid)
    {
        auto t = s;
        replace(t.begin(), t.end(), 'S', '.');
        replace(t.begin(), t.end(), 'x', '.');

        ostringstream os;
        for (int c = 0; c < copies; ++c)
           os << t;
        grid2.push_back(os.str());
    }

    vector<string> grid3;
    for (int c = 0; c < copies; ++c)
        for (const auto& s: grid2)
            grid3.push_back(s);

    auto mid = grid2[0].size() / 2;        
    grid3[mid][mid] = 'S';
    return grid3;
}


template <typename T>
auto part1(T grid)
{
    aoc::timer timer;

    for (int s: aoc::range(64))
        step_grid(grid);

    size_t count = 0;
    for (auto r: aoc::range(grid.size()))
        for (auto c: aoc::range(grid[0].size()))
            count += (grid[r][c] == 'O');

    return count;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
 
    // Some slightly magic numbers here.
    // This the size of the grid in both dimensions. Note that this is 64 * 2 + 1.
    constexpr size_t SIZE = 131U;
    // This is the number of steps as stated in the problem.
    constexpr size_t STEPS = 26501365U;
    // This turns out to be 65. I think the same approach would work for some other
    // remainder but it doesn't matter. This value is significant because it takes the 
    // frontier of the Os to the edge of the first subgrid (the input). There is a 
    // diamond of blanks in the grid which serves to tidy up any loose edges to the 
    // diamond of Os. Not totally convinced this matters, but that's the input.
    constexpr size_t STEPS_MOD = STEPS % SIZE; 
    // The idea is to check the number of Os in each subgrid whenever the number of 
    // steps MOD SIZE is SIZE_MOD. It turns out that a very clear pattern emerges as 
    // the diamond of Os grows in increments of SIZE.
    constexpr size_t STEPS_DIV = STEPS / SIZE;
    // This many repeats gives us all the data we need about the repeating pattern.
    constexpr int COPIES = 5;

    auto grid = duplicate_grid(input, COPIES);

    array<array<size_t, COPIES>, COPIES> counts;

    for (int s: aoc::range(1U, STEPS_MOD + SIZE << 1U))
    {
        step_grid(grid);

        if ((s % SIZE) == STEPS_MOD)
        {
            auto count_subgrid = [&](int row, int col)
            {
                size_t count = 0;
                for (auto r: aoc::range(row*SIZE, (row+1)*SIZE))
                    for (auto c: aoc::range(col*SIZE, (col+1)*SIZE))
                        count += (grid[r][c] == 'O');
                return count;
            };

            // Extract the number of Os in each subgrid. When we're done, we have 
            // the data we need for the final step. 
            for (int r: aoc::range(COPIES))
                for (int c: aoc::range(COPIES))
                    counts[r][c] = count_subgrid(r, c);
        }
    }
    
    // Running the grid with a larger number of repeats, such as 11 or 15, 
    // and printing the number of Os in each subgrid, shows a very clear pattern 
    // in which:
    //   - the four corners each have constant values, 
    //   - the four edges each have two values repeated N and N-1 times respectively, 
    //     where N is the number of cycles / SIZE,
    //   - the interior has two values repeated N*N and (N-1)*(N-1) time respectively.
    size_t result = 0;
    result += counts[0][2]; // Top
    result += counts[4][2]; // Bottom
    result += counts[2][0]; // Left
    result += counts[2][4]; // Right

    // Sides
    result += counts[0][1] * STEPS_DIV; 
    result += counts[0][3] * STEPS_DIV; 
    result += counts[4][1] * STEPS_DIV; 
    result += counts[4][3] * STEPS_DIV;

    // More sides
    result += counts[1][1] * (STEPS_DIV -1U); 
    result += counts[1][3] * (STEPS_DIV -1U); 
    result += counts[3][1] * (STEPS_DIV -1U); 
    result += counts[3][3] * (STEPS_DIV -1U);

    // Interior
    result += counts[1][2] * STEPS_DIV * STEPS_DIV; 
    result += counts[2][2] * (STEPS_DIV - 1U) * (STEPS_DIV - 1U);
    
    return result;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3671U);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 609708004316870U);
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
