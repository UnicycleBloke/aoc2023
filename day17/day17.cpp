#include "utils.h"


enum Dir { Left, Right, Up, Down, None };


// row, col, entry direction, direction count
using  Node     = tuple<size_t, size_t, Dir, int>; 
using  NodeSet  = set<Node>;
using  NodeMap  = map<Node, map<Dir, Node>>;


template <typename T>
auto part1(T& blocks, NodeSet& nodes, NodeMap& edges)
{
    aoc::timer timer;

    Node source{0, 0, Right, 1};

    map<Node, size_t> heat_map; // distance
    for (auto node: nodes)
    {
        heat_map[node] = 0xFFFF'FFFF;
    }
    heat_map[source] = 0;

    set<Node> visited;

    vector<Node> next;
    next.push_back(source);

    // BFS starting at the node in the top left and moving right
    while (next.size() > 0)
    {
        vector<Node> next2;

        for (auto node: next)
        {
            auto update = [&](Node node2)
            {
                auto [row2, col2, dir2, cnt2] = node2;
                size_t heat2 = size_t(blocks[row2][col2] - '0');

                size_t old_heat = heat_map[node2];
                size_t new_heat = heat_map[node] + heat2;
                heat_map[node2] = min(old_heat, new_heat);

                if (visited.find(node2) == visited.end()) 
                    next2.push_back(node2);
            };

            // The node has at most 4 egdes other nodes. Probably only three due to not reversing 
            // direction, but this doens't seem to make any difference.
            for (auto [dirx, nodex]: edges[node])
            {
                update(nodex);
            }

            visited.insert(node);
        }

        next = next2;
    }

    // Just printing stuff.
    vector<size_t>         row(blocks[0].size(), 0xFFFF'FFFF);
    vector<vector<size_t>> grid(blocks.size(), row);

    for (auto node: nodes)
    {
        auto [row, col, dir, cnt] = node;
        if (heat_map[node] > 0)
            grid[row][col] = min(grid[row][col], heat_map[node]); 
    }

    for (auto r: aoc::range(grid.size()))
    {
        for (auto c: aoc::range(grid[0].size()))
        {
            char buffer[32];
            snprintf(buffer, 32, "%5ld ", grid[r][c]);
            cout << buffer;
        }
        cout << "\n";
    }

    return 0;
}


template <typename T>
auto part2(T& blocks)
{
    aoc::timer timer;

    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    set<Node> nodes;
    // Expand each node into a set of 12 nodes using the direction and count to differentiate them. 
    // The idea is that, for example, you cannot go left from a node whose left count is 3.
    for (auto r: aoc::range(lines.size()))
    {
        for (auto c: aoc::range(lines[0].size()))
        {
            // For the count of steps 1, 2, 3 - 1-based.
            for (auto i: aoc::range(1, 4))
            {
                nodes.insert(Node{r, c, Left,  i});
                nodes.insert(Node{r, c, Right, i});
                nodes.insert(Node{r, c, Up,    i});
                nodes.insert(Node{r, c, Down,  i});
            }
        }
    }

    // Create a set of edges to connect the new nodes. These capture the constraint about 3 moves.
    // The idea is that, for example going left from {r, c, h, Left, 1} will go to {r, c-1, h, Left, 2}.
    // But going left from {r, c, h, Left, 3} is not possible because there is no edge. 
    map<Node, map<Dir, Node>> edges;
    for (auto node: nodes)
    {
        auto [row, col, dir, cnt] = node;

        // Left
        if (col > 0) 
        {
            switch (dir)
            {
                case Left:
                    if (cnt < 3) edges[node][Left] = Node{row, col-1, Left, cnt+1};
                    break; 
                //case Right:
                case Up:
                case Down:
                    edges[node][Left] = Node{row, col-1, Left, 1};
            }
        }

        // Right
        if ((col+1) < lines[0].size())
        {
            switch (dir)
            {
                case Right:
                    if (cnt < 3) edges[node][Right] = Node{row, col+1, Right, cnt+1}; 
                    break; 
                //case Left:
                case Up:
                case Down:
                    edges[node][Right] = Node{row, col+1, Right, 1};
            }
        }

        // Up
        if (row > 0) 
        {
            switch (dir)
            {
                case Up:
                    if (cnt < 3) edges[node][Up] = Node{row-1, col, Up, cnt+1}; 
                    break; 
                //case Down:
                case Left:
                case Right:
                    edges[node][Up] = Node{row-1, col, Up, 1};
            }
        }

        // Down
        if ((row+1) < lines.size()) 
        {
            switch (dir)
            {
                case Down:
                    if (cnt < 3) edges[node][Down] = Node{row+1, col, Down, cnt+1}; 
                    break; 
                //case Up:
                case Left:
                case Right:
                   edges[node][Down] = Node{row+1, col, Down, 1};
            }
        }
    }

    auto p1 = part1(lines, nodes, edges); // 1225 too high
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(lines);
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
