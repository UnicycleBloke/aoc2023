#include "utils.h"


// Longest path is apparently NP hard. This solution is fine for P1 but the relaxed constraint 
// for P2 makes the search tree ridiculously big. There must be some trick ... Find the lengths
// of the paths between nodes: a node is where three or more paths meet.
template <typename T, typename U >
void visit(const T& input, U& grid, size_t r, size_t c, size_t steps, vector<vector<bool>>& visited)
{
    if (input[r][c] == '#') return;
    if (visited[r][c]) return;
    visited[r][c] = true;

    //if ((steps + 100) < grid[r][c]) return;
    grid[r][c] = max(grid[r][c], steps);

    const size_t ROWS = input.size();
    const size_t COLS = input[0].size();

    // Left
    if ((c > 0) && (input[r][c] == '.') || (input[r][c] == '<'))
        visit(input, grid, r, c-1, steps+1, visited);

    // Right
    if (((c+1) < COLS) && (input[r][c] == '.') || (input[r][c] == '>'))
        visit(input, grid, r, c+1, steps+1, visited);

    // Up
    if ((r > 0) && (input[r][c] == '.') || (input[r][c] == '^'))
        visit(input, grid, r-1, c, steps+1, visited);

    // Down 
    if (((r+1) < ROWS) && (input[r][c] == '.') || (input[r][c] == 'v'))
        visit(input, grid, r+1, c, steps+1, visited);

    visited[r][c] = false;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    const size_t ROWS = input.size();
    const size_t COLS = input[0].size();

    vector<size_t> row(COLS, 0);
    vector<vector<size_t>> grid(ROWS, row);

    vector<bool> row2(COLS, 0);
    vector<vector<bool>> visited(ROWS, row2);

    visit(input, grid, 0, 1, 0, visited);

    return grid[ROWS-1][COLS-2];
}


using Pos = pair<size_t, size_t>;


template <typename T, typename U>
auto visit2(T& nodes, U& edges, map<Pos, size_t>& max_paths, Pos node, size_t path, map<Pos, bool>& visited)
{
    if (visited[node]) return;
    visited[node] = true;

    max_paths[node] = max(max_paths[node], path);

    for (auto [node2, length]: edges[node])
        visit2(nodes, edges, max_paths, node2, path + length, visited);

    visited[node] = false;
}


// To make this tractable, reduce the number of nodes in the graph. The 
// input has a node at every spot in the grid which is not '#'. There are 
// only a limited number of these nodes which off a choice in which way to 
// to next (they have 3 or 4 exits). Call these "true nodes". All the nodes 
// with 1 or 2 exits are tunnel sections on the journey between two true nodes. 
// We can count the tunnel sections joining the various true nodes and treat 
// these values as the weight for an edge.
//
// The reduced graph comprises the true nodes and the lengths of the tunnels 
// between some pairs of them (edges).
//
// I'm certain my code for reducing the graph could be greatly improved, but 
// it gets the job done.
template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    const size_t ROWS = input.size();
    const size_t COLS = input[0].size();

    // Find the set of nodes in the graph
    set<Pos> nodes;
    nodes.insert({0, 1});
    nodes.insert({ROWS-1, COLS-2});
    for (auto r: aoc::range(ROWS))
    {
        for (auto c: aoc::range(COLS))
        {
            if (input[r][c] == '#') continue;

            int exits = 0;
            if (c > 0)        exits += (input[r][c-1] != '#');
            if ((c+1) < COLS) exits += (input[r][c+1] != '#');
            if (r > 0)        exits += (input[r-1][c] != '#');
            if ((r+1) < ROWS) exits += (input[r+1][c] != '#');

            if (exits > 2)
                nodes.insert({r, c});
        }
    }

    auto find_length = [&](size_t r, size_t c, char dir)
    {
        set<Pos> visited;

        auto is_visited = [&](size_t r, size_t c)
        {
            return visited.find({r, c}) != visited.end();
        };

        visited.insert({r, c});
        switch (dir)
        {
            case '<': c -= 1; break;
            case '>': c += 1; break;
            case '^': r -= 1; break;
            case 'v': r += 1; break;
        }

        size_t length = 0;
        if (input[r][c] == '#') 
            return make_tuple(r, c, length);

        while (nodes.find({r, c}) == nodes.end())
        {
            ++length;
            visited.insert({r, c});

            if ((c > 0) && (input[r][c-1] != '#') && !is_visited(r, c-1))
                c -= 1; 

            else if ((r > 0) && (input[r-1][c] != '#') && !is_visited(r-1, c))
                r -= 1; 

            else if (((c+1) < COLS) && (input[r][c+1] != '#') && !is_visited(r, c+1))
                c += 1; 

            else if (((r+1) < ROWS) && (input[r+1][c] != '#') && !is_visited(r+1, c))
                r += 1; 
        }

        return make_tuple(r, c, length + 1);
    };

    // Find the set of edges (the lengths anyway) between the nodes.
    map<Pos, map<Pos, size_t>> edges;

    for (auto [r, c]: nodes)
    {
        if (c > 0)
        {
            auto [r2, c2, len] = find_length(r, c, '<');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }

        if ((c+1) < COLS)
        {
            auto [r2, c2, len] = find_length(r, c, '>');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }

        if (r > 0)
        {
            auto [r2, c2, len] = find_length(r, c, '^');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }

        if ((r+1) < ROWS)
        {
            auto [r2, c2, len] = find_length(r, c, 'v');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }
    }

    // Now we have the reduced graph of nodes and edges, we might be 
    // in better shape to calculate the max path. No edges can be 
    // traversed twice. Turns out it takes about 3s.
    map<Pos, size_t> max_paths;
    map<Pos, bool>   visited;
    visit2(nodes, edges, max_paths, Pos{0, 1}, 0, visited);

    return max_paths[Pos{ROWS-1, COLS-2}];
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 2278U);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 6734U);
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
