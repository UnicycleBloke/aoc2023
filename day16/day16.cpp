#include "utils.h"


constexpr int Left  = 1;
constexpr int Right = 2;
constexpr int Up    = 4;
constexpr int Down  = 8;


using Move = tuple<int, int, int>;


template <typename T>
auto reflect(const T& input, Move start)
{
    // Surely not the most efficient algo, especially for P2, but not terrible.
    // Made faster with a grid of ints (bitfields) in place of the visited set.
    // Made faster be using a vector instead of a set for the moves in the next step.

    // Keep track of any Moves we have already executed, so as to avoid looping.
    // A move comprises the location and direction since it is possible to visit the 
    // same location but from a different direction.
    vector<int> row(input[0].size(), 0);
    vector<vector<int>> visited(input.size(), row);

    // The set of Moves to execute in the current step. This will be a growing set 
    // as a result of the bifurcations, but will diminish as all the possible Moves 
    // are exhausted.
    vector<Move> next;
    next.push_back(start);

    while (next.size() > 0)
    {
        // The set of Moves to execute in the next step. 
        vector<Move> next2;
        for (auto move: next)
        {
            auto [row, col, dir] = move;

            if (visited[row][col] & dir) continue;
            visited[row][col] |= dir;

            switch (dir)
            {
                case Left:
                    switch (input[row][col])
                    {
                        // Note there is no action for 'X', which means we have gone off the edge.
                        case '-':  
                        case '.':  next2.push_back(make_tuple(row, col-1, Left)); break;
                        case '\\': next2.push_back(make_tuple(row-1, col, Up)); break;
                        case '/':  next2.push_back(make_tuple(row+1, col, Down)); break;
                        case '|':  next2.push_back(make_tuple(row-1, col, Up)); 
                                   next2.push_back(make_tuple(row+1, col, Down)); break;
                    }
                    break;

                case Right:
                    switch (input[row][col])
                    {
                        case '-':  
                        case '.':  next2.push_back(make_tuple(row, col+1, Right)); break;
                        case '\\': next2.push_back(make_tuple(row+1, col, Down)); break;
                        case '/':  next2.push_back(make_tuple(row-1, col, Up)); break;
                        case '|':  next2.push_back(make_tuple(row+1, col, Down)); 
                                   next2.push_back(make_tuple(row-1, col, Up)); break;
                    }
                    break;

                case Up:
                    switch (input[row][col])
                    {
                        case '|':  
                        case '.':  next2.push_back(make_tuple(row-1, col, Up)); break;
                        case '\\': next2.push_back(make_tuple(row, col-1, Left)); break;                       
                        case '/':  next2.push_back(make_tuple(row, col+1, Right)); break;
                        case '-':  next2.push_back(make_tuple(row, col-1, Left)); 
                                   next2.push_back(make_tuple(row, col+1, Right)); break;
                    }
                    break;

                case Down:
                    switch (input[row][col])
                    {
                        case '|':  
                        case '.':  next2.push_back(make_tuple(row+1, col, Down)); break;
                        case '\\': next2.push_back(make_tuple(row, col+1, Right)); break;
                        case '/':  next2.push_back(make_tuple(row, col-1, Left)); break;
                        case '-':  next2.push_back(make_tuple(row, col+1, Right)); 
                                   next2.push_back(make_tuple(row, col-1, Left)); break;
                    }
                    break;
            }
        }

        next = next2;
    } 

    size_t counter = 0;
    for (auto r: aoc::range(1U, visited.size()-1U))
    {
        for (auto c: aoc::range(1U, visited[0].size()-1U))
        {
            counter += (visited[r][c] > 0);
        }

    }
    return counter;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    return reflect(input, make_tuple(1, 1, Right));
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    size_t energised = 0;

    for (auto row: aoc::range(1U, input.size()-1U))
    {
        energised = max(energised, reflect(input, make_tuple(row, 1, Right)));
        energised = max(energised, reflect(input, make_tuple(row, input[0].size()-2, Left)));
    }

    for (auto col: aoc::range(1U, input[0].size()-1U))
    {
        energised = max(energised, reflect(input, make_tuple(1, col, Down)));
        energised = max(energised, reflect(input, make_tuple(input.size()-2, col, Up)));
    }

    return energised;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    // Extend the grid with a border to make it simpler to deal with off-the-edge conditions.
    string row(lines[0].size() + 2, 'x');
    vector<string> grid;
    grid.push_back(row);
    for (const auto& line: lines)
        grid.push_back("x" + line + "x");
    grid.push_back(row);

    auto p1 = part1(grid);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 6855U);

    auto p2 = part2(grid);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 7513U);
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
