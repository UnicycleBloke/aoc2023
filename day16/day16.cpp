#include "utils.h"


enum class Dir { L, R, U, D };
using Move = tuple<int, int, Dir>;


template <typename T>
auto reflect(const T& input, Move start)
{
    // Surely not the most efficient algo, especially for P2, but not terrible.
    // Might be faster with a vector for next Moves and a grid of ints (bitfields).
    // in place of the visited set.

    // Keep track of any Moves we have already executed, so as to avoid looping.
    // A move comprises the location and direction since it is possible to visit the 
    // same location but from a different direction.
    set<Move> visited;

    // The set of Moves to execute in the current step. This will be a growing set 
    // as a result of the bifurcations, but will diminish as all the possible Moves 
    // are exhausted.
    set<Move> next;
    next.insert(start);

    while (next.size() > 0)
    {
        // The set of Moves to execute in the next step. 
        set<Move> next2;
        for (auto move: next)
        {
            // Avoid loops - could just make a grid with a bitfield of NESW instead
            if (visited.find(move) != visited.end())
                continue;
            visited.insert(move);

            auto [row, col, dir] = move;
            switch (dir)
            {
                case Dir::L:
                    switch (input[row][col])
                    {
                        // Note there is no action for 'X', which means we have gone off the edge.
                        case '-':  
                        case '.':  next2.insert(make_tuple(row, col-1, Dir::L)); break;
                        case '\\': next2.insert(make_tuple(row-1, col, Dir::U)); break;
                        case '/':  next2.insert(make_tuple(row+1, col, Dir::D)); break;
                        case '|':  next2.insert(make_tuple(row-1, col, Dir::U)); 
                                   next2.insert(make_tuple(row+1, col, Dir::D)); break;
                    }
                    break;

                case Dir::R:
                    switch (input[row][col])
                    {
                        case '-':  
                        case '.':  next2.insert(make_tuple(row, col+1, Dir::R)); break;
                        case '\\': next2.insert(make_tuple(row+1, col, Dir::D)); break;
                        case '/':  next2.insert(make_tuple(row-1, col, Dir::U)); break;
                        case '|':  next2.insert(make_tuple(row+1, col, Dir::D)); 
                                   next2.insert(make_tuple(row-1, col, Dir::U)); break;
                    }
                    break;

                case Dir::U:
                    switch (input[row][col])
                    {
                        case '|':  
                        case '.':  next2.insert(make_tuple(row-1, col, Dir::U)); break;
                        case '\\': next2.insert(make_tuple(row, col-1, Dir::L)); break;                       
                        case '/':  next2.insert(make_tuple(row, col+1, Dir::R)); break;
                        case '-':  next2.insert(make_tuple(row, col-1, Dir::L)); 
                                   next2.insert(make_tuple(row, col+1, Dir::R)); break;
                    }
                    break;

                case Dir::D:
                    switch (input[row][col])
                    {
                        case '|':  
                        case '.':  next2.insert(make_tuple(row+1, col, Dir::D)); break;
                        case '\\': next2.insert(make_tuple(row, col+1, Dir::R)); break;
                        case '/':  next2.insert(make_tuple(row, col-1, Dir::L)); break;
                        case '-':  next2.insert(make_tuple(row, col+1, Dir::R)); 
                                   next2.insert(make_tuple(row, col-1, Dir::L)); break;
                    }
                    break;
            }
        }

        next = next2;
    } 

    set<Move> counter;
    for (auto [row, col, dir]: visited)
        if ((row > 0) && (row < (input.size()-1)) && (col > 0) && (col < (input[0].size() - 1)))
            counter.insert(make_tuple(row, col, Dir::L));
    return counter.size();
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    return reflect(input, make_tuple(1, 1, Dir::R));
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    size_t energised = 0;

    for (auto row: aoc::range(1U, input.size()-1U))
    {
        energised = max(energised, reflect(input, make_tuple(row, 1, Dir::R)));
        energised = max(energised, reflect(input, make_tuple(row, input[0].size()-2, Dir::L)));
    }

    for (auto col: aoc::range(1U, input[0].size()-1U))
    {
        energised = max(energised, reflect(input, make_tuple(1, col, Dir::D)));
        energised = max(energised, reflect(input, make_tuple(input.size()-2, col, Dir::U)));
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
