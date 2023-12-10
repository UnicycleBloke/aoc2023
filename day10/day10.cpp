#include "utils.h"


// We'll convert the letters to a bit field representing the tile.
constexpr int NORTH = 1;
constexpr int SOUTH = 2;
constexpr int EAST  = 4;
constexpr int WEST  = 8;
constexpr int DUMMY = 16;
constexpr int FILL  = 32;


struct Point
{
    int  row; // N-S
    int  col; // E-W
};


bool operator<(Point a, Point b)
{
    if (a.row != b.row) return a.row < b.row;
    return a.col < b.col;
}


template <typename T>
auto part1(const T& grid, Point start, T& grid2)
{
    aoc::timer timer;

    int   steps = 0;
    Point curr  = start;

    // The path is a cycle and must have even length. Simple runs the whole 
    // path in one direction until we get back to the start.
    set<Point> visited;
    while (true)
    {
        visited.insert(curr);

        // Added for part 2: any tiles not in the path are possibly enclosed.
        grid2[curr.row][curr.col] = grid[curr.row][curr.col];

        // Can exit north
        if ( ((grid[curr.row][curr.col] & NORTH) == NORTH) && 
             (visited.find(Point{curr.row-1, curr.col}) == visited.end()) )
        {
            curr = Point{curr.row-1, curr.col}; 
            ++steps;
        }      

        else if ( ((grid[curr.row][curr.col] & SOUTH) == SOUTH) && 
             (visited.find(Point{curr.row+1, curr.col}) == visited.end()) )
        {
            curr = Point{curr.row+1, curr.col}; 
            ++steps;
        }      

        else if ( ((grid[curr.row][curr.col] & EAST) == EAST) && 
             (visited.find(Point{curr.row, curr.col+1}) == visited.end()) )
        {
            curr = Point{curr.row, curr.col+1}; 
            ++steps;
        }      

        else if ( ((grid[curr.row][curr.col] & WEST) == WEST) && 
             (visited.find(Point{curr.row, curr.col-1}) == visited.end()) )
        {
            curr = Point{curr.row, curr.col-1}; 
            ++steps;
        }    

        else break;  
    }

    // We are one step short as the start is already in the set of visited locations.
    return (steps + 1) / 2;
}


// This is quite a nice but simple visualisation of the path.
template <typename T>
void print(const T& grid2)
{
    for (auto r: aoc::range(grid2.size()))
    {
        for (auto c: aoc::range(grid2[0].size()))        
        {           
            switch (grid2[r][c])
            {
                case 0:     cout << "XX"; break;
                case DUMMY: cout << "  "; break;
                case FILL:  cout << "  "; break;
                default:    cout << setfill('0') << setw(1) << hex << grid2[r][c] << ' ';
            }  
        }
        cout << '\n';
    }
    cout << dec << '\n';
}


template <typename T>
auto part2(T& grid, Point start)
{
    aoc::timer timer;

    int   steps = 0;
    Point curr  = start;

    // Expand the grid to insert gaps between adjacent parallel tracks. Add more 
    // pipes where necessary to span the gaps E-W and N-S. The path is now much 
    // longer but we don't care about that. All the new tiles are initially DUMMY
    // so we don't count them later.
    vector<int> row(grid[0].size() * 2, DUMMY);
    vector<vector<int>> grid2(grid.size() * 2, row);

    for (auto r: aoc::range(grid.size()))
    {
        for (auto c: aoc::range(grid[0].size()))
        {
            grid2[r*2][c*2] = grid[r][c];
            if (((grid[r][c] & SOUTH) == SOUTH) && ((grid[r+1][c] & NORTH) == NORTH))
                grid2[r*2+1][c*2] = NORTH | SOUTH;
            if (((grid[r][c] & EAST) == EAST) && ((grid[r][c+1] & WEST) == WEST))
                grid2[r*2][c*2+1] = EAST | WEST;
        }
    }

    //print(grid2);

    int rmin = 0;
    int rmax = grid2.size() - 1;
    int cmin = 0;
    int cmax = grid2[0].size() - 1;

    // Flood fill the grid and replace all reachable DUMMY and zero with FILL. The point is to blitz 
    // the zeroes which are not in the loop. The gaps add above allow the fill to procedd unhindered.
    set<Point> nodes;
    nodes.insert(Point{0,0});
    while (nodes.size() > 0)
    {     
        set<Point> nodes2;
        for (const auto [r, c]: nodes)
        {
            grid2[r][c] = FILL;
            if ((r > rmin) && ((grid2[r-1][c] == DUMMY) || (grid2[r-1][c] == 0))) nodes2.insert(Point{r-1, c});
            if ((r < rmax) && ((grid2[r+1][c] == DUMMY) || (grid2[r+1][c] == 0))) nodes2.insert(Point{r+1, c});
            if ((c > cmin) && ((grid2[r][c-1] == DUMMY) || (grid2[r][c-1] == 0))) nodes2.insert(Point{r, c-1});
            if ((c < cmax) && ((grid2[r][c+1] == DUMMY) || (grid2[r][c+1] == 0))) nodes2.insert(Point{r, c+1});
        }

        nodes = nodes2;
    }

    //print(grid2);

    // Any remaining zeroes in the grid were not reachable by the fill, so they must be enclosed.
    int enclosed = 0;
    for (auto r: aoc::range(grid2.size()))
    {
        for (auto c: aoc::range(grid2[0].size()))
        {
            if (grid2[r][c] == 0) ++enclosed;
        }
    }

    return enclosed;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    vector<int>         row(lines[0].size() + 2, 0);
    vector<vector<int>> grid(lines.size() + 2, row);
    auto grid2 = grid;

    int start_r;
    int start_c;

    for (auto r: aoc::range(lines.size()))
    {
        for (auto c: aoc::range(lines[0].size()))
        {
            int exits = 0;
            switch (lines[r][c])
            {
                // | is a vertical pipe connecting north and south.
                case '|': exits = NORTH | SOUTH; break;
                // - is a horizontal pipe connecting east and west.
                case '-': exits = EAST | WEST; break;
                // L is a 90-degree bend connecting north and east.
                case 'L': exits = NORTH | EAST; break;
                // J is a 90-degree bend connecting north and west.
                case 'J': exits = NORTH | WEST; break;
                // 7 is a 90-degree bend connecting south and west.
                case '7': exits = SOUTH | WEST; break;
                // F is a 90-degree bend connecting south and east.
                case 'F': exits = SOUTH | EAST; break;

                case 'S':
                    start_r = r+1;
                    start_c = c+1;
            }
            grid[r+1][c+1] = exits;
        }
    }

    int exits = 0;
    if ((grid[start_r-1][start_c]   & SOUTH) == SOUTH) exits |= NORTH;
    if ((grid[start_r+1][start_c]   & NORTH) == NORTH) exits |= SOUTH;
    if ((grid[start_r  ][start_c+1] & WEST)  == WEST)  exits |= EAST;
    if ((grid[start_r  ][start_c-1] & EAST)  == EAST)  exits |= WEST;
    grid[start_r][start_c] = exits;

    Point start{start_r, start_c};

    auto p1 = part1(grid, start, grid2);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 6599);

    auto p2 = part2(grid2, start);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 477);
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
